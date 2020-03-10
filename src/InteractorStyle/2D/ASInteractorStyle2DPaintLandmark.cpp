#include "ASInteractorStyle2DPaintLandmark.h"
#include "ASTransGeneralInteractor.h"
#include "ASPipelineBase.h"
#include "ASVisualizationManager.h"
#include "ASFModelLandmark.h"
#include "ASTransLandmark.h"
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkObjectFactory.h>   //拾取有关
#include <vtkPropPicker.h>

vtkStandardNewMacro(ASInteractorStyle2DPaintLandmark);

ASInteractorStyle2DPaintLandmark::ASInteractorStyle2DPaintLandmark()
{

}

ASInteractorStyle2DPaintLandmark::~ASInteractorStyle2DPaintLandmark()
{

}

void ASInteractorStyle2DPaintLandmark::OnMouseMove()
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
		switch (m_MoveState)
		{
		case -1:
			// 默认切换焦点
			ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
			break;
		case 1:
			// 平移标定点
			ASTransLandmark::LandmarkOperation(posfinal, m_ViewLabel, CtrOpe_MovingLandM, nullptr);
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
void ASInteractorStyle2DPaintLandmark::OnLeftButtonDown()
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
	else if(this->Interactor->GetControlKey() && ASFModelLandmark::GetLandmarkModleEnabled())
	{
		if (ASTransLandmark::GetCurrentLMData() == nullptr)
		{
			return;
		}
		//拾取
		vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(x, y, 0, this->GetDefaultRenderer());
		vtkActor* pickedActor = picker->GetActor();
		if (pickedActor)
		{
			// 开始平移标定点
			ASTransLandmark::LandmarkOperation(posfinal, m_ViewLabel, CtrOpe_MoveChooseLandM, pickedActor);
			m_MoveState = 1;
		} 
		else
		{
			// 添加标定点
			ASTransLandmark::LandmarkOperation(posfinal, m_ViewLabel, CtrOpe_AddLandM, nullptr);
			m_MoveState = 0;
			m_isLeftButtonDown = false;
		}
	} 
	else if (this->Interactor->GetShiftKey() && ASFModelLandmark::GetLandmarkModleEnabled())
	{
		if (ASTransLandmark::GetCurrentLMData() == nullptr)
		{
			return;
		}
		//拾取
		vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
		picker->Pick(x, y, 0, this->GetDefaultRenderer());
		vtkActor* pickedActor = picker->GetActor();
		if (pickedActor)
		{
			// 删除标定点
			ASTransLandmark::LandmarkOperation(posfinal, m_ViewLabel, CtrOpe_DeleteLandM, pickedActor);
			m_MoveState = 0;
		}
	}
	else
	{
		ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
		m_MoveState = -1;
	}		

}
