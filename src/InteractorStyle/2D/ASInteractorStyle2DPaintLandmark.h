#pragma once

#include "ASInteractorStyle2DGeneral.h"
#include "ASArrayImageData.h"

class ASInteractorStyle2DPaintLandmark : public ASInteractorStyle2DGeneral
{
public:
	static ASInteractorStyle2DPaintLandmark *New();
	vtkTypeMacro(ASInteractorStyle2DPaintLandmark, ASInteractorStyle2DGeneral);

	// 重写左键拖动部分
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	//virtual void OnLeftButtonUp();
	//virtual void OnMouseWheelForward();
	//virtual void OnMouseWheelBackward();
	//virtual void OnKeyPress();
	//virtual void OnKeyRelease();

private:
	ASInteractorStyle2DPaintLandmark();
	~ASInteractorStyle2DPaintLandmark();

	int m_MoveState = 0; // -1: 正常切换焦点; 0: 无动作; 1: 平移控制点时的移动;

};
