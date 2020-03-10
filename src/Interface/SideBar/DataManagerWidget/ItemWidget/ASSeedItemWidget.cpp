#include "ASSeedItemWidget.h"
#include "ASTransSeeds.h"
#include "ASAbstractData.h"
#include "ASSeedData.h"
#include <QHBoxLayout>

ASSeedItemWidget::ASSeedItemWidget(QWidget *parent)
	: ASMarkEleItemWidget(parent)
{
	m_ItemType = ItemType_Seed;
	this->UIInit();
}

ASSeedItemWidget::~ASSeedItemWidget()
{
	
}

// ID
int ASSeedItemWidget::getMarkEleID()
{
	return m_SeedID;
}
// Focus
void ASSeedItemWidget::SetFocus(const bool c_f)
{
	if (m_isBold == c_f)
	{
		return;
	}
	if (c_f)
	{
		//ASTransSeeds::SetCrntSeedsID(m_SeedID);
	}
	ASMarkEleItemWidget::SetFocus(c_f);
}
// 设置参数
void ASSeedItemWidget::SetSeedID(const int c_ID)
{
	m_SeedID = c_ID;
}
int ASSeedItemWidget::GetSeedID()
{
	return m_SeedID;
}
void ASSeedItemWidget::SetLabel(const int c_Label)
{
	//m_LabelLabel->setText(tr("Label:") + QString::number(c_Label));
}
void ASSeedItemWidget::SetColor(const QColor c_Color)
{
	//QString str("background-color: rgb(" + QString::number(c_Color.red()) + ','
	//	+ QString::number(c_Color.green()) + ','
	//	+ QString::number(c_Color.blue()) + ");"
	//	"border: 1px solid gray;");
	//m_LabelColor->setStyleSheet(str);
	QString s = QString::number(qRgb(c_Color.red(), c_Color.green(), c_Color.blue()), 16);
	m_pLabelForName->setStyleSheet("color:#" + s + ";");
}
void ASSeedItemWidget::UIInit()
{
	//m_pLabelForName->setFixedWidth(150);
	//m_LabelLabel = new QLabel(this);
	//m_LabelLabel->setFixedWidth(60);
	//m_LabelLabel->setAlignment(Qt::AlignCenter);
	//m_LabelColor = new QLabel(this);
	//m_LabelColor->setFixedWidth(40);

	QHBoxLayout* MainLayout = new QHBoxLayout(this);
	MainLayout->setContentsMargins(0, 0, 0, 0);	//边界距离，左上右下
	//MainLayout->addWidget(m_pCheckbox);
	//MainLayout->addWidget(m_pLabelForIcon);
	//MainLayout->addWidget(m_LabelNo);
	MainLayout->addWidget(m_pLabelForName);
	MainLayout->addStretch();
	//MainLayout->addWidget(m_LabelLabel);
	//MainLayout->addWidget(m_LabelColor);
	MainLayout->addStretch();

	this->setLayout(MainLayout);
}
void ASSeedItemWidget::slotCheckboxStateChanged(int state)
{
	ASAbstractData* Datanode = ASAbstractData::getDataNodeFromID(m_ID);
	ASSeedData* crntCtrData = static_cast<ASSeedData*>(Datanode);
	structSeeds* crntContour = crntCtrData->getSeedsFromID(m_SeedID);
	if (state == Qt::Checked)
	{
		if (crntContour->isShown == true)
		{
			return;
		}
		crntContour->isShown = true;
	}
	else
	{
		if (crntContour->isShown == false)
		{
			return;
		}
		crntContour->isShown = false;
	}
	ASTransSeeds::RefreshSeedsDisplay();
}
