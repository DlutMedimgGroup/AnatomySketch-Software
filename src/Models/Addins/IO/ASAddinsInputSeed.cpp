#include "ASAddinsInputSeed.h"
#include "ASSeedData.h"
#include "ASArrayImageData.h"
#include "ASDataStructureWalker.h"
#include "ASOperation.h"
#include "ASAddinsBase.h"
#include "ASAddinsComboBox.h"
#include "ASTransSidebarGeneral.h"
#include "ASCustomizeDataWriter.h"
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QObject>

using namespace std::placeholders;

ASAddinsInputSeed::ASAddinsInputSeed(ASAddinsBase* parent)
	: ASAddinsInputNode(parent)
{
	m_Type = AddinInput_Seed;
	m_listInputSeed = new QList<structInputSeed *>;
	m_Name = "Seed";
}

ASAddinsInputSeed::~ASAddinsInputSeed()
{
	while (m_listInputSeed->size() > 0)
	{
		delete m_listInputSeed->first()->ComboBox;
		delete m_listInputSeed->first();
		m_listInputSeed->removeFirst();
	}
	delete m_listInputSeed;
}

// Value
void ASAddinsInputSeed::SetValue(const QStringList c_value)
{
	if (c_value.size() % 3 != 0)
	{
		return;
	}
	int NumOfSeed = c_value.size() / 3;
	while (m_listInputSeed->size() > 0)
	{
		delete m_listInputSeed->first()->ComboBox;
		delete m_listInputSeed->first();
		m_listInputSeed->removeFirst();
	}
	for (int i = 0; i < NumOfSeed; i++)
	{
		structInputSeed* crntIS = new structInputSeed;
		crntIS->Name = c_value.at(i * 3);
		crntIS->Label = c_value.at(i * 3 + 1).toInt();
		QStringList slcolor = c_value.at(i * 3 + 2).split("-");
		crntIS->Color.setRed(slcolor.at(0).toInt());
		crntIS->Color.setGreen(slcolor.at(1).toInt());
		crntIS->Color.setBlue(slcolor.at(2).toInt());
		crntIS->ComboBox = new ASAddinsComboBox(nullptr);
		crntIS->ComboBox->addItem(u8"新建种子点");
		Fun f = std::bind(&ASAddinsInputSeed::ComboBoxCallBack, this, _1);
		crntIS->ComboBox->SetCallBack(f);
		m_listInputSeed->append(crntIS);
	}
}
// DataNode
void ASAddinsInputSeed::SetSeedData(ASSeedData* datanode)
{
	this->m_SeedData = datanode;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsInputSeed::GetVoidPoint(const QString s)
{
	// 1 对应数据
	if (m_SeedData == nullptr)
	{
		ASAbstractData* abdata = this->m_Parent->GetHost()->getChild(DataType_SeedData);
		m_SeedData = static_cast<ASSeedData*>(abdata);
		if (m_SeedData == nullptr)
		{
			return nullptr;
		}
	}
	// 2 
	if (s == "Array")
	{
		m_Size = this->UpdatemData();
		if (m_Size == 0)
		{
			return nullptr;
		}
		return m_Data;
	}
	else if (s == "Size")
	{
		m_Size = this->UpdatemData();
		if (m_Size == 0)
		{
			return nullptr;
		}
		return &m_Size;
	}
	else
	{
		return nullptr;
	}
}
// 保存输入到Python程序中的临时文件.1成功，-1失败
int ASAddinsInputSeed::PythonInputGenerate()
{
	// 1 对应数据
	if (m_SeedData == nullptr)
	{
		ASAbstractData* abdata = this->m_Parent->GetHost()->getChild(DataType_SeedData);
		m_SeedData = static_cast<ASSeedData*>(abdata);
		if (m_SeedData == nullptr)
		{
			return -1;
		}
	}
	if (m_SeedData->getNumberOfSeeds() == 0)
	{
		return -1;
	}	
	// 2 保存文件
	QString path = _pgmptr;
	QFileInfo fi = QFileInfo(path);
	QString file_path = fi.absolutePath();
	file_path = file_path + "/Plugins/_datacacke";
	QDir dir(file_path);
	if (dir.exists() == false)
	{
		dir.mkpath(file_path);
	}
	file_path = file_path + "/" + this->GetName() + ".seed";
	ASCustomizeDataWriter Writer;
	Writer.WriteSeedData(m_SeedData, file_path);
	return 1;
}

// 运算后释放数据
void ASAddinsInputSeed::FreeMemory()
{
	if (m_Data)
	{
		delete m_Data;
		m_Data = nullptr;
	}
}
// InputSeeds
int ASAddinsInputSeed::GetNumOfInputSeed()
{
	return m_listInputSeed->size();
}
structInputSeed* ASAddinsInputSeed::GetInputSeedAt(const int c_index)
{
	return m_listInputSeed->at(c_index);
}
void ASAddinsInputSeed::RefreshCombo(int datatype)
{
	if (!ASSeedData::isTypeInheritedStatic(datatype))
	{
		return;
	}
	for (int j = 0; j < m_listInputSeed->size(); j++)
	{
		QComboBox* crntCombo = m_listInputSeed->at(j)->ComboBox;
		QString oriStr = crntCombo->currentText();
		crntCombo->clear();
		crntCombo->addItem(QObject::tr(u8"请选择种子点"));
		crntCombo->addItem(QObject::tr(u8"新建种子点"));
		ASOperation* crntOperation = this->m_Parent->GetHost();
		ASSeedData* crntSeed = static_cast<ASSeedData*>(
			crntOperation->getChild(DataType_SeedData));
		if (crntSeed)
		{
			int NumOfSeeds = crntSeed->getNumberOfSeeds();
			for (int k = 0; k < NumOfSeeds; k++)
			{
				crntCombo->addItem(crntSeed->getSeedsAt(k)->name);
			}
		}
		int i = crntCombo->findText(oriStr);
		if (i >= 2)
		{
			crntCombo->setCurrentIndex(i);
		}
		else
		{
			crntCombo->setCurrentIndex(0);
		}
	}
}
// ComboBox回调函数
void ASAddinsInputSeed::ComboBoxCallBack(ASAddinsComboBox* sender)
{
	for (int i = 0; i < m_listInputSeed->size(); i++)
	{
		if (sender == m_listInputSeed->at(i)->ComboBox &&
			m_listInputSeed->at(i)->ComboBox->currentIndex() == 1)
		{
			structInputSeed* crntSeed = m_listInputSeed->at(i);
			ASTransSidebarGeneral::NewFromAddin(DataType_SeedData, crntSeed->Name,
				crntSeed->Label, crntSeed->Color);
			//crntSeed->ComboBox->addItem(crntSeed->Name);
			crntSeed->ComboBox->setCurrentText(crntSeed->Name);
		}
	}
}

int ASAddinsInputSeed::UpdatemData()
{
	int TotalPoints = 0;
	int numOfSeeds = m_SeedData->getNumberOfSeeds();
	int numOfTarget = m_listInputSeed->size();
	for (int i = 0; i < numOfTarget; i++)
	{
		for (int ncrntSeeds = 0; ncrntSeeds < numOfSeeds; ncrntSeeds++)
		{
			structSeeds* crntSeed = m_SeedData->getSeedsAt(ncrntSeeds);
			if (crntSeed->name != m_listInputSeed->at(i)->ComboBox->currentText())
			{
				continue;
			}
			int numOfLines = crntSeed->pListSeedsData->size();
			for (int ncrntLine = 0; ncrntLine < numOfLines; ncrntLine++)
			{
				structLine* crntLine = crntSeed->pListSeedsData->at(ncrntLine);
				int numOfPoints = crntLine->pvectorPoint.size();
				TotalPoints += numOfPoints;
			}
		}
	}
	// x; y; z; Lable; ViewLabel; nSerial
	m_Data = new float[TotalPoints * 7];
	int ncrntTotalPoints = 0;
	int ncrntID = 0;
	for (int i = 0; i < numOfTarget; i++)
	{
		for (int ncrntSeeds = 0; ncrntSeeds < numOfSeeds; ncrntSeeds++)
		{
			structSeeds* crntSeed = m_SeedData->getSeedsAt(ncrntSeeds);
			if (crntSeed->name != m_listInputSeed->at(i)->ComboBox->currentText())
			{
				continue;
			}
			int crntLabel = crntSeed->label;
			int numOfLines = crntSeed->pListSeedsData->size();
			for (int ncrntLine = 0; ncrntLine < numOfLines; ncrntLine++)
			{
				structLine* crntLine = crntSeed->pListSeedsData->at(ncrntLine);
				int crntViewLabel = crntLine->ViewLabel;
				int crntSerial = crntLine->nSerial;
				int numOfPoints = crntLine->pvectorPoint.size();
				for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
				{
					structPoint* crntPoint = crntLine->pvectorPoint.at(ncrntPoint);
					m_Data[ncrntTotalPoints * 7] = ncrntID;
					m_Data[ncrntTotalPoints * 7 + 1] = crntPoint->dX;
					m_Data[ncrntTotalPoints * 7 + 2] = crntPoint->dY;
					m_Data[ncrntTotalPoints * 7 + 3] = crntPoint->dZ;
					m_Data[ncrntTotalPoints * 7 + 4] = crntLabel;
					m_Data[ncrntTotalPoints * 7 + 5] = crntViewLabel;
					m_Data[ncrntTotalPoints * 7 + 6] = crntSerial;
					ncrntTotalPoints++;
				}
				ncrntID++;
			}
		}
	}
	return TotalPoints;
}