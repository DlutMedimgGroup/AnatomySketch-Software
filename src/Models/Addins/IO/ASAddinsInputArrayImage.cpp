#include "ASAddinsInputArrayImage.h"
#include "ASArrayImageData.h"
#include "ASAbstractData.h"
#include "ASDataStructureWalker.h"
#include "ASVisualizationManager.h"
#include "ASArrayImageData.h"
#include <vtkImageIterator.h>
#include <vtkImageExtractComponents.h>
#include <vtkMetaImageWriter.h>
#include <QFileInfo>
#include <QDir>
#include <QTextCodec>
#include "stdlib.h"

template<typename TIn>
void ValueFilling(vtkImageData* WorkingData, float* target)
{
	vtkImageIterator<TIn>it(WorkingData, WorkingData->GetExtent());
	unsigned long int index = 0;
	while (!it.IsAtEnd())
	{
		TIn* inSI = it.BeginSpan();
		TIn* inSIEnd = it.EndSpan();
		while (inSI != inSIEnd)
		{
			target[index] = (float)*inSI;
			++index;
			++inSI;
		}
		it.NextSpan();
	}
}

ASAddinsInputArrayImage::ASAddinsInputArrayImage(ASAddinsBase* parent)
	: ASAddinsInputNode(parent)
{
	m_Type = AddinInput_ArrayImage;
	m_Combobox = new QComboBox();
}

ASAddinsInputArrayImage::~ASAddinsInputArrayImage()
{
	m_Combobox->deleteLater();
}

// DataNode
void ASAddinsInputArrayImage::SetImageData(ASArrayImageData* data)
{
	this->m_ImageData = data;
}
// Combobox
QComboBox* ASAddinsInputArrayImage::GetCombobox()
{
	return this->m_Combobox;
}
// ImageData
ASArrayImageData* ASAddinsInputArrayImage::GetImageData()
{
	if (m_ImageData)
	{
		return m_ImageData;
	}
	// 1 对应数据
	ASArrayImageData* ArrayImage = nullptr;
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ArrayImageData);
		item != nullptr;
		item = ASDataStructureWalker::next_inherit(DataType_ArrayImageData))
	{
		if (item->getName() == m_Combobox->currentText())
		{
			ArrayImage = static_cast<ASArrayImageData*>(item);
		}
	}
	if (ArrayImage == nullptr)
	{
		return nullptr;
	}
	return ArrayImage;
}
// 得到空指针，用于argc，s为描述
void* ASAddinsInputArrayImage::GetVoidPoint(const QString s)
{
	ASArrayImageData* datanode = this->GetImageData();
	vtkSmartPointer<vtkImageData> data = datanode->getArrayImageData();
	if (s == "Array")
	{
		vtkSmartPointer<vtkImageData> crntData = nullptr;
		int dim[3];
		unsigned long int sz;
		if (datanode->GetColorMode() == ColorMode_RGB)
		{
			crntData = data;
			sz = dim[0] * dim[1] * dim[2] * 3;
			crntData->GetDimensions(dim);
		}
		else
		{
			int Components = data->GetNumberOfScalarComponents();
			if (Components > 1)
			{
				vtkSmartPointer<vtkImageExtractComponents> ext =
					vtkSmartPointer<vtkImageExtractComponents>::New();
				ext->SetInputData(data);
				int F = ASVisualizationManager::GetDisplayFrame();
				if (F >= Components)
				{
					F = Components - 1;
				}
				ext->SetComponents(F);
				ext->Update();
				crntData = ext->GetOutput();
			}
			else
			{
				crntData = data;
			}
			sz = dim[0] * dim[1] * dim[2];
			crntData->GetDimensions(dim);
		}
		m_Data = new float[sz];
		switch (crntData->GetScalarType())
		{
			vtkTemplateMacro(ValueFilling<VTK_TT>(crntData, m_Data));
		default:
			break;
		}
		return m_Data;
	}
	else if (s == "Dimensions")
	{
		data->GetDimensions(m_Dim);
		return m_Dim;
	}
	else if (s == "Origin")
	{
		data->GetOrigin(m_Origin);
		return m_Origin;
	}
	else if (s == "Spacing")
	{
		data->GetSpacing(m_Spacing);
		return m_Spacing;
	}
	else if (s == "WindowLevel")
	{
		m_WinLevel[0] = datanode->getWindowLevel()[0];
		m_WinLevel[1] = datanode->getWindowLevel()[1];
		return m_WinLevel;
	}
	else
	{
		return nullptr;
	}
}
// 保存输入到Python程序中的临时文件
int ASAddinsInputArrayImage::PythonInputGenerate()
{
	ASArrayImageData* datanode = this->GetImageData();
	vtkSmartPointer<vtkImageData> data = datanode->getArrayImageData();
	vtkSmartPointer<vtkImageData> crntData = nullptr;
	if (datanode->GetColorMode() == ColorMode_RGB)
	{
		crntData = data;
	}
	else
	{
		int Components = data->GetNumberOfScalarComponents();
		if (Components > 1)
		{
			vtkSmartPointer<vtkImageExtractComponents> ext =
				vtkSmartPointer<vtkImageExtractComponents>::New();
			ext->SetInputData(data);
			int F = ASVisualizationManager::GetDisplayFrame();
			if (F >= Components)
			{
				F = Components - 1;
			}
			ext->SetComponents(F);
			ext->Update();
			crntData = ext->GetOutput();
		}
		else
		{
			crntData = data;
		}
	}
	QString path = _pgmptr;
	QFileInfo fi = QFileInfo(path);
	QString file_path = fi.absolutePath();
	file_path = file_path + "/Plugins/_datacacke";
	QDir dir(file_path);
	if (dir.exists() == false)
	{
		dir.mkpath(file_path);
	}
	file_path = file_path + "/" + this->GetName() + ".mhd";
	vtkSmartPointer<vtkMetaImageWriter> NIFTIImageWriter = vtkSmartPointer<vtkMetaImageWriter>::New();
	NIFTIImageWriter->SetInputData(crntData);
	std::string strStd = file_path.toStdString();
	QTextCodec *code = QTextCodec::codecForName("gb18030");
	if (code) strStd = code->fromUnicode(file_path).data();
	NIFTIImageWriter->SetFileName(strStd.data());
	NIFTIImageWriter->Write();
	return 1;
}

// 运算后释放数据
void ASAddinsInputArrayImage::FreeMemory()
{
	if (m_Data)
	{
		delete m_Data;
		m_Data = nullptr;
	}
}

void ASAddinsInputArrayImage::RefreshCombo(int datatype)
{
	if (datatype != DataType_ArrayImageData)
	{
		return;
	}
	QString oriStr = m_Combobox->currentText();
	m_Combobox->clear();
	m_Combobox->addItem(QObject::tr(u8"请选择体数据"));
	m_Combobox->addItem(QObject::tr(u8"新建体数据"));
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ArrayImageData);
		item != nullptr;
		item = ASDataStructureWalker::next_inherit(DataType_ArrayImageData))
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