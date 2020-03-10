#pragma once
#include "ASPipelineBase.h"
#include <vtkSmartPointer.h>
#include <vtkPolyData.h>
#include <vtkPlane.h>
#include <vtkCleanPolyData.h>
#include <vtkCutter.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkPolyDataMapper.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <QColor>

#define LineWidth_Nomal		2
#define LineWidth_Bold		3

class ASPipelinePolyImage : public ASPipelineBase
{
public:
	ASPipelinePolyImage(const int c_ID);
	~ASPipelinePolyImage();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);

	// 设置输入数据
	void SetInputData(vtkPolyData* InputData);
	// 设置显示属性
	virtual void SetShown(const bool c_Shown); // 是否显示
	void SetColor(const QColor c_Color);
	void SetLineWidth(const double c_Width);
	void SetOpacity(const double c_Opacity); // 设置三维显示不透明度，0至1
	void SetContourLineWidth(const double c_Width = 2);
	void SetContourLinePickable(const bool c_Pickable);
	// Actor
	void GetActor2D(vtkActor** ppActor);
	// 输入曲面数据变化，刷新管线
	void UpdataInput();

	// 虚函数
	virtual void DeleteNode();	// 调用此函数来释放管线
	virtual void Update2DTransformMatrix(); // 切图焦点改变

private:

	// 管线结构
	// 2D
	vtkSmartPointer<vtkPlane>					m_pPlane2D[3];
	vtkSmartPointer<vtkCleanPolyData>			m_pCleanPolyData2D;
	vtkSmartPointer<vtkCutter>					m_pCutter2D[3];
	vtkSmartPointer<vtkTransform>				m_pTransform2D[3];
	vtkSmartPointer<vtkTransformPolyDataFilter>	m_pTransformPolyDataFilter2D[3];
	vtkSmartPointer<vtkPolyDataMapper>			m_pPolyDataMapper[3];
	vtkSmartPointer<vtkActor>					m_pActor2D[3];
	vtkSmartPointer<vtkRenderer>				m_pRenderer2D[3];
	// 3D
	vtkSmartPointer<vtkPolyDataMapper>			m_pPolyDataMapper3D;
	vtkSmartPointer<vtkActor>					m_pActor3D;
	vtkSmartPointer<vtkRenderer>				m_pRenderer3D;

	// 对应数据
	vtkSmartPointer<vtkPolyData> m_InputData;

	// 显示参数
	bool m_isShown = false;
	QColor m_Color;
	double m_LineWidth;
	double m_Opacity; // 三维显示不透明度
	double m_ContourLineWidth;
	bool m_ContourLinePickable;

	//三个默认正交界面的方向，用于计算轮廓线时设定截面方向
	double m_cPlaneNormal[3][3] = { { 1,0,0 },{ 0,1,0 },{ 0,0,1 } };

	
};
