#include "ASAddinsComboBox.h"

ASAddinsComboBox::ASAddinsComboBox(QWidget* parent)
	: QComboBox(parent)
{
	connect(this, SIGNAL(currentIndexChanged(int)), this, SLOT(slotCrntIndexChanged(int)));
}

ASAddinsComboBox::~ASAddinsComboBox()
{

}

void ASAddinsComboBox::SetCallBack(Fun f)
{
	m_f = f;
}

void ASAddinsComboBox::slotCrntIndexChanged(int i)
{
	if (m_f)
	{
		m_f(this);
	}
}
