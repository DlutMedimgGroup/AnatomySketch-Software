#include "ASAddinsOutputPolyImage.h"
#include "ASAbstractData.h"
#include "ASPolyImageData.h"
#include "ASAddinsBase.h"
#include "ASPipelineBase.h"
#include "ASPipelineManager.h"
#include "ASPipelinePolyImage.h"
#include "ASVisualizationManager.h"
#include "ASDataTree.h"
#include "ASTransAddDeleteData.h"
#include "ASOperation.h"
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkPolygon.h>
#include <vtkPolyData.h>
#include <vtkSTLReader.h>
#include <QFileInfo>
#include <QTextCodec>

ASAddinsOutputPolyImage::ASAddinsOutputPolyImage(ASAddinsBase* parent)
	: ASAddinsOutputNode(parent)
{
	m_Type = AddinOutput_PolyImage;
}

ASAddinsOutputPolyImage::~ASAddinsOutputPolyImage()
{

}

// 得到空指针，用于argc，s为描述
void* ASAddinsOutputPolyImage::GetVoidPoint(const QString s)
{
	if (s == "PointsArray")
	{
		m_pPoints = new float*;
		return m_pPoints;
	}
	else if (s == "PointsSize")
	{
		return &m_NumOfPoints;
	}
	else if (s == "Edges")
	{
		m_pEdges = new int*;
		return m_pEdges;
	}
	else if (s == "EdgesGroup")
	{
		m_pGroupEdges = new int*;
		return m_pGroupEdges;
	}
	else if (s == "EdgesGroupSize")
	{
		return &m_NumOfGroup;
	}
	return nullptr;
}
void ASAddinsOutputPolyImage::GenerateFinished(const int InterfaceType)
{
	vtkSmartPointer<vtkPolyData> PolyData;
	if (InterfaceType == InterfaceType_DLL)
	{
		if (m_pPoints == nullptr)
		{
			qDebug("error: Bad return points array.");
			return;
		}
		if (m_pEdges == nullptr)
		{
			qDebug("error: Bad return edges array.");
			return;
		}
		// 0 建立数据
		vtkSmartPointer<vtkPoints> Points = vtkSmartPointer<vtkPoints>::New();
		for (int i = 0; i < m_NumOfPoints; i++)
		{
			Points->InsertNextPoint((*m_pPoints) + i * 3);
		}
		Points->Modified();
		vtkSmartPointer<vtkCellArray> CellArray = vtkSmartPointer<vtkCellArray>::New();
		int index = 0;
		for (int i = 0; i < m_NumOfGroup; i++)
		{
			vtkSmartPointer<vtkPolygon> Polygon = vtkSmartPointer<vtkPolygon>::New();
			Polygon->GetPointIds()->SetNumberOfIds((*m_pGroupEdges)[i]);
			for (int j = 0; j < (*m_pGroupEdges)[i]; j++)
			{
				int aaa = (*m_pEdges)[index];
				Polygon->GetPointIds()->SetId(j, (*m_pEdges)[index++]);
			}
			CellArray->InsertNextCell(Polygon);
		}
		CellArray->Modified();
		PolyData = vtkSmartPointer<vtkPolyData>::New();
		PolyData->SetPoints(Points);
		PolyData->SetPolys(CellArray);
		PolyData->Modified();
	}
	else if (InterfaceType == InterfaceType_Python)
	{
		vtkSmartPointer<vtkSTLReader> reader = vtkSmartPointer<vtkSTLReader>::New();
		QString path = _pgmptr;
		QFileInfo fi = QFileInfo(path);
		QString file_path = fi.absolutePath();
		file_path = file_path + "/Plugins/_datacacke";
		file_path = file_path + "/" + this->GetName() + ".stl";
		std::string strStd = file_path.toStdString();
		QTextCodec *code = QTextCodec::codecForName("gb18030");
		if (code) strStd = code->fromUnicode(file_path).data();
		reader->SetFileName(strStd.data());
		reader->Update();
		PolyData = reader->GetOutput();
	}
	else
	{
		return;
	}

	// 1 判断覆盖或新建
	ASOperation* host = this->m_Parent->GetHost();
	QString s = host->getName() + "_result";
	QList<ASAbstractData*>* child = host->getChild();
	for (int i = 0; i < child->size(); i++)
	{
		ASAbstractData* c = child->at(i);
		if (!c->isTypeInherited(DataType_PolyImageData))
		{
			continue;
		}
		if (c->getName() == s)
		{
			ASPolyImageData* a = static_cast<ASPolyImageData*>(c);
			a->setPolyData(PolyData);

			QList<ASPipelineBase*>* listPipe = new QList<ASPipelineBase*>;
			ASPipelineManager::GetPipelineForDataID(listPipe, a->getDataNodeID());
			for (int ncrntPipe = 0; ncrntPipe < listPipe->size(); ncrntPipe++)
			{
				if (listPipe->at(ncrntPipe)->isTypeInherited(PipeLineType_PolyImage))
				{
					ASPipelinePolyImage* crntPipe = static_cast<ASPipelinePolyImage*>(listPipe->at(ncrntPipe));
					crntPipe->SetInputData(a->getPolyImageData());
					crntPipe->UpdataInput();
					ASVisualizationManager::Render();
				}
			}
			return;
		}
	}
	// New datanode
	ASPolyImageData* NewData = new ASPolyImageData();
	NewData->setPolyData(PolyData);
	NewData->setName(s);
	NewData->setVoidNode(false);
	NewData->DataInitAfterLoad();
	ASDataTree::NewDataNode(NewData, host);
	bool bResetCarmera = (ASPipelineBase::GetNumOfObj2D() == 0);
	ASTransAddDeleteData::AddNewPipeline(NewData, DataType_PolyImageData);	
	ASTransAddDeleteData::UpdateDispleyAfterAddDelete(bResetCarmera);
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_PolyImageData);
}

// 运算后释放数据
void ASAddinsOutputPolyImage::FreeMemory()
{
	if (m_pPoints)
	{
		if (*m_pPoints)
		{
			delete *m_pPoints;
			*m_pPoints = nullptr;
		}
		delete m_pPoints;
		m_pPoints = nullptr;
	}
	if (m_pEdges)
	{
		if (*m_pEdges)
		{
			delete *m_pEdges;
			*m_pEdges = nullptr;
		}
		delete m_pEdges;
		m_pEdges = nullptr;
	}
	if (m_pGroupEdges)
	{
		if (*m_pGroupEdges)
		{
			delete *m_pGroupEdges;
			*m_pGroupEdges = nullptr;
		}
		delete m_pGroupEdges;
		m_pGroupEdges = nullptr;
	}
}