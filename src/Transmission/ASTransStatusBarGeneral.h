#pragma once
#include "ASTransmissionBase.h"
#include <QKeyEvent>

class ASTransStatusBarGeneral : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransStatusBarGeneral(QObject *parent = 0);
	~ASTransStatusBarGeneral();

	// 唯一对象
	static ASTransStatusBarGeneral* GetSelfPointer();

	// 设置状态栏右侧焦点信息
	static void RefreshStatusBarFocusInfo();

private:
	// 唯一对象
	static ASTransStatusBarGeneral* ms_TransStatusBarGeneral;

signals:
	// 设置状态栏焦点信息
	void signalStatusBarFocusInformationRefresh(double* Pos, int* Index, double PixValue, int* extent, double* space, double* origin, double* windowlevel);
};
