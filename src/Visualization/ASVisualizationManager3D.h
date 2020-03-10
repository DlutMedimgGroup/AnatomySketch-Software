#pragma once
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageStack.h>

class ASQVTKWidget;

class ASVisualizationManager3D
{
public:
	ASVisualizationManager3D();
	~ASVisualizationManager3D();

	// 设定显示QVTKWidget指针
	void SetQVTKWidget(ASQVTKWidget* pQVTKWidget);
	// 更新ImageStack的变换矩阵
	void UpdateImageStack3DTransform();
	// 重置相机
	void ResetCamera();
	// 切换3D视图2D截面是否显示，返回切换后是否显示
	static bool Switch3DImageStackShown();

	// ImageStack
	static void GetImageStack3D(vtkSmartPointer<vtkImageStack>* ImageStack);
	// Renderer
	static void GetRenderer3D(vtkSmartPointer<vtkRenderer>* Renderer);

	// 渲染
	static void Render();
	 

private:

	bool m_ImageStack3DShown = true; // ImageStack3D是否显示

	// 管线元素
	ASQVTKWidget* m_QVTKWidget;	// 显示QVTKWidget指针
	vtkSmartPointer<vtkRenderer> m_Renderer3D;
	vtkSmartPointer<vtkImageStack> m_ImageStack3D[3];
	// 三维显示初始化
	void DisplayInit3D();

	// 唯一对象的指针
	static ASVisualizationManager3D* ms_SelfPointer;

};
