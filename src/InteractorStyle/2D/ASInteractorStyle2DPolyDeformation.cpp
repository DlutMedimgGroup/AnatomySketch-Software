#include "ASInteractorStyle2DPolyDeformation.h"
#include "ASTransGeneralInteractor.h"
#include "ASPipelineBase.h"
#include "ASVisualizationManager.h"
#include "ASTransPolyDeformation.h"
#include "ASFModelPolyDeformation.h"
#include "ASTransContour.h"
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>   //拾取有关
#include <vtkCellPicker.h>

vtkStandardNewMacro(ASInteractorStyle2DPolyDeformation);

ASInteractorStyle2DPolyDeformation::ASInteractorStyle2DPolyDeformation()
{

}

ASInteractorStyle2DPolyDeformation::~ASInteractorStyle2DPolyDeformation()
{

}

//鼠标移动
void ASInteractorStyle2DPolyDeformation::OnMouseMove()
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
		switch (m_crntMoveState)
		{
		case State_Nomal:
			ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
			break;
		case State_Panning:
			ASTransPolyDeformation::PolyDefContinue(posfinal, m_ViewLabel, PolyDeformation_Panning);
			break;
		case State_FFD:
			ASTransPolyDeformation::PolyDefContinue(posfinal, m_ViewLabel, PolyDeformation_FFD);
			break;
		default:
			break;
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
void ASInteractorStyle2DPolyDeformation::OnLeftButtonDown()
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

	if(this->Interactor->GetAltKey())
	{
		vtkSmartPointer<vtkCellPicker> CellPicker = vtkSmartPointer<vtkCellPicker>::New();
		CellPicker->SetTolerance(0.01);
		CellPicker->Pick(x, y, 0, this->GetDefaultRenderer());
		if (ASTransPolyDeformation::ConourPicked(CellPicker->GetActor()))
		{
			m_crntMoveState = State_None;
		}
		else
		{
			// Alt按下窗位窗宽
			this->WinLevStart();
		}
	}
	else if (this->Interactor->GetControlKey() || m_isKey_x_down)
	{
		m_crntMoveState = State_FFD;
		ASTransPolyDeformation::PolyDefStart(posfinal, m_ViewLabel, PolyDeformation_FFD);
	}
	else if (this->Interactor->GetShiftKey())
	{
		m_crntMoveState = State_Panning;
		ASTransPolyDeformation::PolyDefStart(posfinal, m_ViewLabel, PolyDeformation_Panning);
	}
	else
	{
		m_crntMoveState = State_Nomal;
		ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
	}
}
//鼠标滚轮向上滚动
void ASInteractorStyle2DPolyDeformation::OnMouseWheelForward()
{
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0 || m_isMiddleButtonDown || m_isLeftButtonDown || m_isRightButtonDown)
	{
		return;
	}
	if (this->Interactor->GetShiftKey())
	{
		//缩放：放大
		ASTransPolyDeformation::PolyDefZoom(0);
	}
	else if (this->Interactor->GetControlKey() || m_isKey_x_down)
	{
		//FFD调整影响范围
		ASTransPolyDeformation::PolyDefWheelFFD(0);
	}
	else if (m_isKey_z_down)
	{
		ASTransPolyDeformation::PolyDefRotate(0, m_ViewLabel);
	}
	else
	{
		// 默认操作 切换焦点
		double Posfinal[3];
		double Spaing[3];
		ASVisualizationManager::GetDisplayPosition(Posfinal);
		ASVisualizationManager::GetDisplaySpacing(Spaing);
		Posfinal[m_ViewLabel] += Spaing[m_ViewLabel];
		ASTransGeneralInteractor::SetVisualizationPosition(Posfinal[0], Posfinal[1], Posfinal[2]);
	}
}

//鼠标滚轮向下滚动
void ASInteractorStyle2DPolyDeformation::OnMouseWheelBackward()
{
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0 || m_isMiddleButtonDown || m_isLeftButtonDown || m_isRightButtonDown)
	{
		return;
	}
	if (this->Interactor->GetShiftKey())
	{
		//缩放：放大
		ASTransPolyDeformation::PolyDefZoom(1);
	}
	else if (this->Interactor->GetControlKey() || m_isKey_x_down)
	{
		//FFD调整影响范围
		ASTransPolyDeformation::PolyDefWheelFFD(1);
	}
	else if (m_isKey_z_down)
	{
		ASTransPolyDeformation::PolyDefRotate(1, m_ViewLabel);
	}
	else
	{
		// 默认操作 切换焦点
		double Posfinal[3];
		double Spaing[3];
		ASVisualizationManager::GetDisplayPosition(Posfinal);
		ASVisualizationManager::GetDisplaySpacing(Spaing);
		Posfinal[m_ViewLabel] -= Spaing[m_ViewLabel];
		ASTransGeneralInteractor::SetVisualizationPosition(Posfinal[0], Posfinal[1], Posfinal[2]);
	}
}
// 按键
void ASInteractorStyle2DPolyDeformation::OnKeyPress()
{
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	if (key == "z" || key == "Z")
	{
		m_isKey_z_down = true;
	}
	if (key == "x" || key == "X")
	{
		m_isKey_x_down = true;
	}
}
void ASInteractorStyle2DPolyDeformation::OnKeyRelease()
{
	// Get the keypress
	vtkRenderWindowInteractor *rwi = this->Interactor;
	std::string key = rwi->GetKeySym();
	if (key == "z" || key == "Z")
	{
		m_isKey_z_down = false;
	}
	if (key == "x" || key == "X")
	{
		m_isKey_x_down = false;
	}
}