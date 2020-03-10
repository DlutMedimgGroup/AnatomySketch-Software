#include "ASInteractorStyle2DGeneral.h"
#include "ASTransGeneralInteractor.h"
#include "ASVisualizationManager2D.h"
#include "ASPipelineBase.h"
#include "ASVisualizationManager.h"
#include <vtkRenderer.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkCallbackCommand.h>
#include <vtkObjectFactory.h>   //拾取有关
#include <vtkPropPicker.h>
#include <ASArrayImageData.h>
#include <vtkCamera.h>
#include <vtkCoordinate.h>

vtkStandardNewMacro(ASInteractorStyle2DGeneral);


ASInteractorStyle2DGeneral::ASInteractorStyle2DGeneral()
{
	m_picker = vtkSmartPointer<vtkPointPicker>::New();
	m_picker->SetTolerance(0.0); // 设置拾取范围/拾取半径
}

ASInteractorStyle2DGeneral::~ASInteractorStyle2DGeneral()
{

}

//鼠标移动
void ASInteractorStyle2DGeneral::OnMouseMove()
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
		m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
		double posfinal[4];

		//cout << x << " - " << y << endl;
		//cout << m_picker->GetPickPosition()[0] << " - " << m_picker->GetPickPosition()[1] <<
		//	" - " << m_picker->GetPickPosition()[2] << endl;

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

//鼠标左键按下
void ASInteractorStyle2DGeneral::OnLeftButtonDown()
{
	m_isLeftButtonDown = true;

	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}

	if (this->Interactor->GetAltKey())
	{
		// Alt按下窗位窗宽
		this->WinLevStart();
	}
	else
	{
		int x = this->Interactor->GetEventPosition()[0];
		int y = this->Interactor->GetEventPosition()[1];

		m_picker->Pick(x, y, 0, this->GetDefaultRenderer());
		double posfinal[4];

		//cout << x << " - " << y << endl;
		//cout << m_picker->GetPickPosition()[0] << " - " << m_picker->GetPickPosition()[1] <<
		//	" - " << m_picker->GetPickPosition()[2] << endl;

		getPickPosFinal(posfinal, m_picker->GetPickPosition());
		ASTransGeneralInteractor::SetVisualizationPosition(posfinal[0], posfinal[1], posfinal[2]);
	}
}

//鼠标左键抬起
void ASInteractorStyle2DGeneral::OnLeftButtonUp()
{
	m_isLeftButtonDown = false;
	m_IsWinlev = false;
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
}

//鼠标中键按下
void ASInteractorStyle2DGeneral::OnMiddleButtonDown()
{
	m_isMiddleButtonDown = true;
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
	this->GrabFocus(this->EventCallbackCommand);
	this->StartPan();
}

//鼠标中键抬起
void ASInteractorStyle2DGeneral::OnMiddleButtonUp()
{
	m_isMiddleButtonDown = false;
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
	this->EndPan();
	if (this->Interactor)
	{
		this->ReleaseFocus();
	}
}

//鼠标右键按下
void ASInteractorStyle2DGeneral::OnRightButtonDown()
{
	m_isRightButtonDown = true;
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
	this->GrabFocus(this->EventCallbackCommand);
	this->StartDolly();
}

//鼠标右键抬起
void ASInteractorStyle2DGeneral::OnRightButtonUp()
{
	m_isRightButtonDown = false;
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0)
	{
		return;
	}
	this->EndDolly();
	if (this->Interactor)
	{
		this->ReleaseFocus();
	}
}

//鼠标滚轮向上滚动
void ASInteractorStyle2DGeneral::OnMouseWheelForward()
{

	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0 || m_isMiddleButtonDown || m_isLeftButtonDown || m_isRightButtonDown)
	{
		return;
	}

	// 默认操作 切换焦点
	double Posfinal[3];
	double Spaing[3];
	ASVisualizationManager::GetDisplayPosition(Posfinal);
	ASVisualizationManager::GetDisplaySpacing(Spaing);
	Posfinal[m_ViewLabel] += Spaing[m_ViewLabel];
	ASTransGeneralInteractor::SetVisualizationPosition(Posfinal[0], Posfinal[1], Posfinal[2]);
}

//鼠标滚轮向下滚动
void ASInteractorStyle2DGeneral::OnMouseWheelBackward()
{
	if (ASPipelineBase::GetNumOfShowingObj2D() <= 0 || m_isMiddleButtonDown || m_isLeftButtonDown || m_isRightButtonDown)
	{
		return;
	}

	// 默认操作 切换焦点
	double Posfinal[3];
	double Spaing[3];
	ASVisualizationManager::GetDisplayPosition(Posfinal);
	ASVisualizationManager::GetDisplaySpacing(Spaing);
	Posfinal[m_ViewLabel] -= Spaing[m_ViewLabel];
	ASTransGeneralInteractor::SetVisualizationPosition(Posfinal[0], Posfinal[1], Posfinal[2]);
}

//设置视图
void ASInteractorStyle2DGeneral::setViewLabel(int ViewLabel)
{
	m_ViewLabel = ViewLabel;
}

// 触控平移
void ASInteractorStyle2DGeneral::TouchPan(double* pos)
{
	vtkSmartPointer<vtkRenderer> renderer[3];
	ASVisualizationManager2D::GetRenderer2D(renderer);

	double viewFocus[4], viewPoint[3];
	double motionVector[4] = {0,0,0,0};

	// Calculate the focal depth since we'll be using it a lot

	vtkCamera *camera = renderer[m_ViewLabel]->GetActiveCamera();
	camera->GetFocalPoint(viewFocus);
	this->ComputeWorldToDisplay(viewFocus[0], viewFocus[1], viewFocus[2],
		viewFocus);

	vtkSmartPointer<vtkCoordinate> Coordinate = vtkSmartPointer<vtkCoordinate>::New();
	Coordinate->SetCoordinateSystemToDisplay();
	Coordinate->SetValue(pos[0], pos[1], 0);
	double* mvector1 = Coordinate->GetComputedWorldValue(renderer[m_ViewLabel]);
	double a[3];
	a[0] = mvector1[0];
	a[1] = mvector1[1];
	Coordinate->SetValue(pos[2], pos[3], 0);
	double* mvector2 = Coordinate->GetComputedWorldValue(renderer[m_ViewLabel]);
	motionVector[0] = mvector2[0] - a[0];
	motionVector[1] = a[1] - mvector2[1];
	motionVector[2] = 0;

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
		renderer[m_ViewLabel]->UpdateLightsGeometryToFollowCamera();
	}

	this->Interactor->Render();
}
// 触控缩放
void ASInteractorStyle2DGeneral::TouchZoom(double factor)
{
	vtkSmartPointer<vtkRenderer> renderer[3];
	ASVisualizationManager2D::GetRenderer2D(renderer);	
	vtkCamera* camera = renderer[m_ViewLabel]->GetActiveCamera();
	if (camera->GetParallelProjection())
	{
		camera->SetParallelScale(camera->GetParallelScale() / factor);
	}
	else
	{
		camera->Dolly(factor);
		if (this->AutoAdjustCameraClippingRange)
		{
			renderer[m_ViewLabel]->ResetCameraClippingRange();
		}
	}
	if (this->Interactor->GetLightFollowCamera())
	{
		renderer[m_ViewLabel]->UpdateLightsGeometryToFollowCamera();
	}
	this->Interactor->Render();
	// 缩放时需要刷新样条轮廓线的控制点半径
	ASTransGeneralInteractor::GenerateViewZoom(m_ViewLabel);
}
// 触控切层
void ASInteractorStyle2DGeneral::TouchWheel(double dis)
{
	if (dis > 0)
	{		
		this->OnMouseWheelForward();
	}
	else
	{
		this->OnMouseWheelBackward();
	}
}

//由vtkPointPicker得到的坐标的z总是0，这是因为vtkImageReslice对图像的变换，需要根据变换矩阵进行换算。
void ASInteractorStyle2DGeneral::getPickPosFinal(double* posfinal, double* PointPickerOutput)
{
	double input[4];
	input[0] = PointPickerOutput[0];
	input[1] = PointPickerOutput[1];
	input[2] = 0;						//防止拾取到Z坐标为1的点---------------------------------------------------------------------------------重要
	input[3] = 1;

	vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
	ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);
	pMatrix[m_ViewLabel]->MultiplyPoint(input, posfinal);
}

// 悬停检测
void ASInteractorStyle2DGeneral::HoverDetection(const int c_x, const int c_y)
{
	//拾取
	vtkSmartPointer<vtkPropPicker> picker = vtkSmartPointer<vtkPropPicker>::New();
	picker->Pick(c_x, c_y, 0, this->GetDefaultRenderer());
	vtkActor* pickedActor = picker->GetActor();
	ASTransGeneralInteractor::HoverActorPicked(pickedActor, c_x, c_y, m_ViewLabel);
}
// 继续改变窗位窗宽
void ASInteractorStyle2DGeneral::WinLevStart()
{
	m_IsWinlev = true;
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
void ASInteractorStyle2DGeneral::WinLevContinue()
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
	double* ScalarRange = m_crntTopData->getArrayImageData()->GetScalarRange();
	dy = dy * (ScalarRange[1] - ScalarRange[0]) / 20;
	if (fabs(window) > 0.01)
	{
		dx = dx * window;
	}
	else
	{
		dx = dx * (window < 0 ? -0.01 : 0.01);
	}
	//if (fabs(level) > 0.01)
	//{
	//	dy = dy * level;
	//}
	//else
	//{
	//	dy = dy * (level < 0 ? -0.01 : 0.01);
	//}
	// Abs so that direction does not flip
	if (window < 0.0)
	{
		dx = -1 * dx;
	}
	//if (level < 0.0)
	//{
	//	dy = -1 * dy;
	//}
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

// 响应
void ASInteractorStyle2DGeneral::Move_Panning(const int c_x, const int c_y)
{
	this->FindPokedRenderer(c_x, c_y);
	this->Pan();
	this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
}
void ASInteractorStyle2DGeneral::Move_Zooming(const int c_x, const int c_y)
{
	this->FindPokedRenderer(c_x, c_y);
	this->Dolly();
	this->InvokeEvent(vtkCommand::InteractionEvent, NULL);
	// 缩放时需要刷新样条轮廓线的控制点半径
	ASTransGeneralInteractor::GenerateViewZoom(m_ViewLabel);
}
