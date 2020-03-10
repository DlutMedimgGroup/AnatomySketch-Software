#include "ASInteractorStyle2DROICube.h"
#include "ASTransGeneralInteractor.h"
#include "ASVisualizationManager.h"
#include "ASPipelineBase.h"
#include "vtkPropPicker.h"
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>   //拾取有关
#include "ASTransROI.h"

vtkStandardNewMacro(ASInteractorStyle2DROICube);

ASInteractorStyle2DROICube::ASInteractorStyle2DROICube()
{

}

ASInteractorStyle2DROICube::~ASInteractorStyle2DROICube()
{

}

void ASInteractorStyle2DROICube::OnMouseMove()
{
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
	int x = this->Interactor->GetEventPosition()[0];
	int y = this->Interactor->GetEventPosition()[1];

	this->HoverDetection(x, y);

	// 按键状态
	if (m_IsWinlev)
	{
		this->WinLevContinue();
	}
	else if (m_isLeftButtonDown)
	{
		if (m_isNormal == false)
		{
			return;
		}
		m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
		double posfinal[4];
		getPickPosFinal(posfinal, m_picker->GetPickPosition());
		ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
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
void ASInteractorStyle2DROICube::OnLeftButtonDown()
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
	else if(m_isKey_a_down)
	{
		m_isNormal = false;
		int dir = DirectionMapping[m_ViewLabel][0];
		ASTransROI::SetROICubeRange(dir, posfinal[dir / 2]);
	}
	else if (m_isKey_d_down)
	{
		m_isNormal = false;
		int dir = DirectionMapping[m_ViewLabel][1];
		ASTransROI::SetROICubeRange(dir, posfinal[dir / 2]);
	}
	else if (m_isKey_s_down)
	{
		m_isNormal = false;
		int dir = DirectionMapping[m_ViewLabel][2];
		ASTransROI::SetROICubeRange(dir, posfinal[dir / 2]);
	}
	else if (m_isKey_w_down)
	{
		m_isNormal = false;
		int dir = DirectionMapping[m_ViewLabel][3];
		ASTransROI::SetROICubeRange(dir, posfinal[dir / 2]);
	}
	else
	{
		m_isNormal = true;
		ASTransGeneralInteractor::SetVisualizationPosition(
			posfinal[0], posfinal[1], posfinal[2]);
	}
}
void ASInteractorStyle2DROICube::OnKeyPress()
{
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	if (key == "w" || key == "W")
	{
		m_isKey_w_down = true;
	}
	if (key == "a" || key == "A")
	{
		m_isKey_a_down = true;
	}
	if (key == "s" || key == "S")
	{
		m_isKey_s_down = true;
	}
	if (key == "d" || key == "D")
	{
		m_isKey_d_down = true;
	}
}
void ASInteractorStyle2DROICube::OnKeyRelease()
{
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	if (key == "w" || key == "W")
	{
		m_isKey_w_down = false;
	}
	if (key == "a" || key == "A")
	{
		m_isKey_a_down = false;
	}
	if (key == "s" || key == "S")
	{
		m_isKey_s_down = false;
	}
	if (key == "d" || key == "D")
	{
		m_isKey_d_down = false;
	}
}