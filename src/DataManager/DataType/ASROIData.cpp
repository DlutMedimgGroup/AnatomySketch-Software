#include "ASROIData.h"
#include "ASROITypeBase.h"
#include "ASROITypeCube.h"

ASROIData::ASROIData()
{
	m_Datatype = DataType_ROI;
	m_plistROIs = new QList<ASROITypeBase *>;
}

ASROIData::~ASROIData()
{
	
}

// 调用此函数来释放节点
void ASROIData::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASROIData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_ROI || ASMarkData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASROIData::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_ROI)
	{
		return true;
	}
	return false;
}
bool ASROIData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_ROI || ASMarkData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}

// 新建ROI
ASROITypeBase* ASROIData::NewROI(const QString c_Name, const int c_Type)
{
	ASROITypeBase* pNewROI;
	switch (c_Type)
	{
	case ROIType_Cube:
		pNewROI = new ASROITypeCube();
		break;
	default:
		break;
	}
	// 为新ROI生成id
	int newID = 0;
	int numOfROI = m_plistROIs->size();
	bool s;
	do 
	{
		s = true;
		for (int it = 0; it < numOfROI; it++)
		{
			if (m_plistROIs->at(it)->GetROIID() == newID)
			{
				newID++;
				s = false;
				break;
			}
		}
	} while (s == false);
	pNewROI->SetROIID(newID);
	pNewROI->SetName(c_Name);
	m_plistROIs->append(pNewROI);
	return pNewROI;
}
void ASROIData::RemoveROI(const int c_ROIId)
{
	ASROITypeBase* roi = this->GetROIWithID(c_ROIId);
	m_plistROIs->removeOne(roi);
}
// ROI访问
int ASROIData::GetNumOfROIs()
{
	return m_plistROIs->size();
}
ASROITypeBase* ASROIData::GetROIAt(const int c_index)
{
	return m_plistROIs->at(c_index);
}
ASROITypeBase* ASROIData::GetROIWithID(const int c_ROIID)
{
	int NumOfROIs = this->m_plistROIs->size();
	for (int index = 0; index < NumOfROIs; index++)
	{
		if (m_plistROIs->at(index)->GetROIID() == c_ROIID)
		{
			return m_plistROIs->at(index);
		}
	}
	return nullptr;
}