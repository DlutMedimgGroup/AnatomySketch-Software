#include "ASInteractorStyle2DMask.h"
#include "ASTransGeneralInteractor.h"
#include "ASVisualizationManager.h"
#include "ASPipelineBase.h"
#include "vtkPropPicker.h"
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>   //拾取有关
#include "ASTransSidebarGeneral.h"

vtkStandardNewMacro(ASInteractorStyle2DMask);

ASInteractorStyle2DMask::ASInteractorStyle2DMask()
{

}

ASInteractorStyle2DMask::~ASInteractorStyle2DMask()
{

}

// 重写左键拖动部分
void ASInteractorStyle2DMask::OnMouseMove()
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
		m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
		double posfinal[4];
		getPickPosFinal(posfinal, m_picker->GetPickPosition());
		if (m_MoveState == 1)
		{
			ASTransSidebarGeneral::SetMaskInteractorly(posfinal[0], posfinal[1], posfinal[2]);
		}
		else
		{
			ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
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
void ASInteractorStyle2DMask::OnLeftButtonDown()
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
	else if (this->Interactor->GetControlKey())
	{
		m_MoveState = 1;
		ASTransSidebarGeneral::SetMaskInteractorly(posfinal[0], posfinal[1], posfinal[2]);
	}
	else
	{
		m_MoveState = 0;
		ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
	}
}