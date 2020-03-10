#include "ASPipelinePolyImage.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager2D.h"
#include "ASVisualizationManager3D.h"
#include <vtkProperty.h>

ASPipelinePolyImage::ASPipelinePolyImage(const int c_ID) : ASPipelineBase(c_ID)
{
	m_PipelineType = PipeLineType_PolyImage;
	ms_NumOfObj2D++; // 曲面数据具有二维显示部分

	// 管线初始化
	// 2D
	//CleanPolyData
	m_pCleanPolyData2D = vtkSmartPointer<vtkCleanPolyData>::New();
	//pContourLineCleanPolyData->SetInputData(crntPolyImageData->getPolyImageData());
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		//Plane
		m_pPlane2D[ViewLabel] = vtkSmartPointer<vtkPlane>::New();
		m_pPlane2D[ViewLabel]->SetNormal(m_cPlaneNormal[ViewLabel]);				//设置截面方向
		//Cutter
		m_pCutter2D[ViewLabel] = vtkSmartPointer<vtkCutter>::New();
		m_pCutter2D[ViewLabel]->SetCutFunction(m_pPlane2D[ViewLabel]);
		m_pCutter2D[ViewLabel]->SetInputConnection(m_pCleanPolyData2D->GetOutputPort());
		//Transform
		m_pTransform2D[ViewLabel] = vtkSmartPointer<vtkTransform>::New();
		//TransformPolyDataFilter
		m_pTransformPolyDataFilter2D[ViewLabel] = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		m_pTransformPolyDataFilter2D[ViewLabel]->SetInputData(m_pCutter2D[ViewLabel]->GetOutput());
		m_pTransformPolyDataFilter2D[ViewLabel]->SetTransform(m_pTransform2D[ViewLabel]);
		//PolyDataMapper
		m_pPolyDataMapper[ViewLabel] = vtkSmartPointer<vtkPolyDataMapper>::New();
		m_pPolyDataMapper[ViewLabel]->ScalarVisibilityOff();
		m_pPolyDataMapper[ViewLabel]->SetInputData(m_pTransformPolyDataFilter2D[ViewLabel]->GetOutput());
		//Actor
		m_pActor2D[ViewLabel] = vtkSmartPointer<vtkActor>::New();
		m_pActor2D[ViewLabel]->SetMapper(m_pPolyDataMapper[ViewLabel]);
	}
	// 3D
	m_pPolyDataMapper3D = vtkSmartPointer<vtkPolyDataMapper>::New();
	m_pPolyDataMapper3D->SetInputConnection(m_pCleanPolyData2D->GetOutputPort());
	m_pActor3D = vtkSmartPointer<vtkActor>::New();
	m_pActor3D->SetPickable(true);
	m_pActor3D->SetMapper(m_pPolyDataMapper3D);
	// Renderer
	ASVisualizationManager2D::GetRenderer2D(m_pRenderer2D);
	ASVisualizationManager3D::GetRenderer3D(&m_pRenderer3D);
}

ASPipelinePolyImage::~ASPipelinePolyImage()
{
	ms_NumOfObj2D--;
	if (m_isShown)
	{
		ms_NumOfShowingObj2D--;
		if (ms_NumOfShowingObj2D < 0)
		{
			ms_NumOfShowingObj2D = 0;
		}
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pRenderer2D[ViewLabel]->RemoveActor(m_pActor2D[ViewLabel]);
		}
		m_pRenderer3D->RemoveActor(m_pActor3D);
	}

	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pActor2D[ViewLabel] = nullptr;
		m_pPolyDataMapper[ViewLabel] = nullptr;
		m_pTransformPolyDataFilter2D[ViewLabel] = nullptr;
		m_pTransform2D[ViewLabel] = nullptr;
		m_pCutter2D[ViewLabel] = nullptr;
		m_pPlane2D[ViewLabel] = nullptr;
	}
	m_pCleanPolyData2D = nullptr;
	m_pActor3D = nullptr;
	m_pPolyDataMapper3D = nullptr;
}

// 管线类型判断
bool ASPipelinePolyImage::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_PolyImage || ASPipelineBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPipelinePolyImage::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_PolyImage)
	{
		return true;
	}
	return false;
}

// 设置输入数据
void ASPipelinePolyImage::SetInputData(vtkPolyData* InputData)
{
	this->m_InputData = InputData;
	m_pCleanPolyData2D->SetInputData(m_InputData);
	//m_pPolyDataMapper3D->SetInputData(m_InputData);
}

// 设置显示属性
void ASPipelinePolyImage::SetShown(const bool c_Shown)
{
	if (m_isShown == c_Shown)
	{
		return;
	}
	m_isShown = c_Shown;
	if (m_isShown)
	{
		ms_NumOfShowingObj2D++;
		this->Update2DTransformMatrix();
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pRenderer2D[ViewLabel]->AddActor(m_pActor2D[ViewLabel]);
		}
		m_pRenderer3D->AddActor(m_pActor3D);
	}
	else
	{
		ms_NumOfShowingObj2D--;
		if (ms_NumOfShowingObj2D < 0)
		{
			ms_NumOfShowingObj2D = 0;
		}
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pRenderer2D[ViewLabel]->RemoveActor(m_pActor2D[ViewLabel]);
		}
		m_pRenderer3D->RemoveActor(m_pActor3D);
	}
}
void ASPipelinePolyImage::SetColor(const QColor c_Color)
{
	m_Color = c_Color;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pActor2D[ViewLabel]->GetProperty()->SetColor(((double)m_Color.red()) / 255, ((double)m_Color.green()) / 255, ((double)m_Color.blue()) / 255);
	}
	m_pActor3D->GetProperty()->SetColor(((double)m_Color.red()) / 255, ((double)m_Color.green()) / 255, ((double)m_Color.blue()) / 255);
}
void ASPipelinePolyImage::SetLineWidth(const double c_Width)
{
	m_LineWidth = c_Width;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pActor2D[ViewLabel]->GetProperty()->SetLineWidth(c_Width);
	}
}
void ASPipelinePolyImage::SetOpacity(const double c_Opacity)
{
	m_Opacity = c_Opacity;
	m_pActor3D->GetProperty()->SetOpacity(m_Opacity);
}
void ASPipelinePolyImage::SetContourLineWidth(const double c_Width)
{
	m_ContourLineWidth = c_Width;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pActor2D[ViewLabel]->GetProperty()->SetLineWidth(m_ContourLineWidth);
	}
}
void ASPipelinePolyImage::SetContourLinePickable(const bool c_Pickable)
{
	m_ContourLinePickable = c_Pickable;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pActor2D[ViewLabel]->SetPickable(m_ContourLinePickable);
	}
}
// Actor
void ASPipelinePolyImage::GetActor2D(vtkActor** ppActor)
{
	ppActor[0] = m_pActor2D[0];
	ppActor[1] = m_pActor2D[1];
	ppActor[2] = m_pActor2D[2];
}
// 输入曲面数据变化，刷新管线
void ASPipelinePolyImage::UpdataInput()
{
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pCutter2D[ViewLabel]->Update();
		m_pTransformPolyDataFilter2D[ViewLabel]->Update();
	}
}

// 虚函数
// 调用此函数来释放管线
void ASPipelinePolyImage::DeleteNode()
{
	delete this;
}
// 切图焦点改变
void ASPipelinePolyImage::Update2DTransformMatrix()
{
	if (m_isShown == false)
	{
		return;
	}
	vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
	ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);
	double position[3];
	ASVisualizationManager::GetDisplayPosition(position);
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pPlane2D[ViewLabel]->SetOrigin(position);
		m_pTransform2D[ViewLabel]->SetMatrix(pMatrixToOne[ViewLabel]);
		m_pPlane2D[ViewLabel]->Modified();
		m_pCutter2D[ViewLabel]->Update();
		m_pTransformPolyDataFilter2D[ViewLabel]->Update();
	}
	
}
