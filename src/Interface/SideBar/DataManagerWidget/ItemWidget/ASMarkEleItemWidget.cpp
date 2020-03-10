#include "ASMarkEleItemWidget.h"

ASMarkEleItemWidget::ASMarkEleItemWidget(QWidget *parent)
	: ASAbstructItemWidget(parent)
{
	m_ItemType = ItemType_DataNode;
	//m_LabelNo = new QLabel(this);
	//m_LabelNo->setFixedWidth(20);
	//m_LabelNo->setAlignment(Qt::AlignCenter);

	m_pLabelForIcon->hide();
}

ASMarkEleItemWidget::~ASMarkEleItemWidget()
{
}

// ItemType
bool ASMarkEleItemWidget::IsMarkEle()
{
	return true;
}
// No.
void ASMarkEleItemWidget::SetNo(const int c_No)
{
	m_No = c_No;
	//m_LabelNo->setText(QString::number(c_No));
}
int ASMarkEleItemWidget::GetNo()
{
	return m_No;
}
// Focus
void ASMarkEleItemWidget::SetFocus(const bool c_f)
{
	m_isBold = c_f;
	QFont ft;
	ft.setBold(m_isBold);
	m_pLabelForName->setFont(ft);
}
bool ASMarkEleItemWidget::GetFocus()
{
	return m_isBold;
}
