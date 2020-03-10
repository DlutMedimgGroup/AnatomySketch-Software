#include "ASTransROI.h"
#include "ASTransDataGeneral.h"
#include "ASTransAddDeleteData.h"
#include "ASROIData.h"
#include "ASMainWidget.h"
#include "ASDataTree.h"
#include "ASDataStructureWalker.h"
#include "ASROITypeBase.h"
#include "ASROITypeCube.h"
#include "ASFModelROI.h"
#include "ASDialogNewElement.h"
#include "ASAbstractData.h"
#include "ASArrayImageData.h"
#include "ASTransCompleter.h"
#include "ASInteractorStyleManager.h"
#include "ASTransInteractorStyle.h"
#include "ASTransSidebarGeneral.h"
#include "ASVisualizationManager.h"

ASTransROI* ASTransROI::ms_SelfPointer = nullptr;

ASTransROI::ASTransROI(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;
	m_FModelROI = new ASFModelROI();
}

ASTransROI::~ASTransROI()
{

}
// 唯一对象
ASTransROI* ASTransROI::GetSelfPointer()
{
	return ms_SelfPointer;
}
//当前ROIID
void ASTransROI::SetCrntROIID(const int c_id)
{
	ms_SelfPointer->m_CurrentROIID = c_id;
}
int ASTransROI::GetCrntROIID()
{
	return ms_SelfPointer->m_CurrentROIID;
}
// 得到当前ROI文件
ASROIData* ASTransROI::GetCurrentROIData()
{
	ASAbstractData* CrntDataNode = ASTransSidebarGeneral::GetCrntDataNode();
	if (CrntDataNode && CrntDataNode->isTypeInherited(DataType_ROI))
	{
		return static_cast<ASROIData*>(CrntDataNode);
	}
	return nullptr;
}
// 刷新轮廓线相关列表
void ASTransROI::UpdateListsOfROIData()
{
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ROI);
}
// 新建ROI数据
void ASTransROI::NewROIData(ASAbstractData* pParent)
{	
	// 1 输入标定的名称
	ASDialogNewElement* dia = new ASDialogNewElement(ASMainWidget::GetMainWidget(), enROI);
	dia->exec();
	if (dia->m_result.size() == 0)
	{
		return;
	}
	QList<structCompleterItem*>* pList = ASTransCompleter::GetStructureList();
	QString NewName = dia->m_result;
	//bool bbb = false;
	//for (int index = 0; index < pList->size(); index++)
	//{
	//	if (pList->at(index)->name == NewName)
	//	{
	//		bbb = true;
	//		break;
	//	}
	//}
	//if (bbb == false)
	//{
	//	return;
	//}

	if (NewName.size() == 0)
	{
		return;
	}
	QColor c(Qt::red);
	for (int index = 0; index < pList->size(); index++)
	{
		if (pList->at(index)->name == NewName)
		{
			c = pList->at(index)->color;
			break;
		}
	}

	ASArrayImageData* GenData = static_cast<ASArrayImageData*>(
		ASTransDataGeneral::GetGeneratingData());
	// 1 判断是否需要新建ROI文件
	ASROIData* ROIData;
	if (pParent->isTypeInherited(DataType_ROI))
	{
		ROIData = static_cast<ASROIData*>(pParent);
	}
	else
	{
		ROIData = static_cast<ASROIData*>(pParent->getChild(DataType_ROI));
	}
	if (!ROIData)
	{
		ROIData = new ASROIData();
		ROIData->setName("Bounding box data");
		ROIData->setShown(true);
		ASDataTree::NewDataNode(ROIData, pParent);
		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ROI);
		if (!ROIData)
		{
			qDebug("Error: Null pointer of current ROIData. From ASROIWidget(2).");
			return;
		}
	}

	// 2 新建CubeROI
	ASROITypeBase* roi = ASTransROI::NewROI(ROIData->getDataNodeID(), NewName, ROIType_Cube);
	if (roi->GetROIType() != ROIType_Cube)
	{
		qDebug("Error: Wrong ROI type.");
		return;
	}
	ASROITypeCube* cube = static_cast<ASROITypeCube*>(roi);
	cube->SetRange(GenData->getImageExtent());

	QStringList StrList;
	cube->SetColor(c);
	// 方形ROI的初始范围为全图
	ASTransSidebarGeneral::SetCrntDataNode(ROIData);
	ASTransROI::SetCrntROIID(cube->GetROIID());
	ASTransROI::RefreshROICubeDisplay();
	ASTransROI::UpdateListsOfROIData();
	// 使能
	if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_ROICube)
	{
		ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_ROICube);
	}

}
void ASTransROI::ASTransROI::NewROIData(ASAbstractData* pParent, QString name, int label, QColor color)
{
	ASArrayImageData* GenData = static_cast<ASArrayImageData*>(
		ASTransDataGeneral::GetGeneratingData());
	// 1 判断是否需要新建ROI文件
	ASROIData* ROIData = static_cast<ASROIData*>(pParent->getChild(DataType_ROI));
	if (!ROIData)
	{
		ROIData = new ASROIData();
		ROIData->setName(tr("ROI data"));
		ROIData->setShown(true);
		ASDataTree::NewDataNode(ROIData, pParent);
		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ROI);
		if (!ROIData)
		{
			qDebug("Error: Null pointer of current ROIData. From ASROIWidget(2).");
			return;
		}
	}

	// 2 新建CubeROI
	ASROITypeBase* roi = ASTransROI::NewROI(ROIData->getDataNodeID(), name, ROIType_Cube);
	if (roi->GetROIType() != ROIType_Cube)
	{
		qDebug("Error: Wrong ROI type.");
		return;
	}
	ASROITypeCube* cube = static_cast<ASROITypeCube*>(roi);
	cube->SetRange(GenData->getImageExtent());

	QStringList StrList;
	cube->SetColor(color);
	// 方形ROI的初始范围为全图
	ASTransSidebarGeneral::SetCrntDataNode(ROIData);
	ASTransROI::SetCrntROIID(cube->GetROIID());
	ASTransROI::RefreshROICubeDisplay();
	ASTransROI::UpdateListsOfROIData();
	// 使能
	if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_ROICube)
	{
		ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_ROICube);
	}
}
// 读取ROI数据
void ASTransROI::LoadROIDataFromDisk(const QStringList c_FileNameList)
{
	ms_SelfPointer->m_FModelROI->InitROICubePipeline();
	for (int nit = 0; nit < c_FileNameList.size(); nit++)
	{
		ms_SelfPointer->m_FModelROI->LoadROIData(c_FileNameList.at(nit));
	}
}
// 新建Cube Roi
ASROITypeBase* ASTransROI::NewROI(const int c_ROIId, const QString c_Name, const int c_Type)
{
	ASROIData* crntData = static_cast<ASROIData*>(ASAbstractData::getDataNodeFromID(c_ROIId));
	return crntData->NewROI(c_Name, c_Type);
}
// 移除ROI
void ASTransROI::RemoveROI(const int c_ROIDataId, const int c_ROIId)
{
	ASROIData* crntData = static_cast<ASROIData*>(ASAbstractData::getDataNodeFromID(c_ROIDataId));
	crntData->RemoveROI(c_ROIId);
	// 刷新显示
	ASTransROI::RefreshROICubeDisplay();
	// 刷新数据列表
	ASTransROI::UpdateListsOfROIData();
}
// 刷新ROI列表
void ASTransROI::RefreshROIList()
{
	emit ms_SelfPointer->signalRefreshList();
}
// 交互设置Cube范围
void ASTransROI::SetROICubeRange(const int c_Index, const double c_Value)
{
	ASROIData* CrntROIData = ASTransROI::GetCurrentROIData();
	if (!CrntROIData)
	{
		qDebug("Error: Null pointer of current ROIData. From ASTrsnsROI.");
		return;
	}
	int crntROIId = ASTransROI::GetCrntROIID();
	ASROITypeBase* roi = CrntROIData->GetROIWithID(crntROIId);
	if (!roi)
	{
		qDebug("Error: Null pointer of current ROI. From ASTrsnsROI.");
		return;
	}
	if (roi->GetROIType() != ROIType_Cube)
	{
		qDebug("Error: Wrong ROI type. From ASTrsnsROI.");
		return;
	}
	ASROITypeCube* crntROI = static_cast<ASROITypeCube*>(roi);
	double crntRange[6];
	crntROI->GetRange(crntRange);
	if (c_Index % 2)
	{
		// 奇数最大值
		if (c_Value < crntRange[c_Index - 1])
		{
			return;
		}
	}
	else
	{
		// 偶数最小值
		if (c_Value > crntRange[c_Index + 1])
		{
			return;
		}
	}
	crntROI->SetRange(c_Index, c_Value);
	emit ms_SelfPointer->signalRefreshROICube();
	ms_SelfPointer->m_FModelROI->RefreshROICubeDisplay();
}
// 刷新ROICube显示
void ASTransROI::RefreshROICubeDisplay()
{
	ms_SelfPointer->m_FModelROI->RefreshROICubeDisplay();
}

//void ASTransROI::fNewROIData(QString name, int label, QColor color)
//{
//	ASArrayImageData* GenData = static_cast<ASArrayImageData*>(
//		ASTransDataGeneral::GetGeneratingData());
//	// 1 判断是否需要新建ROI文件
//	ASROIData* ROIData = ASTransROI::GetCurrentROIData();
//	if (!ROIData)
//	{
//		ROIData = new ASROIData();
//		ROIData->setName(tr("ROI data"));
//		ROIData->setShown(true);
//		ASDataTree::NewDataNode(ROIData, ASTransDataGeneral::GetGeneratingData());
//		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ROI);
//		if (!ROIData)
//		{
//			qDebug("Error: Null pointer of current ROIData. From ASROIWidget(2).");
//			return;
//		}
//	}
//
//	// 2 新建CubeROI
//	ASROITypeBase* roi = ASTransROI::NewROI(ROIData->getDataNodeID(), name, ROIType_Cube);
//	if (roi->GetROIType() != ROIType_Cube)
//	{
//		qDebug("Error: Wrong ROI type.");
//		return;
//	}
//	ASROITypeCube* cube = static_cast<ASROITypeCube*>(roi);
//	cube->SetRange(GenData->getImageExtent());
//
//	QStringList StrList;
//	cube->SetColor(color);
//	// 方形ROI的初始范围为全图
//	ASTransROI::SetCrntROIID(cube->GetROIID());
//	ASTransROI::RefreshROICubeDisplay();
//	ASTransROI::UpdateListsOfROIData();
//	// 使能
//	if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_ROICube)
//	{
//		ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_ROICube);
//	}
//}