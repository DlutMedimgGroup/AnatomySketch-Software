#include "ASPipelineCrossLine2D.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager2D.h"
#include <QDebug>

ASPipelineCrossLine2D* ASPipelineCrossLine2D::ms_SelfPointer = nullptr;

ASPipelineCrossLine2D::ASPipelineCrossLine2D(const int c_ID) : ASPipelineBase(c_ID)
{
	m_PipelineType = PipeLineType_CrossLine2D;

	ms_SelfPointer = this;

	// 管线初始化
	//vtkProperty
	m_pCrossLineProperty[0] = vtkSmartPointer<vtkProperty>::New();
	m_pCrossLineProperty[0]->SetColor(0, 0.0902, 0.7392);
	m_pCrossLineProperty[0]->SetLineWidth(1);
	m_pCrossLineProperty[1] = vtkSmartPointer<vtkProperty>::New();
	m_pCrossLineProperty[1]->SetColor(0.7196, 0.1294, 0);
	m_pCrossLineProperty[1]->SetLineWidth(1);
	m_pCrossLineProperty[2] = vtkSmartPointer<vtkProperty>::New();
	m_pCrossLineProperty[2]->SetColor(0, 0.7647, 0.0588);
	m_pCrossLineProperty[2]->SetLineWidth(1);
	//各对象实例并建立管线链接
	for (int nI = 0; nI < 6; nI++)
	{
		m_pCrossLinePoints[nI] = vtkSmartPointer<vtkPoints>::New();
		m_pCrossLineCells[nI] = vtkSmartPointer<vtkCellArray>::New();
		m_pCrossLineActor[nI] = vtkSmartPointer<vtkActor>::New();
		m_pCrossLineData[nI] = vtkSmartPointer<vtkPolyData>::New();
		m_pCrosslineMapper[nI] = vtkSmartPointer<vtkPolyDataMapper>::New();
		m_pCrossLineTransform[nI] = vtkSmartPointer<vtkTransform>::New();
		m_pCrossLineTransformPolyDataFilter[nI] = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
		m_pCrossLineTransformPolyDataFilter[nI]->SetInputData(m_pCrossLineData[nI]);
		m_pCrossLineTransformPolyDataFilter[nI]->SetTransform(m_pCrossLineTransform[nI]);
		m_pCrosslineMapper[nI]->SetInputData(m_pCrossLineTransformPolyDataFilter[nI]->GetOutput());
		m_pCrossLineActor[nI]->SetMapper(m_pCrosslineMapper[nI]);
		m_pCrossLineActor[nI]->SetPickable(false);
		m_pCrossLineData[nI]->Modified();
	}
	m_pCrossLineActor[0]->SetProperty(m_pCrossLineProperty[0]);
	m_pCrossLineActor[1]->SetProperty(m_pCrossLineProperty[1]);
	m_pCrossLineActor[2]->SetProperty(m_pCrossLineProperty[1]);
	m_pCrossLineActor[3]->SetProperty(m_pCrossLineProperty[2]);
	m_pCrossLineActor[4]->SetProperty(m_pCrossLineProperty[0]);
	m_pCrossLineActor[5]->SetProperty(m_pCrossLineProperty[2]);

	//建立顶点链接生成十字线，顶点坐标的初始值均为0
	vtkIdType          CurrentPoints[2];
	m_pCrossLinePoints[0]->InsertNextPoint(0, 0, 0);
	m_pCrossLinePoints[0]->InsertNextPoint(0, 0, 0);
	CurrentPoints[0] = m_pCrossLinePoints[0]->GetNumberOfPoints() - 2;
	CurrentPoints[1] = CurrentPoints[0] + 1;
	m_pCrossLineCells[0]->InsertNextCell(2, CurrentPoints);

	m_pCrossLinePoints[1]->InsertNextPoint(0, 0, 0);
	m_pCrossLinePoints[1]->InsertNextPoint(0, 0, 0);
	CurrentPoints[0] = m_pCrossLinePoints[1]->GetNumberOfPoints() - 2;
	CurrentPoints[1] = CurrentPoints[0] + 1;
	m_pCrossLineCells[1]->InsertNextCell(2, CurrentPoints);

	m_pCrossLinePoints[3]->InsertNextPoint(0, 0, 0);
	m_pCrossLinePoints[3]->InsertNextPoint(0, 0, 0);
	CurrentPoints[0] = m_pCrossLinePoints[3]->GetNumberOfPoints() - 2;
	CurrentPoints[1] = CurrentPoints[0] + 1;
	m_pCrossLineCells[3]->InsertNextCell(2, CurrentPoints);

	m_pCrossLinePoints[2]->InsertNextPoint(0, 0, 0);
	m_pCrossLinePoints[2]->InsertNextPoint(0, 0, 0);
	CurrentPoints[0] = m_pCrossLinePoints[2]->GetNumberOfPoints() - 2;
	CurrentPoints[1] = CurrentPoints[0] + 1;
	m_pCrossLineCells[2]->InsertNextCell(2, CurrentPoints);

	m_pCrossLinePoints[4]->InsertNextPoint(0, 0, 0);
	m_pCrossLinePoints[4]->InsertNextPoint(0, 0, 0);
	CurrentPoints[0] = m_pCrossLinePoints[4]->GetNumberOfPoints() - 2;
	CurrentPoints[1] = CurrentPoints[0] + 1;
	m_pCrossLineCells[4]->InsertNextCell(2, CurrentPoints);

	m_pCrossLinePoints[5]->InsertNextPoint(0, 0, 0);
	m_pCrossLinePoints[5]->InsertNextPoint(0, 0, 0);
	CurrentPoints[0] = m_pCrossLinePoints[5]->GetNumberOfPoints() - 2;
	CurrentPoints[1] = CurrentPoints[0] + 1;
	m_pCrossLineCells[5]->InsertNextCell(2, CurrentPoints);

	for (int nI = 0; nI < 6; nI++)
	{
		m_pCrossLineData[nI]->SetPoints(m_pCrossLinePoints[nI]);
		m_pCrossLineData[nI]->SetLines(m_pCrossLineCells[nI]);
	}
}

ASPipelineCrossLine2D::~ASPipelineCrossLine2D()
{
	
}

// 管线类型判断
bool ASPipelineCrossLine2D::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_CrossLine2D || ASPipelineBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPipelineCrossLine2D::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_CrossLine2D)
	{
		return true;
	}
	return false;
}
void ASPipelineCrossLine2D::DeleteNode()
{

}

// 设置显示属性
void ASPipelineCrossLine2D::setShown(const bool c_Shown)
{
	if (ms_SelfPointer->m_isShown)
	{
		qDebug("CrossLine setShown 0  true");
	} 
	else
	{
		qDebug("CrossLine setShown 0  false");
	}

	if (ms_SelfPointer->m_isShown == c_Shown)
	{
		qDebug("CrossLine setShown 1");
		return;
	}
	qDebug("CrossLine setShown 2");
	ms_SelfPointer->m_isShown = c_Shown;
	vtkSmartPointer<vtkRenderer> Renderer2D[3];
	ASVisualizationManager2D::GetRenderer2D(Renderer2D);
	if (ms_SelfPointer->m_isShown)
	{
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			Renderer2D[ViewLabel]->AddActor(ms_SelfPointer->m_pCrossLineActor[ViewLabel * 2]);
			Renderer2D[ViewLabel]->AddActor(ms_SelfPointer->m_pCrossLineActor[ViewLabel * 2 + 1]);
		}
	} 
	else
	{
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			Renderer2D[ViewLabel]->RemoveActor(ms_SelfPointer->m_pCrossLineActor[ViewLabel * 2]);
			Renderer2D[ViewLabel]->RemoveActor(ms_SelfPointer->m_pCrossLineActor[ViewLabel * 2 + 1]);
		}
	}
}

// 刷新显示
void ASPipelineCrossLine2D::Update()
{


}
// 虚函数，切图焦点改变
void ASPipelineCrossLine2D::Update2DTransformMatrix()
{
	if (m_isShown == false)
	{
		return;
	}

	double crntPosition[3];
	double crntExtent[6];
	ASVisualizationManager::GetDisplayPosition(crntPosition);
	ASVisualizationManager::GetDisplayExtent(crntExtent);

	//更新十字交叉线顶点坐标
	m_pCrossLinePoints[0]->SetPoint(0, crntPosition[0], crntPosition[1], crntExtent[4]);
	m_pCrossLinePoints[0]->SetPoint(1, crntPosition[0], crntPosition[1], crntExtent[5]);
	m_pCrossLinePoints[1]->SetPoint(0, crntPosition[0], crntExtent[2], crntPosition[2]);
	m_pCrossLinePoints[1]->SetPoint(1, crntPosition[0], crntExtent[3], crntPosition[2]);

	m_pCrossLinePoints[2]->SetPoint(0, crntExtent[0], crntPosition[1], crntPosition[2]);
	m_pCrossLinePoints[2]->SetPoint(1, crntExtent[1], crntPosition[1], crntPosition[2]);
	m_pCrossLinePoints[3]->SetPoint(0, crntPosition[0], crntPosition[1], crntExtent[4]);
	m_pCrossLinePoints[3]->SetPoint(1, crntPosition[0], crntPosition[1], crntExtent[5]);

	m_pCrossLinePoints[4]->SetPoint(0, crntExtent[0], crntPosition[1], crntPosition[2]);
	m_pCrossLinePoints[4]->SetPoint(1, crntExtent[1], crntPosition[1], crntPosition[2]);
	m_pCrossLinePoints[5]->SetPoint(0, crntPosition[0], crntExtent[2], crntPosition[2]);
	m_pCrossLinePoints[5]->SetPoint(1, crntPosition[0], crntExtent[3], crntPosition[2]);

	//相关对象update()
	vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
	ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);
	for (int nI = 0; nI < 6; nI++)
	{
		m_pCrossLineData[nI]->Modified();
		m_pCrossLineTransform[nI]->SetMatrix(pMatrixToOne[nI / 2]);
		m_pCrossLineTransformPolyDataFilter[nI]->Update();
		m_pCrosslineMapper[nI]->Modified();
		m_pCrosslineMapper[nI]->Update();
		m_pCrossLineActor[nI]->Modified();
	}
}