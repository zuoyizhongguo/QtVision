#include "hub.h"
#include<QDebug>

Hub::Hub(QObject *parent)
    : QObject{parent}
{
}

void Hub::openCam()
{
    qDebug()<<"打开相机";

    connect(&m_Cam1,&BaslerCamera::sigCurrentImage,[&](QImage& img){
        m_ImgProvider.m_imgQ=img;  //直接赋图
        emit callQmlShowImg();
        qDebug()<<"emit callQmlShowImg";
    });
    connect(&m_Cam2,&BaslerCamera::sigCurrentImage,[&](QImage& img){
        m_ImgProvider.m_imgQ=img;  //直接赋图
        emit callQmlShowImg2();
        qDebug()<<"emit callQmlShowImg2";
    });

    m_Cam1.openCamera("40078020");
    m_Cam2.openCamera("40076341");
}
void Hub::startCam()
{
    qDebug()<<"开始拉流";

    connect(&m_Timer,&QTimer::timeout,&m_Cam1,&BaslerCamera::updateImage);
    connect(&m_Timer,&QTimer::timeout,&m_Cam2,&BaslerCamera::updateImage);

    m_Cam1.startGrab();
    m_Cam2.startGrab();
    m_Timer.start(100);
}
void Hub::stopCam()
{
    qDebug()<<"停止拉流";

    m_Timer.disconnect();       //断信号，养成好习惯

    m_Timer.stop();
    m_Cam1.stopGrab();
    m_Cam2.stopGrab();
}
void Hub::closeCam()
{
    qDebug()<<"关闭相机";

    m_Cam1.disconnect();
    m_Cam2.disconnect();

    m_Cam1.closeCamera();
    m_Cam2.closeCamera();
}
