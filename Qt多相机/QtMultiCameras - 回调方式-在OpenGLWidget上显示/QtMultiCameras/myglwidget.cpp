#include "myglwidget.h"
#include<QPainter>

MyGLWidget::MyGLWidget(QWidget *parent)
	: QOpenGLWidget(parent)
{}

MyGLWidget::~MyGLWidget()
{}

void MyGLWidget::setImageData(const QImage& img)
{
	mImg = img;
	update();
}

void MyGLWidget::paintEvent(QPaintEvent* e)
{
	QPainter p;
	p.begin(this);
	p.drawImage(rect(), mImg);
	p.end();
}