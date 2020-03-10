#include "ASDialogNewSeedsFile.h"
#include "ASDataManager.h"
#include "ASColorMap.h"
#include <QHBoxLayout>
#include <QVBoxLayout>

ASDialogNewSeedsFile::ASDialogNewSeedsFile(QWidget *parent)
	: QDialog(parent)
{
	this->resize(QSize(300, 300));
	UIInit();
	m_SpinBoxValueInput->setMaximum(255);
	m_SpinBoxValueInput->setMinimum(1);
	m_LineEditNameInput->setText("Place input label name...");
	m_NameOfNewSeeds = m_LineEditNameInput->text();
	m_SpinBoxValueInput->setValue(1);
	slotSpinBoxValueChanged();

	connect(m_PushButtonOK, SIGNAL(clicked()), this, SLOT(accept()));
	connect(m_PushButtonCancel, SIGNAL(clicked()), this, SLOT(reject()));
	connect(m_LineEditNameInput, SIGNAL(textChanged(const QString &)), this, SLOT(slotLineEdittextChanged()));
	connect(m_SpinBoxValueInput, SIGNAL(valueChanged(int)), this, SLOT(slotSpinBoxValueChanged()));

	m_PushButtonOK->setDefault(true);
}

ASDialogNewSeedsFile::~ASDialogNewSeedsFile()
{
}

//GUI init
void ASDialogNewSeedsFile::UIInit()
{
	QHBoxLayout* HBoxLayout1 = new QHBoxLayout();
	m_LabelName = new QLabel(this);
	m_LineEditNameInput = new QLineEdit(this);
	HBoxLayout1->addWidget(m_LabelName);
	HBoxLayout1->addWidget(m_LineEditNameInput);

	QHBoxLayout* HBoxLayout2 = new QHBoxLayout();
	m_LabelValue = new QLabel(this);
	m_SpinBoxValueInput = new QSpinBox(this);
	HBoxLayout2->addWidget(m_LabelValue);
	HBoxLayout2->addWidget(m_SpinBoxValueInput);

	QHBoxLayout* HBoxLayout3 = new QHBoxLayout();
	m_LabelColor = new QLabel(this);
	m_LabelColorDisplay = new QLabel(this);
	HBoxLayout3->addWidget(m_LabelColor);
	HBoxLayout3->addWidget(m_LabelColorDisplay);

	QHBoxLayout* HBoxLayout4 = new QHBoxLayout();
	m_PushButtonCancel = new QPushButton(this);
	m_PushButtonOK = new QPushButton(this);
	HBoxLayout4->addStretch();
	HBoxLayout4->addWidget(m_PushButtonCancel);
	HBoxLayout4->addWidget(m_PushButtonOK);

	QVBoxLayout* mainLayout = new QVBoxLayout();
	mainLayout->addLayout(HBoxLayout1);
	mainLayout->addLayout(HBoxLayout2);
	mainLayout->addLayout(HBoxLayout3);
	mainLayout->addStretch();
	mainLayout->addLayout(HBoxLayout4);

	this->setLayout(mainLayout);

	m_LabelName->setText("Name: ");
	m_LabelValue->setText("Value: ");
	m_LabelColor->setText("Color: ");
	m_LabelName->setFixedWidth(35);
	m_LabelValue->setFixedWidth(35);
	m_LabelColor->setFixedWidth(35);
	m_PushButtonCancel->setText("Cancel");
	m_PushButtonOK->setText("OK");

}
//刷新颜色显示label
void ASDialogNewSeedsFile::refreshColorDisplay()
{
	QString str;
	str = "background-color: rgb(" + QString::number(m_Color.red()) + ','
		+ QString::number(m_Color.green()) + ','
		+ QString::number(m_Color.blue()) + ");";

	QString str2;
	str2 = "color: rgb(" + QString::number(255 - m_Color.red()) + ','
		+ QString::number(255 - m_Color.green()) + ','
		+ QString::number(255 - m_Color.blue()) + ");";
	m_LabelColorDisplay->setStyleSheet(str + str2);

	QString str1;
	str1 = "Color: " + QString::number(m_Color.red()) + " - "
		+ QString::number(m_Color.green()) + " - "
		+ QString::number(m_Color.blue());
	m_LabelColorDisplay->setText(str1);
}

void ASDialogNewSeedsFile::slotLineEdittextChanged()
{
	m_NameOfNewSeeds = m_LineEditNameInput->text();
}
void ASDialogNewSeedsFile::slotSpinBoxValueChanged()
{
	m_nLabel = m_SpinBoxValueInput->value();
	int labeltocolor = m_nLabel;
	//根据标签值按照Multilabel颜色表的规则确定颜色
	while (labeltocolor > 255)
	{
		labeltocolor -= 255;
	}
	while (labeltocolor < 0)
	{
		labeltocolor += 255;
	}
	//for (int ni = 0; ni < 13; ni++)
	//{
	//	if (labeltocolor >= ColorMap_Multilabel_Data[ni][0] && labeltocolor < ColorMap_Multilabel_Data[ni + 1][0])
	//	{
	//		m_Color.setRed(
	//			((int)((float)labeltocolor - ColorMap_Multilabel_Data[ni][0]) / (ColorMap_Multilabel_Data[ni + 1][0] - ColorMap_Multilabel_Data[ni][0]) *
	//			(ColorMap_Multilabel_Data[ni + 1][1] - ColorMap_Multilabel_Data[ni][1]) + ColorMap_Multilabel_Data[ni][1]) * 255
	//		);
	//		m_Color.setGreen(
	//			((int)((float)labeltocolor - ColorMap_Multilabel_Data[ni][0]) / (ColorMap_Multilabel_Data[ni + 1][0] - ColorMap_Multilabel_Data[ni][0]) *
	//			(ColorMap_Multilabel_Data[ni + 1][2] - ColorMap_Multilabel_Data[ni][2]) + ColorMap_Multilabel_Data[ni][2]) * 255
	//		);
	//		m_Color.setBlue(
	//			((int)((float)labeltocolor - ColorMap_Multilabel_Data[ni][0]) / (ColorMap_Multilabel_Data[ni + 1][0] - ColorMap_Multilabel_Data[ni][0]) *
	//			(ColorMap_Multilabel_Data[ni + 1][3] - ColorMap_Multilabel_Data[ni][3]) + ColorMap_Multilabel_Data[ni][3]) * 255
	//		);				
	//		refreshColorDisplay();
	//		return;
	//	}
	//}
	m_Color.setRed(ColorMap_Multilabel_Data[labeltocolor][1]*255);
	m_Color.setGreen(ColorMap_Multilabel_Data[labeltocolor][2]*255);
	m_Color.setBlue(ColorMap_Multilabel_Data[labeltocolor][3]*255);
	refreshColorDisplay();
}