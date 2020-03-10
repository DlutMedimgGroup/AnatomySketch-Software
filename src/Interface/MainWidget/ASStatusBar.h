#pragma once

#include <QStatusBar>
#include <QLabel>

#define STATUSBAR_HEIGHT 30         // 状态栏高度;

class ASStatusBar : public QStatusBar
{
	Q_OBJECT

public:
	ASStatusBar(QWidget *parent);
	~ASStatusBar();

private:

	QLabel* m_PermanentWidget;
	QLabel* m_Origin;

	public slots:

	//焦点改变更新状态点焦点信息显示
	void slotStatusBarFocusInformationRefresh(double* Pos, int* Index, double PixValue, int* extent, double* space, double* origin, double* windowlevel);
};
