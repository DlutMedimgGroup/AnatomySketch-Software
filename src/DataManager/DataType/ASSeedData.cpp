#include "ASSeedData.h"

int ASSeedData::ms_nNumOfSeedsData = 0;

ASSeedData::ASSeedData()
{
	m_Datatype = DataType_SeedData;
	ASSeedData::ms_nNumOfSeedsData += 1;

	m_listSeedsData = new QList<structSeeds *>;
	m_listOperaSequence = new QList<int>;
}

ASSeedData::~ASSeedData()
{
	ASSeedData::ms_nNumOfSeedsData -= 1;

	while (m_listSeedsData->size())
	{
		structSeeds* crntSeeds = m_listSeedsData->first();
		m_listSeedsData->removeFirst();
		while (crntSeeds->pListSeedsData->size())
		{
			structLine* crntLine = crntSeeds->pListSeedsData->first();
			crntSeeds->pListSeedsData->removeFirst();
			while (crntLine->pvectorPoint.size())
			{
				structPoint* crntPoint = crntLine->pvectorPoint.first();
				crntLine->pvectorPoint.removeFirst();
				delete crntPoint;
			}
			delete crntLine;
		}
		delete crntSeeds->pListSeedsData;
		delete crntSeeds;
	}
	delete m_listSeedsData;
}

// 调用此函数来释放节点
void ASSeedData::DeleteNode()
{
	delete this;
}

// 数据类型判断
// 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
bool ASSeedData::isTypeInherited(const int c_Type)
{
	if (c_Type == DataType_SeedData || ASMarkData::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASSeedData::isTypeAbsolute(const int c_Type)
{
	if (c_Type == DataType_SeedData)
	{
		return true;
	}
	return false;
}
bool ASSeedData::isTypeInheritedStatic(const int c_Type)
{
	if (c_Type == DataType_SeedData || ASMarkData::isTypeInheritedStatic(c_Type))
	{
		return true;
	}
	return false;
}
// 新建种子点
int ASSeedData::NewSeed(const QString c_name, const int c_lable, const QColor c_color)
{
	int Label = c_lable;
	QColor crntColor = c_color;
	int numOfSeeds = m_listSeedsData->size();
	if (Label == -1)
	{
		// 自动选择label和颜色
		bool f;
		do 
		{
			Label++;
			f = false;
			for (int i = 0; i < numOfSeeds; i++)
			{
				if (m_listSeedsData->at(i)->label == Label)
				{
					f = true;
					break;
				}
			}
		} while (f);
		crntColor.setRed(qrand() % 256);
		crntColor.setGreen(qrand() % 256);
		crntColor.setBlue(qrand() % 256);
	}

	for (int i = 0; i < numOfSeeds; i++)
	{
		if (m_listSeedsData->at(i)->label == Label || m_listSeedsData->at(i)->name == c_name)
		{
			return -1;
		}
	}
	structSeeds* crntSeed = new structSeeds;
	crntSeed->isShown = true;
	crntSeed->color = crntColor;
	crntSeed->label = Label;
	crntSeed->name = c_name;
	crntSeed->seedId = GetNewSeedsID();
	crntSeed->pListSeedsData = new QList<structLine *>;
	m_listSeedsData->append(crntSeed);
	return crntSeed->seedId;
}

// 种子点数据数量
int ASSeedData::GetNumOfSeedsData()
{
	return ASSeedData::ms_nNumOfSeedsData;
}
// 标签值
int ASSeedData::getLabel()
{
	return m_nLabel;
}
void ASSeedData::setLabel(const int label)
{
	m_nLabel = label;
}
// 添加线
void ASSeedData::AppandLine(structLine* NewLine)
{
	//m_pListSeedsData->append(NewLine);
}
structSeeds* ASSeedData::getSeedsFromID(const int c_SeedsID)
{
	int numofseeds = m_listSeedsData->size();
	for (int i = 0; i < numofseeds; i++)
	{
		if (m_listSeedsData->at(i)->seedId == c_SeedsID)
		{
			return m_listSeedsData->at(i);
		}
	}
	return nullptr;
}
// 撤销
void ASSeedData::AddOperaSequence(const int c_SeedsID)
{
	m_listOperaSequence->append(c_SeedsID);
}
bool ASSeedData::UndoALine()
{
	if (this == nullptr || m_listOperaSequence->isEmpty())
	{
		return false;
	}
	if (m_listSeedsData->isEmpty())
	{
		return false;
	}
	int seedID = m_listOperaSequence->last();
	structSeeds* crntSeed = getSeedsFromID(seedID);
	if (!crntSeed)
	{
		return false;
	}
	if (!crntSeed->pListSeedsData || crntSeed->pListSeedsData->isEmpty())
	{
		return false;
	}
	structLine* crntLine = crntSeed->pListSeedsData->last();
	while (crntLine->pvectorPoint.isEmpty() == false)
	{
		delete crntLine->pvectorPoint.first();
		crntLine->pvectorPoint.removeFirst();
	}
	crntSeed->pListSeedsData->removeLast();
	m_listOperaSequence->removeLast();
	delete crntLine;
	return true;
}
// 属性
double* ASSeedData::getDataExtent()
{
	return nullptr;
}

// 画种子点，由静态函数调用
void ASSeedData::StartPaintingSeeds(double* pPosition, const int ViewLabel, const int c_SeedID)
{
	this->m_pWorkingSeedsLine = new structLine;
	m_pWorkingSeedsLine->ViewLabel = ViewLabel;
	m_pWorkingSeedsLine->dLayer = pPosition[ViewLabel];
	m_pWorkingSeedsLine->nSerial = ++m_crntSerialOfSeeds;

	structPoint* crntPoint = new structPoint;
	crntPoint->dX = pPosition[0];
	crntPoint->dY = pPosition[1];
	crntPoint->dZ = pPosition[2];

	m_pWorkingSeedsLine->pvectorPoint.append(crntPoint);
	structSeeds* crntSeed = getSeedsFromID(c_SeedID);
	if (crntSeed)
	{
		crntSeed->pListSeedsData->append(m_pWorkingSeedsLine);
	}
}
void ASSeedData::ContinuePaintingSeeds(double* pPosition, const int ViewLabel)
{
	structPoint* crntPoint = new structPoint;
	crntPoint->dX = pPosition[0];
	crntPoint->dY = pPosition[1];
	crntPoint->dZ = pPosition[2];
	m_pWorkingSeedsLine->pvectorPoint.append(crntPoint);
}
void ASSeedData::EndPaintingSeeds()
{
	m_pWorkingSeedsLine = NULL;
}
// 删除种子点
void ASSeedData::DeleteWholeSeeds(const int c_SeedID)
{
	structSeeds* crntSeeds = this->getSeedsFromID(c_SeedID);
	if (!crntSeeds)
	{
		return;
	}
	m_listSeedsData->removeOne(crntSeeds);
	while (crntSeeds->pListSeedsData->size())
	{
		structLine* crntLine = crntSeeds->pListSeedsData->first();
		crntSeeds->pListSeedsData->removeFirst();
		while (crntLine->pvectorPoint.size())
		{
			structPoint* crntPoint = crntLine->pvectorPoint.first();
			crntLine->pvectorPoint.removeFirst();
			delete crntPoint;
		}
		delete crntLine;
	}
	delete crntSeeds->pListSeedsData;
	delete crntSeeds;
}
// 得到一个新的不重复ID
int ASSeedData::GetNewSeedsID()
{
	static int newSeedID = 0;
	return newSeedID++;
}