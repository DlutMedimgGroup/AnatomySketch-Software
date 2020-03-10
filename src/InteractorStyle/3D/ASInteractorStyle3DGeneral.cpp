#include "ASInteractorStyle3DGeneral.h"
#include "ASTransGeneralInteractor.h"
#include "ASVisualizationManager3D.h"
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkObjectFactory.h>   //Ê°È¡ÓÐ¹Ø
#include <vtkCamera.h>

vtkStandardNewMacro(ASInteractorStyle3DGeneral);

ASInteractorStyle3DGeneral::ASInteractorStyle3DGeneral()
{
	m_picker = vtkSmartPointer<vtkCellPicker>::New();
	//m_picker->SetTolerance(0.0); // ÉèÖÃÊ°È¡·¶Î§/Ê°È¡°ë¾¶
}

ASInteractorStyle3DGeneral::~ASInteractorStyle3DGeneral()
{

}

void ASInteractorStyle3DGeneral::OnLeftButtonDown()
{
	this->FindPokedRenderer(this->Interactor->GetEventPosition()[0],
		this->Interactor->GetEventPosition()[1]);
	if (this->CurrentRenderer == nullptr)
	{
		return;
	}

	this->GrabFocus(this->EventCallbackCommand);

	if (this->Interactor->GetControlKey())
	{
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];

		m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
		if (m_picker->GetCellId() != -1)
		{
			double* pos = m_picker->GetPickPosition();
			ASTransGeneralInteractor::SetVisualizationPosition(pos[0], pos[1], pos[2]);
		}
	} 
	else
	{
		this->StartRotate();
	}	
}

// ´¥¿ØÆ½ÒÆ
void ASInteractorStyle3DGeneral::TouchPan(double* pos)
{
	vtkSmartPointer<vtkRenderer> renderer;
	ASVisualizationManager3D::GetRenderer3D(&renderer);
	this->SetCurrentRenderer(renderer);
	vtkCamera* camera = renderer->GetActiveCamera();

	double viewFocus[4], FocalDepth, viewPoint[3];
	double NewPickPoint[4], OldPickPoint[4], motionVector[4];

	// Calculate the focal depth since we'll be using it a lot
	camera->GetFocalPoint(viewFocus);
	this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2],
		viewFocus);
	FocalDepth = viewFocus[2];
	this->ComputeDisplayToWorld(pos[2], pos[1], FocalDepth, NewPickPoint);
	this->ComputeDisplayToWorld(pos[0], pos[3], FocalDepth, OldPickPoint);

	motionVector[0] = NewPickPoint[0] - OldPickPoint[0];
	motionVector[1] = NewPickPoint[1] - OldPickPoint[1];
	motionVector[2] = NewPickPoint[2] - OldPickPoint[2];

	camera->GetFocalPoint(viewFocus);
	camera->GetPosition(viewPoint);
	camera->SetFocalPoint(motionVector[0] + viewFocus[0],
		motionVector[1] + viewFocus[1],
		motionVector[2] + viewFocus[2]);

	camera->SetPosition(motionVector[0] + viewPoint[0],
		motionVector[1] + viewPoint[1],
		motionVector[2] + viewPoint[2]);

	if (this->Interactor->GetLightFollowCamera())
	{
		renderer->UpdateLightsGeometryToFollowCamera();
	}

	this->Interactor->Render();
}
// ´¥¿ØËõ·Å
void ASInteractorStyle3DGeneral::TouchZoom(double factor)
{
	vtkSmartPointer<vtkRenderer> renderer;
	ASVisualizationManager3D::GetRenderer3D(&renderer);
	vtkCamera* camera = renderer->GetActiveCamera();
	if (camera->GetParallelProjection())
	{
		camera->SetParallelScale(camera->GetParallelScale() / factor);
	}
	else
	{
		camera->Dolly(factor);
		if (this->AutoAdjustCameraClippingRange)
		{
			renderer->ResetCameraClippingRange();
		}
	}
	if (this->Interactor->GetLightFollowCamera())
	{
		renderer->UpdateLightsGeometryToFollowCamera();
	}
	this->Interactor->Render();
}