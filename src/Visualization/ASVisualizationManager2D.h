#pragma once
#include <vtkSmartPointer.h>
#include <vtkRenderer.h>
#include <vtkImageStack.h>
#include <vtkMatrix4x4.h>
#include <QString>

class ASQVTKWidget;

/*
全局得到变换矩阵
vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);
*/

class ASVisualizationManager2D
{
public:
	ASVisualizationManager2D();
	~ASVisualizationManager2D();

	// 设定显示QVTKWidget指针
	void SetQVTKWidget(ASQVTKWidget** pQVTKWidget);
	// 显示参数设定
	void setImagePosition(double* ImagePosition);
	// Matrix4x4
	static void GetTransformMatrix4x4(vtkSmartPointer<vtkMatrix4x4>* Matrix4x4, 
		vtkSmartPointer<vtkMatrix4x4>* Matrix4x4InvertToZero, 
		vtkSmartPointer<vtkMatrix4x4>* Matrix4x4InvertToOne);
	// ImageStack
	static void GetImageStack2D(vtkSmartPointer<vtkImageStack>* ImageStack);
	// Renderer
	static void GetRenderer2D(vtkSmartPointer<vtkRenderer>* Renderer);
	// 计算横向视野范围
	static double GetViewableRange(const int c_ViewLabel);
	// 轮廓线是否可以拾取
	static void SetContoursPickable(const bool c_pickable);
	static bool GetContoursPickable();

	// 根据当前焦点刷新变换矩阵
	void UpdateTransformMatrix();
	// 重置相机
	void ResetCamera();

	// 渲染
	static void Render();
	static void Render(const int c_ViewLabel);

private:

	// 管线元素
	ASQVTKWidget* m_QVTKWidget[3];	// 显示QVTKWidget指针
	vtkSmartPointer<vtkRenderer> m_Renderer2D[3];
	vtkSmartPointer<vtkImageStack> m_ImageStack2D[3];

	// 唯一对象的指针
	static ASVisualizationManager2D* ms_SelfPointer;

	double* m_ImagePosition;			// 焦点，世界坐标系，ASVisualizationManager中的引用

	//构造横截面、矢状面、冠状面变换矩阵是旋转参数。
	const double m_constDirectionCosines[3][9] = { { 0, 1, 0, 0, 0, 1, 1, 0, 0 },{ 1, 0, 0, 0, 0, 1, 0, -1, 0 },{ 1, 0, 0, 0, -1, 0, 0, 0, -1 } };
	//变换矩阵
	vtkSmartPointer<vtkMatrix4x4> m_pMatrix4x4[3];				//由显示坐标系到世界坐标系的变换矩阵
	vtkSmartPointer<vtkMatrix4x4> m_pMatrix4x4InvertToZero[3];	//由世界坐标系到显示坐标系的变换矩阵，结果的Z坐标为0
	vtkSmartPointer<vtkMatrix4x4> m_pMatrix4x4InvertToOne[3];	//由世界坐标系到显示坐标系的变换矩阵，结果的Z坐标为1

	// 轮廓线是否可以拾取
	bool m_ContoursPickable = false;

	// 二维显示初始化
	void DisplayInit2D();
};
