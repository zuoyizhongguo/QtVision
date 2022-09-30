#include "imageprovider.h"

ImageProvider::ImageProvider(QObject *parent):QQuickImageProvider(QQuickImageProvider::Image)
{

}


QImage ImageProvider::requestImage(const QString &id, QSize *size, const QSize &requestedSize)
{
    if(size){
        *size=m_imgQ.size();
    }
    if(requestedSize.width()>0 && requestedSize.height()>0){
        m_imgQ=m_imgQ.scaled(requestedSize.width(),requestedSize.height(),Qt::KeepAspectRatio);//等比缩放
    }
    return m_imgQ;
}
