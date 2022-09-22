#pragma once
#pragma execution_character_set("utf-8")				//防止中文乱码

#include <QObject>
#include <QImage>
#include <pylon/PylonIncludes.h>
#include <QThread>
#include <QtConcurrent>

using namespace Pylon;

class BaslerCamera  : 	public QObject
{
	Q_OBJECT

public:
	BaslerCamera(QString sn,QObject *parent=nullptr);	//含参构造，相机的序列号
	~BaslerCamera();

	bool openCamera();
	bool closeCamera();
	bool startGrab();
	bool stopGrab();


	void grabbed();										//线程函数，获取帧
	void toQImage(CGrabResultPtr ptrGrabResult, QImage &OutImage);
	void updateImage();
	

signals:
	void sigCurrentImage(QImage qImg);


private:
	QImage qImage;										//相机获取的图像

	QString m_sn;										//相机的序列号
	Pylon::CInstantCamera* m_baslerCamera;				//相机
	Pylon::CGrabResultPtr  m_ptrGrabResult;				//实时帧
	Pylon::CGrabResultPtr* m_ptrGrabResult_success;		//成功获取的帧	

	//帧率统计，固定时间间隔统计帧数法
	int m_iFps;
	int m_iFrameCount;
	DWORD m_lastTime, m_curTime;

	CPylonImage* img;
};
