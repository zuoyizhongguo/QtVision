#include "baslercamera.h"
#include <QImage>
#include <QDebug>

//BaslerCamera::BaslerCamera(QString sn,QObject *parent)
BaslerCamera::BaslerCamera(QObject* parent)
	: QObject(parent)
{	
	Pylon::PylonInitialize();							//调用其他 pylon 函数之前必须调用 PylonInitialize 完成初始化

	m_baslerCamera = new Pylon::CInstantCamera();		//在析构中delete
	//注册回调函数。调用本类内的回调函数OnImageGrabed，与下文GrabLoop_ProvidedByInstantCamera一起使用
	m_baslerCamera->RegisterImageEventHandler(this, Pylon::RegistrationMode_ReplaceAll, Pylon::Cleanup_None); 
	
	//帧率统计
	m_lastTime = GetTickCount();				
	m_iFps = 0;
	m_iFrameCount = 0;
}

BaslerCamera::~BaslerCamera()
{
	if (m_baslerCamera != nullptr)
	{
		delete m_baslerCamera;				//delete指针
		m_baslerCamera = nullptr;
	}

	Pylon::PylonTerminate();				// 释放 pylon 运行时系统分配的资源
}

bool BaslerCamera::openCamera(QString sn)
{
	try
	{
		m_baslerCamera->Attach(CTlFactory::GetInstance().CreateDevice(Pylon::CDeviceInfo().SetSerialNumber(sn.toStdString().c_str())));
		m_baslerCamera->Open();				//连接相机
		qDebug() << "相机连接成功" << m_baslerCamera->GetDeviceInfo().GetModelName();
	}
	catch (const std::exception&)
	{
		qDebug() << "相机连接失败！";
		return false;
	}
	return true;	
}

bool BaslerCamera::startGrab()
{
	if (!m_baslerCamera->IsOpen())
	{
		qDebug() << "相机未连接，拉流失败";
		return false;
	}
	try
	{
		if (m_baslerCamera->IsGrabbing())
		{
			m_baslerCamera->StopGrabbing();		//如果在拉流就先停止，再拉流
		}
		//使用回调函数处理实时帧
		m_baslerCamera->StartGrabbing(Pylon::GrabStrategy_LatestImageOnly, Pylon::GrabLoop_ProvidedByInstantCamera); 
	}
	catch (const std::exception&)
	{
		qDebug() << "相机拉流失败！";
		return false;
	}
	return true;
}

bool BaslerCamera::stopGrab()
{
	try
	{
		if (m_baslerCamera->IsGrabbing())
		{			
			qDebug() << "相机停止拉流"<<m_baslerCamera->GetDeviceInfo().GetModelName();
			m_baslerCamera->StopGrabbing();	
		}
	}
	catch (const std::exception&)
	{
		qDebug() << "相机停止拉流失败！";
		return false;
	}
	return true;
}

bool BaslerCamera::closeCamera()
{
	try
	{
		if (m_baslerCamera->IsOpen())
		{
			if (m_baslerCamera->IsGrabbing())
			{
				m_baslerCamera->StopGrabbing();
			}
			qDebug() << "相机关闭" << m_baslerCamera->GetDeviceInfo().GetModelName();
			m_baslerCamera->Close();				//先关闭，再分离
			m_baslerCamera->DetachDevice();			//分离pylon设备
		}
	}
	catch (const std::exception&)
	{
		qDebug() << "相机关闭失败！";
		return false;
	}
	return true;
}

//回调函数，处理采图、转换、显示
void BaslerCamera::OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& ptrGrabResult)
{
	//帧获取时会有这样的操作：When overwriting the current CGrabResultPtr, the previous result will automatically be released
	//加锁，保证在格式转换过程中，m_ptrGrabResult不被操作。
	QMutexLocker lock(&m_mutex);
	if (ptrGrabResult->GrabSucceeded()) //帧获取成功
	{
		//统计帧率
		++m_iFrameCount;
		m_curTime = GetTickCount();
		if (m_curTime - m_lastTime > 1000)   //取固定时间间隔为1秒
		{
			m_iFps = m_iFrameCount;
			m_iFrameCount = 0;
			m_lastTime = m_curTime;
			qDebug() << "帧率" << m_baslerCamera->GetDeviceInfo().GetModelName() << m_iFps;
			qDebug() << "相机开始拉流" << m_baslerCamera->GetDeviceInfo().GetModelName() << QThread::currentThreadId();
		}
		
		m_ptrGrabResult = ptrGrabResult;
	}
}


bool BaslerCamera::toQImage(const CGrabResultPtr& ptrGrabResult, QImage& OutImage)	//输出型参数用&修饰，因为它是要被改变的。
{	
	if (ptrGrabResult ==NULL)	//判断是否存在
	{
		return false;
	}
	try
	{
		int width = static_cast<int>(ptrGrabResult->GetWidth());
		int height = static_cast<int>(ptrGrabResult->GetHeight());
		if (ptrGrabResult->GetPixelType() == Pylon::PixelType_Mono8)
		{
			uchar* buffer = (uchar*)ptrGrabResult->GetBuffer();
			OutImage = QImage(buffer, width, height, QImage::Format_Grayscale8).copy();//深拷贝，防止 QPixmap::fromImage(img)访问冲突
		}
		else //bayer格式等
		{
			try
			{
				CImageFormatConverter   fc;
				fc.OutputPixelFormat = PixelType_RGB8packed;//通过官方函数先转为 RGB8
				CPylonImage tempImg;
				
				fc.Convert(tempImg, ptrGrabResult);			
				uchar* buffer = (uchar*)tempImg.GetBuffer();
				
				OutImage = QImage(buffer, width, height, QImage::Format_RGB888).copy();
			}
			catch (const Pylon::GenericException& e)
			{
				qDebug() << "官方函数格式转换失败" + QString(e.GetDescription());
				return false;
			}
		}
	}
	catch (const Pylon::GenericException& e)
	{
		qDebug() << "失败" + QString(e.GetDescription());
		return false;
	}	

	return true;
}

//转换并输出给UI展示
void BaslerCamera::updateImage()
{
	QMutexLocker lock(&m_mutex); //m_ptrGrabResult在转换时可能被回调函数清空
	if (toQImage(m_ptrGrabResult, qImage))
	{
		emit sigCurrentImage(qImage);
	}
}