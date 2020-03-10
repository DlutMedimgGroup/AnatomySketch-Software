#include "ASAddinsInputLandmark.h"
#include "ASLandmarkData.h"
#include "ASDataStructureWalker.h"
#include "ASAddinsComboBox.h"
#include "ASAddinsBase.h"
#include "ASOperation.h"
#include "ASTransSidebarGeneral.h"
#include "ASCustomizeDataWriter.h"
#include <QByteArray>
#include <QFileInfo>
#include <QDir>

using namespace std::placeholders;

ASAddinsInputLandmark::ASAddinsInputLandmark(ASAddinsBase* parent)
	: ASAddinsInputNode(parent)
{
	m_Type = AddinInput_Landmark;
	m_listInputLandMark = new QList<structInputLandMark *>;
	m_Name = "Landmark";
}

ASAddinsInputLandmark::~ASAddinsInputLandmark()
{
	while (m_listInputLandMark->size() > 0)
	{
		delete m_listInputLandMark->first()->ComboBox;
		delete m_listInputLandMark->first();
		m_listInputLandMark->removeFirst();
	}
	delete m_listInputLandMark;
}

// Value
void ASAddinsInputLandmark::SetValue(const QStringList c_value)
{
	if (c_value.size() % 3 != 0)
	{
		return;
	}
	int NumOfContour = c_value.size() / 3;
	while (m_listInputLandMark->size() > 0)
	{
		delete m_listInputLandMark->first()->ComboBox;
		delete m_listInputLandMark->first();
		m_listInputLandMark->removeFirst();
	}
	for (int i = 0; i < NumOfContour; i++)
	{
		structInputLandMark* crntIC = new structInputLandMark;
		crntIC->Name = c_value.at(i * 3);
		crntIC->Label = c_value.at(i * 3 + 1).toInt();
		QStringList slcolor = c_value.at(i * 3 + 2).split("-");
		crntIC->Color.setRed(slcolor.at(0).toInt());
		crntIC->Color.setGreen(slcolor.at(1).toInt());
		crntIC->Color.setBlue(slcolor.at(2).toInt());
		crntIC->ComboBox = new ASAddinsComboBox(nullptr);
		crntIC->ComboBox->addItem(u8"新建标定点");
		Fun f = std::bind(&ASAddinsInputLandmark::ComboBoxCallBack, this, _1);
		crntIC->ComboBox->SetCallBack(f);
		m_listInputLandMark->append(crntIC);
	}
}
// DataNode
void ASAddinsInputLandmark::SetLandmarkData(ASLandmarkData* datanode)
{
	this->m_LandmarkData = datanode;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsInputLandmark::GetVoidPoint(const QString s)
{
	// 1 对应数据
	if (m_LandmarkData == nullptr)
	{
		ASAbstractData* abdata = this->m_Parent->GetHost()->getChild(DataType_LandmarkData);
		m_LandmarkData = static_cast<ASLandmarkData*>(abdata);
		if (m_LandmarkData == nullptr)
		{
			return nullptr;
		}
	}
	if (m_LandmarkData->GetNumberOfLandmark() == 0)
	{
		return nullptr;
	}
	// 2 生成空指针
	if (s == "Array")
	{
		int numOfLdm = m_LandmarkData->GetNumberOfLandmark();
		m_Data = new float[numOfLdm * 4];
		for (int i = 0; i < numOfLdm; i++)
		{
			structLandmark* crntLandmark = m_LandmarkData->GetLandmarkAt(i);
			m_Data[i * 4] = crntLandmark->LandmarkID;
			m_Data[i * 4 + 1] = crntLandmark->dPosition[0];
			m_Data[i * 4 + 2] = crntLandmark->dPosition[1];
			m_Data[i * 4 + 3] = crntLandmark->dPosition[2];
		}
		return m_Data;
	}
	else if (s == "Size")
	{
		m_Size = m_LandmarkData->GetNumberOfLandmark();
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
int ASAddinsInputLandmark::PythonInputGenerate()
{
	// 1 对应数据
	if (m_LandmarkData == nullptr)
	{
		ASAbstractData* abdata = this->m_Parent->GetHost()->getChild(DataType_LandmarkData);
		m_LandmarkData = static_cast<ASLandmarkData*>(abdata);
		if (m_LandmarkData == nullptr)
		{
			return -1;
		}
	}
	if (m_LandmarkData->GetNumberOfLandmark() == 0)
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
	file_path = file_path + "/" + this->GetName() + ".ldm";
	ASCustomizeDataWriter Writer;
	Writer.WriteLandmarkData(m_LandmarkData, file_path);
	return 1;
}
// 运算后释放数据
void ASAddinsInputLandmark::FreeMemory()
{
	if (m_Data)
	{
		delete m_Data;
		m_Data = nullptr;
	}
}
// InputContours
int ASAddinsInputLandmark::GetNumOfInputLandMark()
{
	return m_listInputLandMark->size();
}
structInputLandMark* ASAddinsInputLandmark::GetInputLandMarkAt(const int c_index)
{
	return m_listInputLandMark->at(c_index);
}
void ASAddinsInputLandmark::RefreshCombo(int datatype)
{
	if (!ASLandmarkData::isTypeInheritedStatic(datatype))
	{
		return;
	}
	for (int j = 0; j < m_listInputLandMark->size(); j++)
	{
		QComboBox* crntCombo = m_listInputLandMark->at(j)->ComboBox;
		QString oriStr = crntCombo->currentText();
		crntCombo->clear();
		crntCombo->addItem(QObject::tr(u8"请选择标定点"));
		crntCombo->addItem(QObject::tr(u8"新建标定点"));
		ASOperation* crntOperation = this->m_Parent->GetHost();
		ASLandmarkData* crntLandmark = static_cast<ASLandmarkData*>(
			crntOperation->getChild(DataType_ContourData));
		if (crntLandmark)
		{
			int NumOfLandmarks = crntLandmark->GetNumberOfLandmark();
			for (int k = 0; k < NumOfLandmarks; k++)
			{
				crntCombo->addItem(crntLandmark->GetLandmarkAt(k)->qsName);
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
void ASAddinsInputLandmark::ComboBoxCallBack(ASAddinsComboBox* sender)
{
	for (int i = 0; i < m_listInputLandMark->size(); i++)
	{
		if (sender == m_listInputLandMark->at(i)->ComboBox &&
			m_listInputLandMark->at(i)->ComboBox->currentIndex() == 1)
		{
			structInputLandMark* crntContour = m_listInputLandMark->at(i);
			ASTransSidebarGeneral::NewFromAddin(DataType_LandmarkData, crntContour->Name,
				crntContour->Label, crntContour->Color);
			//crntContour->ComboBox->addItem(crntContour->Name);
			crntContour->ComboBox->setCurrentText(crntContour->Name);
		}
	}
}