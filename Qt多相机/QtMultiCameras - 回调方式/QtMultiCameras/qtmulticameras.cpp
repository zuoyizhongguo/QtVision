﻿#include "qtmulticameras.h"


QtMultiCameras::QtMultiCameras(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::QtMultiCamerasClass())
{
    ui->setupUi(this);
    
    connect(ui->pushButton_openCamera, &QPushButton::clicked, this, &QtMultiCameras::openCamera);
    connect(ui->pushButton_startGrab, &QPushButton::clicked, this, &QtMultiCameras::startGrab);
    connect(ui->pushButton_stopGrab, &QPushButton::clicked, this, &QtMultiCameras::stopGrab);
    connect(ui->pushButton_closeCamera, &QPushButton::clicked, this, &QtMultiCameras::closeCamera);
    
    qDebug() << "Main:" << QThread::currentThreadId();  

    setButtonStatus(true, false);
    isCameraWork = false;
    
}

QtMultiCameras::~QtMultiCameras()
{
    if (isCameraWork)
    {
        closeCamera();
    }

    delete ui;
}



void QtMultiCameras::openCamera()
{
    m_cam1 = new BaslerCamera();
    m_cam2 = new BaslerCamera();
    m_cam1->openCamera("40078020");
    m_cam2->openCamera("40076341");

    connect(m_cam1, &BaslerCamera::sigCurrentImage, this, &QtMultiCameras::slotShowImg);
    connect(m_cam2, &BaslerCamera::sigCurrentImage, this, &QtMultiCameras::slotShowImg2);
    connect(&timer, &QTimer::timeout, m_cam1, &BaslerCamera::updateImage);
    connect(&timer, &QTimer::timeout, m_cam2, &BaslerCamera::updateImage);

    setButtonStatus(false, true);
    isCameraWork = true;
}

void QtMultiCameras::startGrab()
{
    if (m_cam1->startGrab() && m_cam2->startGrab()) //拉流在回调函数中进行（效率高于自己写的线程死循环）
    {
        timer.start(100);      //UI显示帧率

        setButtonStatus(false, false);
    }   

}

void QtMultiCameras::stopGrab()
{
    timer.stop();

    m_cam1->stopGrab();
    m_cam2->stopGrab();

    setButtonStatus(false, true);
}

void QtMultiCameras::closeCamera()
{
    disconnect(m_cam1, &BaslerCamera::sigCurrentImage, this, &QtMultiCameras::slotShowImg);
    disconnect(m_cam2, &BaslerCamera::sigCurrentImage, this, &QtMultiCameras::slotShowImg2);
    disconnect(&timer, &QTimer::timeout, m_cam1, &BaslerCamera::updateImage);
    disconnect(&timer, &QTimer::timeout, m_cam2, &BaslerCamera::updateImage);

    m_cam1->closeCamera();
    m_cam2->closeCamera();
    
    if (m_cam1 != nullptr)
    {
        delete m_cam1;
        m_cam1 = nullptr;
    }
    if (m_cam2!=nullptr)
    {
        delete m_cam2;
        m_cam2 = nullptr;
    } 

    ui->label_show1->clear();
    ui->label_show2->clear();
    setButtonStatus(true, false);
    isCameraWork = false;
}

void QtMultiCameras::slotShowImg(QImage img)
{
    QPixmap pix = QPixmap::fromImage(img);
    ui->label_show1->setPixmap(pix);
    ui->label_show1->setScaledContents(true); //图像适应label大小
}
void QtMultiCameras::slotShowImg2(QImage img)
{
    QPixmap pix = QPixmap::fromImage(img);
    ui->label_show2->setPixmap(pix);
    ui->label_show2->setScaledContents(true); //图像适应label大小
}

void QtMultiCameras::setButtonStatus(bool canOpen, bool canGrab)
{
    ui->pushButton_openCamera->setEnabled(canOpen);             //开、关 逻辑互斥
    ui->pushButton_closeCamera->setEnabled(!canOpen);
    ui->pushButton_startGrab->setEnabled(!canOpen && canGrab);  //拉流、停止拉流 逻辑互斥
    ui->pushButton_stopGrab->setEnabled(!canOpen && !canGrab);
    QApplication::processEvents();
}

