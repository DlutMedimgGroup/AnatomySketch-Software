#include "ASLandMarkItemWidget.h"
#include <QHBoxLayout>

ASLandMarkItemWidget::ASLandMarkItemWidget(QWidget *parent)
	: ASMarkEleItemWidget(parent)
{
	m_ItemType = ItemType_Landmark;
	this->UIInit();
}

ASLandMarkItemWidget::~ASLandMarkItemWidget()
{
	
}

// ID
int ASLandMarkItemWidget::getMarkEleID()
{
	return m_LandmarkID;
}
// Focus
void ASLandMarkItemWidget::SetFocus(const bool c_f)
{

}
// 设置参数
void ASLandMarkItemWidget::SetLandmarkID(const int c_ID)
{
	m_LandmarkID = c_ID;
}
int ASLandMarkItemWidget::GetLandmarkID()
{
	return m_LandmarkID;
}
void ASLandMarkItemWidget::SetColor(const QColor c_Color)
{
	//QString str("background-color: rgb(" + QString::number(c_Color.red()) + ','
	//	+ QString::number(c_Color.green()) + ','
	//	+ QString::number(c_Color.blue()) + ");"
	//	"border: 1px solid gray;");
	//m_LabelColor->setStyleSheet(str);
	QString s = QString::number(qRgb(c_Color.red(), c_Color.green(), c_Color.blue()), 16);
	m_pLabelForName->setStyleSheet("color:#" + s + ";");
}

void ASLandMarkItemWidget::UIInit()
{
	//m_pLabelForName->setFixedWidth(150);
	//m_LabelLabel = new QLabel(this);
	//m_LabelLabel->setFixedWidth(60);
	//m_LabelColor = new QLabel(this);
	//m_LabelColor->setFixedWidth(40);
	//m_pCheckbox->hide();

	QHBoxLayout* MainLayout = new QHBoxLayout();
	MainLayout->setContentsMargins(0, 0, 0, 0);	//边界距离，左上右下
	//m_pCheckbox->setChecked(true);
	//m_pCheckbox->setEnabled(false);
	//MainLayout->addWidget(m_pCheckbox);
	//MainLayout->addWidget(m_LabelNo);
	MainLayout->addWidget(m_pLabelForName);
	MainLayout->addStretch();
	//MainLayout->addWidget(m_LabelLabel);
	//MainLayout->addWidget(m_LabelColor);
	MainLayout->addStretch();
	this->setLayout(MainLayout);
}

void ASLandMarkItemWidget::slotCheckboxStateChanged(int state)
{

}