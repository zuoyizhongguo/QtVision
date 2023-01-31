/********************************************************************************
** Form generated from reading UI file 'qtmulticameras.ui'
**
** Created by: Qt User Interface Compiler version 5.15.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QTMULTICAMERAS_H
#define UI_QTMULTICAMERAS_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QtMultiCamerasClass
{
public:
    QPushButton *pushButton_openCamera;
    QPushButton *pushButton_closeCamera;
    QPushButton *pushButton_startGrab;
    QPushButton *pushButton_stopGrab;
    QWidget *verticalLayoutWidget;
    QVBoxLayout *verticalLayout;
    QWidget *verticalLayoutWidget_2;
    QVBoxLayout *verticalLayout_2;

    void setupUi(QWidget *QtMultiCamerasClass)
    {
        if (QtMultiCamerasClass->objectName().isEmpty())
            QtMultiCamerasClass->setObjectName(QString::fromUtf8("QtMultiCamerasClass"));
        QtMultiCamerasClass->resize(998, 480);
        pushButton_openCamera = new QPushButton(QtMultiCamerasClass);
        pushButton_openCamera->setObjectName(QString::fromUtf8("pushButton_openCamera"));
        pushButton_openCamera->setGeometry(QRect(280, 410, 75, 24));
        pushButton_closeCamera = new QPushButton(QtMultiCamerasClass);
        pushButton_closeCamera->setObjectName(QString::fromUtf8("pushButton_closeCamera"));
        pushButton_closeCamera->setGeometry(QRect(550, 410, 75, 24));
        pushButton_startGrab = new QPushButton(QtMultiCamerasClass);
        pushButton_startGrab->setObjectName(QString::fromUtf8("pushButton_startGrab"));
        pushButton_startGrab->setGeometry(QRect(370, 410, 75, 24));
        pushButton_stopGrab = new QPushButton(QtMultiCamerasClass);
        pushButton_stopGrab->setObjectName(QString::fromUtf8("pushButton_stopGrab"));
        pushButton_stopGrab->setGeometry(QRect(460, 410, 75, 24));
        verticalLayoutWidget = new QWidget(QtMultiCamerasClass);
        verticalLayoutWidget->setObjectName(QString::fromUtf8("verticalLayoutWidget"));
        verticalLayoutWidget->setGeometry(QRect(30, 60, 451, 321));
        verticalLayout = new QVBoxLayout(verticalLayoutWidget);
        verticalLayout->setSpacing(6);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        verticalLayoutWidget_2 = new QWidget(QtMultiCamerasClass);
        verticalLayoutWidget_2->setObjectName(QString::fromUtf8("verticalLayoutWidget_2"));
        verticalLayoutWidget_2->setGeometry(QRect(500, 60, 451, 321));
        verticalLayout_2 = new QVBoxLayout(verticalLayoutWidget_2);
        verticalLayout_2->setSpacing(6);
        verticalLayout_2->setContentsMargins(11, 11, 11, 11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 0, 0);

        retranslateUi(QtMultiCamerasClass);

        QMetaObject::connectSlotsByName(QtMultiCamerasClass);
    } // setupUi

    void retranslateUi(QWidget *QtMultiCamerasClass)
    {
        QtMultiCamerasClass->setWindowTitle(QCoreApplication::translate("QtMultiCamerasClass", "QtMultiCameras", nullptr));
        pushButton_openCamera->setText(QCoreApplication::translate("QtMultiCamerasClass", "\346\211\223\345\274\200\347\233\270\346\234\272", nullptr));
        pushButton_closeCamera->setText(QCoreApplication::translate("QtMultiCamerasClass", "\345\205\263\351\227\255\347\233\270\346\234\272", nullptr));
        pushButton_startGrab->setText(QCoreApplication::translate("QtMultiCamerasClass", "\345\274\200\345\247\213\346\213\211\346\265\201", nullptr));
        pushButton_stopGrab->setText(QCoreApplication::translate("QtMultiCamerasClass", "\346\232\202\345\201\234\346\213\211\346\265\201", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QtMultiCamerasClass: public Ui_QtMultiCamerasClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QTMULTICAMERAS_H
