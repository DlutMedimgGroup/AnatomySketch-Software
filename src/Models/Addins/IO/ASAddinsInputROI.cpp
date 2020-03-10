#include "ASAddinsInputROI.h"
#include "ASROIData.h"
#include "ASDataStructureWalker.h"
#include "ASROITypeBase.h"
#include "ASROITypeCube.h"
#include "ASOperation.h"
#include "ASROIData.h"
#include "ASAddinsBase.h"
#include "ASAddinsComboBox.h"
#include "ASTransSidebarGeneral.h"
#include "ASCustomizeDataWriter.h"
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

using namespace std::placeholders;

ASAddinsInputROI::ASAddinsInputROI(ASAddinsBase* parent)
	: ASAddinsInputNode(parent)
{
	m_Type = AddinInput_ROI;
	m_listInputROI = new QList<structInputROI *>;
	m_Name = "ROI";
}

ASAddinsInputROI::~ASAddinsInputROI()
{

}

// Value
void ASAddinsInputROI::SetValue(const QStringList c_value)
{
	// 0 name; 1 label; 2 color; 3 auto or unauto
	if (c_value.size() % 4 != 0)
	{
		return;
	}
	int NumOfROI = c_value.size() / 3;
	while (m_listInputROI->size() > 0)
	{
		delete m_listInputROI->first()->ComboBox;
		delete m_listInputROI->first();
		m_listInputROI->removeFirst();
	}
	for (int i = 0; i < NumOfROI; i++)
	{
		structInputROI* crntROI = new structInputROI;
		crntROI->Name = c_value.at(i * 3);
		crntROI->Label = c_value.at(i * 3 + 1).toInt();
		QStringList slcolor = c_value.at(i * 3 + 2).split("-");
		crntROI->Color.setRed(slcolor.at(0).toInt());
		crntROI->Color.setGreen(slcolor.at(1).toInt());
		crntROI->Color.setBlue(slcolor.at(2).toInt());
		if (c_value.at(i * 3 + 3) == "auto")
		{
			crntROI->autoROI = true;
		} 
		else
		{
			crntROI->autoROI = false;
		}
		crntROI->ComboBox = new ASAddinsComboBox(nullptr);
		crntROI->ComboBox->addItem(u8"新建ROI");
		Fun f = std::bind(&ASAddinsInputROI::ComboBoxCallBack, this, _1);
		crntROI->ComboBox->SetCallBack(f);
		m_listInputROI->append(crntROI);
	}
}
// DataNode
void ASAddinsInputROI::SetROIData(ASROIData* datanode)
{
	this->m_ROIData = datanode;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsInputROI::GetVoidPoint(const QString s)
{
	// 1 对应数据
	if (m_ROIData == nullptr)
	{
		ASAbstractData* abdata = this->m_Parent->GetHost()->getChild(DataType_ROI);
		m_ROIData = static_cast<ASROIData*>(abdata);
		if (m_ROIData == nullptr)
		{
			return nullptr;
		}
	}
	if (m_ROIData->GetNumOfROIs() == 0)
	{
		return nullptr;
	}
	// 2 
	if (s == "Cube")
	{
		// 方形roi
		int numOfROIs = m_ROIData->GetNumOfROIs();
		int numOfCubes = 0;
		for (int ncrntROI = 0; ncrntROI < numOfROIs; ncrntROI++)
		{
			ASROITypeBase* crntROI = m_ROIData->GetROIAt(ncrntROI);
			if (crntROI->isTypeInherited(ROIType_Cube))
			{
				numOfCubes += 1;
			}
		}
		if (numOfCubes == 0)
		{
			return nullptr;
		}
		m_Data = new float[numOfCubes * 7];
		int ncrntTotalCube = 0;
		double* range;
		for (int ncrntROI = 0; ncrntROI < numOfROIs; ncrntROI++)
		{
			ASROITypeBase* crntROI = m_ROIData->GetROIAt(ncrntROI);
			if (crntROI->isTypeInherited(ROIType_Cube))
			{
				ASROITypeCube* crntCube = static_cast<ASROITypeCube*>(crntROI);
				range = crntCube->GetRange();
				m_Data[ncrntTotalCube * 7] = ncrntTotalCube;
				m_Data[ncrntTotalCube * 7 + 1] = range[0];
				m_Data[ncrntTotalCube * 7 + 2] = range[1];
				m_Data[ncrntTotalCube * 7 + 3] = range[2];
				m_Data[ncrntTotalCube * 7 + 4] = range[3];
				m_Data[ncrntTotalCube * 7 + 5] = range[4];
				m_Data[ncrntTotalCube * 7 + 6] = range[5];
				ncrntTotalCube += 1;
			}
		}
		return m_Data;
	}
	else if (s == "CubeSize")
	{
		int numOfROIs = m_ROIData->GetNumOfROIs();
		int numOfCubes = 0;
		for (int ncrntROI = 0; ncrntROI < numOfROIs; ncrntROI++)
		{
			ASROITypeBase* crntROI = m_ROIData->GetROIAt(ncrntROI);
			if (crntROI->isTypeInherited(ROIType_Cube))
			{
				numOfCubes += 1;
			}
		}
		if (numOfCubes == 0)
		{
			return nullptr;
		}
		m_CubeSize = numOfCubes;
		return &m_CubeSize;
	}
	return nullptr;
}
// 保存输入到Python程序中的临时文件.1成功，-1失败
int ASAddinsInputROI::PythonInputGenerate()
{
	// 1 对应数据
	if (m_ROIData == nullptr)
	{
		ASAbstractData* abdata = this->m_Parent->GetHost()->getChild(DataType_ROI);
		m_ROIData = static_cast<ASROIData*>(abdata);
		if (m_ROIData == nullptr)
		{
			return -1;
		}
	}
	if (m_ROIData->GetNumOfROIs() == 0)
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
	file_path = file_path + "/" + this->GetName() + ".roi";
	ASCustomizeDataWriter Writer;
	Writer.WriteROIData(m_ROIData, file_path);
	return 1;
}

// 运算后释放数据
void ASAddinsInputROI::FreeMemory()
{
	if (m_Data)
	{
		delete m_Data;
		m_Data = nullptr;
	}
}
// InputROI
int ASAddinsInputROI::GetNumOfInputROI()
{
	return m_listInputROI->size();
}
structInputROI* ASAddinsInputROI::GetInputROIAt(const int c_index)
{
	return m_listInputROI->at(c_index);
}
void ASAddinsInputROI::RefreshCombo(int datatype)
{
	if (!ASROIData::isTypeInheritedStatic(datatype))
	{
		return;
	}
	for (int j = 0; j < m_listInputROI->size(); j++)
	{
		QComboBox* crntCombo = m_listInputROI->at(j)->ComboBox;
		QString oriStr = crntCombo->currentText();
		crntCombo->clear();
		crntCombo->addItem(QObject::tr(u8"请选择包围盒"));
		crntCombo->addItem(QObject::tr(u8"新建包围盒"));
		if (m_listInputROI->first()->autoROI)
		{
			crntCombo->addItem(QObject::tr(u8"自动选取"));
		}

		ASOperation* crntOperation = this->m_Parent->GetHost();
		ASROIData* crntROI = static_cast<ASROIData*>(
			crntOperation->getChild(DataType_ROI));
		if (crntROI)
		{
			int NumOfROIs = crntROI->GetNumOfROIs();
			for (int k = 0; k < NumOfROIs; k++)
			{
				crntCombo->addItem(crntROI->GetROIAt(k)->GetName());
			}
		}
		int i = crntCombo->findText(oriStr);
		if (i >= 2)
		{
			crntCombo->setCurrentIndex(i);
		}
		else
		{
			if (m_listInputROI->first()->autoROI)
			{
				crntCombo->setCurrentIndex(2);
			} 
			else
			{
				crntCombo->setCurrentIndex(0);
			}			
		}
	}
}
// ComboBox回调函数
void ASAddinsInputROI::ComboBoxCallBack(ASAddinsComboBox* sender)
{
	for (int i = 0; i < m_listInputROI->size(); i++)
	{
		if (sender == m_listInputROI->at(i)->ComboBox &&
			m_listInputROI->at(i)->ComboBox->currentIndex() == 1)
		{
			structInputROI* crntROI = m_listInputROI->at(i);
			ASTransSidebarGeneral::NewFromAddin(DataType_ROI, crntROI->Name,
				crntROI->Label, crntROI->Color);
			//crntROI->ComboBox->addItem(crntROI->Name);
			crntROI->ComboBox->setCurrentText(crntROI->Name);
		}
	}
}