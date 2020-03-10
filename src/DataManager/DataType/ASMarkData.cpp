#include "ASMarkData.h"

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

ASMarkData::ASMarkData()
{
	m_Datatype = DataType_MarkData;
}

ASMarkData::~ASMarkData()
{
}

// 调用此函数来释放节点
void ASMarkData::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASMarkData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_MarkData || ASAbstractData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASMarkData::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_MarkData)
	{
		return true;
	}
	return false;
}
bool ASMarkData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_MarkData || ASAbstractData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}

// 颜色
void ASMarkData::setColor(const QColor c_Color)
{
	m_Color = c_Color;
}
QColor ASMarkData::getColor()
{
	return m_Color;
}
