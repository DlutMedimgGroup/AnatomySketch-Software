#include "ASAddinsInputContour.h"
#include "ASContourData.h"
#include "ASDataStructureWalker.h"
#include "ASOperation.h"
#include "ASAbstractData.h"
#include "ASAddinsBase.h"
#include "ASAddinsComboBox.h"
#include "ASTransSidebarGeneral.h"
#include "stdlib.h"
#include "ASCustomizeDataWriter.h"
#include <QFileInfo>
#include <QDir>
#include <QTextStream>

using namespace std::placeholders;

ASAddinsInputContour::ASAddinsInputContour(ASAddinsBase* parent)
	: ASAddinsInputNode(parent)
{
	m_Type = AddinInput_Contour;
	m_listInputContour = new QList<structInputContour *>;
	m_Name = "Contour";
}

ASAddinsInputContour::~ASAddinsInputContour()
{
	while (m_listInputContour->size() > 0)
	{
		delete m_listInputContour->first()->ComboBox;
		delete m_listInputContour->first();
		m_listInputContour->removeFirst();
	}
	delete m_listInputContour;
}

// Value
void ASAddinsInputContour::SetValue(const QStringList c_value)
{
	if (c_value.size() % 3 != 0)
	{
		return;
	}
	int NumOfContour = c_value.size() / 3;
	while (m_listInputContour->size() > 0)
	{
		delete m_listInputContour->first()->ComboBox;
		delete m_listInputContour->first();
		m_listInputContour->removeFirst();
	}
	for (int i = 0; i < NumOfContour; i++)
	{
		structInputContour* crntIC = new structInputContour;
		crntIC->Name = c_value.at(i * 3);
		crntIC->Label = c_value.at(i * 3 + 1).toInt();
		QStringList slcolor = c_value.at(i * 3 + 2).split("-");
		crntIC->Color.setRed(slcolor.at(0).toInt());
		crntIC->Color.setGreen(slcolor.at(1).toInt());
		crntIC->Color.setBlue(slcolor.at(2).toInt());
		crntIC->ComboBox = new ASAddinsComboBox(nullptr);
		crntIC->ComboBox->addItem(u8"新建轮廓线");
		Fun f = std::bind(&ASAddinsInputContour::ComboBoxCallBack, this, _1);
		crntIC->ComboBox->SetCallBack(f);
		m_listInputContour->append(crntIC);
	}
}
// DataNode
void ASAddinsInputContour::SetContourData(ASContourData* datanode)
{
	this->m_ContourData = datanode;

}
// 得到空指针，用于argc，s为描述
void* ASAddinsInputContour::GetVoidPoint(const QString s)
{
	// 1 对应数据
	if (m_ContourData == nullptr)
	{
		ASAbstractData* abdata = this->m_Parent->GetHost()->getChild(DataType_ContourData);
		m_ContourData = static_cast<ASContourData*>(abdata);
		if (m_ContourData == nullptr)
		{
			return nullptr;
		}
	}
	// 2 生成空指针
	if (s == "Array")
	{
		// Size
		int TotalPoints = 0;
		int numOfContour = m_ContourData->GetNumberOfContour();
		int numOfTarget = m_listInputContour->size();
		for (int i = 0; i < numOfTarget; i++)
		{
			for (int ncrntContour = 0; ncrntContour < numOfContour; ncrntContour++)
			{
				structContour* crntContour = m_ContourData->GetContourAt(ncrntContour);
				if (crntContour->name != m_listInputContour->at(i)->ComboBox->currentText())
				{
					continue;
				}
				int numOfCurves = crntContour->pListCurves->size();
				for (int ncrntCurve = 0; ncrntCurve < numOfCurves; ncrntCurve++)
				{
					structCurve* crntCurve = crntContour->pListCurves->at(ncrntCurve);
					int numOfPoints = crntCurve->pvectorPoint.size();
					TotalPoints += numOfPoints;
				}
			}
		}
		m_Size = TotalPoints;
		if (m_Size == 0)
		{
			return nullptr;
		}
		// Array
		// id; x; y; z; Lable; ViewLabel; closure
		m_Data = new float[TotalPoints * 7];
		int ncrntTotalPoints = 0;
		int ncrntID = 0;
		for (int i = 0; i < numOfTarget; i++)
		{
			for (int ncrntContour = 0; ncrntContour < numOfContour; ncrntContour++)
			{
				structContour* crntContour = m_ContourData->GetContourAt(ncrntContour);
				if (crntContour->name != m_listInputContour->at(i)->ComboBox->currentText())
				{
					continue;
				}
				int crntLabel = crntContour->label;
				int numOfCurves = crntContour->pListCurves->size();
				for (int ncrntCurve = 0; ncrntCurve < numOfCurves; ncrntCurve++)
				{
					structCurve* crntCurve = crntContour->pListCurves->at(ncrntCurve);
					int crntViewLabel = crntCurve->ViewLabel;
					int crntClosure = crntCurve->isClosure;
					int numOfPoints = crntCurve->pvectorPoint.size();
					for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
					{
						structPoint* crntPoint = crntCurve->pvectorPoint.at(ncrntPoint);
						m_Data[ncrntTotalPoints * 7] = ncrntID;
						m_Data[ncrntTotalPoints * 7 + 1] = crntPoint->dX;
						m_Data[ncrntTotalPoints * 7 + 2] = crntPoint->dY;
						m_Data[ncrntTotalPoints * 7 + 3] = crntPoint->dZ;
						m_Data[ncrntTotalPoints * 7 + 4] = crntLabel;
						m_Data[ncrntTotalPoints * 7 + 5] = crntViewLabel;
						m_Data[ncrntTotalPoints * 7 + 6] = crntClosure;
						ncrntTotalPoints++;
					}
					ncrntID++;
				}
			}
		}
		return m_Data;
	}
	else if (s == "Size")
	{
		int TotalPoints = 0;
		int numOfContour = m_ContourData->GetNumberOfContour();
		int numOfTarget = m_listInputContour->size();
		for (int i = 0; i < numOfTarget; i++)
		{
			for (int ncrntContour = 0; ncrntContour < numOfContour; ncrntContour++)
			{
				structContour* crntContour = m_ContourData->GetContourAt(ncrntContour);
				if (crntContour->name != m_listInputContour->at(i)->ComboBox->currentText())
				{
					continue;
				}
				int numOfCurves = crntContour->pListCurves->size();
				for (int ncrntCurve = 0; ncrntCurve < numOfCurves; ncrntCurve++)
				{
					structCurve* crntCurve = crntContour->pListCurves->at(ncrntCurve);
					int numOfPoints = crntCurve->pvectorPoint.size();
					TotalPoints += numOfPoints;
				}
			}
		}
		m_Size = TotalPoints;
		if (m_Size == 0)
		{
			return nullptr;
		}
		return &m_Size;
	}
	return nullptr;
}
// 保存输入到Python程序中的临时文件.1成功，-1失败
int ASAddinsInputContour::PythonInputGenerate()
{
	// 1 对应数据
	if (m_ContourData == nullptr)
	{
		ASAbstractData* abdata = this->m_Parent->GetHost()->getChild(DataType_ContourData);
		m_ContourData = static_cast<ASContourData*>(abdata);
		if (m_ContourData == nullptr)
		{
			return -1;
		}
	}
	if (m_ContourData->GetNumberOfContour() == 0)
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
	file_path = file_path + "/" + this->GetName() + ".ctr";
	ASCustomizeDataWriter Writer;
	Writer.WriteContourData(m_ContourData, file_path);
	return 1;

	//QFile data(file_path);
	//if (data.open(QFile::WriteOnly | QIODevice::Truncate))
	//{
	//	QTextStream out(&data);

	//	// 1 文件头
	//	// 1.1 版本
	//	out << "#Anatomy Sketch Contour File" << endl;

	//	// 1.2 轮廓线数量
	//	out << "#NUMBER OF DATA:" << endl;
	//	int numOfContour = m_ContourData->GetNumberOfContour();
	//	out << numOfContour << endl;
	//	out << "#DATA LIST DEFINITION:" << endl;

	//	// 2 保存各个轮廓线文件
	//	int crntPolyDataNo = 0;
	//	// 2.1 遍历曲面，依次保存		
	//	for (int crntContourDataNo = 0; crntContourDataNo < numOfContour; crntContourDataNo++)
	//	{
	//		// 2.1.0 当前轮廓
	//		structContour* crntContour = m_ContourData->GetContourAt(crntContourDataNo);
	//		// 2.1.1 文件头
	//		out << "#DATA LIST NUMBER " << crntPolyDataNo << " DEFINITION:" << endl;
	//		out << crntPolyDataNo << " # data_list_index" << endl;
	//		out << crntContour->name << " # data_name" << endl;
	//		out << crntContour->color.red() << " " << crntContour->color.green() << " "
	//			<< crntContour->color.blue() << " # r_color g_color b_color" << endl;
	//		out << crntContour->label << " # data_label" << endl;

	//		// 2.1.2 number_of_rois
	//		int NumOfCurves = crntContour->pListCurves->size();
	//		out << NumOfCurves << " # number_of_contours" << endl;

	//		// 2.1.3 遍历轮廓，依次保存
	//		for (int ncrntCurves = 0; ncrntCurves < NumOfCurves; ncrntCurves++)
	//		{
	//			structCurve* crntCurve = crntContour->pListCurves->at(ncrntCurves);

	//			// 各层的截面信息
	//			out << "#CONTOUR LIST NUMBER " << ncrntCurves << " DEFINITION:" << endl;
	//			// 写当前Contour
	//			out << ncrntCurves << " # contour_index" << endl;
	//			out << crntCurve->ViewLabel << " # contour_ViewLabel" << endl;
	//			if (crntCurve->isClosure)
	//			{
	//				out << "true # contour_isClosure" << endl;
	//			}
	//			else
	//			{
	//				out << "false # contour_isClosure" << endl;
	//			}
	//			out << crntCurve->dLayer << " # contour_dLayer" << endl;
	//			QList<structPoint*> listPoints = crntCurve->pvectorPoint;
	//			out << "#START_POINTS_POS" << endl;
	//			for (int nCrntPoints = 0; nCrntPoints < listPoints.size(); nCrntPoints++)
	//			{
	//				out << listPoints.at(nCrntPoints)->dX << " " << listPoints.at(nCrntPoints)->dY << " " << listPoints.at(nCrntPoints)->dZ << endl;
	//			}
	//			out << "#END_POINTS_POS" << endl;
	//		}
	//		crntPolyDataNo += 1;
	//	}
	//	// 3 写文件尾
	//	out << "#" << endl;
	//	out << "#END OF CONTOURS DEFINITION" << endl;

	//	return 1;
	//}
	//else
	//{
	//	return -1;
	//}	
}

// 运算后释放数据
void ASAddinsInputContour::FreeMemory()
{
	if (m_Data)
	{
		delete m_Data;
		m_Data = nullptr;
	}
}
// InputContours
int ASAddinsInputContour::GetNumOfInputContour()
{
	return m_listInputContour->size();
}
structInputContour* ASAddinsInputContour::GetInputContourAt(const int c_index)
{
	return m_listInputContour->at(c_index);
}
void ASAddinsInputContour::RefreshCombo(int datatype)
{
	if (!ASContourData::isTypeInheritedStatic(datatype))
	{
		return;
	}
	for (int j = 0; j < m_listInputContour->size(); j++)
	{
		QComboBox* crntCombo = m_listInputContour->at(j)->ComboBox;
		QString oriStr = crntCombo->currentText();
		crntCombo->clear();
		crntCombo->addItem(QObject::tr(u8"请选择轮廓线"));
		crntCombo->addItem(QObject::tr(u8"新建轮廓线"));
		ASOperation* crntOperation = this->m_Parent->GetHost();
		ASContourData* crntContour = static_cast<ASContourData*>(
			crntOperation->getChild(DataType_ContourData));
		if (crntContour)
		{
			int NumOfSeeds = crntContour->GetNumberOfContour();
			for (int k = 0; k < NumOfSeeds; k++)
			{
				crntCombo->addItem(crntContour->GetContourAt(k)->name);
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
void ASAddinsInputContour::ComboBoxCallBack(ASAddinsComboBox* sender)
{
	for (int i = 0; i < m_listInputContour->size(); i++)
	{
		if (sender == m_listInputContour->at(i)->ComboBox &&
			m_listInputContour->at(i)->ComboBox->currentIndex() == 1)
		{
			structInputContour* crntContour = m_listInputContour->at(i);
			ASTransSidebarGeneral::NewFromAddin(DataType_ContourData, crntContour->Name,
				crntContour->Label, crntContour->Color);
			//crntContour->ComboBox->addItem(crntContour->Name);
			crntContour->ComboBox->setCurrentText(crntContour->Name);
		}
	}
}