#pragma once

#include <QLabel>
#include <QMouseEvent>

class ASClicklabel : public QLabel
{
	Q_OBJECT
public:
	explicit ASClicklabel(QWidget *parent = 0);
	~ASClicklabel();
signals:
	void clicked();//自定义clicked()信号,在mousePressEvent事件发生时触发
protected:
	virtual void mousePressEvent(QMouseEvent* event);//重写mousePressEvent事件
};

