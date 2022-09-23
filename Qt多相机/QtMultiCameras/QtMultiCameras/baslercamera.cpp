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
}

BaslerCamera::~BaslerCamera()
{
	Pylon::PylonTerminate();					// 释放 pylon 运行时系统分配的资源
}

bool BaslerCamera::openCamera()
{
	try
	{
		m_baslerCamera = new Pylon::CInstantCamera();		//在closeCamera中delete
		m_baslerCamera->Attach(CTlFactory::GetInstance().CreateDevice(Pylon::CDeviceInfo().SetSerialNumber(m_sn.toStdString().c_str())));
		m_baslerCamera->Open();								//连接相机
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
		if (!m_baslerCamera->IsGrabbing())
		{
			m_baslerCamera->StartGrabbing(Pylon::GrabStrategy_LatestImageOnly);  //使用GrabStrategy_LatestImages。因为GrabStrategy_OneByOne会有很多缓存图，处理的可能不是最新的。
		}
		
		//qDebug() << "相机开始拉流！";
		
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
			qDebug() << "相机停止拉流"<<m_baslerCamera->GetDeviceInfo().GetModelName();
			m_baslerCamera->StopGrabbing();	
			//m_ptrGrabResult_success.Release();		//如果不释放，toQImage中方式一报tempImg.GetBuffer()访问冲突
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
			m_baslerCamera->DetachDevice();			//分离pylon设备
			m_baslerCamera->Close();			

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
	qDebug() << "相机开始拉流"<<m_baslerCamera->GetDeviceInfo().GetModelName() << QThread::currentThreadId();

	while (m_baslerCamera->IsGrabbing())
	{
		//QMutexLocker lock(&m_mutex);	//加锁会影响到相机的帧率，toQImage中已经判断 m_ptrGrabResult_success，不再加锁。
		if (m_baslerCamera->RetrieveResult(5000, m_ptrGrabResult, TimeoutHandling_Return) && m_ptrGrabResult->GrabSucceeded() )
		{			
			m_ptrGrabResult_success = m_ptrGrabResult;	//深拷贝帧，调试可以看出创建时有自己单独内存

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
				qDebug() << "帧率" << m_baslerCamera->GetDeviceInfo().GetModelName()<< m_iFps;
			}			
		}		
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
				CPylonImage tempImg, copyImg;
				
				//方式一：可能导致tempImg.GetBuffer()发生访问冲突。如果想使用，可以在stopGrab()函数中 Release()
				//fc.Convert(tempImg, ptrGrabResult);			
				//uchar* buffer = (uchar*)tempImg.GetBuffer();

				//方式二：无问题，很稳
				tempImg.AttachGrabResultBuffer(ptrGrabResult);
				fc.Convert(copyImg, tempImg);				//此函数可能失败，因此放到try中			
				uchar* buffer = (uchar*)copyImg.GetBuffer();
				
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
	//QMutexLocker lock(&m_mutex); //m_ptrGrabResult_success在转换时可能被grabbed函数清空
	if (toQImage(m_ptrGrabResult_success, qImage))
	{
		emit sigCurrentImage(qImage);
	}
}