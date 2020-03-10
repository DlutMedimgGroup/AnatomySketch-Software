#pragma once

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkSmartPointer.h>
#include <vtkCellPicker.h>

#define PickRadius		0.01       // 拾取轮廓线时使用的拾取半径

class ASInteractorStyle3DGeneral : public vtkInteractorStyleTrackballCamera
{
public:
	static ASInteractorStyle3DGeneral *New();
	vtkTypeMacro(ASInteractorStyle3DGeneral, vtkInteractorStyleTrackballCamera);

	// Description:
	// Event bindings controlling the effects of pressing mouse buttons
	// or moving the mouse.
	virtual void OnLeftButtonDown();

	// 触控平移
	void TouchPan(double* pos);
	// 触控缩放
	void TouchZoom(double factor);

protected:

	vtkSmartPointer<vtkCellPicker> m_picker;

	ASInteractorStyle3DGeneral();
	~ASInteractorStyle3DGeneral();

};
