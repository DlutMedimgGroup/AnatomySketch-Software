#include "ASAbstructItemWidget.h"

ASAbstructItemWidget::ASAbstructItemWidget(QWidget *parent)
	: QWidget(parent)
{
	m_ItemType = ItemType_Abstract;

	m_ListWidgetItem = new QStandardItem();

	//m_pCheckbox = new QCheckBox(this);
	//connect(m_pCheckbox, SIGNAL(stateChanged(int)), this, SLOT(slotCheckboxStateChanged(int)));	
	m_pLabelForName = new QLabel(this);
	m_pLabelForIcon = new ASClicklabel(this);
	connect(m_pLabelForIcon, SIGNAL(clicked()), this, SLOT(slotIconClicked()));
}

ASAbstructItemWidget::~ASAbstructItemWidget()
{
	/* 
	m_ListWidgetItem 被加入到QStandardItemModel中，QStandardItemModel会自动管理
	已经加入其中的各个Item的内存。
	*/
}

// ItemType
int ASAbstructItemWidget::GetItemType()
{
	return m_ItemType;
}
bool ASAbstructItemWidget::IsMarkEle()
{
	return false;
}
// ListWidgetItem
QStandardItem* ASAbstructItemWidget::GetListWidgetItem()
{
	return m_ListWidgetItem;
}
void ASAbstructItemWidget::SetListWidgetItem(QStandardItem* item)
{
	m_ListWidgetItem = item;
}
// ID
void ASAbstructItemWidget::SetID(int ID)
{
	m_ID = ID;
}
int ASAbstructItemWidget::GetID()
{
	return m_ID;
}
int ASAbstructItemWidget::getMarkEleID()
{
	return -1;
}
// Checked
void ASAbstructItemWidget::SetChecked(bool checked)
{
	//m_pCheckbox->setChecked(checked);
	m_Checked = checked;
}
bool ASAbstructItemWidget::GetChecked()
{
	return m_Checked;
}
// Font
void ASAbstructItemWidget::SetSizeHWithUpdate(const int c_nH)
{
	m_nH = c_nH;
	// Checkbox
	//m_pCheckbox->setFixedSize(m_nH - 3, m_nH - 3);	//固定宽度
	//QString checkboxStyleSheet = "ASSideBar QCheckBox::indicator { width: %1px; height: %2px; }";
	//m_pCheckbox->setStyleSheet(checkboxStyleSheet.arg(m_nH - 2).arg(m_nH - 2));
	// Icon
	m_pLabelForIcon->setFixedSize(m_nH, m_nH);
	// Font
	QFont ft;
	ft.setPointSize(m_nH * 0.6);
	ft.setBold(m_isBold);
	m_pLabelForName->setFont(ft);
}
void ASAbstructItemWidget::SetBold(const bool c_b)
{
	m_isBold = c_b;
}
void ASAbstructItemWidget::SetBoldWithUpdate(const bool c_b)
{
	m_isBold = c_b;
	QFont ft;
	ft.setPointSize(m_nH * 0.6);
	ft.setBold(m_isBold);
	m_pLabelForName->setFont(ft);
}
// Name
void ASAbstructItemWidget::SetName(QString name)
{
	m_pLabelForName->setText(name);
}
QString ASAbstructItemWidget::GetName()
{
	return m_pLabelForName->text();
}
// Icon
void ASAbstructItemWidget::SetIcon(QPixmap icon)
{
	m_Icon = icon;
}

void ASAbstructItemWidget::slotCheckboxStateChanged(int state)
{
	int i = 0;
}
void ASAbstructItemWidget::slotIconClicked()
{
	int i = 0;
}