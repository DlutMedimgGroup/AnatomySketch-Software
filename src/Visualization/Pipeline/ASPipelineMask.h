#pragma once
#include "ASPipelineBase.h"
#include <vtkSmartPointer.h>
#include <vtkImageReslice.h>
#include <vtkImageActor.h>
#include <vtkImageData.h>
#include <vtkImageStack.h>

class ASPipelineMask : public ASPipelineBase
{
public:
	ASPipelineMask(const int c_ID);
	~ASPipelineMask();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);

	// 设置显示属性
	virtual void SetShown(const bool c_Shown);
	// 设置遮罩数据
	void SetMaskImageData(vtkImageData* InputData);
	// 更新
	void Update();

	// 虚函数，切图焦点改变
	virtual void Update2DTransformMatrix();

private:

	// 该管线是唯一的，唯一对象
	static ASPipelineMask* ms_PipelineMask;
	// 显示参数
	bool m_isShown = false;
	// 对应遮罩数据
	vtkSmartPointer<vtkImageData> m_InputData;
	// 管线结构
	vtkSmartPointer<vtkImageReslice>	m_ImageReslice[3];
	vtkSmartPointer<vtkImageData>		m_ImageMapped[3];
	vtkSmartPointer<vtkImageActor>		m_ImageActor[3];
	vtkSmartPointer<vtkImageStack>		m_pImageStack2D[3];
	vtkSmartPointer<vtkImageStack>		m_pImageStack3D[3];

	// 更新m_ImageMapped
	void UpdateImageMapped();
};
