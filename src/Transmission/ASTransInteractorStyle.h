#pragma once
#include "ASTransmissionBase.h"

class ASTransInteractorStyle : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransInteractorStyle(QObject *parent = 0);
	~ASTransInteractorStyle();

	// 唯一对象
	static ASTransInteractorStyle* GetSelfPointer();
	// 设置当前2D交互方式：InteractorStyle2D_
	static void SetInteratorStyle2D(const int c_Interactor);
	// 触控双指平移缩放切层
	static void TouchPan(const int c_ViewLabel, double* pos);
	static void TouchZoom(const int c_ViewLabel, const double c_Factor);
	static void TouchWheel(const int c_ViewLabel, const double c_dis);

private:

	// 唯一对象
	static ASTransInteractorStyle* ms_GeneralInteractor;

signals:

	// 当前2D交互方式改变
	void signalInteractorStyle2DChanged(int interactor);
};
