#include "ASAddinsInputPolyImage.h"
#include "ASPolyImageData.h"
#include "ASDataStructureWalker.h"
#include <vtkPolyData.h>
#include <vtkSTLWriter.h>
#include <QTextCodec>
#include <QFileInfo>
#include <QDir>

ASAddinsInputPolyImage::ASAddinsInputPolyImage(ASAddinsBase* parent)
	: ASAddinsInputNode(parent)
{
	m_Type = AddinInput_PolyImage;
	m_Combobox = new QComboBox();
}

ASAddinsInputPolyImage::~ASAddinsInputPolyImage()
{
	m_Combobox->deleteLater();
}

// DataNode
void ASAddinsInputPolyImage::SetPolyData(ASPolyImageData* data)
{
	this->m_PolyData = data;
}
// Combobox
QComboBox* ASAddinsInputPolyImage::GetCombobox()
{
	return this->m_Combobox;
}
// ImageData
ASPolyImageData* ASAddinsInputPolyImage::GetPolyData()
{
	if (m_PolyData)
	{
		return m_PolyData;
	}
	// 1 对应数据
	ASPolyImageData* PolyImage = nullptr;
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_PolyImageData);
		item != nullptr;
		item = ASDataStructureWalker::next_inherit(DataType_PolyImageData))
	{
		if (item->getName() == m_Combobox->currentText())
		{
			PolyImage = static_cast<ASPolyImageData*>(item);
		}
	}
	if (PolyImage == nullptr)
	{
		return nullptr;
	}
	return PolyImage;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsInputPolyImage::GetVoidPoint(const QString s)
{
	ASPolyImageData* datanode = this->GetPolyData();
	vtkSmartPointer<vtkPolyData> PolyData = datanode->getPolyImageData();
	if (s == "PointsArray")
	{
		int NumOfPoints = PolyData->GetPoints()->GetNumberOfPoints();
		m_PointsArray = new float[NumOfPoints * 3];
		for (int i = 0; i < NumOfPoints; i++)
		{
			m_PointsArray[3 * i] = PolyData->GetPoint(i)[0];
			m_PointsArray[3 * i + 1] = PolyData->GetPoint(i)[1];
			m_PointsArray[3 * i + 2] = PolyData->GetPoint(i)[2];
		}
		return m_PointsArray;
	}
	else if (s == "PointsSize")
	{
		m_PointsSize = PolyData->GetPoints()->GetNumberOfPoints();
		return &m_PointsSize;
	}
	else if (s == "Edges")
	{
		int numOfCells = PolyData->GetPolys()->GetNumberOfCells();
		int numOfCellPoints = 0;
		for (int i = 0; i < numOfCells; i++)
		{
			vtkCell* crntCell = PolyData->GetCell(i);
			vtkIdList* idlist = crntCell->GetPointIds();
			int numofIDs = idlist->GetNumberOfIds();
			numOfCellPoints += numofIDs;
		}
		int* Edges = new int[numOfCellPoints];
		int EdgesIndex = 0;
		for (int i = 0; i < numOfCells; i++)
		{
			vtkCell* crntCell = PolyData->GetCell(i);
			vtkIdList* idlist = crntCell->GetPointIds();
			int numofIDs = idlist->GetNumberOfIds();
			for (int j = 0; j < numofIDs; j++)
			{
				Edges[EdgesIndex] = idlist->GetId(j);
				EdgesIndex++;
			}
		}
		return Edges;
	}
	else if (s == "EdgesGroup")
	{
		int numOfCells = PolyData->GetPolys()->GetNumberOfCells();
		m_EdgesGroup = new int[numOfCells];
		for (int i = 0; i < numOfCells; i++)
		{
			vtkCell* crntCell = PolyData->GetCell(i);
			vtkIdList* idlist = crntCell->GetPointIds();
			int numofIDs = idlist->GetNumberOfIds();
			m_EdgesGroup[i] = numofIDs;
		}
		return m_EdgesGroup;
	}
	else if (s == "EdgesGroupSize")
	{
		m_EdgesGroupSize = PolyData->GetPolys()->GetNumberOfCells();
		return &m_EdgesGroupSize;
	}
	else
	{
		return nullptr;
	}
}
// 保存输入到Python程序中的临时文件
int ASAddinsInputPolyImage::PythonInputGenerate()
{
	ASPolyImageData* datanode = this->GetPolyData();
	vtkSmartPointer<vtkPolyData> data = datanode->getPolyImageData();
	QString path = _pgmptr;
	QFileInfo fi = QFileInfo(path);
	QString file_path = fi.absolutePath();
	file_path = file_path + "/Plugins/_datacacke";
	QDir dir(file_path);
	if (dir.exists() == false)
	{
		dir.mkpath(file_path);
	}
	file_path = file_path + "/" + this->GetName() + ".stl";
	vtkSmartPointer<vtkSTLWriter> STLWriter = vtkSmartPointer<vtkSTLWriter>::New();
	STLWriter->SetInputData(data);
	std::string strStd = file_path.toStdString();
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	if (code) strStd = code->fromUnicode(file_path).data();
	STLWriter->SetFileName(strStd.data());
	STLWriter->Write();
	return 1;
}

// 运算后释放数据
void ASAddinsInputPolyImage::FreeMemory()
{
	if (m_PointsArray)
	{
		delete m_PointsArray;
		m_PointsArray = nullptr;
	}
	if (m_Edges)
	{
		delete m_Edges;
		m_Edges = nullptr;
	}
	if (m_EdgesGroup)
	{
		delete m_EdgesGroup;
		m_EdgesGroup = nullptr;
	}
}

void ASAddinsInputPolyImage::RefreshCombo(int datatype)
{
	if (!ASPolyImageData::isTypeInheritedStatic(datatype))
	{
		return;
	}
	QString oriStr = m_Combobox->currentText();
	m_Combobox->clear();
	m_Combobox->addItem(QObject::tr(u8"请选择网格数据"));
	m_Combobox->addItem(QObject::tr(u8"新建网格数据"));
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_PolyImageData);
		item != nullptr;
		item = ASDataStructureWalker::next_inherit(DataType_PolyImageData))
	{
		m_Combobox->addItem(item->getName());
	}
	int i = m_Combobox->findText(oriStr);
	if (i >= 2)
	{
		m_Combobox->setCurrentIndex(i);
	}
	else
	{
		m_Combobox->setCurrentIndex(0);
	}
}