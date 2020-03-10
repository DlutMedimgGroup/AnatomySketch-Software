#pragma once
#include "ASPipelineBase.h"
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>

class ASPipelineCrossLine2D : public ASPipelineBase
{
public:
	ASPipelineCrossLine2D(const int c_ID);
	~ASPipelineCrossLine2D();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);
	virtual void DeleteNode();	// 调用此函数来释放管线

	// 设置显示属性
	static void setShown(const bool c_Shown);
	// 刷新显示
	void Update();

	// 虚函数，切图焦点改变
	virtual void Update2DTransformMatrix();

private:

	// 该管线是唯一的，唯一对象
	static ASPipelineCrossLine2D* ms_SelfPointer;

	// 管线结构
	vtkSmartPointer<vtkPoints>					m_pCrossLinePoints[6];
	vtkSmartPointer<vtkCellArray>				m_pCrossLineCells[6];
	vtkSmartPointer<vtkActor>					m_pCrossLineActor[6];
	vtkSmartPointer<vtkPolyData>				m_pCrossLineData[6];
	vtkSmartPointer<vtkTransform>				m_pCrossLineTransform[6];
	vtkSmartPointer<vtkTransformPolyDataFilter>	m_pCrossLineTransformPolyDataFilter[6];
	vtkSmartPointer<vtkProperty>				m_pCrossLineProperty[3];
	vtkSmartPointer<vtkPolyDataMapper>			m_pCrosslineMapper[6];

	// 显示参数
	bool m_isShown = false;
	
};
