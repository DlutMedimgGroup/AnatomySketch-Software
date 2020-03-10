#include "ASTransGeneralInteractor.h"
#include "ASTransSidebarGeneral.h"
#include "ASTransStatusBarGeneral.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager2D.h"
#include "ASVisualizationManager3D.h"
#include "ASAbstractData.h"
#include "ASArrayImageData.h"
#include "ASPolyImageData.h"
#include "ASPipelineBase.h"
#include "ASPipelineManager.h"
#include "ASPipelineArrayImage.h"
#include "ASPipelinePolyImage.h"
#include "ASPipelineOuterBorder3D.h"
#include "ASTransContour.h"
#include "ASTransLandmark.h"
#include "ASTransROI.h"
#include "ASFModelGeneralInteractor.h"
#include "ASPipelineText.h"
#include <QList>

ASTransGeneralInteractor* ASTransGeneralInteractor::ms_SelfPointer = nullptr;

ASTransGeneralInteractor::ASTransGeneralInteractor(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;

	// 功能模块
	m_FModelGenInter = new ASFModelGeneralInteractor();
}

ASTransGeneralInteractor::~ASTransGeneralInteractor()
{

}

// 唯一对象
ASTransGeneralInteractor* ASTransGeneralInteractor::GetSelfPointer()
{
	return ms_SelfPointer;
}
// 设置显示焦点
void ASTransGeneralInteractor::SetVisualizationPosition(const double c_x, const double c_y, const double c_z)
{
	ASTransGeneralInteractor::fSetVisualizationPosition(c_x, c_y, c_z);
	ASVisualizationManager::Render();
}
// 切换3D视图2D截面是否显示，返回切换后是否显示
bool ASTransGeneralInteractor::Switch3DImageStackShown()
{
	bool shown = ASVisualizationManager3D::Switch3DImageStackShown();
	ASPipelineOuterBorder3D::setCrossShairShown(shown);
	ASVisualizationManager3D::Render();
	return shown;
}
// 设置当前帧
void ASTransGeneralInteractor::SetVisualizationFrame(const int c_frame)
{
	ASVisualizationManager::SetDisplayFrame(c_frame);
	// 遍历体数据管线
	for (ASPipelineBase* item = ASPipelineManager::bigen_inherit(PipeLineType_ArrayImage); item != nullptr; item = ASPipelineManager::next_inherit(PipeLineType_ArrayImage))
	{
		ASPipelineArrayImage* CrntPipeline = (ASPipelineArrayImage*)item;
		CrntPipeline->SetFrame(c_frame);
	}
	// 渲染
	ASVisualizationManager::Render();
	// 侧边栏
	ASTransSidebarGeneral::SetDisplayFrame(c_frame);
	// 设置状态栏焦点像素信息
	ASTransStatusBarGeneral::RefreshStatusBarFocusInfo();
}
// 设置当前图像窗位窗宽
void ASTransGeneralInteractor::SetVisualizationLevelWindow(const double c_level, const double c_window)
{
	ASArrayImageData* crntTopData = ASVisualizationManager::GetToplayerData();
	double windowleble[2];
	windowleble[0] = c_window;
	windowleble[1] = c_level;
	SetArrayDataWindowLevel(crntTopData, windowleble);
}
// 设置某数据是否显示
void ASTransGeneralInteractor::SetDateShown(const int c_ID, const bool c_isShown)
{
	ASAbstractData* crntDataNode = ASAbstractData::getDataNodeFromID(c_ID);
	if (c_isShown != crntDataNode->isShown())
	{		
		ms_SelfPointer->fSetDataShown(crntDataNode, c_isShown);
		emit ms_SelfPointer->signalDataShownChanged(crntDataNode);
		// 渲染
		ASVisualizationManager::Render();
	}
}
// 重置数据
void ASTransGeneralInteractor::ReinitDataNode(ASAbstractData* DataNode)
{
	// 0 如果隐藏改为显示
	if (DataNode->isShown() == false)
	{
		ms_SelfPointer->fSetDataShown(DataNode, true);
		emit ms_SelfPointer->signalDataShownChanged(DataNode);
	}
	// 1 如果为体数据，重置窗位窗宽
	if (DataNode->isTypeInherited(DataType_ArrayImageData))
	{
		ASArrayImageData* crntDataNode = (ASArrayImageData*)DataNode;
		crntDataNode->setWindowLevel();
		QList<ASPipelineBase*>* crntListPipeline = new QList<ASPipelineBase*>;
		ASPipelineManager::GetPipelineForDataID(crntListPipeline, DataNode->getDataNodeID());
		for (int crntPipe = 0; crntPipe < crntListPipeline->size(); crntPipe++)
		{
			if (crntListPipeline->at(crntPipe)->GetPipeLineType() == PipeLineType_ArrayImage)
			{
				ASPipelineArrayImage* crntPipeline = (ASPipelineArrayImage*)crntListPipeline->at(crntPipe);
				crntPipeline->SetColorMapping(crntDataNode->getColorMap(), crntDataNode->getWindowLevel()[0], crntDataNode->getWindowLevel()[1]);
			}
		}
	}
	// 2 焦点重置为数据中心
	// 更新焦点显示
	double* aExtent = DataNode->getDataExtent();
	ASVisualizationManager::SetDisplayPostion((aExtent[0] + aExtent[1]) / 2, (aExtent[2] + aExtent[3]) / 2, (aExtent[4] + aExtent[5]) / 2);
	ASTransStatusBarGeneral::RefreshStatusBarFocusInfo();
	// 重置相机
	ASVisualizationManager::ResetCamera();
	// 3 渲染
	ASVisualizationManager::Render();
}
// 设置体数据的颜色表
void ASTransGeneralInteractor::SetArrayDataColorMap(ASArrayImageData* DataNode, const int c_ColorMap)
{
	if (DataNode->getColorMap() == c_ColorMap)
	{
		return;
	}
	DataNode->setColorMap(c_ColorMap);
	QList<ASPipelineBase*>* crntListPipeline = new QList<ASPipelineBase*>;
	ASPipelineManager::GetPipelineForDataID(crntListPipeline, DataNode->getDataNodeID());
	for (int crntPipe = 0; crntPipe < crntListPipeline->size(); crntPipe++)
	{
		if (crntListPipeline->at(crntPipe)->GetPipeLineType() == PipeLineType_ArrayImage)
		{
			((ASPipelineArrayImage*)crntListPipeline->at(crntPipe))->SetColorMapping(c_ColorMap, DataNode->getWindowLevel()[0], DataNode->getWindowLevel()[1]);
			
		}
	}
	ASVisualizationManager::Render();
}
// 设置体数据窗位窗宽
void ASTransGeneralInteractor::SetArrayDataWindowLevel(ASArrayImageData* DataNode, double* WindowLevel)
{
	// 0 设置数据的窗位窗宽参数
	DataNode->setWindowLevel(WindowLevel[0], WindowLevel[1]);
	// 1 更新管线
	QList<ASPipelineBase*>* listPipe = new QList<ASPipelineBase*>;
	ASPipelineManager::GetPipelineForDataID(listPipe, DataNode->getDataNodeID());
	for (int ncrntPipe = 0; ncrntPipe < listPipe->size(); ncrntPipe++)
	{
		if (listPipe->at(ncrntPipe)->GetPipeLineType() == PipeLineType_ArrayImage)
		{
			((ASPipelineArrayImage*)listPipe->at(ncrntPipe))->SetColorMapping(DataNode->getColorMap(), DataNode->getWindowLevel()[0], DataNode->getWindowLevel()[1]);
		}
	}
	// 2 渲染
	ASVisualizationManager::Render();
	// 3 更新参数显示
	ASTransStatusBarGeneral::RefreshStatusBarFocusInfo();
	ASTransSidebarGeneral::SetWindowLevel(WindowLevel);
}
// 设置面数据不透明度
void ASTransGeneralInteractor::SetPolyDataOpacity(ASAbstractData* DataNode, const int c_Opacity)
{
	if (DataNode->isTypeInherited(DataType_PolyImageData))
	{
		ASPolyImageData* PolyData = static_cast<ASPolyImageData*>(DataNode);
		PolyData->setOpacity(c_Opacity);
		QList<ASPipelineBase*>* crntListPipeline = new QList<ASPipelineBase*>;
		ASPipelineManager::GetPipelineForDataID(crntListPipeline, PolyData->getDataNodeID());
		for (int crntPipe = 0; crntPipe < crntListPipeline->size(); crntPipe++)
		{
			if (crntListPipeline->at(crntPipe)->GetPipeLineType() == PipeLineType_PolyImage)
			{
				((ASPipelinePolyImage*)crntListPipeline->at(crntPipe))->SetOpacity(((double)c_Opacity) / 100);
			}
		}
		ASVisualizationManager3D::Render();
	}
	if (DataNode->isTypeInherited(DataType_ArrayImageData))
	{
		ASArrayImageData* ArrayData = static_cast<ASArrayImageData*>(DataNode);
		ArrayData->setOpacity(c_Opacity);
		QList<ASPipelineBase*>* crntListPipeline = new QList<ASPipelineBase*>;
		ASPipelineManager::GetPipelineForDataID(crntListPipeline, ArrayData->getDataNodeID());
		for (int crntPipe = 0; crntPipe < crntListPipeline->size(); crntPipe++)
		{
			if (crntListPipeline->at(crntPipe)->GetPipeLineType() == PipeLineType_ArrayImage)
			{
				((ASPipelineArrayImage*)crntListPipeline->at(crntPipe))->SetOpacity(((double)c_Opacity) / 100);
			}
		}
		ASVisualizationManager::Render();
	}

}
// 设置面数据颜色
void ASTransGeneralInteractor::SetPolyDataColor(ASPolyImageData* DataNode, const QColor c_Color)
{
	DataNode->setColor(c_Color);
	QList<ASPipelineBase*>* crntListPipeline = new QList<ASPipelineBase*>;
	ASPipelineManager::GetPipelineForDataID(crntListPipeline, DataNode->getDataNodeID());
	for (int crntPipe = 0; crntPipe < crntListPipeline->size(); crntPipe++)
	{
		if (crntListPipeline->at(crntPipe)->GetPipeLineType() == PipeLineType_PolyImage)
		{
			((ASPipelinePolyImage*)crntListPipeline->at(crntPipe))->SetColor(c_Color);
		}
	}
	ASVisualizationManager::Render();
}
// 视图缩放
void ASTransGeneralInteractor::GenerateViewZoom(const int c_view)
{
	ASVisualizationManager::RefreshZoomPara(c_view);
	ASTransContour::ViewZoom(c_view);
	ASTransLandmark::LandmarkOperation(nullptr, c_view, CtrOpe_ZoomLandM, nullptr);
}

// 设置是否显示
void ASTransGeneralInteractor::fSetDataShown(ASAbstractData* DataNode, const bool c_isShown)
{
	DataNode->setShown(c_isShown);
	bool flagResetSpacing = false;
	QList<ASPipelineBase*>* crntListPipeline = new QList<ASPipelineBase*>;
	ASPipelineManager::GetPipelineForDataID(crntListPipeline, DataNode->getDataNodeID());
	for (int crntPipe = 0; crntPipe < crntListPipeline->size(); crntPipe++)
	{
		crntListPipeline->at(crntPipe)->SetShown(c_isShown);
		if (crntListPipeline->at(crntPipe)->GetPipeLineType() == PipeLineType_ArrayImage)
		{
			flagResetSpacing = true;
		}
	}
	if (DataNode->isTypeInherited(DataType_ContourData))
	{
		ASTransContour::RefreshContourDisplay();
	}
	if (DataNode->isTypeInherited(DataType_LandmarkData))
	{
		ASTransLandmark::RefreshLandMDisplay();
	}
	if (DataNode->isTypeInherited(DataType_ROI))
	{
		ASTransROI::RefreshROICubeDisplay();
	}
	// 根据当前各管线的数量更改辅助显示元素
	ASVisualizationManager::UpdateAuxiliaryPipeline();
	// 重新计算顶层体数据和像素尺寸
	if (flagResetSpacing)
	{
		ASVisualizationManager::ResetTopArrayImage();
	}	
}

// 设置显示焦点，不渲染
void ASTransGeneralInteractor::fSetVisualizationPosition(const double c_x, const double c_y, const double c_z)
{
	// 设置显示焦点
	ASVisualizationManager::SetDisplayPostion(c_x, c_y, c_z);
	// 设置侧边栏焦点坐标参数
	ASTransSidebarGeneral::SetDisplayPosition(c_x, c_y, c_z);
	// 设置状态栏焦点像素信息
	ASTransStatusBarGeneral::RefreshStatusBarFocusInfo();
}

// 悬停信息
void ASTransGeneralInteractor::HoverActorPicked(vtkActor* actor, const int c_x, const int c_y, const int c_ViewLabel)
{
	 ms_SelfPointer->m_FModelGenInter->HoverActorPicked(actor, c_x, c_y, c_ViewLabel);
}
void ASTransGeneralInteractor::AddHoverInfor(vtkActor* actor, QString infor)
{
	ms_SelfPointer->m_FModelGenInter->AddHoverInfor(actor, infor);
}
void ASTransGeneralInteractor::DeleteHoverInfor(vtkActor* actor)
{
	ms_SelfPointer->m_FModelGenInter->DeleteHoverInfor(actor);
}
int ASTransGeneralInteractor::ShowHoverText(const int c_ViewLabel, const QString c_text, double* pos1, double* pos2)
{
	int n = ASPipelineText::ShowText(c_ViewLabel, c_text, pos1, pos2);
	ASVisualizationManager2D::Render(c_ViewLabel);
	return n;
}
void ASTransGeneralInteractor::DeleteHoverText(const int c_TextID, const int c_ViewLabel)
{
	ASPipelineText::DeleteText(c_TextID);
	ASVisualizationManager2D::Render(c_ViewLabel);
}

void ASTransGeneralInteractor::SetLevel(double m_Level)
{
	ms_SelfPointer->WindowLevel[1] = m_Level;	
}

void ASTransGeneralInteractor::SetWindow(double m_Window)
{
	ms_SelfPointer->WindowLevel[0] = m_Window;
}

double ASTransGeneralInteractor::GetLevel()
{
	return ms_SelfPointer->WindowLevel[1];
}

double ASTransGeneralInteractor::GetWindow()
{
	return ms_SelfPointer->WindowLevel[0];
}




