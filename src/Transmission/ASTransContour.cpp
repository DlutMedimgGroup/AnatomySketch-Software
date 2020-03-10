#include "ASTransContour.h"
#include "ASMainWidget.h"
#include "ASDialogNewSeedsFile.h"
#include "ASContourData.h"
#include "ASTransAddDeleteData.h"
#include "ASFModelContour.h"
#include "ASInteractorStyleManager.h"
#include "ASTransInteractorStyle.h"
#include "ASVisualizationManager2D.h"
#include "ASVisualizationManager.h"
#include "ASPipelineContour.h"
#include "ASPipelineManager.h"
#include "ASDataTree.h"
#include "ASTransDataGeneral.h"
#include "ASDataStructureWalker.h"
#include "ASArrayImageData.h"
#include "ASDialogNewElement.h"
#include "ASTransCompleter.h"
#include "ASTransSidebarGeneral.h"
#include "ASAnnotation.h"

ASTransContour* ASTransContour::ms_SelfPointer = nullptr;

ASTransContour::ASTransContour(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;
	m_FModelContour = new ASFModelContour();
}

ASTransContour::~ASTransContour()
{

}
// 唯一对象
ASTransContour* ASTransContour::GetSelfPointer()
{
	return ms_SelfPointer;
}
//当前SeedsID
void ASTransContour::SetCrntContourID(const int c_id)
{
	ms_SelfPointer->m_FModelContour->SetCurrentContourID(c_id);
	//if (c_id >= 0)
	//{
	//	ms_SelfPointer->m_FModelContour->SetContourModleEnabled(true);
	//}
	//else
	//{
	//	ms_SelfPointer->m_FModelContour->SetContourModleEnabled(false);
	//}
}
int ASTransContour::GetCrntContourID()
{
	return ms_SelfPointer->m_FModelContour->GetCurrentContourID();
}
// 得到当前Contour数据
ASContourData* ASTransContour::GetCurrentContourData()
{
	ASAbstractData* CrntDataNode = ASTransSidebarGeneral::GetCrntDataNode();
	if (CrntDataNode && CrntDataNode->isTypeInherited(DataType_ContourData))
	{
		return static_cast<ASContourData*>(CrntDataNode);
	}
	return nullptr;
}
// 新建轮廓线文件
void ASTransContour::NewContourData(ASAbstractData* pParent)
{
	// 1 输入标定的名称
	//ASDialogNewElement* dia = new ASDialogNewElement(ASMainWidget::GetMainWidget(), enContour);
	//dia->exec();
	//if (dia->m_result.size() == 0)
	//{
	//	return;
	//}
	//QString NewName = dia->m_result;
	//if (NewName.size() == 0)
	//{
	//	return;
	//}
	QString NewName = "Contour";
	QList<structCompleterItem*>* pList = ASTransCompleter::GetStructureList();
	int contourid = -1;
	QColor c(Qt::red);
	for (int index = 0; index < pList->size(); index++)
	{
		if (pList->at(index)->name == NewName)
		{
			contourid = pList->at(index)->id;
			c = pList->at(index)->color;
			break;
		}
	}

	ASContourData* ContourData;
	if (pParent->isTypeInherited(DataType_ContourData))
	{
		ContourData = static_cast<ASContourData*>(pParent);
	}
	else
	{
		ContourData = static_cast<ASContourData*>(pParent->getChild(DataType_ContourData));
	}	
	if (!ContourData)
	{
		ContourData = new ASContourData();
		ContourData->setName(tr("Contour data"));
		ContourData->setShown(true);
		ASDataTree::NewDataNode(ContourData, pParent);
		if (!ContourData)
		{
			qDebug("Error: Null pointer of current ContourData. From ASPaintingContourWidget.");
			return;
		}
	}
	int id = ContourData->NewContour(NewName, contourid, c);
	if (id == -1)
	{
		qDebug("error: Bad new seed id in ASPaintingSeedsWidget.cpp");
		return;
	}
	ASTransSidebarGeneral::SetCrntDataNode(ContourData);
	ASTransContour::SetCrntContourID(id);
	// 刷新
	ASTransContour::UpdateListsOfContourData();
	// 使能
	ASTransContour::SetModelEnabled(true);
	
}
void ASTransContour::NewContourData(ASAbstractData* pParent, QString name, int label, QColor color)
{
	// 判断是否需要新建轮廓线文件
	ASContourData* ContourData = static_cast<ASContourData*>(pParent->getChild(DataType_ContourData));
	if (!ContourData)
	{
		ContourData = new ASContourData();
		ContourData->setName(tr("Contour data"));
		ContourData->setShown(true);
		ASDataTree::NewDataNode(ContourData, pParent);
		if (!ContourData)
		{
			qDebug("Error: Null pointer of current ContourData. From ASPaintingContourWidget.");
			return;
		}
	}
	int id = ContourData->NewContour(name, label, color);
	if (id == -1)
	{
		qDebug("error: Bad new seed id in ASPaintingSeedsWidget.cpp");
		return;
	}
	ASTransSidebarGeneral::SetCrntDataNode(ContourData);
	ASTransContour::SetCrntContourID(id);
	// 刷新
	ASTransContour::UpdateListsOfContourData();
	// 使能
	ASTransContour::SetModelEnabled(true);
}
// 刷新轮廓线相关列表
void ASTransContour::UpdateListsOfContourData()
{
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ContourData);
}
// 初始化轮廓线管线
void ASTransContour::InitContourPipeline()
{
	ms_SelfPointer->m_FModelContour->InitContourPipeline();
	ms_SelfPointer->m_FModelContour->ReSetCtrlRadius(0);
	ms_SelfPointer->m_FModelContour->ReSetCtrlRadius(1);
	ms_SelfPointer->m_FModelContour->ReSetCtrlRadius(2);
}
// 设置模块是否使能
void ASTransContour::SetModelEnabled(const bool c_enable)
{
	ms_SelfPointer->m_FModelContour->SetContourModleEnabled(c_enable);
	emit ms_SelfPointer->signalSetModelEnabled(c_enable);
	if (c_enable)
	{
		if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_PaintingContour)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_PaintingContour);
		}
	}
	else
	{
		if (ASInteractorStyleManager::GetInteractorStyle2D() == InterStyle2D_PaintingContour)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_General);
		}
	}
}
// 轮廓线是否显示
void ASTransContour::SetContourShown(const bool c_Shown)
{
	ms_SelfPointer->m_FModelContour->SetContourShown(c_Shown);
}
// 设置交互方式。0, 普通; 1, 触控
void ASTransContour::SetInterfaceModel(const int c_model)
{
	ms_SelfPointer->m_FModelContour->SetInterfaceModel(c_model);
}
int ASTransContour::GetInterfaceModel()
{
	return ms_SelfPointer->m_FModelContour->GetInterfaceModel();
}
// 刷新轮廓线列表
void ASTransContour::RefreshContourList()
{
	ms_SelfPointer->m_FModelContour->SetIDOfPaintingData(-1);
}
// 设置当前正在绘制的轮廓线数据的ID
void ASTransContour::SetPaintingContourID(const int c_ID)
{
	ms_SelfPointer->m_FModelContour->SetIDOfPaintingData(c_ID);
}
// 样条操作。operate: 0-2：添加（按下，移动，闭合）; 3-4: 移动（按下，移动）; 5: 删除; 6-7: 插入（按下，移动）; 8: 刷新(缩放时) ; 9: 删除整条轮廓
void ASTransContour::SplineContourOperation(double* pos, int viewlabel, int operate, vtkActor* actor)
{
	static int s_GeneratingID = -1;
	static int s_GeneratingContourID = -1;
	static int s_GeneratingViewLabel = -1;
	static int s_GeneratingIndexOfContour = -1;
	static int s_GeneratingIndexOfHandle = -1;

	double ImageSpacing[3];
	ASVisualizationManager::GetDisplaySpacing(ImageSpacing);
	switch (operate)
	{
	case CtrOpe_Addtemp:
	{
		// 添加临时控制点
		ms_SelfPointer->m_FModelContour->AddContrlPointsTemp(pos, viewlabel, ImageSpacing);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	case CtrOpe_Movetemp:
	{
		// 添加控制点的过程中平移最后一个控制点
		ms_SelfPointer->m_FModelContour->MoveContrlPoints(pos, viewlabel, ImageSpacing);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	case CtrOpe_Deletetemp:
	{
		// 添加控制点的过程中删除最后一个控制点
		ms_SelfPointer->m_FModelContour->DeleteContrlPointsTemp(pos, viewlabel, ImageSpacing);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	case CtrOpe_Closetemp:
	{
		// 试图闭合轮廓，如果点击的是未闭合轮廓的第一个控制点则可以成功闭合
		if (ms_SelfPointer->m_FModelContour->ReserchHandleSplineContour(
			actor, &(ms_SelfPointer->ID_Closetemp), &(ms_SelfPointer->ContourID_Closetemp), 
			&(ms_SelfPointer->viewLabel_Closetemp),
			&(ms_SelfPointer->indexOfContour_Closetemp), &(ms_SelfPointer->indexOfHandle_Closetemp)))
		{
			if (ms_SelfPointer->viewLabel_Closetemp != viewlabel)
			{
				return;
			}
			// 添加控制点的过程中删除最后一个控制点
			ms_SelfPointer->m_FModelContour->DeleteContrlPointsTemp(pos, viewlabel, ImageSpacing);
			// 闭合轮廓线
			ms_SelfPointer->m_FModelContour->SetContourClosed(ms_SelfPointer->ID_Closetemp, ms_SelfPointer->viewLabel_Closetemp, ms_SelfPointer->indexOfContour_Closetemp, ms_SelfPointer->indexOfHandle_Closetemp, true);
			// 刷新数据
			ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
			// 渲染
			ASVisualizationManager2D::Render(viewlabel);
			ms_SelfPointer->Closetemp = true;
		}
		break;
	}
	case CtrOpe_ClosetempStop:
	{
		if (ms_SelfPointer->Closetemp == false)
		{
			return;
		}
		// 取消闭合轮廓线
		ms_SelfPointer->m_FModelContour->SetContourClosed(ms_SelfPointer->ID_Closetemp, ms_SelfPointer->viewLabel_Closetemp, ms_SelfPointer->indexOfContour_Closetemp, ms_SelfPointer->indexOfHandle_Closetemp, false);
		// 添加临时控制点
		ms_SelfPointer->m_FModelContour->AddContrlPointsTemp(pos, viewlabel, ImageSpacing);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		ms_SelfPointer->Closetemp = false;
		break;
	}
	case CtrOpe_Add:
	{
		// 添加控制点
		ms_SelfPointer->m_FModelContour->AddContrlPoints(pos, viewlabel, ImageSpacing);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	case CtrOpe_Close:
	{
		if (ms_SelfPointer->Closetemp == false)
		{
			return;
		}
		// 添加临时控制点
		ms_SelfPointer->m_FModelContour->AddContrlPointsTemp(pos, viewlabel, ImageSpacing);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		ms_SelfPointer->Closetemp = false;
		ms_SelfPointer->ID_Closetemp = -1;
		ms_SelfPointer->indexOfContour_Closetemp = -1;
		break;
	}
	case CtrOpe_MoveChoose:
	{
		// 准备平移控制点
		ms_SelfPointer->m_FModelContour->ReserchHandleSplineContour(actor, &s_GeneratingID, 
			&s_GeneratingContourID, &s_GeneratingViewLabel, &s_GeneratingIndexOfContour, 
			&s_GeneratingIndexOfHandle);
		break;
	}
	case CtrOpe_Moving:
	{
		// 平移控制点
		ms_SelfPointer->m_FModelContour->MoveContrlPoints(pos, viewlabel, ImageSpacing, s_GeneratingID, 
			s_GeneratingContourID, s_GeneratingIndexOfContour, s_GeneratingIndexOfHandle);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	case CtrOpe_Delete:
	{
		// 删除控制点
		int ID;
		int ContourID;
		int viewLabel;
		int indexOfContour;
		int indexOfHandle;
		if (ms_SelfPointer->m_FModelContour->ReserchHandleSplineContour(actor, &ID, &ContourID, &viewLabel, &indexOfContour, &indexOfHandle))
		{
			if (viewLabel != viewlabel)
			{
				return;
			}
			// 删除控制点
			ms_SelfPointer->m_FModelContour->DeleteContrlPoints(viewlabel, ImageSpacing, ID, ContourID, 
				indexOfContour, indexOfHandle);
			// 刷新数据
			ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
			// 渲染
			ASVisualizationManager2D::Render(viewlabel);
		}
		break;
	}
	case CtrOpe_InsertStart:
	{
		// 插入控制点，按下
		ms_SelfPointer->m_FModelContour->GetInsertHandleIndex(
			pos, viewlabel, &s_GeneratingID, &s_GeneratingIndexOfContour, &s_GeneratingIndexOfHandle);
		// 插入控制点
		ms_SelfPointer->m_FModelContour->InsertContrlPoints(
			pos, viewlabel, ImageSpacing, s_GeneratingID, s_GeneratingIndexOfContour, s_GeneratingIndexOfHandle);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	case CtrOpe_InsertContinue:
	{
		// 插入控制点，移动
		// 删除控制点
		ms_SelfPointer->m_FModelContour->DeleteContrlPoints(viewlabel, ImageSpacing, s_GeneratingID, 
			s_GeneratingContourID, s_GeneratingIndexOfContour, s_GeneratingIndexOfHandle + 1);
		// 插入控制点
		ms_SelfPointer->m_FModelContour->InsertContrlPoints(pos, viewlabel, ImageSpacing, s_GeneratingID, s_GeneratingIndexOfContour, s_GeneratingIndexOfHandle);
		// 刷新数据
		ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	case CtrOpe_Refresh:
	{
		// 刷新数据
		ms_SelfPointer->m_FModelContour->ReSetCtrlRadius(viewlabel);
		// 渲染
		ASVisualizationManager2D::Render(viewlabel);
		break;
	}
	case CtrOpe_DeleteWhole:
	{
		int ID;
		int ContourID;
		int viewLabel;
		int indexOfContour;
		int indexOfHandle;
		if (ms_SelfPointer->m_FModelContour->ReserchHandleSplineContour(actor, &ID, &ContourID, &viewLabel, 
			&indexOfContour, &indexOfHandle))
		{
			if (viewLabel != viewlabel)
			{
				return;
			}
			// 闭合轮廓线
			ms_SelfPointer->m_FModelContour->DeleteContour(ID, ContourID, viewLabel, indexOfContour, indexOfHandle);
			// 刷新数据
			ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(viewlabel);
			// 渲染
			ASVisualizationManager2D::Render(viewlabel);
		}
		break;
	}
	default:
		break;
	}
}
// 判断是否为第一个控制点
bool ASTransContour::CheckFirstCtrlPoint(vtkActor* pActor, int viewlabel)
{
	int ID;
	int ContourID;
	int viewLabel;
	int indexOfContour;
	int indexOfHandle;
	if (ms_SelfPointer->m_FModelContour->ReserchHandleSplineContour(pActor, &ID, &ContourID, &viewLabel, 
		&indexOfContour, &indexOfHandle))
	{
		if (viewLabel == viewlabel && indexOfHandle == 0)
		{
			ASContourData* crntData = static_cast<ASContourData*>(ASAbstractData::getDataNodeFromID(ID));
			structContour* crntContour = crntData->GetContourFromID(ContourID);
			structCurve* crntCurve = crntContour->pListCurves->at(indexOfContour);

			if (crntCurve->isClosure == false)
			{
				return true;			
			}
			else
			{
				if (ID == ms_SelfPointer->ID_Closetemp && indexOfContour == ms_SelfPointer->indexOfContour_Closetemp)
				{
					return true;
				}
			}
		}
	}
	return false;
}
// 刷新
void ASTransContour::RefreshContourDisplay()
{
	ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(0);
	ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(1);
	ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(2);
	ASVisualizationManager2D::Render();
}
// 从硬盘读文件
void ASTransContour::LoadContourDataFromDisk(const QStringList c_FileNameList)
{
	for (int nit = 0; nit < c_FileNameList.size(); nit++)
	{
		ms_SelfPointer->m_FModelContour->LoadContourData(c_FileNameList.at(nit));
	}
}
// 视图缩放
void ASTransContour::ViewZoom(const int c_view)
{
	SplineContourOperation(nullptr, c_view, CtrOpe_Refresh);
}
// 移除轮廓线
void ASTransContour::RemoveContour(const int c_ContourDataNodeID, const int c_ContourID)
{
	// 删除轮廓线
	ms_SelfPointer->m_FModelContour->DeleteWholeContour(c_ContourDataNodeID, c_ContourID);
	// 刷新显示
	ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(0);
	ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(1);
	ms_SelfPointer->m_FModelContour->RefreshSplineContourDisplay(2);
	// 渲染
	ASVisualizationManager2D::Render();
	// 刷新数据列表
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ContourData);
}