#pragma once

#include <QOpenglWidget>

class MyGLWidget  : public QOpenGLWidget
{
	Q_OBJECT

public:
	MyGLWidget(QWidget *parent);
	~MyGLWidget();

	//重写事件
	void paintEvent(QPaintEvent* e) override;

	void setImageData(const QImage& img);

private:
	QImage mImg;
};
