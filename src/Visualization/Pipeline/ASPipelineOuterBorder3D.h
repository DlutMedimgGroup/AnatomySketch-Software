#pragma once
#include "ASPipelineBase.h"
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <vtkRenderer.h>

class ASPipelineOuterBorder3D : public ASPipelineBase
{
public:
	ASPipelineOuterBorder3D(const int c_ID);
	~ASPipelineOuterBorder3D();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);

	// 设置显示属性
	static void setShown(const bool c_Shown);
	static void setCrossShairShown(const bool c_Shown);
	// 刷新显示
	void Update();

	// 虚函数，切图焦点改变
	virtual void Update2DTransformMatrix();

private:

	// 该管线是唯一的，唯一对象
	static ASPipelineOuterBorder3D* ms_SelfPointer;

	// 管线结构
	vtkSmartPointer<vtkPoints>			m_pFrameLinePoints[3];
	vtkSmartPointer<vtkCellArray>		m_pFrameLineCells[3];
	vtkSmartPointer<vtkActor>			m_pFrameLineActor[3];
	vtkSmartPointer<vtkPolyData>		m_pFrameLineData[3];
	vtkSmartPointer<vtkProperty>		m_pFrameLineProperty[3];
	vtkSmartPointer<vtkPolyDataMapper>	m_pFramelineMapper[3];
	vtkSmartPointer<vtkRenderer>		m_pRenderer3D;


	// 显示参数
	bool m_isShown = false;
	bool m_isCrossShairShown = true;
	
};
