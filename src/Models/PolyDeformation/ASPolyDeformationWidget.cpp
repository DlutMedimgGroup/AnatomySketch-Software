#include "ASPolyDeformationWidget.h"
#include "ASDataStructureWalker.h"
#include "ASPolyImageData.h"
#include "ASAbstractData.h"
#include "ASTransSidebarGeneral.h"
#include "ASTransPolyDeformation.h"
#include "ASFModelPolyDeformation.h"
#include "ASInteractorStyleManager.h"
#include "ASTransInteractorStyle.h"
#include "ASTransGlobalKeyBoard.h"
#include "ASTransGeneralInteractor.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

ASPolyDeformationWidget::ASPolyDeformationWidget(QWidget *parent)
	: ASWidgetInSideBarBase(parent)
{
	m_listItemOfListWidget = new QList<structPolyDeformationInputDataItem*>;

	//界面初始化
	UIInit();
	this->setStyleSheet("padding: 2px;");
	m_pListWidget ->setObjectName("PolyDeformationWidgetList");
	//信号槽连接
	connect(m_PushButtonUndo, SIGNAL(clicked()), this, SLOT(slotUndoClicked()));

	// 接收Transmission信号
	connect(ASTransGlobalKeyBoard::GetSelfPointer(), SIGNAL(signalGlobalKeyBoard(QKeyEvent*)), this, SLOT(slotGlobalKeyBoard(QKeyEvent*)));
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalRefreshDataComboBox(int)), this, SLOT(slotRefreshDataComboBox(int)));
	connect(ASTransPolyDeformation::GetSelfPointer(), SIGNAL(signalChoosnChanged(int)), this, SLOT(slotChoosnChanged(int)));
	connect(ASTransInteractorStyle::GetSelfPointer(), SIGNAL(signalInteractorStyle2DChanged(int)), this, SLOT(slotInteractorStyle2DChanged(int)));
	connect(ASTransGeneralInteractor::GetSelfPointer(), SIGNAL(signalDataShownChanged(ASAbstractData*)), this, SLOT(slotDataShownChanged(ASAbstractData*)));
}

ASPolyDeformationWidget::~ASPolyDeformationWidget()
{

}

void ASPolyDeformationWidget::SetEnabled(const bool c_bEnabled)
{
	if (c_bEnabled)
	{
		// 使能
		m_pListWidget->setEnabled(true);
		m_PushButtonUndo->setEnabled(true);
		ASTransPolyDeformation::SetEnabled(true);
		if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_PolyDeformation)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_PolyDeformation);
		}
	}
	else
	{
		// 未使能
		m_pListWidget->setEnabled(false);
		m_PushButtonUndo->setEnabled(false);
		ASTransPolyDeformation::SetEnabled(false);
		if (ASInteractorStyleManager::GetInteractorStyle2D() == InterStyle2D_PolyDeformation)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_General);
		}
	}
}

//界面初始化
void ASPolyDeformationWidget::UIInit()
{
	//文件选择
	m_LabelInput = new QLabel("Input:");
	m_LabelInput->setMaximumWidth(40);
	m_pListWidget = new QListWidget();
	m_pListWidget->setEnabled(false);
	//Undo按键
	m_PushButtonUndo = new QPushButton("Undo");
	m_PushButtonUndo->setEnabled(false);

	QVBoxLayout* pMainLayout = new QVBoxLayout();

	pMainLayout->addSpacing(10);
	pMainLayout->addWidget(m_LabelInput);
	pMainLayout->addWidget(m_pListWidget);
	pMainLayout->addSpacing(10);
	pMainLayout->addWidget(m_PushButtonUndo);
	pMainLayout->addStretch();
	this->setLayout(pMainLayout);
}

//由列表中的QListWidgetItem地址得到InputDataItem地址
structPolyDeformationInputDataItem* ASPolyDeformationWidget::getInputDataItemFromQListWidgetItem(QListWidgetItem* pItem)
{
	int numOfItem = m_listItemOfListWidget->size();
	for (int crntItem = 0; crntItem < numOfItem; crntItem++)
	{
		if (m_listItemOfListWidget->at(crntItem)->listWedgetItem == pItem)
		{
			return m_listItemOfListWidget->at(crntItem);
		}
	}
	return NULL;
}


// 刷新文件下拉选框
void ASPolyDeformationWidget::slotRefreshDataComboBox(int c_AffectType)
{
	// 0 检查类型
	ASPolyImageData* Temporary = new ASPolyImageData();
	if (Temporary->isTypeInherited(c_AffectType) == false)
	{
		Temporary->DeleteNode();
		return;
	}
	Temporary->DeleteNode();

	// 1 现将原有内容清空
	while (m_pListWidget->count())
	{
		structPolyDeformationInputDataItem* crntItem = getInputDataItemFromQListWidgetItem(m_pListWidget->item(0));
		disconnect(crntItem->checkboxInItem, SIGNAL(stateChanged(int)), this, SLOT(slotFFDCheckBoxChecked(int)));
		m_pListWidget->takeItem(0);
	}
	m_listItemOfListWidget->clear();
	// 2 遍历数据，重新添加内容
	QList<int> ChoosenID;
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_PolyImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_PolyImageData))
	{
		ASPolyImageData* crntDataNode = (ASPolyImageData*)item;
		structPolyDeformationInputDataItem* crntItem = new structPolyDeformationInputDataItem;
		crntItem->listWedgetItem = new QListWidgetItem();
		crntItem->checkboxInItem = new QCheckBox(crntDataNode->getName());
		crntItem->ID = crntDataNode->getDataNodeID();
		m_pListWidget->addItem(crntItem->listWedgetItem);
		m_pListWidget->setItemWidget(crntItem->listWedgetItem, crntItem->checkboxInItem);
		if (ASFModelPolyDeformation::IsPolyChoosen(crntDataNode->getDataNodeID()))
		{
			crntItem->checkboxInItem->setChecked(true);
			ChoosenID.append(crntDataNode->getDataNodeID());
		}		
		m_listItemOfListWidget->append(crntItem);
		connect(crntItem->checkboxInItem, SIGNAL(stateChanged(int)), this, SLOT(slotFFDCheckBoxChecked(int)));
	}
	ASTransPolyDeformation::ResetPolyDataChoosen(ChoosenID);
}
//撤销键按下
void ASPolyDeformationWidget::slotUndoClicked()
{
	ASTransPolyDeformation::PolyDefUndo();
}
//checkbox状态改变
void ASPolyDeformationWidget::slotFFDCheckBoxChecked(int s)
{
	int numOfPolyData = m_listItemOfListWidget->size();
	for (int ncrntItem = 0; ncrntItem < numOfPolyData; ncrntItem++)
	{
		structPolyDeformationInputDataItem* pcrntItem = m_listItemOfListWidget->at(ncrntItem);
		ASPolyImageData* pcrntDataNode = (ASPolyImageData*)ASAbstractData::getDataNodeFromID(pcrntItem->ID);		
		if (ASFModelPolyDeformation::IsPolyChoosen(pcrntDataNode->getDataNodeID()) != pcrntItem->checkboxInItem->isChecked())
		{
			ASTransPolyDeformation::SetPolyDataChoosen(pcrntDataNode->getDataNodeID(), pcrntItem->checkboxInItem->isChecked());
		}
	}
}
// 软件当前交互方式改变
void ASPolyDeformationWidget::slotInteractorStyle2DChanged(int interactor)
{
	if (interactor == InterStyle2D_PolyDeformation)
	{
		this->SetEnabled(true);
	}
	else
	{
		this->SetEnabled(false);
	}
}
// 数据选择状态改变
void ASPolyDeformationWidget::slotChoosnChanged(int ID)
{
	int numOfPolyData = m_listItemOfListWidget->size();
	for (int ncrntItem = 0; ncrntItem < numOfPolyData; ncrntItem++)
	{
		structPolyDeformationInputDataItem* pcrntItem = m_listItemOfListWidget->at(ncrntItem);
		if (pcrntItem->ID == ID)
		{
			pcrntItem->checkboxInItem->setChecked(ASFModelPolyDeformation::IsPolyChoosen(pcrntItem->ID));
			break;
		}
	}
}
// 接收全局键盘信号
void ASPolyDeformationWidget::slotGlobalKeyBoard(QKeyEvent* pKeyEvent)
{
	if (ASTransPolyDeformation::GetEnabled() == false)
	{
		return;
	}
	// 数字键切换工作种子点，Ctrl+z撤销
	switch (pKeyEvent->key())
	{
	case Qt::Key_Z:
		if (pKeyEvent->modifiers() == Qt::ControlModifier)
		{
			this->slotUndoClicked();
		}
		break;
	default:
		break;
	}
}

// 使用Alt+左键选择polyDeformation作用曲面，接收来自ASDisplay2D的ID
void ASPolyDeformationWidget::slotPolyDeformationChoosePolyData(int nID)
{
	foreach(structPolyDeformationInputDataItem* crntItem, *m_listItemOfListWidget)
	{
		if (crntItem->ID == nID)
		{
			if (crntItem->checkboxInItem->isChecked())
			{
				crntItem->checkboxInItem->setChecked(false);
			} 
			else
			{
				crntItem->checkboxInItem->setChecked(true);
			}			
		}
	}
}

// 数据是否显示状态改变
void ASPolyDeformationWidget::slotDataShownChanged(ASAbstractData* DataNode)
{
	if (DataNode->isTypeInherited(DataType_PolyImageData) && DataNode->isShown() == false)
	{
		int numOfPolyData = m_listItemOfListWidget->size();
		for (int ncrntItem = 0; ncrntItem < numOfPolyData; ncrntItem++)
		{
			structPolyDeformationInputDataItem* pcrntItem = m_listItemOfListWidget->at(ncrntItem);
			if (pcrntItem->ID == DataNode->getDataNodeID() && pcrntItem->checkboxInItem->isChecked())
			{
				pcrntItem->checkboxInItem->setChecked(false);
				ASTransPolyDeformation::SetPolyDataChoosen(DataNode->getDataNodeID(), pcrntItem->checkboxInItem->isChecked());
			}
		}
	}
}