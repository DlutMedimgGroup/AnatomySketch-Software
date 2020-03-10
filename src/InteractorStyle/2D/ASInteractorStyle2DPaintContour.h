#pragma once

#include "ASInteractorStyle2DGeneral.h"
#include "ASArrayImageData.h"

#define TouchThresholdCoeff 1.5

class ASInteractorStyle2DPaintContour : public ASInteractorStyle2DGeneral
{
public:
	static ASInteractorStyle2DPaintContour *New();
	vtkTypeMacro(ASInteractorStyle2DPaintContour, ASInteractorStyle2DGeneral);

	// 重写左键拖动部分
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnKeyPress();
	virtual void OnKeyRelease();

private:
	ASInteractorStyle2DPaintContour();
	~ASInteractorStyle2DPaintContour();

	// -1: 正常切换焦点; 0: 无动作; 1: 添加控制点时的移动; 2: 平移控制点时的移动; 
	// 3: 插入控制点时移动; 4: 添加控制点时移动到闭合; 5: 触控模式移动
	int m_MoveState = 0; 
	// 触控模式中的上一个控制点坐标
	double m_TouchLastCtr[3];

	// 按键状态状态
	bool m_isKey_a_down = false;
	bool m_isKey_d_down = false;
	bool m_isKey_q_down = false;
	bool m_isKey_Ctrl_down = false;

};
