#include "ASVisualizationManager2D.h"
#include "ASQVTKWidget.h"
#include "GlobelDeclaration.h"
#include "ASPipelineManager.h"
#include "ASPipelineBase.h"
#include "ASVisualizationManager.h"
#include "ASPipelinePolyImage.h"
#include "ASTransGeneralInteractor.h"
#include <vtkCamera.h>
#include <vtkRenderWindow.h>
#include <vtkPropCollection.h>
#include <QTextCodec.h>

ASVisualizationManager2D* ASVisualizationManager2D::ms_SelfPointer = nullptr;

ASVisualizationManager2D::ASVisualizationManager2D()
{
	ms_SelfPointer = this;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pMatrix4x4[ViewLabel] = vtkSmartPointer<vtkMatrix4x4>::New();
		m_pMatrix4x4InvertToZero[ViewLabel] = vtkSmartPointer<vtkMatrix4x4>::New();
		m_pMatrix4x4InvertToOne[ViewLabel] = vtkSmartPointer<vtkMatrix4x4>::New();
	}
	
	ASPipelineManager::AddPipeline(PipeLineType_CrossLine2D); // 添加十字线管线
	ASPipelineManager::AddPipeline(PipeLineType_Text); // 添加文字显示管线
}

ASVisualizationManager2D::~ASVisualizationManager2D()
{

}


// 设定显示QVTKWidget指针
void ASVisualizationManager2D::SetQVTKWidget(ASQVTKWidget** pQVTKWidget)
{
	m_QVTKWidget[0] = pQVTKWidget[0];
	m_QVTKWidget[1] = pQVTKWidget[1];
	m_QVTKWidget[2] = pQVTKWidget[2];

	this->DisplayInit2D();
}
// 显示参数设定
void ASVisualizationManager2D::setImagePosition(double* ImagePosition)
{
	this->m_ImagePosition = ImagePosition;
}
// Matrix4x4
void ASVisualizationManager2D::GetTransformMatrix4x4(vtkSmartPointer<vtkMatrix4x4>* Matrix4x4, 
	vtkSmartPointer<vtkMatrix4x4>* Matrix4x4InvertToZero, 
	vtkSmartPointer<vtkMatrix4x4>* Matrix4x4InvertToOne)
{
	Matrix4x4[0] = ms_SelfPointer->m_pMatrix4x4[0];
	Matrix4x4[1] = ms_SelfPointer->m_pMatrix4x4[1];
	Matrix4x4[2] = ms_SelfPointer->m_pMatrix4x4[2];
	Matrix4x4InvertToZero[0] = ms_SelfPointer->m_pMatrix4x4InvertToZero[0];
	Matrix4x4InvertToZero[1] = ms_SelfPointer->m_pMatrix4x4InvertToZero[1];
	Matrix4x4InvertToZero[2] = ms_SelfPointer->m_pMatrix4x4InvertToZero[2];
	Matrix4x4InvertToOne[0] = ms_SelfPointer->m_pMatrix4x4InvertToOne[0];
	Matrix4x4InvertToOne[1] = ms_SelfPointer->m_pMatrix4x4InvertToOne[1];
	Matrix4x4InvertToOne[2] = ms_SelfPointer->m_pMatrix4x4InvertToOne[2];
}
// ImageStack
void ASVisualizationManager2D::GetImageStack2D(vtkSmartPointer<vtkImageStack>* ImageStack)
{
	ImageStack[0] = ms_SelfPointer->m_ImageStack2D[0];
	ImageStack[1] = ms_SelfPointer->m_ImageStack2D[1];
	ImageStack[2] = ms_SelfPointer->m_ImageStack2D[2];
}
// Renderer
void ASVisualizationManager2D::GetRenderer2D(vtkSmartPointer<vtkRenderer>* Renderer)
{
	Renderer[0] = ms_SelfPointer->m_Renderer2D[0];
	Renderer[1] = ms_SelfPointer->m_Renderer2D[1];
	Renderer[2] = ms_SelfPointer->m_Renderer2D[2];
}
// 计算横向视野范围
double ASVisualizationManager2D::GetViewableRange(const int c_ViewLabel)
{
	//获得render当前的横向的视野范围
	double vp[4];
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->GetViewport(vp);
	double wp1[3] = { vp[0], vp[1], 0.0 };
	double wp2[3] = { vp[2], vp[3], 0.0 };
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->NormalizedDisplayToViewport(wp1[0], wp1[1]);
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->ViewportToNormalizedViewport(wp1[0], wp1[1]);
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->NormalizedViewportToView(wp1[0], wp1[1], wp1[2]);
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->ViewToWorld(wp1[0], wp1[1], wp1[2]);
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->NormalizedDisplayToViewport(wp2[0], wp2[1]);
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->ViewportToNormalizedViewport(wp2[0], wp2[1]);
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->NormalizedViewportToView(wp2[0], wp2[1], wp2[2]);
	ms_SelfPointer->m_Renderer2D[c_ViewLabel]->ViewToWorld(wp2[0], wp2[1], wp2[2]);

	return wp2[0] - wp1[0];
}
// 轮廓线是否可以拾取
void ASVisualizationManager2D::SetContoursPickable(const bool c_pickable)
{
	if (ms_SelfPointer->m_ContoursPickable == c_pickable)
	{
		return;
	}
	ms_SelfPointer->m_ContoursPickable = c_pickable;
	// 遍历轮廓管线设置是否可以拾取
	for (ASPipelineBase* item = ASPipelineManager::bigen_inherit(PipeLineType_PolyImage); item != nullptr; item = ASPipelineManager::next_inherit(PipeLineType_PolyImage))
	{
		ASPipelinePolyImage* crntPipeline = (ASPipelinePolyImage*)item;
		crntPipeline->SetContourLinePickable(ms_SelfPointer->m_ContoursPickable);
	}
}
bool ASVisualizationManager2D::GetContoursPickable()
{
	return ms_SelfPointer->m_ContoursPickable;
}

// 渲染
void ASVisualizationManager2D::Render()
{
	ms_SelfPointer->m_QVTKWidget[ViewLabel_Sagittal]->GetRenderWindow()->Render();
	ms_SelfPointer->m_QVTKWidget[ViewLabel_Coronal]->GetRenderWindow()->Render();
	ms_SelfPointer->m_QVTKWidget[ViewLabel_Axial]->GetRenderWindow()->Render();
}
void ASVisualizationManager2D::Render(const int c_ViewLabel)
{
	ms_SelfPointer->m_QVTKWidget[c_ViewLabel]->GetRenderWindow()->Render();
}

// 二维显示初始化
void ASVisualizationManager2D::DisplayInit2D()
{
	for (int crntViewLabel = 0; crntViewLabel < 3; crntViewLabel++)
	{
		// vtkImageStack
		m_ImageStack2D[crntViewLabel] = vtkSmartPointer<vtkImageStack>::New();
		// vtkRenderer
		m_Renderer2D[crntViewLabel] = vtkSmartPointer<vtkRenderer>::New();
		m_Renderer2D[crntViewLabel]->AddActor(m_ImageStack2D[crntViewLabel]);
		m_Renderer2D[crntViewLabel]->GetActiveCamera()->ParallelProjectionOn(); // 平行投影
		m_QVTKWidget[crntViewLabel]->GetRenderWindow()->AddRenderer(m_Renderer2D[crntViewLabel]);
	}
}

// 根据当前焦点刷新变换矩阵
void ASVisualizationManager2D::UpdateTransformMatrix()
{
	//计算三个正交截面的变换矩阵
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		double element[16];
		for (int i = 0; i < 3; i++)
		{
			for (int j = 0; j < 3; j++)
			{
				element[i * 4 + j] = m_constDirectionCosines[ViewLabel][j * 3 + i];
			}
			element[i + 12] = 0;
		}
		switch (ViewLabel)
		{
		case ViewLabel_Sagittal:
			element[3] = m_ImagePosition[0];
			element[7] = 0;
			element[11] = 0;
			break;
		case ViewLabel_Coronal:
			element[3] = 0;
			element[7] = m_ImagePosition[1];
			element[11] = 0;
			break;
		case ViewLabel_Axial:
			element[3] = 0;
			element[7] = 0;
			element[11] = m_ImagePosition[2];
			break;
		default:
			break;
		}
		element[15] = 1;
		m_pMatrix4x4[ViewLabel]->DeepCopy(element);
		//求逆矩阵Matrix4x4InvertToZero和Matrix4x4InvertToOne
		vtkSmartPointer<vtkMatrix4x4> pInverter = vtkSmartPointer<vtkMatrix4x4>::New();
		pInverter->Invert(m_pMatrix4x4[ViewLabel], m_pMatrix4x4InvertToZero[ViewLabel]);
		m_pMatrix4x4InvertToOne[ViewLabel]->DeepCopy(m_pMatrix4x4InvertToZero[ViewLabel]);
		m_pMatrix4x4InvertToOne[ViewLabel]->SetElement(2, 3, m_pMatrix4x4InvertToZero[ViewLabel]->GetElement(2, 3) + 10);
	}
}
// 重置相机
void ASVisualizationManager2D::ResetCamera()
{
	// 二维视图范围，需要根据变换矩阵和世界坐标系范围来计算
	vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
	ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);
	double xRange[3][2];
	double yRange[3][2];
	double WorldExtent[6];
	ASVisualizationManager::GetDisplayExtent(WorldExtent);
	double WorldVertex[8][4]; // 世界坐标系八个顶点
	int i = 0;
	for (int xx = 0; xx < 2; xx++)
		for (int yy = 0; yy < 2; yy++)
			for (int zz = 0; zz < 2; zz++)
			{
				WorldVertex[i][0] = WorldExtent[xx];
				WorldVertex[i][1] = WorldExtent[2+yy];
				WorldVertex[i][2] = WorldExtent[4+zz];
				WorldVertex[i][3] = 1;
				i++;
			}

	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		double crntPoint[4];
		pMatrixToZero[ViewLabel]->MultiplyPoint(WorldVertex[0], crntPoint);
		xRange[ViewLabel][0] = crntPoint[0];
		xRange[ViewLabel][1] = crntPoint[0];
		yRange[ViewLabel][0] = crntPoint[1];
		yRange[ViewLabel][1] = crntPoint[1];
		// 剩余7点
		for (int k = 1; k < 8; k++)
		{
			pMatrixToZero[ViewLabel]->MultiplyPoint(WorldVertex[k], crntPoint);
			if (crntPoint[0] < xRange[ViewLabel][0])
			{
				xRange[ViewLabel][0] = crntPoint[0];
			}
			if (crntPoint[0] > xRange[ViewLabel][1])
			{
				xRange[ViewLabel][1] = crntPoint[0];
			}
			if (crntPoint[1] < yRange[ViewLabel][0])
			{
				yRange[ViewLabel][0] = crntPoint[1];
			}
			if (crntPoint[1] > yRange[ViewLabel][1])
			{
				yRange[ViewLabel][1] = crntPoint[1];
			}
		}
	}

	// 计算缩放
	double ScalingFactor = -1;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		if (m_Renderer2D[ViewLabel]->GetViewProps()->GetNumberOfItems() <= 1)
		{
			continue;
		}
		m_Renderer2D[ViewLabel]->ResetCamera();
		
		int* crntViewPixelSize = m_QVTKWidget[ViewLabel]->GetRenderWindow()->GetSize();

		double wp1[3] = { crntViewPixelSize[0], crntViewPixelSize[1], 0.0 };
		m_Renderer2D[ViewLabel]->DisplayToNormalizedDisplay(wp1[0], wp1[1]);
		m_Renderer2D[ViewLabel]->NormalizedDisplayToViewport(wp1[0], wp1[1]);
		m_Renderer2D[ViewLabel]->ViewportToNormalizedViewport(wp1[0], wp1[1]);
		m_Renderer2D[ViewLabel]->NormalizedViewportToView(wp1[0], wp1[1], wp1[2]);
		m_Renderer2D[ViewLabel]->ViewToWorld(wp1[0], wp1[1], wp1[2]);
		double wp2[3] = { 0.0, 0.0, 0.0 };
		m_Renderer2D[ViewLabel]->DisplayToNormalizedDisplay(wp2[0], wp2[1]);
		m_Renderer2D[ViewLabel]->NormalizedDisplayToViewport(wp2[0], wp2[1]);
		m_Renderer2D[ViewLabel]->ViewportToNormalizedViewport(wp2[0], wp2[1]);
		m_Renderer2D[ViewLabel]->NormalizedViewportToView(wp2[0], wp2[1], wp2[2]);
		m_Renderer2D[ViewLabel]->ViewToWorld(wp2[0], wp2[1], wp2[2]);

		double WorldRange[2];
		WorldRange[0] = wp1[0] - wp2[0];
		WorldRange[1] = wp1[1] - wp2[1];

		double crntScalingFactor;
		crntScalingFactor = WorldRange[0] / (xRange[ViewLabel][1] - xRange[ViewLabel][0]);
		if (ScalingFactor < 0)
		{
			ScalingFactor = crntScalingFactor;
		}
		else if (crntScalingFactor < ScalingFactor)
		{
			ScalingFactor = crntScalingFactor;
		}
		crntScalingFactor = WorldRange[1] / (yRange[ViewLabel][1] - yRange[ViewLabel][0]);
		if (crntScalingFactor < ScalingFactor)
		{
			ScalingFactor = crntScalingFactor;
		}
	}
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		if (m_Renderer2D[ViewLabel]->GetViewProps()->GetNumberOfItems() <= 1)
		{
			continue;
		}
		double* xRange1 = m_ImageStack2D[ViewLabel]->GetXRange();
		double* yRange1 = m_ImageStack2D[ViewLabel]->GetYRange();
		double a = m_Renderer2D[ViewLabel]->GetActiveCamera()->GetParallelScale();
		double d = m_Renderer2D[ViewLabel]->GetActiveCamera()->GetDistance();
		m_Renderer2D[ViewLabel]->GetActiveCamera()->SetParallelScale(a / ScalingFactor);
		m_Renderer2D[ViewLabel]->GetActiveCamera()->SetFocalPoint((xRange[ViewLabel][0] + xRange[ViewLabel][1])*0.5, (yRange[ViewLabel][0] + yRange[ViewLabel][1])*0.5, 0.0);
		m_Renderer2D[ViewLabel]->GetActiveCamera()->SetPosition((xRange[ViewLabel][0] + xRange[ViewLabel][1])*0.5, (yRange[ViewLabel][0] + yRange[ViewLabel][1])*0.5, d);
		// 重置缩放后可能需要计算控制点的直径
		ASTransGeneralInteractor::GenerateViewZoom(ViewLabel);
	}	
}
