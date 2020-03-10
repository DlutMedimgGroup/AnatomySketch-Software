#include "ASROIWidget.h"
#include "ASArrayImageData.h"
#include "ASDataStructureWalker.h"
#include "ASTransDataGeneral.h"
#include "ASTransROI.h"
#include "ASROIData.h"
#include "ASROITypeBase.h"
#include "ASROITypeCube.h"
#include "ASROIItemWidget.h"
#include "ASMainWidget.h"
#include "ASInteractorStyleManager.h"
#include "ASTransInteractorStyle.h"
#include "ASTransCompleter.h"
#include "ASTransGeneralInteractor.h"
#include "ASTransSidebarGeneral.h"
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QGridLayout>
#include <QColor>
#include <QMessageBox>
#include <QString>
#include <QStringList>

ASROIWidget::ASROIWidget(QWidget *parent)
	: ASWidgetInSideBarBase(parent)
{
	this->UIInit();
	this->CompleterInit();

	connect(ASTransROI::GetSelfPointer(), SIGNAL(signalRefreshROICube()),
		this, SLOT(slotRefreshCubeWidget()));
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalNewFromAddin(int, QString, int, QColor)),
		this, SLOT(slotNewFromAddin(int, QString, int, QColor)));
}

ASROIWidget::~ASROIWidget()
{

}

void ASROIWidget::SetEnabled(const bool c_bEnabled)
{
	if (c_bEnabled)
	{
		if (ASInteractorStyleManager::GetInteractorStyle2D() != InterStyle2D_ROICube)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_ROICube);
		}
	}
	else
	{
		if (ASInteractorStyleManager::GetInteractorStyle2D() == InterStyle2D_ROICube)
		{
			ASTransInteractorStyle::SetInteratorStyle2D(InterStyle2D_General);
		}
	}
}

void ASROIWidget::UIInit()
{
	// 0 总界面
	// 0.1 该文件中的ROI列表
	// 0.2 各工具标签页
	m_TabWTools = new QTabWidget(this);
	QWidget* Tab1 = new QWidget(m_TabWTools);
	m_TabWTools->addTab(Tab1, tr("Cube"));
	// 0.3 Finish
	m_BtnFinish = new QPushButton(tr(u8"完成"), this);
	m_BtnFinish->hide();
	connect(m_BtnFinish, SIGNAL(clicked()), this, SLOT(slotFinish()));
	// 0.4 layout
	QVBoxLayout* mainlayout = new QVBoxLayout();
	mainlayout->addWidget(m_TabWTools);
	mainlayout->addWidget(m_BtnFinish);
	mainlayout->addStretch();
	this->setLayout(mainlayout);
	
	// 1 Tab Cube
	// 1.1 添加方形ROI
	QLabel* label111 = new QLabel(tr("Add New Bounding Box:"), Tab1);
	m_CubeName = new QLineEdit(Tab1);
	connect(m_CubeName, SIGNAL(returnPressed()), this, SLOT(slotCubeNameReturn()));
	QHBoxLayout* layout11 = new QHBoxLayout();
	layout11->addWidget(label111);
	layout11->addWidget(m_CubeName);
	// 1.2 范围坐标显示
	QLabel* label120 = new QLabel(tr("Coordinate Range:"), Tab1);
	QGridLayout* layout12 = new QGridLayout();
	QLabel*	label121 = new QLabel("Min", this);
	QLabel*	label122 = new QLabel("Max", this);
	QLabel*	label123 = new QLabel("X", this);
	label123->setMaximumWidth(10);
	QLabel*	label124 = new QLabel("Y", this);
	label124->setMaximumWidth(10);
	QLabel*	label125 = new QLabel("Z", this);
	label125->setMaximumWidth(10);
	for (int i = 0; i < 6; i++)
	{
		m_CubeRange[i] = new QDoubleSpinBox(this);
		m_CubeRange[i]->setMaximum(999999999);
		m_CubeRange[i]->setMinimum(-999999999);
		connect(m_CubeRange[i], SIGNAL(valueChanged(double)), 
			this, SLOT(slotCubeValueChanged()));
	}
	layout12->addWidget(label121, 0, 1); // row = 0, column = 1.
	layout12->addWidget(label122, 0, 2);
	layout12->addWidget(label123, 1, 0);
	layout12->addWidget(label124, 2, 0);
	layout12->addWidget(label125, 3, 0);
	layout12->addWidget(m_CubeRange[0], 1, 1);
	layout12->addWidget(m_CubeRange[1], 1, 2);
	layout12->addWidget(m_CubeRange[2], 2, 1);
	layout12->addWidget(m_CubeRange[3], 2, 2);
	layout12->addWidget(m_CubeRange[4], 3, 1);
	layout12->addWidget(m_CubeRange[5], 3, 2);
	// 1.3 layout
	QVBoxLayout* layout1 = new QVBoxLayout();
	layout1->addLayout(layout11);
	layout1->addWidget(label120);
	layout1->addLayout(layout12);
	Tab1->setLayout(layout1);
}
void ASROIWidget::CompleterInit()
{
	QList<structCompleterItem*>* pList = ASTransCompleter::GetStructureList();
	QStringList StrList;
	for (int index = 0; index < pList->size(); index++)
	{
		StrList << pList->at(index)->name;
	}
	m_CubeStrLiMo = new QStringListModel();
	m_CubeStrLiMo->setStringList(StrList);
	m_CubeCmlt = new QCompleter();
	m_CubeCmlt->setModel(m_CubeStrLiMo);
	m_CubeCmlt->setCaseSensitivity(Qt::CaseInsensitive);
	EventFilterROI* filter = new EventFilterROI(m_CubeName);
	m_CubeCmlt->popup()->installEventFilter(filter);
	m_CubeName->setCompleter(m_CubeCmlt);
	connect(filter, SIGNAL(EnterPress()), this, SLOT(slotCubeNameReturn()));
}
void ASROIWidget::NewROI(QString name, int label, QColor color)
{
	//ASArrayImageData* GenData = static_cast<ASArrayImageData*>(
	//	ASTransDataGeneral::GetGeneratingData());
	//if (!GenData)
	//{
	//	// 无当前数据，新建失败
	//	QMessageBox::information(ASMainWidget::GetMainWidget(), tr("New ROI"),
	//		tr("Please set current greyscale data first."), QMessageBox::Ok, QMessageBox::Ok);
	//	return;
	//}
	//// 判断是否需要新建ROI文件
	//ASROIData* ROIData = ASTransROI::GetCurrentROIData();
	//if (!ROIData)
	//{
	//	//ROIData = ASTransROI::NewROIData();
	//	if (!ROIData)
	//	{
	//		qDebug("Error: Null pointer of current ROIData. From ASROIWidget(2).");
	//		return;
	//	}
	//}
	//// 新建CubeROI
	//ASROITypeBase* roi = ASTransROI::NewROI(ROIData->getDataNodeID(), name, ROIType_Cube);
	//if (roi->GetROIType() != ROIType_Cube)
	//{
	//	qDebug("Error: Wrong ROI type.");
	//	return;
	//}
	//ASROITypeCube* cube = static_cast<ASROITypeCube*>(roi);
	//cube->SetRange(GenData->getImageExtent());

	//QStringList StrList;
	//cube->SetColor(color);
	//// 方形ROI的初始范围为全图
	//ASTransROI::SetCrntROIID(cube->GetROIID());
	//ASTransROI::RefreshROICubeDisplay();
	//ASTransROI::UpdateListsOfROIData();
}

// Cube ROI 新建ROI
void ASROIWidget::slotCubeNameReturn()
{
	QList<structCompleterItem*>* pList = ASTransCompleter::GetStructureList();
	QString NewName = m_CubeName->text();
	bool bbb = false;
	for (int index = 0; index < pList->size(); index++)
	{
		if (pList->at(index)->name == NewName)
		{
			bbb = true;
			break;
		}
	}
	if (bbb == false)
	{
		return;
	}

	if (NewName.size() == 0)
	{
		return;
	}
	m_CubeName->clear();
	m_CubeName->setText("");
	QColor c(Qt::red);
	for (int index = 0; index < pList->size(); index++)
	{
		if (pList->at(index)->name == NewName)
		{
			c = pList->at(index)->color;
			break;
		}
	}
	this->NewROI(NewName, 0, c);
}
// 刷新Cube页
void ASROIWidget::slotRefreshCubeWidget()
{
	ASROIData* CrntROIData = ASTransROI::GetCurrentROIData();
	if (CrntROIData == nullptr)
	{
		for (size_t i = 0; i < 6; i++)
		{
			m_CubeRange[i]->setValue(0);
		}
		return;
	}
	int i = ASTransROI::GetCrntROIID();
	ASROITypeBase* roi = CrntROIData->GetROIWithID(ASTransROI::GetCrntROIID());
	if (roi == nullptr || roi->GetROIType() != ROIType_Cube)
	{
		for (size_t i = 0; i < 6; i++)
		{
			m_CubeRange[i]->setValue(0);
		}
		return;
	}
	ASROITypeCube* crntROI = static_cast<ASROITypeCube*>(roi);
	double range[6];
	crntROI->GetRange(range);
	for (size_t i = 0; i < 6; i++)
	{
		m_CubeRange[i]->setValue(range[i]);
	}
}
// Cube页输入参数
void ASROIWidget::slotCubeValueChanged()
{
	ASROIData* CrntROIData = ASTransROI::GetCurrentROIData();
	if (CrntROIData == nullptr)
	{
		return;
	}
	ASROITypeBase* roi = CrntROIData->GetROIWithID(ASTransROI::GetCrntROIID());
	if (roi == false || roi->isTypeInherited(ROIType_Cube) == false)
	{
		return;
	}
	ASROITypeCube* crntCube = static_cast<ASROITypeCube*>(roi);
	double range[6];
	crntCube->GetRange(range);
	for (int i = 0; i < 6; i++)
	{
		if (m_CubeRange[i]->value() != range[i])
		{
			crntCube->SetRange(i, m_CubeRange[i]->value());
			ASTransROI::SetROICubeRange(i, m_CubeRange[i]->value());
		}
	}
}
// 从插件界面中新建数据
//void ASROIWidget::slotNewFromAddin(int c_Type, QString name, int label, QColor color)
//{
//	if (c_Type == SIDEBAR_ROI)
//	{
//		//ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_ROI, true);
//		//this->m_BtnFinish->show();
//		//this->m_FlagFromAddin = true;
//
//		ASTransROI::NewROIData(name, label, color);
//		//this->NewROI(name, label, color);
//	}
//}
// Finish
void ASROIWidget::slotFinish()
{
	ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_Operation, true);
}