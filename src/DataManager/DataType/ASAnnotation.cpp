#include "ASAnnotation.h"

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

ASAnnotation::ASAnnotation()
{
	m_Datatype = DataType_Annotation;
}

ASAnnotation::~ASAnnotation()
{
}

// 调用此函数来释放节点
void ASAnnotation::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASAnnotation::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_Annotation || ASAbstractData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASAnnotation::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_Annotation)
	{
		return true;
	}
	return false;
}
bool ASAnnotation::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_Annotation || ASAbstractData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}
