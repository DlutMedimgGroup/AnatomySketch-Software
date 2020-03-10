#include "ASAddinsInputBase.h"

ASAddinsInputBase::ASAddinsInputBase(ASAddinsBase* parent)
	: ASAddinsIOBase(parent)
{
	m_Type = AddinInput_Base;
}

ASAddinsInputBase::~ASAddinsInputBase()
{

}

bool ASAddinsInputBase::isInput()
{
	return true;
}
void ASAddinsInputBase::SetValue(const QStringList c_value)
{
	this->m_Name = c_value.first();
}
void ASAddinsInputBase::RefreshCombo(int datatype)
{

}
