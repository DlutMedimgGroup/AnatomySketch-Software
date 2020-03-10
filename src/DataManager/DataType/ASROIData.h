#pragma once

#include "ASMarkData.h"
#include <QList>

class ASROITypeBase;

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

class ASROIData : public ASMarkData
{
public:
	ASROIData();
	~ASROIData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);
	
	// 新建ROI
	ASROITypeBase* NewROI(const QString c_Name, const int c_Type);
	void RemoveROI(const int c_ROIId);
	// ROI访问
	int GetNumOfROIs();
	ASROITypeBase* GetROIAt(const int c_index);
	ASROITypeBase* GetROIWithID(const int c_ROIID);

protected:

	// 存储ROI
	QList<ASROITypeBase*>* m_plistROIs;

private:

};
