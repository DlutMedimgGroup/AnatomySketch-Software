#include "ASLandmarkData.h"
#include "ASTransLandmark.h"
#include "ASTransCompleter.h"

int ASLandmarkData::ms_nNumOfLandmarkData = 0;

ASLandmarkData::ASLandmarkData()
{
	m_Datatype = DataType_LandmarkData;
	m_listLandmarks = new QList<structLandmark*>;

	if (ASLandmarkData::ms_nNumOfLandmarkData == 0)
	{
		// 建立标定点显示管线
		ASTransLandmark::InitLandmarkPipeline();
	}
	ASLandmarkData::ms_nNumOfLandmarkData += 1;
}

ASLandmarkData::~ASLandmarkData()
{
	ASLandmarkData::ms_nNumOfLandmarkData -= 1;
}

// 调用此函数来释放节点
void ASLandmarkData::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASLandmarkData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_LandmarkData || ASMarkData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASLandmarkData::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_LandmarkData)
	{
		return true;
	}
	return false;
}
bool ASLandmarkData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_LandmarkData || ASMarkData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}
// 标定点数据数量
int ASLandmarkData::GetNumOfLandmarkData()
{
	return ms_nNumOfLandmarkData;
}
// 访问标定点
int ASLandmarkData::GetNumberOfLandmark()
{
	return m_listLandmarks->size();
}
structLandmark* ASLandmarkData::GetLandmarkAt(const int c_Landmark)
{
	return m_listLandmarks->at(c_Landmark);
}
structLandmark* ASLandmarkData::GetLandmarkWithID(const int c_id)
{
	int num = m_listLandmarks->size();
	for (int i = 0; i < num; i++)
	{
		if (m_listLandmarks->at(i)->LandmarkID == c_id)
		{
			return m_listLandmarks->at(i);
		}
	}
	return nullptr;
}
void ASLandmarkData::AppandLandmark(structLandmark* NewLandmark)
{
	m_listLandmarks->append(NewLandmark);
}

// 轮廓线操作
void ASLandmarkData::AddLandMark(double* pPosition, QString qsname)
{
	structLandmark* crntLandmark = new structLandmark;
	crntLandmark->dPosition[0] = pPosition[0];
	crntLandmark->dPosition[1] = pPosition[1];
	crntLandmark->dPosition[2] = pPosition[2];
	crntLandmark->qsName = qsname;

	// 得到一个唯一ID
	int crntID = 0;
	int nNumOfLandmark = m_listLandmarks->size();
	bool succ = false;
	while (succ == false)
	{
		succ = true;
		for (int ncrtLandM = 0; ncrtLandM < nNumOfLandmark; ncrtLandM++)
		{
			if (m_listLandmarks->at(ncrtLandM)->LandmarkID == crntID)
			{
				succ = false;
				crntID++;
				break;
			}
		}
	}

	crntLandmark->LandmarkID = crntID;
	crntLandmark->color.setRed(255);
	crntLandmark->color.setGreen(255);
	crntLandmark->color.setBlue(255);
	QList<structCompleterItem*>* pList = ASTransCompleter::GetLandmarkList();
	for (int index = 0; index < pList->size(); index++)
	{
		if (pList->at(index)->name == crntLandmark->qsName)
		{
			crntLandmark->color = pList->at(index)->color;
			break;
		}
	}

	m_listLandmarks->append(crntLandmark);
}
void ASLandmarkData::MoveLandMark(double* pPos, const int c_PointID)
{
	for (int ncrntLandmark = 0; ncrntLandmark < m_listLandmarks->size(); ncrntLandmark++)
	{
		structLandmark* crntLandmark = m_listLandmarks->at(ncrntLandmark);
		if (crntLandmark->LandmarkID == c_PointID)
		{
			crntLandmark->dPosition[0] = pPos[0];
			crntLandmark->dPosition[1] = pPos[1];
			crntLandmark->dPosition[2] = pPos[2];
			break;
		}
	}
}
void ASLandmarkData::DeleteLandMark(const int c_PointID)
{
	for (int ncrntLandmark = 0; ncrntLandmark < m_listLandmarks->size(); ncrntLandmark++)
	{
		structLandmark* crntLandmark = m_listLandmarks->at(ncrntLandmark);
		if (crntLandmark->LandmarkID == c_PointID)
		{
			m_listLandmarks->removeOne(crntLandmark);
			break;
		}
	}
}
// 属性
double* ASLandmarkData::getDataExtent()
{
	return nullptr;
}
