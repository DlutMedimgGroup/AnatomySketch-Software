#include "ASPipelineOuterBorder3D.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager3D.h"

ASPipelineOuterBorder3D* ASPipelineOuterBorder3D::ms_SelfPointer = nullptr;

ASPipelineOuterBorder3D::ASPipelineOuterBorder3D(const int c_ID) : ASPipelineBase(c_ID)
{
	m_PipelineType = PipeLineType_OuterBorder3D;

	ms_SelfPointer = this;

	// 管线初始化
	//vtkProperty
	m_pFrameLineProperty[0] = vtkSmartPointer<vtkProperty>::New();
	m_pFrameLineProperty[0]->SetColor(0, 0.0902, 0.7392);
	m_pFrameLineProperty[0]->SetLineWidth(1);
	m_pFrameLineProperty[1] = vtkSmartPointer<vtkProperty>::New();
	m_pFrameLineProperty[1]->SetColor(0.7196, 0.1294, 0);
	m_pFrameLineProperty[1]->SetLineWidth(1);
	m_pFrameLineProperty[2] = vtkSmartPointer<vtkProperty>::New();
	m_pFrameLineProperty[2]->SetColor(0, 0.7647, 0.0588);
	m_pFrameLineProperty[2]->SetLineWidth(1);

	//各对象实例并建立管线链接
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pFrameLinePoints[ViewLabel] = vtkSmartPointer<vtkPoints>::New();
		m_pFrameLineCells[ViewLabel] = vtkSmartPointer<vtkCellArray>::New();
		m_pFrameLineActor[ViewLabel] = vtkSmartPointer<vtkActor>::New();
		m_pFrameLineData[ViewLabel] = vtkSmartPointer<vtkPolyData>::New();
		m_pFramelineMapper[ViewLabel] = vtkSmartPointer<vtkPolyDataMapper>::New();
		m_pFramelineMapper[ViewLabel]->SetInputData(m_pFrameLineData[ViewLabel]);
		m_pFrameLineActor[ViewLabel]->SetMapper(m_pFramelineMapper[ViewLabel]);
		m_pFrameLineData[ViewLabel]->Modified();

		m_pFrameLineActor[ViewLabel]->SetProperty(m_pFrameLineProperty[ViewLabel]);
	}
	vtkIdType          CurrentPoints[4];
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pFrameLinePoints[ViewLabel]->InsertNextPoint(0, 0, 0);
		m_pFrameLinePoints[ViewLabel]->InsertNextPoint(0, 0, 0);
		CurrentPoints[0] = m_pFrameLinePoints[ViewLabel]->GetNumberOfPoints() - 2;
		CurrentPoints[1] = CurrentPoints[0] + 1;
		m_pFrameLineCells[ViewLabel]->InsertNextCell(2, CurrentPoints);

		m_pFrameLinePoints[ViewLabel]->InsertNextPoint(0, 0, 0);
		CurrentPoints[0] = m_pFrameLinePoints[ViewLabel]->GetNumberOfPoints() - 2;
		CurrentPoints[1] = CurrentPoints[0] + 1;
		m_pFrameLineCells[ViewLabel]->InsertNextCell(2, CurrentPoints);

		m_pFrameLinePoints[ViewLabel]->InsertNextPoint(0, 0, 0);
		CurrentPoints[0] = m_pFrameLinePoints[ViewLabel]->GetNumberOfPoints() - 2;
		CurrentPoints[1] = CurrentPoints[0] + 1;
		m_pFrameLineCells[ViewLabel]->InsertNextCell(2, CurrentPoints);

		CurrentPoints[0] = m_pFrameLinePoints[ViewLabel]->GetNumberOfPoints() - 1;
		CurrentPoints[1] = CurrentPoints[0] - 3;
		m_pFrameLineCells[ViewLabel]->InsertNextCell(2, CurrentPoints);

		m_pFrameLineData[ViewLabel]->SetPoints(m_pFrameLinePoints[ViewLabel]);
		m_pFrameLineData[ViewLabel]->SetLines(m_pFrameLineCells[ViewLabel]);
	}

	// Renderer
	ASVisualizationManager3D::GetRenderer3D(&m_pRenderer3D);
}

ASPipelineOuterBorder3D::~ASPipelineOuterBorder3D()
{

}

// 管线类型判断
bool ASPipelineOuterBorder3D::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_OuterBorder3D || ASPipelineBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPipelineOuterBorder3D::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_OuterBorder3D)
	{
		return true;
	}
	return false;
}

// 设置显示属性
void ASPipelineOuterBorder3D::setShown(const bool c_Shown)
{
	if (ms_SelfPointer->m_isShown == c_Shown)
	{
		return;
	}
	ms_SelfPointer->m_isShown = c_Shown;

	if (ms_SelfPointer->m_isShown && ms_SelfPointer->m_isCrossShairShown)
	{
		ms_SelfPointer->m_pRenderer3D->AddActor(ms_SelfPointer->m_pFrameLineActor[0]);
		ms_SelfPointer->m_pRenderer3D->AddActor(ms_SelfPointer->m_pFrameLineActor[1]);
		ms_SelfPointer->m_pRenderer3D->AddActor(ms_SelfPointer->m_pFrameLineActor[2]);
	}
	else
	{
		ms_SelfPointer->m_pRenderer3D->RemoveActor(ms_SelfPointer->m_pFrameLineActor[0]);
		ms_SelfPointer->m_pRenderer3D->RemoveActor(ms_SelfPointer->m_pFrameLineActor[1]);
		ms_SelfPointer->m_pRenderer3D->RemoveActor(ms_SelfPointer->m_pFrameLineActor[2]);
	}
}
void ASPipelineOuterBorder3D::setCrossShairShown(const bool c_Shown)
{
	if (ms_SelfPointer->m_isCrossShairShown == c_Shown)
	{
		return;
	}
	ms_SelfPointer->m_isCrossShairShown = c_Shown;

	if (ms_SelfPointer->m_isShown && ms_SelfPointer->m_isCrossShairShown)
	{
		ms_SelfPointer->m_pRenderer3D->AddActor(ms_SelfPointer->m_pFrameLineActor[0]);
		ms_SelfPointer->m_pRenderer3D->AddActor(ms_SelfPointer->m_pFrameLineActor[1]);
		ms_SelfPointer->m_pRenderer3D->AddActor(ms_SelfPointer->m_pFrameLineActor[2]);
	}
	else
	{
		ms_SelfPointer->m_pRenderer3D->RemoveActor(ms_SelfPointer->m_pFrameLineActor[0]);
		ms_SelfPointer->m_pRenderer3D->RemoveActor(ms_SelfPointer->m_pFrameLineActor[1]);
		ms_SelfPointer->m_pRenderer3D->RemoveActor(ms_SelfPointer->m_pFrameLineActor[2]);
	}
}

// 刷新显示
void ASPipelineOuterBorder3D::Update()
{


}
// 虚函数，切图焦点改变
void ASPipelineOuterBorder3D::Update2DTransformMatrix()
{
	if (m_isShown == false)
	{
		return;
	}
	double crntPosition[3];
	double crntExtent[6];
	ASVisualizationManager::GetDisplayPosition(crntPosition);
	ASVisualizationManager::GetDisplayExtent(crntExtent);
	//更新轮廓线顶点坐标
	m_pFrameLinePoints[0]->SetPoint(0, crntPosition[0], crntExtent[2], crntExtent[4]);
	m_pFrameLinePoints[0]->SetPoint(1, crntPosition[0], crntExtent[2], crntExtent[5]);
	m_pFrameLinePoints[0]->SetPoint(2, crntPosition[0], crntExtent[3], crntExtent[5]);
	m_pFrameLinePoints[0]->SetPoint(3, crntPosition[0], crntExtent[3], crntExtent[4]);

	m_pFrameLinePoints[1]->SetPoint(0, crntExtent[0], crntPosition[1], crntExtent[4]);
	m_pFrameLinePoints[1]->SetPoint(1, crntExtent[0], crntPosition[1], crntExtent[5]);
	m_pFrameLinePoints[1]->SetPoint(2, crntExtent[1], crntPosition[1], crntExtent[5]);
	m_pFrameLinePoints[1]->SetPoint(3, crntExtent[1], crntPosition[1], crntExtent[4]);

	m_pFrameLinePoints[2]->SetPoint(0, crntExtent[0], crntExtent[2], crntPosition[2]);
	m_pFrameLinePoints[2]->SetPoint(1, crntExtent[0], crntExtent[3], crntPosition[2]);
	m_pFrameLinePoints[2]->SetPoint(2, crntExtent[1], crntExtent[3], crntPosition[2]);
	m_pFrameLinePoints[2]->SetPoint(3, crntExtent[1], crntExtent[2], crntPosition[2]);

	//相关对象update()
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pFrameLineData[ViewLabel]->Modified();
		m_pFramelineMapper[ViewLabel]->Modified();
		m_pFramelineMapper[ViewLabel]->Update();
		m_pFrameLineActor[ViewLabel]->Modified();
	}
}