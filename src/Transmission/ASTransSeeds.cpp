#include "ASTransSeeds.h"
#include "ASDialogNewSeedsFile.h"
#include "ASMainWidget.h"
#include "ASTransAddDeleteData.h"
#include "ASAbstractData.h"
#include "ASSeedData.h"
#include "ASFModelSeeds.h"
#include "ASInteractorStyleManager.h"
#include "ASTransInteractorStyle.h"
#include "ASVisualizationManager.h"
#include "ASDataTree.h"
#include "ASTransDataGeneral.h"
#include "ASDataStructureWalker.h"
#include "ASArrayImageData.h"
#include "ASDialogNewElement.h"
#include "ASTransCompleter.h"
#include "ASTransSidebarGeneral.h"

ASTransSeeds* ASTransSeeds::ms_SelfPointer = nullptr;

ASTransSeeds::ASTransSeeds(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;
	m_FModelSeeds = new ASFModelSeeds();
}

ASTransSeeds::~ASTransSeeds()
{

}

// 唯一对象
ASTransSeeds* ASTransSeeds::GetSelfPointer()
{
	return ms_SelfPointer;
}
//当前SeedsID
void ASTransSeeds::SetCrntSeedsID(const int c_id)
{
	ms_SelfPointer->m_CurrentSeedID = c_id;
	//if (c_id >= 0)
	//{
	//	ms_SelfPointer->m_FModelSeeds->SetSeedsModleEnabled(true);
	//}
	//else
	//{
	//	ms_SelfPointer->m_FModelSeeds->SetSeedsModleEnabled(false);
	//}
}
int ASTransSeeds::GetCrntSeedsID()
{
	return ms_SelfPointer->m_CurrentSeedID;
}
// 得到当前种子点文件
ASSeedData* ASTransSeeds::GetCurrentSeedData()
{
	ASAbstractData* CrntDataNode = ASTransSidebarGeneral::GetCrntDataNode();
	if (CrntDataNode && CrntDataNode->isTypeInherited(DataType_SeedData))
	{
		return static_cast<ASSeedData*>(CrntDataNode);
	}
	return nullptr;
}
// 新建种子点文件
void ASTransSeeds::NewSeedsData(ASAbstractData* pParent)
{
	// 1 输入标定的名称
	ASDialogNewElement* dia = new ASDialogNewElement(ASMainWidget::GetMainWidget(), enSeed);
	dia->exec();
	if (dia->m_result.size() == 0)
	{
		return;
	}
	QString NewName = dia->m_result;
	if (NewName.size() == 0)
	{
		return;
	}
	QList<structCompleterItem*>* pList = ASTransCompleter::GetStructureList();
	int seedid = -1;
	QColor c(Qt::red);
	for (int index = 0; index < pList->size(); index++)
	{
		if (pList->at(index)->name == NewName)
		{
			seedid = pList->at(index)->id;
			c = pList->at(index)->color;
			break;
		}
	}
	// 判断是否需要新建种子点文件
	ASSeedData* SeedData;
	if (pParent->isTypeInherited(DataType_SeedData))
	{
		SeedData = static_cast<ASSeedData*>(pParent);
	}
	else
	{
		SeedData = static_cast<ASSeedData*>(pParent->getChild(DataType_SeedData));
	}
	if (!SeedData)
	{
		SeedData = new ASSeedData();
		SeedData->setName(tr("Seed data"));
		SeedData->setShown(true);
		ASDataTree::NewDataNode(SeedData, pParent);
		if (!SeedData)
		{
			qDebug("Error: Null pointer of current SeedsData. From ASPaintingSeedsWidget.");
			return;
		}
	}
	int id = SeedData->NewSeed(NewName, seedid, c);
	if (id == -1)
	{
		qDebug("error: Bad new seed id in ASPaintingSeedsWidget.cpp");
		return;
	}
	ASTransSidebarGeneral::SetCrntDataNode(SeedData);
	ASTransSeeds::SetCrntSeedsID(id);
	// 刷新
	ASTransSeeds::UpdateListsOfSeedData();
	// 使能
	ASTransSeeds::SetModelEnabled(true);
}
void ASTransSeeds::NewSeedsData(ASAbstractData* pParent, QString name, int label, QColor color)
{
	// 判断是否需要新建种子点文件
	ASSeedData* SeedData = static_cast<ASSeedData*>(pParent->getChild(DataType_SeedData));
	if (!SeedData)
	{
		SeedData = new ASSeedData();
		SeedData->setName(tr("Seed data"));
		SeedData->setShown(true);
		ASDataTree::NewDataNode(SeedData, pParent);
		if (!SeedData)
		{
			qDebug("Error: Null pointer of current SeedsData. From ASPaintingSeedsWidget.");
			return;
		}
	}
	int id = SeedData->NewSeed(name, label, color);
	if (id == -1)
	{
		qDebug("error: Bad new seed id in ASPaintingSeedsWidget.cpp");
		return;
	}
	ASTransSidebarGeneral::SetCrntDataNode(SeedData);
	ASTransSeeds::SetCrntSeedsID(id);
	// 刷新
	ASTransSeeds::UpdateListsOfSeedData();
	// 使能
	ASTransSeeds::SetModelEnabled(true);
}
// 刷新种子点相关列表
void ASTransSeeds::UpdateListsOfSeedData()
{
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_SeedData);
}
// 设置模块是否使能
void ASTransSeeds::SetModelEnabled(const bool c_enable)
{
	ms_SelfPointer->m_FModelSeeds->SetSeedsModleEnabled(c_enable);
	emit ms_SelfPointer->signalSetModelEnabled(c_enable);
	if (c_enable)
	{
		if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_PaintingSeeds)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_PaintingSeeds);
		}
	}
	else
	{
		if (ASInteractorStyleManager::GetInteractorStyle2D() == InterStyle2D_PaintingSeeds)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_General);
		}
	}
}
// 刷新种子点列表
void ASTransSeeds::RefreshSeedsList()
{
	ms_SelfPointer->m_FModelSeeds->SetIDOfPaintingData(-1);
	// 第一次新增种子点文件需要建立种子点显示的管线
	if (ASSeedData::GetNumOfSeedsData() > 0)
	{
		ms_SelfPointer->m_FModelSeeds->InitSeedsPipeline();
	}
}
// 设置当前正在绘制的种子点数据的ID
//void ASTransSeeds::SetPaintingSeedsID(const int c_ID)
//{
//	ms_SelfPointer->m_FModelSeeds->SetIDOfPaintingData(c_ID);
//}
// 绘制种子点
void ASTransSeeds::StartPaintSeeds(double* pPosition, const int c_ViewLabel)
{
	double Extent[6];
	ASVisualizationManager::GetDisplayExtent(Extent);
	if (pPosition[0] < Extent[0] || pPosition[0] > Extent[1])
	{
		return;
	}
	if (pPosition[1] < Extent[2] || pPosition[1] > Extent[3])
	{
		return;
	}
	if (pPosition[2] < Extent[4] || pPosition[2] > Extent[5])
	{
		return;
	}
	// 更新数据
	ms_SelfPointer->m_WorkingSeedData = ASTransSeeds::GetCurrentSeedData();
	ms_SelfPointer->m_WorkingSeedData->AddOperaSequence(ms_SelfPointer->m_CurrentSeedID);

	ms_SelfPointer->m_WorkingSeedData->StartPaintingSeeds(pPosition, c_ViewLabel, ms_SelfPointer->m_CurrentSeedID);
	// 刷新显示
	ms_SelfPointer->m_FModelSeeds->StartPaintingData(pPosition);
	// 渲染
	ASVisualizationManager::Render();
}
void ASTransSeeds::ContinuePaintSeeds(double* pPosition, const int c_ViewLabel)
{
	double Extent[6];
	ASVisualizationManager::GetDisplayExtent(Extent);
	if (pPosition[0] < Extent[0] || pPosition[0] > Extent[1])
	{
		return;
	}
	if (pPosition[1] < Extent[2] || pPosition[1] > Extent[3])
	{
		return;
	}
	if (pPosition[2] < Extent[4] || pPosition[2] > Extent[5])
	{
		return;
	}
	// 更新数据
	ms_SelfPointer->m_WorkingSeedData->ContinuePaintingSeeds(pPosition, c_ViewLabel);
	// 刷新显示
	ms_SelfPointer->m_FModelSeeds->ContinuePaintingData(pPosition);
	// 渲染
	ASVisualizationManager::Render();
}
void ASTransSeeds::EndPaintSeeds()
{
	ms_SelfPointer->m_WorkingSeedData->EndPaintingSeeds();
	ms_SelfPointer->m_WorkingSeedData = nullptr;
}
// 刷新种子点显示
void ASTransSeeds::RefreshSeedsDisplay()
{
	ms_SelfPointer->m_FModelSeeds->RefreshSeedsDisplay();
	// 渲染
	ASVisualizationManager::Render();
}
// 种子点是否显示
void ASTransSeeds::SetSeedShown(const bool c_Shown)
{
	ms_SelfPointer->m_FModelSeeds->SetSeedShown(c_Shown);
}
// 撤销一次
void ASTransSeeds::UndoPaintingSedds()
{
	bool b = ASTransSeeds::GetCurrentSeedData()->UndoALine();
	if (b)
	{
		// 刷新数据
		ms_SelfPointer->m_FModelSeeds->RefreshSeedsDisplay();
		// 渲染
		ASVisualizationManager::Render();
	}
}
// 种子点数据发生变化时，如有需要调整撤销列表。就是说删除种子点文件时，将撤销列表中的对应ID 剔除
void ASTransSeeds::AdjustUndoList()
{
	ms_SelfPointer->m_FModelSeeds->AdjustUndoList();
}
// 从硬盘读文件
void ASTransSeeds::LoadseedDataFromDisk(const QStringList c_FileNameList)
{
	ms_SelfPointer->m_FModelSeeds->InitSeedsPipeline();
	for (int nit = 0; nit < c_FileNameList.size(); nit++)
	{
		ms_SelfPointer->m_FModelSeeds->LoadSeedData(c_FileNameList.at(nit));
	}
}
// 移除轮廓线
void ASTransSeeds::RemoveSeeds(const int c_SeedDataID, const int c_SeedID)
{
	// 移除种子点
	ms_SelfPointer->m_FModelSeeds->DeleteSeeds(c_SeedDataID, c_SeedID);
	// 刷新显示
	ms_SelfPointer->m_FModelSeeds->RefreshSeedsDisplay();
	// 渲染
	ASVisualizationManager::Render();
	// 刷新数据列表
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_SeedData);
}

//void ASTransSeeds::fNewSeedData(QString name, int label, QColor color)
//{
//	// 判断是否需要新建种子点文件
//	ASSeedData* SeedData = ASTransSeeds::GetCurrentSeedData();
//	if (!SeedData)
//	{
//		SeedData = new ASSeedData();
//		SeedData->setName(tr("Seed data"));
//		SeedData->setShown(true);
//		ASDataTree::NewDataNode(SeedData, ASTransDataGeneral::GetGeneratingData());
//		if (!SeedData)
//		{
//			qDebug("Error: Null pointer of current SeedsData. From ASPaintingSeedsWidget.");
//			return;
//		}
//	}
//	int id = SeedData->NewSeed(name, label, color);
//	if (id == -1)
//	{
//		qDebug("error: Bad new seed id in ASPaintingSeedsWidget.cpp");
//		return;
//	}
//
//	ASTransSeeds::SetCrntSeedsID(id);
//	// 刷新
//	ASTransSeeds::UpdateListsOfSeedData();
//	// 使能
//	ASTransSeeds::SetModelEnabled(true);
//}