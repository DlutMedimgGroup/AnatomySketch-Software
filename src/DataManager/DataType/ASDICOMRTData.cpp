#include "ASDICOMRTData.h"
#include "ASDataStructureWalker.h"

// 当前打开的RT数据的数量
int ASDICOMRTData::ms_nNumOfDICOMRTData = 0;

ASDICOMRTData::ASDICOMRTData()
{
	m_Datatype = DataType_DICOMRTData;
	ASDICOMRTData::ms_nNumOfDICOMRTData += 1;

	m_ListRTDataSlice = new QList<structDICOMRTDataSlice *>;
}

ASDICOMRTData::~ASDICOMRTData()
{
	ASDICOMRTData::ms_nNumOfDICOMRTData -= 1;
}

// 调用此函数来释放节点
void ASDICOMRTData::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASDICOMRTData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_DICOMRTData || ASMarkData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASDICOMRTData::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_DICOMRTData)
	{
		return true;
	}
	return false;
}
bool ASDICOMRTData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_DICOMRTData || ASMarkData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}

// 添加一个闭合曲线
void ASDICOMRTData::appendSlice(const double* c_pdPoints, const int c_nNumOfPoints)
{
	structDICOMRTDataSlice* crntDICOMRTDataSlice = new structDICOMRTDataSlice;
	crntDICOMRTDataSlice->LayerDataBounds[0] = c_pdPoints[0];
	crntDICOMRTDataSlice->LayerDataBounds[1] = c_pdPoints[0];
	crntDICOMRTDataSlice->LayerDataBounds[2] = c_pdPoints[1];
	crntDICOMRTDataSlice->LayerDataBounds[3] = c_pdPoints[1];
	crntDICOMRTDataSlice->layer = c_pdPoints[2];
	for (int item = 0; item < c_nNumOfPoints; item++)
	{
		crntDICOMRTDataSlice->points.append(c_pdPoints[3 * item]);
		crntDICOMRTDataSlice->points.append(c_pdPoints[3 * item + 1]);
		crntDICOMRTDataSlice->points.append(c_pdPoints[3 * item + 2]);
		switch (item % 3)
		{
		case 0:  //x
			if (c_pdPoints[item] > crntDICOMRTDataSlice->LayerDataBounds[1])
			{
				crntDICOMRTDataSlice->LayerDataBounds[1] = c_pdPoints[item];
			}
			if (c_pdPoints[item] < crntDICOMRTDataSlice->LayerDataBounds[0])
			{
				crntDICOMRTDataSlice->LayerDataBounds[0] = c_pdPoints[item];
			}
			break;
		case 1:  //y
			if (c_pdPoints[item] > crntDICOMRTDataSlice->LayerDataBounds[3])
			{
				crntDICOMRTDataSlice->LayerDataBounds[3] = c_pdPoints[item];
			}
			if (c_pdPoints[item] < crntDICOMRTDataSlice->LayerDataBounds[2])
			{
				crntDICOMRTDataSlice->LayerDataBounds[2] = c_pdPoints[item];
			}
			break;
		default:
			break;
		}
	}
	this->m_ListRTDataSlice->append(crntDICOMRTDataSlice);

	// 计算坐标范围
	//计算RT数据坐标范围
	if (m_ListRTDataSlice->size() <= 1) //第一次遍历
	{
		this->m_adDataExtent[0] = crntDICOMRTDataSlice->LayerDataBounds[0];//xmin
		this->m_adDataExtent[1] = crntDICOMRTDataSlice->LayerDataBounds[1];//xmax
		this->m_adDataExtent[2] = crntDICOMRTDataSlice->LayerDataBounds[2];//ymin
		this->m_adDataExtent[3] = crntDICOMRTDataSlice->LayerDataBounds[3];//ymax
		this->m_adDataExtent[4] = crntDICOMRTDataSlice->layer;//zmin
		this->m_adDataExtent[5] = crntDICOMRTDataSlice->layer;//zmax
	}
	else
	{
		//x
		if (crntDICOMRTDataSlice->LayerDataBounds[0] < this->m_adDataExtent[0])
		{
			this->m_adDataExtent[0] = crntDICOMRTDataSlice->LayerDataBounds[0];
		}
		if (crntDICOMRTDataSlice->LayerDataBounds[1] > this->m_adDataExtent[1])
		{
			this->m_adDataExtent[1] = crntDICOMRTDataSlice->LayerDataBounds[1];
		}
		//y
		if (crntDICOMRTDataSlice->LayerDataBounds[2] < this->m_adDataExtent[2])
		{
			this->m_adDataExtent[2] = crntDICOMRTDataSlice->LayerDataBounds[2];
		}
		if (crntDICOMRTDataSlice->LayerDataBounds[3] > this->m_adDataExtent[3])
		{
			this->m_adDataExtent[3] = crntDICOMRTDataSlice->LayerDataBounds[3];
		}
		//z
		if (crntDICOMRTDataSlice->layer < this->m_adDataExtent[4])
		{
			this->m_adDataExtent[4] = crntDICOMRTDataSlice->layer;
		}
		if (crntDICOMRTDataSlice->layer > this->m_adDataExtent[5])
		{
			this->m_adDataExtent[5] = crntDICOMRTDataSlice->layer;
		}
	}
}

// slice数量
int ASDICOMRTData::getNumOfSlice()
{
	return m_ListRTDataSlice->size();
}
// 获取Slice
structDICOMRTDataSlice* ASDICOMRTData::getSliceAt(int index)
{
	return m_ListRTDataSlice->at(index);
}
// 静态函数
int ASDICOMRTData::getNumOfDICOMRTData()
{
	return ASDICOMRTData::ms_nNumOfDICOMRTData;
}

// 静态函数
// 得到所有DICOM-RT数据的范围，返回0成功，返回-1表示没有RT数据或只有空数据
int ASDICOMRTData::getDICOMRTDataBounds(double* pdBounds)
{
	int numOfEffectiveData = 0;
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_DICOMRTData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_DICOMRTData))
	{
		ASDICOMRTData* crntDataNode = (ASDICOMRTData*)item;
		if (crntDataNode->getNumOfSlice() == 0)
		{
			continue;
		}
		double* crntDataBounds = crntDataNode->getDataExtent();
		if (numOfEffectiveData == 0)
		{
			for (size_t i = 0; i < 6; i++)
			{
				pdBounds[i] = crntDataBounds[i];
			}			
		}
		else
		{
			for (size_t i = 0; i < 6; i+=2)
			{
				if (pdBounds[i] > crntDataBounds[i])
				{
					pdBounds[i] = crntDataBounds[i];
				}				
			}
			for (size_t i = 1; i < 6; i += 2)
			{
				if (pdBounds[i] < crntDataBounds[i])
				{
					pdBounds[i] = crntDataBounds[i];
				}
			}
		}
		numOfEffectiveData++;
	}
	if (numOfEffectiveData == 0)
	{
		return -1;
	}
	else
	{
		return 0;
	}
}