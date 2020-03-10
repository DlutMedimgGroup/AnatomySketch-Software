#pragma once

#include "ASAbstractData.h"

class ASAddinsBase;

class ASOperation : public ASAbstractData
{
public:
	ASOperation();
	~ASOperation();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	void SetAddinType(const int c_Type);
	int GetAddinType();
	// method type
	ASAddinsBase* GetAddin();
	void InitMethod(const QString c_name, const QString c_path);
	void ClearMethod();

protected:

	ASAddinsBase* m_Addin = nullptr;
	int m_AddinType = 0; //AddinType_ImgProcess

};
