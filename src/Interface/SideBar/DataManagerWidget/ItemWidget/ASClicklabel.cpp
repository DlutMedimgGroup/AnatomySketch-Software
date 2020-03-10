#include "ASClicklabel.h"

ASClicklabel::ASClicklabel(QWidget* parent)
	: QLabel(parent)
{
}

ASClicklabel::~ASClicklabel()
{
}

void ASClicklabel::mousePressEvent(QMouseEvent *event)//重写mousePressEvent事件,检测事件类型是不是点击了鼠标左键
{
	//如果单击了就触发clicked信号
	if (event->button() == Qt::LeftButton) {
		//触发clicked信号
		emit clicked();
	}
	//将该事件传给父类处理
	QLabel::mousePressEvent(event);
}