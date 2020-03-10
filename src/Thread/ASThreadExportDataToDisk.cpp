#include "ASThreadExportDataToDisk.h"
#include "ASAbstractData.h"
#include "ASArrayImageData.h"
#include "ASPolyImageData.h"
#include "ASDataStructureWalker.h"
#include "GlobelDeclaration.h"
#include "ASCustomizeDataWriter.h"
#include "ASSeedData.h"
#include "ASROIData.h"
#include "ASContourData.h"
#include "ASLandmarkData.h"
#include <QTextCodec>
#include <QFileInfo>
#include <QTextStream>
#include <QList>

ASThreadExportDataToDisk::ASThreadExportDataToDisk(QObject *parent)
	: ASThreadBase(parent)
{
	this->setTerminationEnabled();
}

ASThreadExportDataToDisk::~ASThreadExportDataToDisk()
{
	if (m_MetaImageWriter != nullptr)
	{
		m_MetaImageWriter = nullptr;
	}
	if (m_stlWriter != nullptr)
	{
		m_stlWriter = nullptr;
	}
	if (m_objWriter != nullptr)
	{
		m_objWriter = nullptr;
	}
	if (m_plyWriter != nullptr)
	{
		m_plyWriter = nullptr;
	}
}

void ASThreadExportDataToDisk::SetFileName(const QString c_FileName)
{
	this->m_FileName = c_FileName;
}
void ASThreadExportDataToDisk::SetDataNode(ASAbstractData* DataNode)
{
	this->m_DataNode = DataNode;
}

void ASThreadExportDataToDisk::run()
{
	switch (m_DataNode->getDataType())
	{
	case DataType_ArrayImageData:
	case DataType_DICOMData:
	{
		ASArrayImageData* crntDataNode = (ASArrayImageData*)m_DataNode;
		m_MetaImageWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
		m_MetaImageWriter->SetInputData(crntDataNode->getArrayImageData());
		m_MetaImageWriter->SetCompression(false); // ²»Ñ¹Ëõ
		std::string strStd = m_FileName.toStdString();
		QTextCodec *code = QTextCodec::codecForName("gb18030");
		if (code) strStd = code->fromUnicode(m_FileName).data();
		m_MetaImageWriter->SetFileName(strStd.data());
		m_MetaImageWriter->Write();
		break;
	}
	case DataType_PolyImageData:
	{
		ASPolyImageData* crntDataNode = (ASPolyImageData*)m_DataNode;
		if (QFileInfo(m_FileName).suffix() == "stl")
		{
			m_stlWriter = vtkSmartPointer<vtkSTLWriter>::New();
			m_stlWriter->SetInputData(crntDataNode->getPolyImageData());
			std::string strStd = m_FileName.toStdString();
			QTextCodec *code = QTextCodec::codecForName("gb18030");
			if (code) strStd = code->fromUnicode(m_FileName).data();
			m_stlWriter->SetFileName(strStd.data());
			m_stlWriter->Update();
		}
		else if (QFileInfo(m_FileName).suffix() == "obj")
		{
			m_objWriter = vtkSmartPointer<vtkOBJWriter>::New();
			m_objWriter->SetInputData(crntDataNode->getPolyImageData());
			std::string strStd = m_FileName.toStdString();
			QTextCodec *code = QTextCodec::codecForName("gb18030");
			if (code) strStd = code->fromUnicode(m_FileName).data();
			m_objWriter->SetFileName(strStd.data());
			m_objWriter->Update();
		}
		else if (QFileInfo(m_FileName).suffix() == "ply")
		{
			m_plyWriter = vtkSmartPointer<vtkPLYWriter>::New();
			m_plyWriter->SetInputData(crntDataNode->getPolyImageData());
			std::string strStd = m_FileName.toStdString();
			QTextCodec *code = QTextCodec::codecForName("gb18030");
			if (code) strStd = code->fromUnicode(m_FileName).data();
			m_plyWriter->SetFileName(strStd.data());
			m_plyWriter->Update();
		}
		break;
	}
	case DataType_SeedData:
		this->WriteSeedData();
		break;
	case DataType_ContourData:
		this->WriteContourData();
		break;
	case DataType_LandmarkData:
		this->WriteLandmarkData();
		break;
	case DataType_ROI:
		this->WriteROIData();
		break;
	default:
		break;
	}
	emit signalSaveDataFromDiskFinished();
}

void ASThreadExportDataToDisk::WriteSeedData()
{
	ASSeedData* DataNode = static_cast<ASSeedData*>(m_DataNode);
	ASCustomizeDataWriter writer;
	writer.WriteSeedData(DataNode, m_FileName);
}
void ASThreadExportDataToDisk::WriteContourData()
{
	ASContourData* DataNode = static_cast<ASContourData*>(m_DataNode);
	ASCustomizeDataWriter writer;
	writer.WriteContourData(DataNode, m_FileName);
}
void ASThreadExportDataToDisk::WriteLandmarkData()
{
	ASLandmarkData* DataNode = static_cast<ASLandmarkData*>(m_DataNode);
	ASCustomizeDataWriter writer;
	writer.WriteLandmarkData(DataNode, m_FileName);
}
void ASThreadExportDataToDisk::WriteROIData()
{
	ASROIData* DataNode = static_cast<ASROIData*>(m_DataNode);
	ASCustomizeDataWriter writer;
	writer.WriteROIData(DataNode, m_FileName);
}
