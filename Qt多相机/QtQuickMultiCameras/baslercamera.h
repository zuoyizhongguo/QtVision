#pragma once
#pragma execution_character_set("utf-8")				//防止中文乱码

#include <QObject>
#include <QImage>
#include <pylon/PylonIncludes.h>
#include <QThread>
//#include <QtConcurrent>

#include <QMutex>
#include <QMutexLocker>

using namespace Pylon;

class BaslerCamera  : 	
	public QObject, 
	public Pylon::CImageEventHandler	//使用回调函数必须继承此项
{
	Q_OBJECT

public:	
	explicit BaslerCamera(QObject* parent = nullptr);
	~BaslerCamera();

	bool openCamera(QString sn);		//含参，相机的序列号
	bool closeCamera();
	bool startGrab();
	bool stopGrab();

	virtual void OnImageGrabbed(Pylon::CInstantCamera& camera, const Pylon::CGrabResultPtr& grabResult) override;	//实现其内容
	bool toQImage(const CGrabResultPtr& ptrGrabResult, QImage &OutImage);
	void updateImage();
	

signals:
    void sigCurrentImage(QImage& qImg);


private:
	QImage qImage;										//相机获取的图像

	Pylon::CInstantCamera* m_baslerCamera;				//相机
	Pylon::CGrabResultPtr  m_ptrGrabResult;				//实时帧

	//帧率统计，固定时间间隔统计帧数法
	int m_iFps;
	int m_iFrameCount;
	DWORD m_lastTime, m_curTime;

	QMutex m_mutex;
};
