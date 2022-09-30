#ifndef IMAGEPROVIDER_H
#define IMAGEPROVIDER_H

#include <QObject>
#include <QImage>
#include <QQuickImageProvider>

//继承QQuickImageProvider，重写requestImage，发信号
class ImageProvider:public QQuickImageProvider
{
    Q_OBJECT
public:
    ImageProvider(QObject *parent = nullptr);
    QImage requestImage(const QString &id,QSize *size,const QSize &requestedSize) override; //重写函数
public:
    QImage m_imgQ;
};

#endif // IMAGEPROVIDER_H
