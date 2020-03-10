#include "ASROITypeBase.h"

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

ASROITypeBase::ASROITypeBase()
{
	m_ROIType = ROIType_Base;
}

ASROITypeBase::~ASROITypeBase()
{
}

// 调用此函数来释放节点
void ASROITypeBase::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASROITypeBase::isTypeInherited(const int c_Type)
{
	if (c_Type == ROIType_Base)
	{
		return true;
	}
	return false;
}
bool ASROITypeBase::isTypeAbsolute(const int c_Type)
{
	if (c_Type == ROIType_Base)
	{
		return true;
	}
	return false;
}
bool ASROITypeBase::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == ROIType_Base)
	{
		return true;
	}
	return false;
}
int ASROITypeBase::GetROIType()
{
	return m_ROIType;
}

void ASROITypeBase::GetCenter(double* center)
{

}

// ROIID
void ASROITypeBase::SetROIID(const int c_nID)
{
	m_ROIID = c_nID;
}
int ASROITypeBase::GetROIID()
{
	return m_ROIID;
}
// Name
void ASROITypeBase::SetName(const QString c_Name)
{
	m_Name = c_Name;
}
QString ASROITypeBase::GetName()
{
	return m_Name;
}
// Shown
void ASROITypeBase::SetShown(const bool c_shown)
{
	m_isShown = c_shown;
}
bool ASROITypeBase::isShown()
{
	return m_isShown;
}
// Color
void ASROITypeBase::SetColor(QColor color)
{
	m_Color = color;
}
QColor ASROITypeBase::GetColor()
{
	return m_Color;
}