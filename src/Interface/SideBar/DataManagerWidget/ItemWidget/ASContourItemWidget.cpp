#include "ASContourItemWidget.h"
#include "ASTransContour.h"
#include "ASAbstractData.h"
#include "ASContourData.h"
#include "ASTransContour.h"
#include <QHBoxLayout>

ASContourItemWidget::ASContourItemWidget(QWidget *parent)
	: ASMarkEleItemWidget(parent)
{
	m_ItemType = ItemType_Contour;
	this->UIInit();
}

ASContourItemWidget::~ASContourItemWidget()
{
	
}

// ID
int ASContourItemWidget::getMarkEleID()
{
	return m_ContourID;
}
// Focus
void ASContourItemWidget::SetFocus(const bool c_f)
{
	if (m_isBold == c_f)
	{
		return;
	}
	if (c_f)
	{
		//ASTransContour::SetCrntContourID(m_ContourID);
	}
	ASMarkEleItemWidget::SetFocus(c_f);
}
void ASContourItemWidget::SetContourID(const int c_ID)
{
	m_ContourID = c_ID;
}
int ASContourItemWidget::GetContourID()
{
	return m_ContourID;
}
void ASContourItemWidget::SetLabel(const int c_Label)
{
	//m_LabelLabel->setText(tr("Label:") + QString::number(c_Label));
}
void ASContourItemWidget::SetColor(const QColor c_Color)
{
	//QString str("background-color: rgb(" + QString::number(c_Color.red()) + ','
	//	+ QString::number(c_Color.green()) + ','
	//	+ QString::number(c_Color.blue()) + ");"
	//	"border: 1px solid gray;");
	//m_LabelColor->setStyleSheet(str);
	QString s = QString::number(qRgb(c_Color.red(), c_Color.green(), c_Color.blue()), 16);
	m_pLabelForName->setStyleSheet("color:#" + s + ";");
	
}

void ASContourItemWidget::UIInit()
{
	//m_pLabelForName->setFixedWidth(150);
	//m_LabelLabel = new QLabel(this);
	//m_LabelLabel->setFixedWidth(60);
	//m_LabelColor = new QLabel(this);
	//m_LabelColor->setFixedWidth(40);

	QHBoxLayout* MainLayout = new QHBoxLayout(this);
	MainLayout->setContentsMargins(0, 0, 0, 0);	//±ß½ç¾àÀë£¬×óÉÏÓÒÏÂ
	//MainLayout->addWidget(m_pCheckbox);
	//m_pCheckbox->setCheckable(true);
	//MainLayout->addWidget(m_LabelNo);
	MainLayout->addWidget(m_pLabelForName);
	MainLayout->addStretch();
	//MainLayout->addWidget(m_LabelLabel);
	//MainLayout->addWidget(m_LabelColor);
	MainLayout->addStretch();

	this->setLayout(MainLayout);
}

void ASContourItemWidget::slotCheckboxStateChanged(int state)
{
	ASAbstractData* Datanode = ASAbstractData::getDataNodeFromID(m_ID);
	ASContourData* crntCtrData = static_cast<ASContourData*>(Datanode);
	structContour* crntContour = crntCtrData->GetContourFromID(m_ContourID);
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
	ASTransContour::RefreshContourDisplay();
}