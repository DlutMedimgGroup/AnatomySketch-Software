#include "ASInteractorStyle2DPaintSeeds.h"
#include "ASTransGeneralInteractor.h"
#include "ASPipelineBase.h"
#include "ASVisualizationManager.h"
#include "ASFModelSeeds.h"
#include "ASTransSeeds.h"
#include "ASTransContour.h"
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>   //拾取有关

vtkStandardNewMacro(ASInteractorStyle2DPaintSeeds);

ASInteractorStyle2DPaintSeeds::ASInteractorStyle2DPaintSeeds()
{

}

ASInteractorStyle2DPaintSeeds::~ASInteractorStyle2DPaintSeeds()
{

}

//鼠标移动
void ASInteractorStyle2DPaintSeeds::OnMouseMove()
{
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	this->HoverDetection(x, y);
	if (m_IsWinlev)
	{
		this->WinLevContinue();
	}
	else if (m_isLeftButtonDown)
	{
		if (m_isPaintingSeeds)
		{
			// Ctrl按下，画种子点
			m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
			double posfinal[4];
			getPickPosFinal(posfinal, m_picker->GetPickPosition());
			ASTransSeeds::ContinuePaintSeeds(posfinal, m_ViewLabel);
		}
		else
		{
			// 普通交互，切换焦点
			if (m_isLeftButtonDown)
			{
				m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
				double posfinal[4];
				getPickPosFinal(posfinal, m_picker->GetPickPosition());
				ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
			}
			else if (m_isMiddleButtonDown)
			{
				this->FindPokedRenderer(x, y);
				this->Pan();
				this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			}
			else if (m_isRightButtonDown)
			{
				this->FindPokedRenderer(x, y);
				this->Dolly();
				this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
			}
		}
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
void ASInteractorStyle2DPaintSeeds::OnLeftButtonDown()
{
	m_isLeftButtonDown = true;

	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}

	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];
	m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
	double posfinal[4];
	getPickPosFinal(posfinal, m_picker->GetPickPosition());

	if (this->Interactor->GetAltKey())
	{
		// Alt按下窗位窗宽
		this->WinLevStart();
	}
	else if(this->Interactor->GetControlKey() && ASFModelSeeds::GetSeedsModleEnabled())
	{
		if (ASTransSeeds::GetCurrentSeedData() == nullptr)
		{
			return;
		}
		if (ASTransSeeds::GetCrntSeedsID() < 0)
		{
			return;
		}
		// Ctrl按下，画种子点
		m_isPaintingSeeds = true;

		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];
		m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
		double posfinal[4];
		getPickPosFinal(posfinal, m_picker->GetPickPosition());
		ASTransSeeds::StartPaintSeeds(posfinal, m_ViewLabel);
	}
	else
	{
		// 普通交互，切换焦点
		m_isPaintingSeeds = false;
		ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
	}	
}
//鼠标左键抬起
void ASInteractorStyle2DPaintSeeds::OnLeftButtonUp()
{
	m_isLeftButtonDown = false;
	m_IsWinlev = false;
	if (m_isPaintingSeeds)
	{
		ASTransSeeds::EndPaintSeeds();
		m_isPaintingSeeds = false;
	}
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
}