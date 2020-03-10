#include "ASDataTree.h"
#include "ASDataManagerWidget.h"
#include "ASDataManagerView.h"
#include "ASDataManagerModel.h"
#include "ASDataManagerDelegate.h"
#include "ASDataNodeItemWidget.h"
#include "ASMarkEleItemWidget.h"
#include "ASTransAddDeleteData.h"
#include "ASTransDICOMLoader.h"
#include "ASTransGeneralInteractor.h"
#include "ASTransDataGeneral.h"
#include "ASImageData.h"
#include "ASArrayImageData.h"
#include "ASColorMap.h"
#include "ASTransAddins.h"
#include "ASTransSidebarGeneral.h"
#include "ASContourData.h"
#include "ASContourItemWidget.h"
#include "ASDataNodeItemWidget.h"
#include "ASTransContour.h"
#include "ASTransGlobalKeyBoard.h"
#include "ASFModelContour.h"
#include "ASSeedData.h"
#include "ASTransSeeds.h"
#include "ASTransLandmark.h"
#include "ASSeedItemWidget.h"
#include "ASFModelSeeds.h"
#include "ASLandmarkData.h"
#include "ASLandMarkItemWidget.h"
#include "ASROIData.h"
#include "ASROIItemWidget.h"
#include "ASROITypeBase.h"
#include "ASTransROI.h"
#include "ASTransTheme.h"
#include <QHBoxLayout>
#include <QModelIndex>

ASDataManagerWidget::ASDataManagerWidget(QWidget *parent)
	: QWidget(parent)
{
	// MVC
	m_pDataManagerView = new ASDataManagerView(this);
	m_pDataManagerModel = new ASDataManagerModel(m_pDataManagerView);
	m_pDataManagerView->setModel(m_pDataManagerModel);
	m_pDataManagerDelegate = new ASDataManagerDelegate(this);
	m_pDataManagerView->setItemDelegate(m_pDataManagerDelegate);
	// layout
	QHBoxLayout* m_pMainLayout = new QHBoxLayout(this);
	m_pMainLayout->addWidget(m_pDataManagerView);
	m_pMainLayout->setContentsMargins(0, 0, 0, 0);

	connect(ASTransAddDeleteData::GetSelfPointer(), SIGNAL(signalUpdateDataNodeLists(int)),
		this, SLOT(slotRefreshDataManagerWidget()));
	connect(m_pDataManagerView, SIGNAL(signalRefreshDataManagerWidget()), 
		this, SLOT(slotRefreshDataManagerWidget()));
	connect(ASTransTheme::GetSelfPointer(), SIGNAL(signalSidebarChange(bool)),
		this, SLOT(slotRefreshDataManagerWidget()));
	connect(m_pDataManagerModel, SIGNAL(signalDragDropFinished(int, int, const QModelIndex*)), 
		this, SLOT(slotDragDropFinished(int, int, const QModelIndex*)));
	connect(m_pDataManagerView, SIGNAL(selectionChanged(const QItemSelection&, const QItemSelection&)), 
		this, SLOT(slotCrntItemSelectionChanged(const QItemSelection&, const QItemSelection&)));
	connect(ASTransDataGeneral::GetSelfPointer(), SIGNAL(signalRefreshCrntData()), 
		this, SLOT(slotRefreshCrntData()));
	connect(m_pDataManagerView, SIGNAL(signalItemClicked(QStandardItem*)), 
		this, SLOT(slotItemClicked(QStandardItem*)));
	connect(ASTransGeneralInteractor::GetSelfPointer(), SIGNAL(signalDataShownChanged(ASAbstractData*)),
		this, SLOT(slotDataShownChanged(ASAbstractData*)));
	connect(ASTransGlobalKeyBoard::GetSelfPointer(), SIGNAL(signalGlobalKeyBoard(QKeyEvent*)),
		this, SLOT(slotGlobalKeyBoard(QKeyEvent*)));
}

ASDataManagerWidget::~ASDataManagerWidget()
{
}

// 刷新文件树显示
void ASDataManagerWidget::slotRefreshDataManagerWidget()
{
	// 清空列表树
	m_pDataManagerModel->RemoveAllItem();

	// 填充树
	ASAbstractData* pRootDataNote = ASDataTree::getRootDataNote();
	QList<ASAbstractData*>* DatalistOfRootChild = pRootDataNote->getChild();
	foreach(ASAbstractData* ChildNote, *DatalistOfRootChild)
	{
		addNoteDisplay(ChildNote, -1, nullptr);
	}
	m_pDataManagerView->expandAll();
}

//拖放操作完成
void ASDataManagerWidget::slotDragDropFinished(int row, int column, const QModelIndex* parent)
{
	bool isNeedRefresh = false;
	// 1 得到被拖动的item的ID
	int startID;
	QStandardItem* StartItem = m_pDataManagerView->getDragDropStartItem();	
	ASAbstructItemWidget* StartItemWidget = m_pDataManagerModel->getItemWidgetFromItem(StartItem);
	startID = StartItemWidget->GetID();
	// 2 得到该Item被拖动到的位置，父节点的ID和为该ID的第几个孩子
	int FaterID;
	int endPos; // 该位置是原来节点删除之前的位置，所以数据树在操作时需要先插入节点，后删除节点。
	if (parent->model() == nullptr)
	{
		FaterID = -1;
		endPos = row;
		if (endPos == -1)
		{
			endPos = m_pDataManagerModel->rowCount() - 1;
			isNeedRefresh = true;
		}
		QModelIndex index2 = m_pDataManagerModel->index(row, column);
		m_pDataManagerView->setIndexWidget(index2, StartItemWidget);
		StartItemWidget->SetListWidgetItem(m_pDataManagerModel->itemFromIndex(index2));
	}
	else
	{
		QStandardItem* fatherItem = m_pDataManagerModel->item(parent->row(), parent->column());
		FaterID = m_pDataManagerModel->getItemWidgetFromItem(fatherItem)->GetID();
		endPos = row;
		if (row == -1)
		{
			row = m_pDataManagerModel->rowCount(*parent) - 1;
		}
		if (column == -1)
		{
			column = 0;
		}
		QModelIndex index2 = parent->child(row, column);
		m_pDataManagerView->setIndexWidget(index2, StartItemWidget);
		StartItemWidget->SetListWidgetItem(m_pDataManagerModel->itemFromIndex(index2));
	}
	// 3 如果父节点不同则拖动无效
	ASAbstractData* crntData = ASAbstractData::getDataNodeFromID(startID);
	if (crntData->getFather()->getDataNodeID() != FaterID)
	{
		// 这时，与qtreeview的默认操作不符，需要在更新数据后对树进行刷新。
		slotRefreshDataManagerWidget();
		return;
	}
	// 更新数据，需要的话刷新列表
	ASTransAddDeleteData::MoveDataNode(startID, FaterID, endPos);
	if (isNeedRefresh)
	{
		// 当释放位置在列表的空白处，即应该移动到树的最下面时我们把该节点移动成为最后一个根节点，
		// 这时，与qtreeview的默认操作不符，需要在更新数据后对树进行刷新。
		slotRefreshDataManagerWidget();
	}
}
// 当前选中item发生改变
void ASDataManagerWidget::slotCrntItemSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected)
{
	//QModelIndexList indexes = selected.indexes();
	//foreach(const QModelIndex &ModelIndex, indexes)
	//{
	//	QStandardItem* itemselected = m_pDataManagerModel->itemFromIndex(ModelIndex);
	//	structItemInListWidget* crntItemStruct = m_pDataManagerModel->getItemInListWidgetFromItem(itemselected);
	//	if (crntItemStruct == nullptr)
	//	{
	//		global_CrntDataManagerSelectionID = -1;
	//		if (global_InteractorType == EMInteractorTypeAddSeeds || global_InteractorType == EMInteractorTypePaintContour)
	//		{
	//			global_InteractorType = EMInteractorTypeDefault;
	//			emit signalRefreshSeedsCuntourPushButtonState();
	//		}
	//	}
	//	else
	//	{
	//		global_CrntDataManagerSelectionID = crntItemStruct->pListItemWidget->getID();
	//	}
	//}
}
// 刷新当前数据显示，用于切换当前数据时
void ASDataManagerWidget::slotRefreshCrntData()
{
	// 遍历
	QList<ASAbstructItemWidget*>* plistItemWidgets = m_pDataManagerModel->getListItemInListWidget();
	int numOfItem = plistItemWidgets->size();
	for (int i = 0; i < numOfItem; i++)
	{
		ASAbstructItemWidget* crntItemWidget = plistItemWidgets->at(i);
		ASAbstractData* GeneratingData = ASTransDataGeneral::GetGeneratingData();
		int genid = -1;
		if (GeneratingData)
		{
			genid = GeneratingData->getDataNodeID();
		}
		if (crntItemWidget->GetID() == genid)
		{
			crntItemWidget->SetBoldWithUpdate(true);
		} 
		else
		{
			crntItemWidget->SetBoldWithUpdate(false);
		}		
	}
}
// 单击切换当前工具
void ASDataManagerWidget::slotItemClicked(QStandardItem* item)
{
	ASAbstructItemWidget* crntItemWidget = m_pDataManagerModel->getItemWidgetFromItem(item);
	int crntID = crntItemWidget->GetID();
	ASAbstractData* datanode = ASAbstractData::getDataNodeFromID(crntID);
	ASTransSidebarGeneral::SetCrntDataNode(datanode);
	switch (datanode->getDataType())
	{
	case DataType_SeedData:
		ASTransSeeds::SetModelEnabled(true);
		break;
	case DataType_ContourData:
		ASTransContour::SetModelEnabled(true);
		break;
	case DataType_LandmarkData:
		ASTransLandmark::SetModelEnabled(true);
		break;
	case DataType_Operation:
		ASTransAddins::SetCrntOperation(datanode);
		ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_Operation, true);
		break;
	case DataType_ROI:
		ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_ROI, true);
		break;
	case DataType_PolyImageData:
		ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_PolyDeformation, true);
		break;
	default:
		break;
	}
	if (crntItemWidget->IsMarkEle())
	{
		ASMarkEleItemWidget* crntItem = static_cast<ASMarkEleItemWidget*>(crntItemWidget);
		if (crntItem->GetFocus() == false)
		{
			this->ResetMarkEleFocus(crntItem->GetItemType());
			int MarkEleID = crntItemWidget->getMarkEleID();
			switch (datanode->getDataType())
			{
			case DataType_SeedData:
				ASTransSeeds::SetCrntSeedsID(MarkEleID);
				break;
			case DataType_ContourData:
				ASTransContour::SetCrntContourID(MarkEleID);
				break;
			case DataType_ROI:
				ASTransROI::SetCrntROIID(MarkEleID);
				break;
			}
			crntItem->SetFocus(true);
		}
	}
}
// 数据是否显示主动改变
void ASDataManagerWidget::slotDataShownChanged(ASAbstractData* DataNode)
{
	ASAbstructItemWidget* crntWidget = m_pDataManagerModel->getItemWidgetFromID(DataNode->getDataNodeID());
	if (crntWidget->GetChecked() != DataNode->isShown())
	{
		crntWidget->SetChecked(DataNode->isShown());
	}	
}
// 接收全局键盘信号
void ASDataManagerWidget::slotGlobalKeyBoard(QKeyEvent* pKeyEvent)
{
	if (ASFModelContour::GetContourModleEnabled())
	{
		int num = this->GetNumFromKeyEvent(pKeyEvent);
		QList<ASAbstructItemWidget*>* ListItemWidget = m_pDataManagerModel->getListItemInListWidget();
		int NumOfWidget = ListItemWidget->size();
		for (int i = 0; i < NumOfWidget; i++)
		{
			if (ListItemWidget->at(i)->GetItemType() == ItemType_Contour)
			{
				ASMarkEleItemWidget* crntWidget = static_cast<ASMarkEleItemWidget*>(ListItemWidget->at(i));
				if (crntWidget->GetNo() == num)
				{
					this->ResetMarkEleFocus(ItemType_Contour);
					crntWidget->SetFocus(true);
				}				
			}
		}
	}
	else if (ASFModelSeeds::GetSeedsModleEnabled())
	{
		int num = this->GetNumFromKeyEvent(pKeyEvent);
		QList<ASAbstructItemWidget*>* ListItemWidget = m_pDataManagerModel->getListItemInListWidget();
		int NumOfWidget = ListItemWidget->size();
		for (int i = 0; i < NumOfWidget; i++)
		{
			if (ListItemWidget->at(i)->GetItemType() == ItemType_Seed)
			{
				ASMarkEleItemWidget* crntWidget = static_cast<ASMarkEleItemWidget*>(ListItemWidget->at(i));
				if (crntWidget->GetNo() == num)
				{
					this->ResetMarkEleFocus(ItemType_Seed);
					crntWidget->SetFocus(true);
				}
			}
		}
		// Ctrl+z撤销
		if (pKeyEvent->modifiers() == Qt::ControlModifier && pKeyEvent->key() == Qt::Key_Z)
		{
			ASTransSeeds::UndoPaintingSedds();
		}
	}
}

// 辅助函数，刷新数据列表显示时先根遍历树的迭代函数，输入父节点id，-1表示无父节点（父节点为根节点）
void ASDataManagerWidget::addNoteDisplay(ASAbstractData* note, int IDOfFather, QStandardItem* fatherItem)
{
	ASDataNodeItemWidget* crntItemWidget = new ASDataNodeItemWidget(this);
	crntItemWidget->SetID(note->getDataNodeID());
	ASAbstractData* GeneratingData = ASTransDataGeneral::GetGeneratingData();
	if (GeneratingData != nullptr && note->getDataNodeID() == GeneratingData->getDataNodeID())
	{
		crntItemWidget->SetBold(true);
	}
	crntItemWidget->SetChecked(note->isShown());
	crntItemWidget->SetName(note->getName());		
	//switch (note->getDataType())
	//{
	//case DataType_ArrayImageData:
	//	crntItemWidget->SetIcon(QPixmap(":/Icon/TypeOfData_Array"));
	//	break;
	//case DataType_DICOMData:
	//	crntItemWidget->SetIcon(QPixmap(":/Icon/TypeOfData_Array"));
	//	break;
	//case DataType_PolyImageData:
	//	crntItemWidget->SetIcon(QPixmap(":/Icon/TypeOfData_PolyData"));
	//	break;
	//case DataType_DICOMRTData:
	//	crntItemWidget->SetIcon(QPixmap(":/Icon/TypeOfData_DICOMRT"));
	//	break;
	//case DataType_SeedData:
	//	crntItemWidget->SetIcon(QPixmap(":/Icon/TypeOfData_Bands"));
	//	break;
	//case DataType_ContourData:
	//	crntItemWidget->SetIcon(QPixmap(":/Icon/TypeOfData_Contour"));
	//	break;
	//case DataType_LandmarkData:
	//	crntItemWidget->SetIcon(QPixmap(":/Icon/TypeOfData_Landmark"));
	//	break;
	//case DataType_ROI:
	//	crntItemWidget->SetIcon(QPixmap(":/Icon/ToolBar_ROI"));
	//	break;
	//default:
	//	break;
	//}
	crntItemWidget->SetSizeHWithUpdate(m_pDataManagerDelegate->getCrntH()); // 在setIcon之后
	m_pDataManagerModel->addItemWidget(crntItemWidget);
	

	this->refreshMarkEleNode(note, crntItemWidget->GetListWidgetItem());

	if (fatherItem == nullptr)
	{
		m_pDataManagerModel->appendRow(crntItemWidget->GetListWidgetItem());
		QModelIndex index = m_pDataManagerModel->indexFromItem(crntItemWidget->GetListWidgetItem());
		m_pDataManagerView->setIndexWidget(index, crntItemWidget);
		m_pDataManagerView->expand(index);
	}
	else
	{
		fatherItem->appendRow(crntItemWidget->GetListWidgetItem());
		QModelIndex index = m_pDataManagerModel->indexFromItem(crntItemWidget->GetListWidgetItem());
		m_pDataManagerView->setIndexWidget(index, crntItemWidget);
		m_pDataManagerView->expand(index);
	}
	QList<ASAbstractData*>* DatalistOfRootChild = note->getChild();
	int numOfRootChild = DatalistOfRootChild->size();
	for (int i = 0; i < numOfRootChild; i++)
	{
		ASAbstractData* crntChildNode = DatalistOfRootChild->at(i);
		addNoteDisplay(crntChildNode, crntChildNode->getDataNodeID(), crntItemWidget->GetListWidgetItem());
	}
}
// 辅助函数，刷新MarkEle条目
void ASDataManagerWidget::refreshMarkEleNode(ASAbstractData* note, QStandardItem* crntItem)
{
	if (note->getDataType() == DataType_ContourData)
	{
		//ASContourData* crntContourData = static_cast<ASContourData*>(note);
		//int numOfContours = crntContourData->GetNumberOfContour();
		//int crntContourID = ASTransContour::GetCrntContourID();
		//for (int i = 0; i < numOfContours; i++)
		//{
		//	structContour* crntContour = crntContourData->GetContourAt(i);
		//	ASContourItemWidget* crntItemWidget = new ASContourItemWidget(nullptr);
		//	m_pDataManagerModel->addItemWidget(crntItemWidget);
		//	crntItemWidget->SetID(crntContourData->getDataNodeID());
		//	crntItemWidget->SetNo(i + 1);
		//	crntItemWidget->SetContourID(crntContour->contourID);
		//	if (crntContourID == crntContour->contourID)
		//	{
		//		crntItemWidget->SetFocus(true);
		//	}
		//	crntItemWidget->SetName(crntContour->name);
		//	crntItemWidget->SetLabel(crntContour->label);
		//	crntItemWidget->SetColor(crntContour->color);
		//	crntItemWidget->SetChecked(crntContour->isShown);
		//	crntItemWidget->SetSizeHWithUpdate(m_pDataManagerDelegate->getCrntH()); // 在setIcon之后
		//	crntItem->appendRow(crntItemWidget->GetListWidgetItem());
		//	QModelIndex index = m_pDataManagerModel->indexFromItem(crntItemWidget->GetListWidgetItem());
		//	m_pDataManagerView->setIndexWidget(index, crntItemWidget);
		//}
	}
	else if (note->getDataType() == DataType_SeedData)
	{
		ASSeedData* crntSeedData = static_cast<ASSeedData*>(note);
		int numOfSeeds = crntSeedData->getNumberOfSeeds();
		int crntSeedID = ASTransSeeds::GetCrntSeedsID();
		for (int i = 0; i < numOfSeeds; i++)
		{
			structSeeds* crntSeed = crntSeedData->getSeedsAt(i);
			ASSeedItemWidget* crntItemWidget = new ASSeedItemWidget(nullptr);
			m_pDataManagerModel->addItemWidget(crntItemWidget);
			crntItemWidget->SetID(crntSeedData->getDataNodeID());
			crntItemWidget->SetNo(i + 1);
			crntItemWidget->SetSeedID(crntSeed->seedId);
			if (crntSeedID == crntSeed->seedId)
			{
				crntItemWidget->SetFocus(true);
			}
			crntItemWidget->SetName(crntSeed->name);
			crntItemWidget->SetLabel(crntSeed->label);
			crntItemWidget->SetColor(crntSeed->color);
			crntItemWidget->SetChecked(crntSeed->isShown);
			crntItemWidget->SetSizeHWithUpdate(m_pDataManagerDelegate->getCrntH()); // 在setIcon之后
			crntItem->appendRow(crntItemWidget->GetListWidgetItem());
			QModelIndex index = m_pDataManagerModel->indexFromItem(crntItemWidget->GetListWidgetItem());
			m_pDataManagerView->setIndexWidget(index, crntItemWidget);
		}
	}
	else if (note->getDataType() == DataType_LandmarkData)
	{
		ASLandmarkData* crntLandmarkData = static_cast<ASLandmarkData*>(note);
		int numOfLandmarks = crntLandmarkData->GetNumberOfLandmark();
		int crntLandmarkID = ASTransSeeds::GetCrntSeedsID();
		for (int i = 0; i < numOfLandmarks; i++)
		{
			structLandmark* crntLandmark = crntLandmarkData->GetLandmarkAt(i);
			ASLandMarkItemWidget* crntItemWidget = new ASLandMarkItemWidget(nullptr);
			m_pDataManagerModel->addItemWidget(crntItemWidget);
			crntItemWidget->SetID(crntLandmarkData->getDataNodeID());
			crntItemWidget->SetNo(i + 1);
			crntItemWidget->SetLandmarkID(crntLandmark->LandmarkID);
			if (crntLandmarkID == crntLandmark->LandmarkID)
			{
				crntItemWidget->SetFocus(true);
			}
			crntItemWidget->SetName(crntLandmark->qsName);
			crntItemWidget->SetColor(crntLandmark->color);
			crntItemWidget->SetSizeHWithUpdate(m_pDataManagerDelegate->getCrntH()); // 在setIcon之后
			crntItem->appendRow(crntItemWidget->GetListWidgetItem());
			QModelIndex index = m_pDataManagerModel->indexFromItem(crntItemWidget->GetListWidgetItem());
			m_pDataManagerView->setIndexWidget(index, crntItemWidget);
		}
	}
	else if (note->getDataType() == DataType_ROI)
	{
		ASROIData* crntROIData = static_cast<ASROIData*>(note);
		int numOfROI = crntROIData->GetNumOfROIs();
		int crntROIID = ASTransROI::GetCrntROIID();
		for (int i = 0; i < numOfROI; i++)
		{
			ASROITypeBase* crntROI = crntROIData->GetROIAt(i);
			ASROIItemWidget* crntItemWidget = new ASROIItemWidget(nullptr);
			m_pDataManagerModel->addItemWidget(crntItemWidget);
			crntItemWidget->SetID(crntROIData->getDataNodeID());
			crntItemWidget->SetNo(i + 1);
			crntItemWidget->SetROIID(crntROI->GetROIID());
			if (crntROIID == crntROI->GetROIID())
			{
				crntItemWidget->SetFocus(true);
			}
			crntItemWidget->SetName(crntROI->GetName());
			crntItemWidget->SetColor(crntROI->GetColor());
			crntItemWidget->SetChecked(crntROI->isShown());
			crntItemWidget->SetSizeHWithUpdate(m_pDataManagerDelegate->getCrntH()); // 在setIcon之后
			crntItem->appendRow(crntItemWidget->GetListWidgetItem());
			QModelIndex index = m_pDataManagerModel->indexFromItem(crntItemWidget->GetListWidgetItem());
			m_pDataManagerView->setIndexWidget(index, crntItemWidget);
		}
	}
}
// 辅助函数，取消某类MarkEle的选择
void ASDataManagerWidget::ResetMarkEleFocus(const int c_Type)
{
	QList<ASAbstructItemWidget*>* WidgetList = m_pDataManagerModel->getListItemInListWidget();
	int size = WidgetList->size();
	for (int i = 0; i < size; i++)
	{
		if (WidgetList->at(i)->GetItemType() == c_Type)
		{
			ASMarkEleItemWidget* crntWidget = static_cast<ASMarkEleItemWidget*>(WidgetList->at(i));
			crntWidget->SetFocus(false);
		}
	}
}
// 辅助函数，从QKeyEvent中得到数字
int ASDataManagerWidget::GetNumFromKeyEvent(QKeyEvent* pKeyEvent)
{
	switch (pKeyEvent->key())
	{
	case Qt::Key_0:
		return 0;
	case Qt::Key_1:
		return 1;
	case Qt::Key_2:
		return 2;
	case Qt::Key_3:
		return 3;
	case Qt::Key_4:
		return 4;
	case Qt::Key_5:
		return 5;
	case Qt::Key_6:
		return 6;
	case Qt::Key_7:
		return 7;
	case Qt::Key_8:
		return 8;
	case Qt::Key_9:
		return 9;
	default:
		return -1;
	}
}