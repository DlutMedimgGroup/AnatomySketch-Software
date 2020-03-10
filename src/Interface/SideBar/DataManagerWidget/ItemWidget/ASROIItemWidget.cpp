#include "ASROIItemWidget.h"
#include "ASTransROI.h"
#include "ASAbstractData.h"
#include "ASROIData.h"
#include "ASROITypeBase.h"
#include <QHBoxLayout>

ASROIItemWidget::ASROIItemWidget(QWidget *parent)
	: ASMarkEleItemWidget(parent)
{
	m_ItemType = ItemType_ROI;
	this->UIInit();
}

ASROIItemWidget::~ASROIItemWidget()
{

}

// ID
int ASROIItemWidget::getMarkEleID()
{
	return m_ROIID;
}
// Focus
void ASROIItemWidget::SetFocus(const bool c_f)
{
	if (m_isBold == c_f)
	{
		return;
	}
	if (c_f)
	{
		ASTransROI::SetCrntROIID(m_ROIID);
	}
	ASMarkEleItemWidget::SetFocus(c_f);
}
// 设置参数
void ASROIItemWidget::SetROIID(const int c_ID)
{
	m_ROIID = c_ID;
}
int ASROIItemWidget::GetROIID()
{
	return m_ROIID;
}
void ASROIItemWidget::SetColor(const QColor c_Color)
{
	//QString str("background-color: rgb(" + QString::number(c_Color.red()) + ','
	//	+ QString::number(c_Color.green()) + ','
	//	+ QString::number(c_Color.blue()) + ");"
	//	"border: 1px solid gray;");
	//m_LabelColor->setStyleSheet(str);
	QString s = QString::number(qRgb(c_Color.red(), c_Color.green(), c_Color.blue()), 16);
	m_pLabelForName->setStyleSheet("color:#" + s + ";");
}

void ASROIItemWidget::UIInit()
{
	//m_pLabelForName->setFixedWidth(150);
	//m_LabelLabel = new QLabel(this);
	//m_LabelLabel->setFixedWidth(60);
	//m_LabelColor = new QLabel(this);
	//m_LabelColor->setFixedWidth(40);

	QHBoxLayout* MainLayout = new QHBoxLayout(this);
	MainLayout->setContentsMargins(0, 0, 0, 0);	//边界距离，左上右下
	//MainLayout->addWidget(m_pCheckbox);
	//MainLayout->addWidget(m_LabelNo);
	MainLayout->addWidget(m_pLabelForName);
	MainLayout->addStretch();
	//MainLayout->addWidget(m_LabelLabel);
	//MainLayout->addWidget(m_LabelColor);
	MainLayout->addStretch();
	this->setLayout(MainLayout);
}
void ASROIItemWidget::slotCheckboxStateChanged(int state)
{
	ASAbstractData* Datanode = ASAbstractData::getDataNodeFromID(m_ID);
	ASROIData* crntROIData = static_cast<ASROIData*>(Datanode);
	ASROITypeBase* crntROI = crntROIData->GetROIWithID(m_ROIID);
	if (state == Qt::Checked)
	{
		if (crntROI->isShown() == true)
		{
			return;
		}
		crntROI->SetShown(true);
	}
	else
	{
		if (crntROI->isShown() == false)
		{
			return;
		}
		crntROI->SetShown(false);
	}
	ASTransROI::RefreshROICubeDisplay();
}