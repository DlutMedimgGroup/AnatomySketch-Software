#include "ASTransLandmark.h"
#include "ASMainWidget.h"
#include "ASLandmarkData.h"
#include "ASDataTree.h"
#include "ASTransAddDeleteData.h"
#include "ASInteractorStyleManager.h"
#include "ASTransInteractorStyle.h"
#include "ASVisualizationManager2D.h"
#include "ASVisualizationManager.h"
#include "ASTransDataGeneral.h"
#include "ASDataStructureWalker.h"
#include "ASDialogNewLM.h"
#include "ASTransSidebarGeneral.h"
#include <QInputDialog>

ASTransLandmark* ASTransLandmark::ms_SelfPointer = nullptr;

ASTransLandmark::ASTransLandmark(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;
	m_FModelLandmark = new ASFModelLandmark();
}

ASTransLandmark::~ASTransLandmark()
{

}
// 唯一对象
ASTransLandmark* ASTransLandmark::GetSelfPointer()
{
	return ms_SelfPointer;
}

// 得到当前ROI文件
ASLandmarkData* ASTransLandmark::GetCurrentLMData()
{
	ASAbstractData* CrntDataNode = ASTransSidebarGeneral::GetCrntDataNode();
	if (CrntDataNode && CrntDataNode->isTypeInherited(DataType_LandmarkData))
	{
		return static_cast<ASLandmarkData*>(CrntDataNode);
	}
	return nullptr;
}
// 新建轮廓线文件
void ASTransLandmark::NewLandmarkData(ASAbstractData* pParent)
{
	ASLandmarkData* LMdata;
	if (pParent->isTypeInherited(DataType_LandmarkData))
	{
		LMdata = static_cast<ASLandmarkData*>(pParent);
	}
	else
	{
		LMdata = static_cast<ASLandmarkData*>(pParent->getChild(DataType_LandmarkData));
	}
	if (!LMdata)
	{
		LMdata = new ASLandmarkData();
		LMdata->setBeingProcess(false);
		LMdata->setName(tr("Landmark data"));
		LMdata->setShown(true);
		ASDataTree::NewDataNode(LMdata, pParent);
		if (!LMdata)
		{
			return;
		}
	}

	// 在数据树上添加节点	
	ASTransSidebarGeneral::SetCrntDataNode(LMdata);
	// 更新列表显示
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_LandmarkData);
	// 使能
	ASTransLandmark::SetModelEnabled(true);
}
// 初始化轮廓线管线
void ASTransLandmark::InitLandmarkPipeline()
{
	ms_SelfPointer->m_FModelLandmark->InitLandmarkPipeline();
	ms_SelfPointer->m_FModelLandmark->ReSetCtrlRadius(0);
	ms_SelfPointer->m_FModelLandmark->ReSetCtrlRadius(1);
	ms_SelfPointer->m_FModelLandmark->ReSetCtrlRadius(2);
}
// 设置模块是否使能
void ASTransLandmark::SetModelEnabled(const bool c_enable)
{
	ms_SelfPointer->m_FModelLandmark->SetLandmarkModleEnabled(c_enable);
	emit ms_SelfPointer->signalSetModelEnabled(c_enable);
	if (c_enable)
	{
		if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_PaintingLandmarks)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_PaintingLandmarks);
		}
	}
	else
	{
		if (ASInteractorStyleManager::GetInteractorStyle2D() == InterStyle2D_PaintingLandmarks)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_General);
		}
	}
}
// 标定点是否显示
void ASTransLandmark::SetLandmarksShown(const bool c_Shown)
{
	ms_SelfPointer->m_FModelLandmark->SetLandmarkShown(c_Shown);
}
// 添加标定点
void ASTransLandmark::LandmarkOperation(double* pos, int viewlabel, int operate, vtkActor* actor)
{
	static int s_crntDataID = -1;
	static int s_crntViewLabel = -1;
	static int s_crntPointID = -1;

	double ImageSpacing[3];
	ASVisualizationManager::GetDisplaySpacing(ImageSpacing);
	switch (operate)
	{
	case CtrOpe_AddLandM:
	{
		// 1 添加标定点
		// 1.1弹出对话框设定标定点名称
		ASDialogNewLM* dia = new ASDialogNewLM(ASMainWidget::GetMainWidget());
		dia->exec();
		if (dia->m_result.size() > 0)
		{
			// 1.2 添加标定点
			ms_SelfPointer->m_FModelLandmark->AddLandmark(pos, dia->m_result);
			// 1.3 刷新显示
			ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(viewlabel);
			// 1.4 渲染
			ASVisualizationManager2D::Render(viewlabel);
		}
		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_LandmarkData);
		break;
	}
	case CtrOpe_MoveChooseLandM:
	{
		// 2 准备平移标定点
		ms_SelfPointer->m_FModelLandmark->ReserchLanmark(actor, &s_crntDataID, &s_crntViewLabel, &s_crntPointID);
		break;
	}
	case CtrOpe_MovingLandM:
	{
		// 3 正在平移标定点
		// 3.1 平移标定点
		ms_SelfPointer->m_FModelLandmark->MoveLandmark(pos, s_crntDataID, s_crntViewLabel, s_crntPointID);
		// 3.2 刷新显示
		ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(0);
		ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(1);
		ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(2);
		// 3.3 渲染
		ASVisualizationManager2D::Render();
		break;
	}
	case CtrOpe_DeleteLandM:
	{
		// 4 删除标定点
		// 4.1 寻找标定点
		ms_SelfPointer->m_FModelLandmark->ReserchLanmark(actor, &s_crntDataID, &s_crntViewLabel, &s_crntPointID);
		// 4.2 删除标定点
		ms_SelfPointer->m_FModelLandmark->DeleteLandmark(s_crntDataID, s_crntPointID);
		// 4.3 刷新显示
		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_LandmarkData);
		ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(0);
		ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(1);
		ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(2);
		// 4.4 渲染
		ASVisualizationManager2D::Render();
		break;
	}
	case CtrOpe_ZoomLandM:
	{
		// 5 图像缩放时保证控制点不缩放
		// 刷新数据
		ms_SelfPointer->m_FModelLandmark->ReSetCtrlRadius(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	default:
		break;
	}
}
// 刷新
void ASTransLandmark::RefreshLandMDisplay()
{
	ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(0);
	ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(1);
	ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(2);
}
// 从硬盘读文件
void ASTransLandmark::LoadLandMDataFromDisk(const QStringList c_FileNameList)
{
	ms_SelfPointer->m_FModelLandmark->InitLandmarkPipeline();
	for (int nit = 0; nit < c_FileNameList.size(); nit++)
	{
		ms_SelfPointer->m_FModelLandmark->LoadLandmarkData(c_FileNameList.at(nit));
	}
}
// 移除标定点
void ASTransLandmark::RemoveLandMark(const int c_LMDataNodeID, const int c_LMID)
{
	// 4.2 删除标定点
	ms_SelfPointer->m_FModelLandmark->DeleteLandmark(c_LMDataNodeID, c_LMID);
	// 4.3 刷新显示
	ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(0);
	ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(1);
	ms_SelfPointer->m_FModelLandmark->RefreshLMDisplay(2);
	// 4.4 渲染
	ASVisualizationManager2D::Render();
	// 刷新数据列表
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_LandmarkData);
}
