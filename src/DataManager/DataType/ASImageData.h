#pragma once

#include "ASAbstractData.h"

//体数据类型，灰度图、标签图
enum enumArrayImageDataType
{
	EMGrayscaleImage,
	EMMultiLabelImage
};

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

class ASImageData : public ASAbstractData
{
public:
	ASImageData();
	~ASImageData();
	virtual void DeleteNode();	// 调用此函数来释放节点
	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);
};
