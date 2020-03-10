#include "ASInteractorStyleManager.h"
#include "ASQVTKWidget.h"
#include "ASOperationBar.h"
#include "ASInteractorStyle2DGeneral.h"
#include "ASInteractorStyle2DWindowLevel.h"
#include "ASInteractorStyle2DPaintSeeds.h"
#include "ASInteractorStyle2DPaintContour.h"
#include "ASInteractorStyle2DMask.h"
#include "GlobelDeclaration.h"
#include "ASVisualizationManager2D.h"
#include "ASInteractorStyle3DGeneral.h"
#include "ASVisualizationManager3D.h"
#include "ASInteractorStyle2DPolyDeformation.h"
#include "ASInteractorStyle2DPaintLandmark.h"
#include "ASInteractorStyle2DROICube.h"
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>

ASInteractorStyleManager* ASInteractorStyleManager::ms_SelfPointer = nullptr;

ASInteractorStyleManager::ASInteractorStyleManager()
{
	ms_SelfPointer = this;
	ASOperationBar::GetQVTKWidget(m_pQVTKWiget);

	// 二维交互方式初始化
	vtkSmartPointer<vtkRenderer> Renderer2D[3];
	ASVisualizationManager2D::GetRenderer2D(Renderer2D);
	for (int ViewLable = 0; ViewLable < 3; ViewLable++)
	{
		m_Style2DGeneral[ViewLable] = vtkSmartPointer<ASInteractorStyle2DGeneral>::New();
		m_Style2DGeneral[ViewLable]->setViewLabel(ViewLable);
		m_Style2DGeneral[ViewLable]->SetDefaultRenderer(Renderer2D[ViewLable]);
		m_Style2DWindowLevel[ViewLable] = vtkSmartPointer<ASInteractorStyle2DWindowLevel>::New();
		m_Style2DWindowLevel[ViewLable]->setViewLabel(ViewLable);
		m_Style2DWindowLevel[ViewLable]->SetDefaultRenderer(Renderer2D[ViewLable]);
		m_Style2DPaintSeeds[ViewLable] = vtkSmartPointer<ASInteractorStyle2DPaintSeeds>::New();
		m_Style2DPaintSeeds[ViewLable]->setViewLabel(ViewLable);
		m_Style2DPaintSeeds[ViewLable]->SetDefaultRenderer(Renderer2D[ViewLable]);
		m_Style2DPaintContour[ViewLable] = vtkSmartPointer<ASInteractorStyle2DPaintContour>::New();
		m_Style2DPaintContour[ViewLable]->setViewLabel(ViewLable);
		m_Style2DPaintContour[ViewLable]->SetDefaultRenderer(Renderer2D[ViewLable]);
		m_Style2DPolyDeformation[ViewLable] = vtkSmartPointer<ASInteractorStyle2DPolyDeformation>::New();
		m_Style2DPolyDeformation[ViewLable]->setViewLabel(ViewLable);
		m_Style2DPolyDeformation[ViewLable]->SetDefaultRenderer(Renderer2D[ViewLable]);
		m_Style2DMask[ViewLable] = vtkSmartPointer<ASInteractorStyle2DMask>::New();
		m_Style2DMask[ViewLable]->setViewLabel(ViewLable);
		m_Style2DMask[ViewLable]->SetDefaultRenderer(Renderer2D[ViewLable]);
		m_Style2DLandmark[ViewLable] = vtkSmartPointer<ASInteractorStyle2DPaintLandmark>::New();
		m_Style2DLandmark[ViewLable]->setViewLabel(ViewLable);
		m_Style2DLandmark[ViewLable]->SetDefaultRenderer(Renderer2D[ViewLable]);
		m_Style2DROICube[ViewLable] = vtkSmartPointer<ASInteractorStyle2DROICube>::New();
		m_Style2DROICube[ViewLable]->setViewLabel(ViewLable);
		m_Style2DROICube[ViewLable]->SetDefaultRenderer(Renderer2D[ViewLable]);
	}
	ASInteractorStyleManager::setInteractorStyle2D(InterStyle2D_General);

	// 三维交互方式初始化
	vtkSmartPointer<vtkRenderer> Renderer3D;
	ASVisualizationManager3D::GetRenderer3D(&Renderer3D);
	m_Style3DGeneral = vtkSmartPointer<ASInteractorStyle3DGeneral>::New();
	m_Style3DGeneral->SetDefaultRenderer(Renderer3D);
	ASInteractorStyleManager::setInteractorStyle3D(InterStyle3D_General);

}

ASInteractorStyleManager::~ASInteractorStyleManager()
{

}

// 得到当前二维交互模式
int ASInteractorStyleManager::GetInteractorStyle2D()
{
	return ms_SelfPointer->m_InteractorStyle2D;
}
// 设置二维交互模式
void ASInteractorStyleManager::setInteractorStyle2D(const int c_style)
{
	if (ms_SelfPointer->m_InteractorStyle2D == c_style)
	{
		return;
	}
	ms_SelfPointer->m_InteractorStyle2D = c_style;
	for (int viewLabel = 0; viewLabel < 3; viewLabel++)
	{
		switch (c_style)
		{
		case InterStyle2D_General:
			ms_SelfPointer->m_pQVTKWiget[viewLabel]->GetRenderWindow()->GetInteractor()->
				SetInteractorStyle(ms_SelfPointer->m_Style2DGeneral[viewLabel]);
			break;
		case InterStyle2D_WindowLevel:
			ms_SelfPointer->m_pQVTKWiget[viewLabel]->GetRenderWindow()->GetInteractor()->
				SetInteractorStyle(ms_SelfPointer->m_Style2DWindowLevel[viewLabel]);
			break;
		case InterStyle2D_PaintingSeeds:
			ms_SelfPointer->m_pQVTKWiget[viewLabel]->GetRenderWindow()->GetInteractor()->
				SetInteractorStyle(ms_SelfPointer->m_Style2DPaintSeeds[viewLabel]);
			break;
		case InterStyle2D_PaintingContour:
			ms_SelfPointer->m_pQVTKWiget[viewLabel]->GetRenderWindow()->GetInteractor()->
				SetInteractorStyle(ms_SelfPointer->m_Style2DPaintContour[viewLabel]);
			break;
		case InterStyle2D_PolyDeformation:
			ms_SelfPointer->m_pQVTKWiget[viewLabel]->GetRenderWindow()->GetInteractor()->
				SetInteractorStyle(ms_SelfPointer->m_Style2DPolyDeformation[viewLabel]);
			break;
		case InterStyle2D_Mask:
			ms_SelfPointer->m_pQVTKWiget[viewLabel]->GetRenderWindow()->GetInteractor()->
				SetInteractorStyle(ms_SelfPointer->m_Style2DMask[viewLabel]);
			break;
		case InterStyle2D_PaintingLandmarks:
			ms_SelfPointer->m_pQVTKWiget[viewLabel]->GetRenderWindow()->GetInteractor()->
				SetInteractorStyle(ms_SelfPointer->m_Style2DLandmark[viewLabel]);
			break;
		case InterStyle2D_ROICube:
			ms_SelfPointer->m_pQVTKWiget[viewLabel]->GetRenderWindow()->GetInteractor()->
				SetInteractorStyle(ms_SelfPointer->m_Style2DROICube[viewLabel]);
			break;
		default:
			break;
		}
	}
}
// 得到当前二维交互模式
ASInteractorStyle2DGeneral* ASInteractorStyleManager::GetInteractorStyle2D(const int c_ViewLabel)
{
	switch (ms_SelfPointer->m_InteractorStyle2D)
	{
	case InterStyle2D_General:
		return ms_SelfPointer->m_Style2DGeneral[c_ViewLabel];
	case InterStyle2D_WindowLevel:
		return ms_SelfPointer->m_Style2DWindowLevel[c_ViewLabel];
	case InterStyle2D_PaintingSeeds:
		return ms_SelfPointer->m_Style2DPaintSeeds[c_ViewLabel];
	case InterStyle2D_PaintingContour:
		return ms_SelfPointer->m_Style2DPaintContour[c_ViewLabel];
	case InterStyle2D_PolyDeformation:
		return ms_SelfPointer->m_Style2DPolyDeformation[c_ViewLabel];
	case InterStyle2D_Mask:
		return ms_SelfPointer->m_Style2DMask[c_ViewLabel];
	case InterStyle2D_PaintingLandmarks:
		return ms_SelfPointer->m_Style2DLandmark[c_ViewLabel];
	case InterStyle2D_ROICube:
		return ms_SelfPointer->m_Style2DROICube[c_ViewLabel];
	default:
		break;
	}
	return nullptr;
}
// 得到当前三维交互模式
int ASInteractorStyleManager::GetInteractorStyle3DType()
{
	return ms_SelfPointer->m_InteractorStyle3D;
}
ASInteractorStyle3DGeneral* ASInteractorStyleManager::GetInteractorStyle3D()
{
	switch (ms_SelfPointer->m_InteractorStyle3D)
	{
	case InterStyle3D_General:
		return ms_SelfPointer->m_Style3DGeneral;
		break;
	default:
		break;
	}
	return nullptr;
}
// 设置三维交互模式
void ASInteractorStyleManager::setInteractorStyle3D(const int c_style)
{
	if (ms_SelfPointer->m_InteractorStyle3D == c_style)
	{
		return;
	}
	ms_SelfPointer->m_InteractorStyle3D = c_style;
	switch (c_style)
	{
	case InterStyle3D_General:
		ms_SelfPointer->m_pQVTKWiget[ViewLabel_3D]->GetRenderWindow()->GetInteractor()->SetInteractorStyle(ms_SelfPointer->m_Style3DGeneral);
		break;
	default:
		break;
	}
}