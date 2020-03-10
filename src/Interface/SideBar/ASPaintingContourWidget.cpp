#include "ASPaintingContourWidget.h"
#include "ASTransContour.h"
#include "ASContourData.h"
#include "ASFModelContour.h"
#include "ASTransGlobalKeyBoard.h"
#include "ASTransInteractorStyle.h"
#include "ASTransCompleter.h"
#include "ASArrayImageData.h"
#include "ASTransDataGeneral.h"
#include "ASInteractorStyleManager.h"
#include "ASMainWidget.h"
#include "ASTransSidebarGeneral.h"
#include <QVBoxLayout>
#include <QMessageBox>
#include <QLabel>

ASPaintingContourWidget::ASPaintingContourWidget(QWidget *parent)
	: ASWidgetInSideBarBase(parent)
{
	this->UIInit();
	this->setStyleSheet("padding: 2px;");
	this->CompleterInit();

	// 接收transmission信号
	connect(ASTransInteractorStyle::GetSelfPointer(), SIGNAL(signalInteractorStyle2DChanged(int)), 
		this, SLOT(slotInteractorStyle2DChanged(int)));
	connect(ASTransSidebarGeneral::GetSelfPointer(), SIGNAL(signalNewFromAddin(int, QString, int, QColor)),
		this, SLOT(slotNewFromAddin(int, QString, int, QColor)));
}

ASPaintingContourWidget::~ASPaintingContourWidget()
{

}

void ASPaintingContourWidget::SetEnabled(const bool c_bEnabled)
{
	if (c_bEnabled == ASFModelContour::GetContourModleEnabled())
	{
		return;
	}
	if (!c_bEnabled && m_FlagFromAddin)
	{
		m_FlagFromAddin = false;
		m_BtnFinish->hide();
	}
	ASTransContour::SetModelEnabled(c_bEnabled);
}

// 界面初始化
void ASPaintingContourWidget::UIInit()
{
	// 1 new
	QLabel* label10 = new QLabel(tr("New Contour:"), this);
	m_LineENewData = new QLineEdit(this);
	QHBoxLayout* layout1 = new QHBoxLayout();
	layout1->addWidget(label10);
	layout1->addWidget(m_LineENewData);
	connect(m_LineENewData, SIGNAL(returnPressed()), this, SLOT(slotContourNameReturn()));
	// 2 Touch model
	m_ComboTouchModel = new QComboBox(this);
	m_ComboTouchModel->addItem(tr("Use mouse to add control point"));
	m_ComboTouchModel->addItem(tr("Use mouse to add serial points"));
	m_ComboTouchModel->addItem(tr("Use stylus to add serial points"));
	m_ComboTouchModel->setCurrentIndex(0);
	connect(m_ComboTouchModel, SIGNAL(currentIndexChanged(int)), this, SLOT(slotTouchModelChanged(int)));
	// 3 Finish
	m_BtnFinish = new QPushButton(tr(u8"完成"), this);
	m_BtnFinish->hide();
	connect(m_BtnFinish, SIGNAL(clicked()), this, SLOT(slotFinish()));
	// 0 mainlayout
	QVBoxLayout* mainlayout = new QVBoxLayout();
	mainlayout->addLayout(layout1);
	mainlayout->addWidget(m_ComboTouchModel);
	mainlayout->addWidget(m_BtnFinish);
	mainlayout->addStretch();
	this->setLayout(mainlayout);
}
void ASPaintingContourWidget::CompleterInit()
{
	QList<structCompleterItem*>* pList = ASTransCompleter::GetStructureList();
	QStringList StrList;
	for (int index = 0; index < pList->size(); index++)
	{
		StrList << pList->at(index)->name;
	}
	m_StrLiMo = new QStringListModel();
	m_StrLiMo->setStringList(StrList);
	m_Cmlt = new QCompleter();
	m_Cmlt->setModel(m_StrLiMo);
	m_Cmlt->setCaseSensitivity(Qt::CaseInsensitive);
	EventFilterContour* filter = new EventFilterContour(m_LineENewData);
	m_Cmlt->popup()->installEventFilter(filter);
	m_LineENewData->setCompleter(m_Cmlt);
	connect(filter, SIGNAL(EnterPress()), this, SLOT(slotContourNameReturn()));
}
void ASPaintingContourWidget::NewContour(QString name, int label, QColor color)
{
	//ASArrayImageData* GenData = static_cast<ASArrayImageData*>(
	//	ASTransDataGeneral::GetGeneratingData());
	//if (!GenData)
	//{
	//	// 无当前数据，新建失败
	//	QMessageBox::information(ASMainWidget::GetMainWidget(), tr("New Contour"),
	//		tr("Please set current greyscale data."), QMessageBox::Ok, QMessageBox::Ok);
	//	return;
	//}
	//// 判断是否需要新建种子点文件
	//ASContourData* ContourData = ASTransContour::GetCurrentContourData();
	//if (!ContourData)
	//{
	//	ContourData = ASTransContour::NewContourData();
	//	if (!ContourData)
	//	{
	//		qDebug("Error: Null pointer of current SeedsData. From ASPaintingSeedsWidget.");
	//		return;
	//	}
	//}
	//int id = ContourData->NewContour(name, label, color);
	//if (id == -1)
	//{
	//	qDebug("error: Bad new seed id in ASPaintingSeedsWidget.cpp");
	//	return;
	//}
	//ASTransContour::SetCrntContourID(id);
	//// 刷新
	//ASTransContour::UpdateListsOfContourData();
}

// 新建文件
void ASPaintingContourWidget::slotContourNameReturn()
{
	QList<structCompleterItem*>* pList = ASTransCompleter::GetStructureList();
	QString NewName = m_LineENewData->text();

	// 只能使用列表中的名称
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
	m_LineENewData->clear();
	m_LineENewData->setText("");

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
	this->NewContour(NewName, contourid, c);
}

// touch model
void ASPaintingContourWidget::slotTouchModelChanged(int state)
{
	ASTransContour::SetInterfaceModel(state);
}

// 软件当前交互方式改变
void ASPaintingContourWidget::slotInteractorStyle2DChanged(int interactor)
{
	if (interactor == InterStyle2D_PaintingContour)
	{
		this->SetEnabled(true);
	}
	else
	{
		this->SetEnabled(false);
	}
}
//// 从插件界面中新建数据
//void ASPaintingContourWidget::slotNewFromAddin(int c_Type, QString name, int label, QColor color)
//{
//	if (c_Type == SIDEBAR_PaintingContour)
//	{
//		//ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_PaintingContour, true);
//		//this->m_BtnFinish->show();
//		//this->m_FlagFromAddin = true;
//		ASTransContour::NewContourData(name, label, color);
//	}
//}
// Finish
void ASPaintingContourWidget::slotFinish()
{
	ASTransSidebarGeneral::SetSidebarBelowWidget(SIDEBAR_Operation, true);
}
