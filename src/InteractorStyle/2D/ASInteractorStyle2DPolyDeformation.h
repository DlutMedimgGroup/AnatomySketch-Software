#pragma once

#include "ASInteractorStyle2DGeneral.h"
#include "ASArrayImageData.h"

#define State_Nomal		0
#define State_None		1
#define State_Panning	2
#define State_FFD		3

class ASInteractorStyle2DPolyDeformation : public ASInteractorStyle2DGeneral
{
public:
	static ASInteractorStyle2DPolyDeformation *New();
	vtkTypeMacro(ASInteractorStyle2DPolyDeformation, ASInteractorStyle2DGeneral);

	// 重写左键拖动部分
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();
	virtual void OnKeyPress();
	virtual void OnKeyRelease();

private:
	ASInteractorStyle2DPolyDeformation();
	~ASInteractorStyle2DPolyDeformation();

	// 本次交互状态
	int m_crntMoveState = State_Nomal;

	// 按键状态状态
	bool m_isKey_z_down = false;
	bool m_isKey_x_down = false;

};
