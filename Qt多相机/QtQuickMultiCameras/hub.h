#ifndef HUB_H
#define HUB_H

#include <QObject>
#include<QTimer>
#include "baslercamera.h"
#include"imageprovider.h"

class Hub : public QObject
{
    Q_OBJECT
public:
    explicit Hub(QObject *parent = nullptr);

public:
    Q_INVOKABLE void openCam();     //可被qml识别
    Q_INVOKABLE void startCam();
    Q_INVOKABLE void stopCam();
    Q_INVOKABLE void closeCam();

signals:
    void callQmlShowImg();
    void callQmlShowImg2();         //相机2刷新UI
public:
    BaslerCamera m_Cam1;
    BaslerCamera m_Cam2;
    QTimer m_Timer;
public:
    //如果定义为指针类型，注意在Hub的构造函数中new。否则main.cpp中engine.addImageProvider()无图（没有开辟内存）
    ImageProvider m_ImgProvider;
};

#endif // HUB_H
