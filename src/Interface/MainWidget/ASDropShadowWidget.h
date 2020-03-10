/*
	设置阴影边框
	主窗口的父类
*/

#pragma once

//#include "common.h"
#include <QDialog>
#include <QWidget>
#include <QMouseEvent>

enum Direction { UP = 0, DOWN = 1, LEFT, RIGHT, LEFTTOP, LEFTBOTTOM, RIGHTBOTTOM, RIGHTTOP, NONE };
#define SHADOW_WIDTH	5		//主窗口边框阴影宽度
#define PADDING			5		//边界缩放区域的宽度,在界面边界的外侧，目前程序的写法PADDING与SHADOW_WIDTH必须相等，否则需要修改ASDropShadowWidget.cpp的getRregion()

class ASDropShadowWidget : public QDialog
{
	Q_OBJECT

public:
	explicit ASDropShadowWidget(QWidget *parent);
	~ASDropShadowWidget();

	void setBorderDragEnabled(bool enabel);

protected:

	virtual void paintEvent(QPaintEvent *event);

	void mouseReleaseEvent(QMouseEvent *event);
	void mouseMoveEvent(QMouseEvent *event);
	void mousePressEvent(QMouseEvent *event);

private:

	//由鼠标屏幕坐标计算其所在的区域，用于拖动边界功能
	void getRregion(const QPoint &cursorGlobalPoint);

	Direction m_currentDir;
	QPoint m_dragPosition;   // 窗口移动拖动时需要记住的点
	bool m_isLeftPressDown;  // 判断左键是否按下
	bool m_BorderDragEnabled;// 标志边界拖拽功能是否开启

public slots:

	// 换背景的槽
	void slotBackgroundChange(bool checked);
};
