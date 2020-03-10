#include "ASDICOMData.h"
#include "ASColorMap.h"
#include <vtkDICOMReader.h>
#include <vtkDICOMApplyRescale.h>

ASDICOMData::ASDICOMData()
{
	m_Datatype = DataType_DICOMData;
}

ASDICOMData::~ASDICOMData()
{
}

// 调用此函数来释放节点
void ASDICOMData::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASDICOMData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_DICOMData || ASArrayImageData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASDICOMData::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_DICOMData)
	{
		return true;
	}
	return false;
}
bool ASDICOMData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_DICOMData || ASArrayImageData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}

// 标签信息
void ASDICOMData::SetPatientItem(const vtkDICOMItem c_item)
{
	m_PatientItem = c_item;
}
void ASDICOMData::SetStudyItem(const vtkDICOMItem c_item)
{
	m_StudyItem = c_item;
}
void ASDICOMData::SetSeriesItem(const vtkDICOMItem c_item)
{
	m_SeriesItem = c_item;
}
vtkDICOMItem ASDICOMData::GetPatientItem(void)
{
	return m_PatientItem;
}
vtkDICOMItem ASDICOMData::GetStudyItem(void)
{
	return m_StudyItem;
}
vtkDICOMItem ASDICOMData::GetSeriesItem(void)
{
	return m_SeriesItem;
}