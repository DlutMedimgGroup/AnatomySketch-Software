#include "ASAddinsOutputArrayImage.h"
#include "ASAddinsBase.h"
#include "ASAddinsInputBase.h"
#include "ASAddinsInputArrayImage.h"
#include "ASArrayImageData.h"
#include "ASDataTree.h"
#include "ASTransAddDeleteData.h"
#include "ASPipelineBase.h"
#include "ASTransDataGeneral.h"
#include "ASTransGeneralInteractor.h"
#include "ASPipelineManager.h"
#include "ASPipelineArrayImage.h"
#include "ASVisualizationManager.h"
#include "ASOperation.h"
#include <vtkSmartPointer.h>
#include <vtkImageImport.h>
#include <vtkImageData.h>
#include <vtkMetaImageReader.h>
#include <QFileInfo>
#include <QTextCodec>

ASAddinsOutputArrayImage::ASAddinsOutputArrayImage(ASAddinsBase* parent)
	: ASAddinsOutputNode(parent)
{
	m_Type = AddinOutput_ArrayImage;
}

ASAddinsOutputArrayImage::~ASAddinsOutputArrayImage()
{
	delete m_pArray;
	delete m_Dim;
	delete m_Spacing;
	delete m_Origin;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsOutputArrayImage::GetVoidPoint(const QString s)
{
	if (s == "Array")
	{
		m_pArray = new float*;
		return m_pArray;
	}
	else if (s == "Dimensions")
	{
		m_Dim = new int[3];
		return m_Dim;
	}
	else if (s == "Spacing")
	{
		m_Spacing = new double[3];
		return m_Spacing;
	}
	else if (s == "Origin")
	{
		m_Origin = new double[3];
		return m_Origin;
	}
	else
	{
		return nullptr;
	}
}
void ASAddinsOutputArrayImage::GenerateFinished(const int InterfaceType)
{
	vtkSmartPointer<vtkImageData> ArrayImageData;
	if (InterfaceType == InterfaceType_DLL)
	{
		if (m_pArray == nullptr)
		{
			qDebug("error: Bad return image array.");
			return;
		}
		if (m_Dim == nullptr)
		{
			qDebug("error: Bad return image dimension.");
			return;
		}
		// Origin & Spacing
		int numOfinput = m_Parent->GetNumOfInput();
		ASAddinsInputArrayImage* inputImage = nullptr;
		for (int i = 0; i < numOfinput; i++)
		{
			if (m_Parent->GetInputAt(i)->GetType() == AddinInput_ArrayImage)
			{
				inputImage = static_cast<ASAddinsInputArrayImage*>(m_Parent->GetInputAt(i));
				break;
			}
		}
		double origin[3];
		if (m_Origin != nullptr)
		{
			origin[0] = m_Origin[0];
			origin[1] = m_Origin[1];
			origin[2] = m_Origin[2];
		}
		else
		{

			if (inputImage != nullptr)
			{
				inputImage->GetImageData()->getArrayImageData()->GetOrigin(origin);
			}
			else
			{
				origin[0] = 0;
				origin[1] = 0;
				origin[2] = 0;
			}
		}
		double spacing[3];
		if (m_Spacing != nullptr)
		{
			spacing[0] = m_Spacing[0];
			spacing[1] = m_Spacing[1];
			spacing[2] = m_Spacing[2];
		}
		else
		{
			if (inputImage != nullptr)
			{
				inputImage->GetImageData()->getArrayImageData()->GetSpacing(spacing);
			}
			else
			{
				spacing[0] = 0;
				spacing[1] = 0;
				spacing[2] = 0;
			}
		}
		// ImageImport
		vtkSmartPointer<vtkImageImport> Importer = vtkSmartPointer<vtkImageImport>::New();
		Importer->SetDataSpacing(spacing);
		Importer->SetDataOrigin(origin);
		Importer->SetWholeExtent(0, m_Dim[0] - 1, 0, m_Dim[1] - 1, 0, m_Dim[2] - 1);
		Importer->SetDataExtentToWholeExtent();
		Importer->SetDataScalarTypeToFloat();
		Importer->SetNumberOfScalarComponents(1);
		Importer->SetImportVoidPointer(*m_pArray);
		Importer->Update();
		ArrayImageData = Importer->GetOutput();
	} 
	else if (InterfaceType == InterfaceType_Python)
	{
		vtkSmartPointer<vtkMetaImageReader> reader = vtkSmartPointer<vtkMetaImageReader>::New();
		QString path = _pgmptr;
		QFileInfo fi = QFileInfo(path);
		QString file_path = fi.absolutePath();
		file_path = file_path + "/Plugins/_datacacke";
		file_path = file_path + "/" + this->GetName() + ".mhd";
		std::string strStd = file_path.toStdString();
		QTextCodec *code = QTextCodec::codecForName("gb18030");
		if (code) strStd = code->fromUnicode(file_path).data();
		reader->SetFileName(strStd.data());
		reader->Update();
		ArrayImageData = reader->GetOutput();
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
		if (!c->isTypeInherited(DataType_ArrayImageData))
		{
			continue;
		}
		if (c->getName() == s)
		{
			ASArrayImageData* a = static_cast<ASArrayImageData*>(c);
			a->setArrayImageData(ArrayImageData);

			QList<ASPipelineBase*>* listPipe = new QList<ASPipelineBase*>;
			ASPipelineManager::GetPipelineForDataID(listPipe, a->getDataNodeID());
			for (int ncrntPipe = 0; ncrntPipe < listPipe->size(); ncrntPipe++)
			{
				if (listPipe->at(ncrntPipe)->isTypeInherited(PipeLineType_ArrayImage))
				{
					ASPipelineArrayImage* crntPipe = static_cast<ASPipelineArrayImage*>(listPipe->at(ncrntPipe));
					crntPipe->SetInputData(a->getArrayImageData());
					crntPipe->UpdataInputData(ASVisualizationManager::GetDisplayFrame());
					ASVisualizationManager::Render();
				}
			}
			return;
		}
	}

	// New datanode
	ASArrayImageData* NewData = new ASArrayImageData();
	NewData->setArrayImageData(ArrayImageData);
	NewData->setLayer(ASArrayImageData::getNumOfArrayImageData());
	
	NewData->setName(s);
	NewData->setVoidNode(false);
	NewData->DataInitAfterLoad();
	ASDataTree::NewDataNode(NewData, host);
	bool bResetCarmera = (ASPipelineBase::GetNumOfObj2D() == 0);
	ASTransAddDeleteData::AddNewPipeline(NewData, PipeLineType_ArrayImage);	
	ASTransAddDeleteData::UpdateDispleyAfterAddDelete(bResetCarmera);
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_AbstractData);
}

// 运算后释放数据
void ASAddinsOutputArrayImage::FreeMemory()
{
	if (m_Dim)
	{
		delete m_Dim;
		m_Dim = nullptr;
	}
	if (m_Spacing)
	{
		delete m_Spacing;
		m_Spacing = nullptr;
	}
	if (m_Origin)
	{
		delete m_Origin;
		m_Origin = nullptr;
	}
	if (m_pArray)
	{
		if (*m_pArray)
		{
			delete *m_pArray;
			*m_pArray = nullptr;
		}
		delete m_pArray;
		m_pArray = nullptr;
	}
}

float* ASAddinsOutputArrayImage::GetArray()
{
	return *m_pArray;
}
int* ASAddinsOutputArrayImage::GetDim()
{
	return m_Dim;
}
double* ASAddinsOutputArrayImage::GetSpacing()
{
	return m_Spacing;
}
double* ASAddinsOutputArrayImage::GetOrigin()
{
	return m_Origin;
}