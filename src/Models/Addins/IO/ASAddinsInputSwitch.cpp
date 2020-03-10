#include "ASAddinsInputSwitch.h"

ASAddinsInputSwitch::ASAddinsInputSwitch(ASAddinsBase* parent)
	: ASAddinsInputBase(parent)
{
	m_Type = AddinInput_Switch;
	m_ComboBox = new QComboBox();
}

ASAddinsInputSwitch::~ASAddinsInputSwitch()
{
	m_ComboBox->deleteLater();
}

// Value
void ASAddinsInputSwitch::SetValue(const QStringList c_value)
{
	QStringList crntSL = c_value.first().split("&");
	m_ComboBox->clear();
	m_Name = crntSL.first();
	for (int j = 1; j < crntSL.size(); j++)
	{
		m_ComboBox->addItem(crntSL.at(j));
	}
}
// QComboBox
QComboBox* ASAddinsInputSwitch::GetComboBox()
{
	return m_ComboBox;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsInputSwitch::GetVoidPoint(const QString s)
{
	if (s == "")
	{
		m_dPara = m_ComboBox->currentIndex();
		return &m_dPara;
	}
	else
	{
		return nullptr;
	}
}

