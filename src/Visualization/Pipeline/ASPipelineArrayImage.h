#pragma once
#include "ASPipelineBase.h"
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <vtkImageExtractComponents.h>
#include <vtkImageReslice.h>
#include <vtkImageMapToColors.h>
#include <vtkImageActor.h>
#include <vtkImageStack.h>

class ASArrayImageData;

class ASPipelineArrayImage : public ASPipelineBase
{
public:
	ASPipelineArrayImage(const int c_ID);
	~ASPipelineArrayImage();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);

	// 设置输入数据
	void SetInputData(vtkImageData* InputData);
	// 输入数据变化后刷新管线
	void UpdataInputData(const int c_Frame);
	// 设置显示属性	
	void SetFrame(const int c_Frame);
	void SetColorMapping(const int c_ColorMap, const double c_ColorWindow, const double c_ColorLevel);
	void SetLayer(const int c_Layer);
	void SetOpacity(const double c_Opacity); // 设置三维显示不透明度，0至1
	// 初始化管线
	void ArrayPipeInit(ASArrayImageData* arrayImage);

	// 虚函数
	virtual void DeleteNode();	// 调用此函数来释放管线
	virtual void Update2DTransformMatrix(); // 切图焦点改变
	virtual void SetShown(const bool c_Shown); // 是否显示
	virtual void ReinitDisplay(); // 重置显示

private:

	// 管线结构
	vtkSmartPointer<vtkImageExtractComponents>		m_pImageExtractComponents;
	vtkSmartPointer<vtkImageReslice>				m_pImageReslice[3];
	vtkSmartPointer<vtkImageMapToColors>			m_pImageMapToColors[3];
	vtkSmartPointer<vtkImageActor>					m_pImageActor[3];
	vtkSmartPointer<vtkImageStack>					m_pImageStack2D[3];
	vtkSmartPointer<vtkImageStack>					m_pImageStack3D[3];

	// 对应数据
	vtkSmartPointer<vtkImageData> m_InputData;

	// 显示参数
	bool m_isShown = false;
	int m_Frame = 0;
	int m_ColorMap;
	int m_Layer;
	double m_ColorWindow;
	double m_ColorLevel;
	double m_Opacity = 1; // 三维显示不透明度

	// 根据颜色表，窗位窗宽更新ImageMapToColors
	void UpdateImageMapToColors();
	
};
