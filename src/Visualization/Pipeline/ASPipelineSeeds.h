#pragma once
#include "ASPipelineBase.h"
#include <vtkSmartPointer.h>
#include <vtkImageData.h>
#include <vtkImageActor.h>
#include <vtkImageStack.h>
#include <vtkMatrix4x4.h>

class ASPipelineSeeds : public ASPipelineBase
{
public:
	ASPipelineSeeds(const int c_ID);
	~ASPipelineSeeds();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);

	// 虚函数
	virtual void DeleteNode();	// 调用此函数来释放管线
	virtual void Update2DTransformMatrix();	// 切图焦点改变
	virtual void SetShown(const bool c_Shown); // 是否显示

	// 在绘制时，新增一个点进行显示
	void AddOnePointDisplay(double* pPositon);

private:
	// 该管线是唯一的，唯一对象
	static ASPipelineSeeds* ms_PipelineSeeds;

	// 是否显示
	bool m_isShown = false;

	// 管线元素
	vtkSmartPointer<vtkImageData>	m_ImageData[3];
	vtkSmartPointer<vtkImageActor>	m_ImageActor[3];
	vtkSmartPointer<vtkImageStack>	m_pImageStack2D[3];
	vtkSmartPointer<vtkImageStack>	m_pImageStack3D[3];

	// 显示基本参数
	double m_ImageExtent[6];
	double m_ImageSpacing[3];
	double m_ImagePosition[3];
	int m_ExtentofPixelInPainting[3];
	vtkSmartPointer<vtkMatrix4x4> m_pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> m_pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> m_pMatrixToOne[3];

	// 当前状态下每个截面是否有种子点
	bool m_bHaveObj[3] = { false, false, false };

	// 更新某视图的显示
	void UpdataDisplayPosition(const int c_ViewLabel);
	// 初始化某方向的vtkImageData
	void InitImageData(const int c_ViewLabel);
};
