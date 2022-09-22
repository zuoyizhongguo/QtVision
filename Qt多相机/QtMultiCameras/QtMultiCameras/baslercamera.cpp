#include "baslercamera.h"
#include <QImage>
#include <QDebug>



BaslerCamera::BaslerCamera(QString sn,QObject *parent)
	: QObject(parent)
{	
	m_sn = sn;

	Pylon::PylonInitialize();					//调用其他 pylon 函数之前必须调用 PylonInitialize 完成初始化

	m_lastTime = GetTickCount();				//帧率统计
	m_iFps = 0;
	m_iFrameCount = 0;

	m_ptrGrabResult_success = new Pylon::CGrabResultPtr();	//在析构中delete
	img = new CPylonImage();
}

BaslerCamera::~BaslerCamera()
{
	Pylon::PylonTerminate();					// 释放 pylon 运行时系统分配的资源

	if (m_ptrGrabResult_success!=nullptr)		//delete指针
	{
		delete m_ptrGrabResult_success;
		m_ptrGrabResult_success = nullptr;
	}
	if (img != nullptr)		//delete指针
	{
		delete img;
		img = nullptr;
	}
}

bool BaslerCamera::openCamera()
{
	try
	{
		m_baslerCamera = new Pylon::CInstantCamera();		//在closeCamera中delete
		m_baslerCamera->Attach(CTlFactory::GetInstance().CreateDevice(Pylon::CDeviceInfo().SetSerialNumber(m_sn.toStdString().c_str())));
		m_baslerCamera->Open();								//连接相机
		qDebug() << "相机连接成功！";
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
		if (!m_baslerCamera->IsGrabbing())
		{
			m_baslerCamera->StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);  //使用GrabStrategy_LatestImages。因为GrabStrategy_OneByOne会有很多缓存图，处理的可能不是最新的。
		}
		
		qDebug() << "相机开始拉流！";
		
		QtConcurrent::run(this,&BaslerCamera::grabbed);	//线程中执行死循环
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
			qDebug() << "相机停止拉流！";
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
			m_baslerCamera->DetachDevice();			//分离pylon设备
			m_baslerCamera->Close();
			qDebug() << "相机关闭！";

			if (m_baslerCamera!=nullptr)
			{
				delete m_baslerCamera;				//delete指针
				m_baslerCamera = nullptr;
			}			
		}
	}
	catch (const std::exception&)
	{
		qDebug() << "相机关闭失败！";
		return false;
	}
	return true;
}

void BaslerCamera::grabbed()						//线程函数，获取帧
{
	qDebug() << "拉流...(设备及线程)"<<m_baslerCamera->GetDeviceInfo().GetModelName() << QThread::currentThreadId();

	while (m_baslerCamera->IsGrabbing())
	{
		if (m_baslerCamera->RetrieveResult(5000, m_ptrGrabResult, TimeoutHandling_Return) && m_ptrGrabResult->GrabSucceeded() )
		{			
			*m_ptrGrabResult_success = m_ptrGrabResult;	//深拷贝帧

			//格式转换大约耗时8ms以上，不在此处进行（防止影响帧率）
			/*
				QImage img;
				toQImage(m_ptrGrabResult, img);
				emit sigCurrentImage(img);
			*/

			//统计帧率
			++m_iFrameCount;
			m_curTime = GetTickCount();
			if (m_curTime - m_lastTime > 1000)   //取固定时间间隔为1秒
			{
				m_iFps = m_iFrameCount;
				m_iFrameCount = 0;
				m_lastTime = m_curTime;
				qDebug() << "帧率" << m_iFps;
			}			
		}		
	}
}


void BaslerCamera::toQImage(CGrabResultPtr ptrGrabResult, QImage& OutImage)	//输出型参数用&修饰，因为它是要被改变的。
{	
	if (ptrGrabResult==NULL)
	{
		return;
	}
	int width = static_cast<int>(ptrGrabResult->GetWidth());
	int height = static_cast<int>(ptrGrabResult->GetHeight());
	
	if (ptrGrabResult->GetPixelType() == Pylon::PixelType_Mono8)
	{
		uchar* buffer = (uchar*)ptrGrabResult->GetBuffer();
		OutImage = QImage(buffer, width, height, QImage::Format_Grayscale8).copy();//深拷贝，防止 QPixmap::fromImage(img)访问冲突
	}
	else //bayer格式等
	{
		CImageFormatConverter   fc;

		//通过官方函数先转为 RGB8
		fc.OutputPixelFormat = PixelType_RGB8packed;

		fc.Convert(*img, ptrGrabResult);
		uchar* buffer = (uchar*)img->GetBuffer();

		OutImage = QImage(buffer, width, height, QImage::Format_RGB888).copy();		
	}
}

//转换并输出给UI展示
void BaslerCamera::updateImage()
{
		toQImage(*m_ptrGrabResult_success, qImage);
		emit sigCurrentImage(qImage);

		qDebug() << "updateImage" << QThread::currentThreadId();
}