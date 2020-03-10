#pragma once

#include "ASArrayImageData.h"
#include <vtkDICOMDirectory.h>
#include <vtkDICOMItem.h>

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

class ASDICOMData : public ASArrayImageData
{
public:
	ASDICOMData();
	~ASDICOMData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	// 标签信息
	void SetPatientItem(const vtkDICOMItem c_item);
	void SetStudyItem(const vtkDICOMItem c_item);
	void SetSeriesItem(const vtkDICOMItem c_item);
	vtkDICOMItem GetPatientItem(void);
	vtkDICOMItem GetStudyItem(void);
	vtkDICOMItem GetSeriesItem(void);

private:
	int m_nSeriesID;
	vtkDICOMItem m_PatientItem;
	vtkDICOMItem m_StudyItem;
	vtkDICOMItem m_SeriesItem;
};
