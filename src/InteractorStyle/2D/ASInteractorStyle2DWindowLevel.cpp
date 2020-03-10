#include "ASInteractorStyle2DWindowLevel.h"
#include "ASTransGeneralInteractor.h"
#include "ASPipelineBase.h"
#include "ASVisualizationManager.h"
#include "ASTransContour.h"
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>   //拾取有关

// !!!弃用!!!

vtkStandardNewMacro(ASInteractorStyle2DWindowLevel);

ASInteractorStyle2DWindowLevel::ASInteractorStyle2DWindowLevel()
{

}

ASInteractorStyle2DWindowLevel::~ASInteractorStyle2DWindowLevel()
{

}

//鼠标移动
void ASInteractorStyle2DWindowLevel::OnMouseMove()
{
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->HoverDetection(x, y);

	if (m_isLeftButtonDown)
	{
		vtkRenderWindowInteractor *rwi = this->Interactor;
		this->m_LevelWindowCurrentPosition[0] = rwi->GetEventPosition()[0];
		this->m_LevelWindowCurrentPosition[1] = rwi->GetEventPosition()[1];
		int *size = this->DefaultRenderer->GetSize();
		double window = this->m_LevelWindowInitial[1];
		double level = this->m_LevelWindowInitial[0];
		// Compute normalized delta
		double dx = (this->m_LevelWindowCurrentPosition[0] -
			this->m_LevelWindowStartPosition[0]) / size[0];
		double dy = (this->m_LevelWindowStartPosition[1] -
			this->m_LevelWindowCurrentPosition[1]) / size[1];
		// Scale by current values
		if (fabs(window) > 0.01)
		{
			dx = dx * window;
		}
		else
		{
			dx = dx * (window < 0 ? -0.01 : 0.01);
		}
		if (fabs(level) > 0.01)
		{
			dy = dy * level;
		}
		else
		{
			dy = dy * (level < 0 ? -0.01 : 0.01);
		}
		// Abs so that direction does not flip
		if (window < 0.0)
		{
			dx = -1 * dx;
		}
		if (level < 0.0)
		{
			dy = -1 * dy;
		}
		// Compute new window level
		double newWindowLevel[2];
		newWindowLevel[0] = dx + window;
		newWindowLevel[1] = level - dy;
		if (newWindowLevel[0] < 0.01)
		{
			newWindowLevel[0] = 0.01;
		}
		ASTransGeneralInteractor::SetArrayDataWindowLevel(m_crntTopData, newWindowLevel);
	}
	else if (m_isMiddleButtonDown)
	{
		this->Move_Panning(x, y);
	}
	else if (m_isRightButtonDown)
	{
		this->Move_Zooming(x, y);
	}
}
//鼠标左键按下
void ASInteractorStyle2DWindowLevel::OnLeftButtonDown()
{
	m_isLeftButtonDown = true;

	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}

	m_crntTopData = ASVisualizationManager::GetToplayerData();
	if (m_crntTopData == nullptr)
	{
		return;
	}
	m_LevelWindowInitial[0] = m_crntTopData->getWindowLevel()[1];
	m_LevelWindowInitial[1] = m_crntTopData->getWindowLevel()[0];

	vtkRenderWindowInteractor *rwi = this->Interactor;

	this->m_LevelWindowStartPosition[0] = rwi->GetEventPosition()[0];
	this->m_LevelWindowStartPosition[1] = rwi->GetEventPosition()[1];
}