#pragma once

#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkPointPicker.h>
#include <vtkSmartPointer.h>
#include <vtkProp.h>

class ASArrayImageData;

#define PickRadius		0.01       // 拾取轮廓线时使用的拾取半径

class ASInteractorStyle2DGeneral : public vtkInteractorStyleTrackballCamera
{
public:
	static ASInteractorStyle2DGeneral *New();
	vtkTypeMacro(ASInteractorStyle2DGeneral, vtkInteractorStyleTrackballCamera);

	// Description:
	// Event bindings controlling the effects of pressing mouse buttons
	// or moving the mouse.
	virtual void OnMouseMove();
	virtual void OnLeftButtonDown();
	virtual void OnLeftButtonUp();
	virtual void OnMiddleButtonDown();
	virtual void OnMiddleButtonUp();
	virtual void OnRightButtonDown();
	virtual void OnRightButtonUp();
	virtual void OnMouseWheelForward();
	virtual void OnMouseWheelBackward();

	//设置视图
	virtual void setViewLabel(int ViewLabel);

	// 触控平移
	void TouchPan(double* pos);
	// 触控缩放
	void TouchZoom(double factor);
	// 触控切层
	void TouchWheel(double dis);

protected:
	int	m_ViewLabel;
	vtkSmartPointer<vtkPointPicker> m_picker;	

	bool m_haveActorDisplaying = false;		//当前是否有内容显示标志位
	bool m_isLeftButtonDown = false;		//左键按下标志位
	bool m_isMiddleButtonDown = false;		//滚轮按下标志位
	bool m_isRightButtonDown = false;		//右键按下标志位

	ASInteractorStyle2DGeneral();
	~ASInteractorStyle2DGeneral();

	//由vtkPointPicker得到的坐标的z总是0，这是因为vtkImageReslice对图像的变换，需要根据变换矩阵进行换算。
	void getPickPosFinal(double* posfinal, double* PointPickerOutput);

	// 悬停检测
	void HoverDetection(const int c_x, const int c_y);
	// 进行窗位窗宽的标志位
	bool m_IsWinlev = false;
	// 窗位窗宽
	void WinLevStart();
	void WinLevContinue();
	// 响应
	virtual void Move_Panning(const int c_x, const int c_y); // 鼠标移动平移
	virtual void Move_Zooming(const int c_x, const int c_y); // 鼠标移动缩放

private:
	//窗位窗宽调整
	double m_LevelWindowInitial[2];
	double m_LevelWindowStartPosition[2];
	double m_LevelWindowCurrentPosition[2];
	// 当前顶层体数据
	ASArrayImageData* m_crntTopData;

};
