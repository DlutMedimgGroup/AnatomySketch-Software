/*
最大化 最小化 关闭等按钮
*/
#pragma once

#include <QPushButton>
#include <QMouseEvent>

class ASPushButtonInTitleWidget : public QPushButton
{
	Q_OBJECT

public:
	explicit ASPushButtonInTitleWidget(QWidget *parent = 0);
	~ASPushButtonInTitleWidget();

	//设置按钮的背景图片
	void setPicName(QString pic_name);

	//设置按钮尺寸
	void setButtomSize(const int width, const int height);

protected:

	void enterEvent(QEvent *);
	void leaveEvent(QEvent *);
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void paintEvent(QPaintEvent *);

private:

	//枚举按钮的几种状态
	enum ButtonStatus { NORMAL, ENTER, PRESS, NOSTATUS };
	ButtonStatus status;
	QString pic_name;

	int m_btn_width = 0; //按钮宽度，0为未指定，尺寸设置为图片尺寸
	int m_btn_height = 0; //按钮高度，0为未指定，尺寸设置为图片尺寸
	bool mouse_press; //按钮左键是否按下
};
