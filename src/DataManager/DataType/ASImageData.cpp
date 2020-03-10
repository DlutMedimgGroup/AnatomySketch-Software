#include "ASImageData.h"

ASImageData::ASImageData()
{
	m_Datatype = DataType_ImageData;
}

ASImageData::~ASImageData()
{
}

// 调用此函数来释放节点
void ASImageData::DeleteNode()
{
	delete this;
}
// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASImageData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_ImageData || ASAbstractData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASImageData::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_ImageData)
	{
		return true;
	}
	return false;
}
bool ASImageData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_ImageData || ASAbstractData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}