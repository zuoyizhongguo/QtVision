#pragma once

#include <QtWidgets/QWidget>
#include "ui_qtmulticameras.h"
#include "baslercamera.h"

QT_BEGIN_NAMESPACE
namespace Ui { class QtMultiCamerasClass; };
QT_END_NAMESPACE

//extern QMutex m_mutex;

class QtMultiCameras : public QWidget
{
    Q_OBJECT

public:
    QtMultiCameras(QWidget *parent = nullptr);
    ~QtMultiCameras();

    void openCamera();
    void closeCamera();
    void startGrab();
    void stopGrab();
    void slotShowImg(QImage img);
    void slotShowImg2(QImage img);

    void setButtonStatus(bool canOpen, bool canGrab); //按钮逻辑

private:
    Ui::QtMultiCamerasClass *ui;
    BaslerCamera* m_cam1;
    BaslerCamera* m_cam2;

    QTimer* timer;

    bool isCameraWork;      //是否有相机连接。用于UI关闭
};
