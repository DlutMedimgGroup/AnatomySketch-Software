#include "ASPipelineArrayImage.h"
#include "ASVisualizationManager2D.h"
#include "ASVisualizationManager3D.h"
#include "ASColorMap.h"
#include "ASArrayImageData.h"
#include <vtkImageProperty.h>

ASPipelineArrayImage::ASPipelineArrayImage(const int c_ID) : ASPipelineBase(c_ID)
{
	m_PipelineType = PipeLineType_ArrayImage;
	ms_NumOfObj2D++;

	m_pImageExtractComponents = vtkSmartPointer<vtkImageExtractComponents>::New();
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		//ImageReslice
		m_pImageReslice[ViewLabel] = vtkSmartPointer<vtkImageReslice>::New();
		m_pImageReslice[ViewLabel]->SetInputData(m_pImageExtractComponents->GetOutput()); // 使用SetInputConnection会出现对于某些图像矢状面或冠状面出现消失的问题。
		m_pImageReslice[ViewLabel]->SetOutputDimensionality(2);
		//m_pImageReslice[ViewLabel]->SetInterpolationModeToLinear();
		m_pImageReslice[ViewLabel]->SetInterpolationModeToNearestNeighbor();
		//ImageMapToColors
		m_pImageMapToColors[ViewLabel] = vtkSmartPointer<vtkImageMapToColors>::New();
		m_pImageMapToColors[ViewLabel]->SetOutputFormatToRGBA();
		m_pImageMapToColors[ViewLabel]->SetInputConnection(m_pImageReslice[ViewLabel]->GetOutputPort());
		//ImageActor
		m_pImageActor[ViewLabel] = vtkSmartPointer<vtkImageActor>::New();
		m_pImageActor[ViewLabel]->SetInputData(m_pImageMapToColors[ViewLabel]->GetOutput());
	}
	//ImageStack
	ASVisualizationManager2D::GetImageStack2D(m_pImageStack2D);
	ASVisualizationManager3D::GetImageStack3D(m_pImageStack3D);
}

ASPipelineArrayImage::~ASPipelineArrayImage()
{
	ms_NumOfObj2D--;
	if (m_isShown)
	{
		ms_NumOfShowingObj2D--;
		if (ms_NumOfShowingObj2D < 0)
		{
			ms_NumOfShowingObj2D = 0;
		}
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pImageStack2D[ViewLabel]->RemoveImage(m_pImageActor[ViewLabel]);
			m_pImageStack3D[ViewLabel]->RemoveImage(m_pImageActor[ViewLabel]);
		}
	}

	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pImageActor[ViewLabel] = nullptr;
		m_pImageMapToColors[ViewLabel] = nullptr;
		m_pImageReslice[ViewLabel] = nullptr;
	}
	m_pImageExtractComponents = nullptr;
}

// 管线类型判断
bool ASPipelineArrayImage::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_ArrayImage || ASPipelineBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPipelineArrayImage::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_ArrayImage)
	{
		return true;
	}
	return false;
}

// 设置输入数据
void ASPipelineArrayImage::SetInputData(vtkImageData* InputData)
{
	m_InputData = InputData;
	m_pImageExtractComponents->SetInputData(m_InputData);
}
// 输入数据变化后刷新管线
void ASPipelineArrayImage::UpdataInputData(const int c_Frame)
{
	if (c_Frame >= m_InputData->GetNumberOfScalarComponents())
	{
		if (m_Frame == m_InputData->GetNumberOfScalarComponents() - 1)
		{
			return;
		}
		m_Frame = m_InputData->GetNumberOfScalarComponents() - 1;
	}
	else
	{
		m_Frame = c_Frame;
	}
	m_pImageExtractComponents->SetComponents(m_Frame);
	m_pImageExtractComponents->Update();
	this->UpdateImageMapToColors();
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pImageReslice[ViewLabel]->Update();
		m_pImageMapToColors[ViewLabel]->Update();
	}
}
void ASPipelineArrayImage::SetFrame(const int c_Frame)
{
	
	if (c_Frame >= m_InputData->GetNumberOfScalarComponents())
	{
		if (m_Frame == m_InputData->GetNumberOfScalarComponents() - 1)
		{
			return;
		}
		m_Frame = m_InputData->GetNumberOfScalarComponents() - 1;
	}
	else
	{
		if (m_Frame == c_Frame)
		{
			return;
		}
		m_Frame = c_Frame;
	}
	
	m_pImageExtractComponents->SetComponents(m_Frame);
	m_pImageExtractComponents->Update();
	this->UpdateImageMapToColors();
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pImageReslice[ViewLabel]->Update();
		m_pImageMapToColors[ViewLabel]->Update();
	}
}
void ASPipelineArrayImage::SetColorMapping(const int c_ColorMap, const double c_ColorWindow, const double c_ColorLevel)
{
	m_ColorMap = c_ColorMap;
	m_ColorWindow = c_ColorWindow;
	m_ColorLevel = c_ColorLevel;
	this->UpdateImageMapToColors();
	m_pImageMapToColors[0]->Update();
	m_pImageMapToColors[1]->Update();
	m_pImageMapToColors[2]->Update();
}
void ASPipelineArrayImage::SetLayer(const int c_Layer)
{
	m_Layer = c_Layer;
	m_pImageActor[0]->GetProperty()->SetLayerNumber(m_Layer);
	m_pImageActor[1]->GetProperty()->SetLayerNumber(m_Layer);
	m_pImageActor[2]->GetProperty()->SetLayerNumber(m_Layer);
}
void ASPipelineArrayImage::SetOpacity(double c_Opacity)
{
	m_Opacity = c_Opacity;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pImageActor[ViewLabel]->GetProperty()->SetOpacity(m_Opacity);
	}
}
// 新建管线初始化
void ASPipelineArrayImage::ArrayPipeInit(ASArrayImageData* arrayImage)
{
	m_Frame = 0;
	switch (arrayImage->GetColorMode())
	{
	case ColorMode_Grayscale:
		m_pImageExtractComponents->SetComponents(0);
		m_pImageExtractComponents->Update();
		m_ColorMap = arrayImage->getColorMap();
		m_ColorWindow = arrayImage->getWindowLevel()[0];
		m_ColorLevel = arrayImage->getWindowLevel()[1];
		break;
	case ColorMode_RGB:
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pImageReslice[ViewLabel]->SetInputData(m_InputData);
			m_pImageActor[ViewLabel]->SetInputData(m_pImageReslice[ViewLabel]->GetOutput());
		}
		break;
	default:
		break;
	}
	m_Layer = arrayImage->getLayer();

	this->UpdateImageMapToColors();
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pImageMapToColors[ViewLabel]->Update();
		m_pImageActor[ViewLabel]->GetProperty()->SetLayerNumber(m_Layer);
	}
	m_isShown = true;
	if (m_isShown)
	{
		ms_NumOfShowingObj2D++;
		this->Update2DTransformMatrix();
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pImageStack2D[ViewLabel]->AddImage(m_pImageActor[ViewLabel]);
			m_pImageStack3D[ViewLabel]->AddImage(m_pImageActor[ViewLabel]);
		}
	}
}
// 虚函数
// 调用此函数来释放管线
void ASPipelineArrayImage::DeleteNode()
{
	delete this;
}
// 切图焦点改变
void ASPipelineArrayImage::Update2DTransformMatrix()
{
	if (m_isShown == false)
	{
		return;
	}
	vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
	ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_pImageReslice[ViewLabel]->SetResliceAxes(pMatrix[ViewLabel]);
		m_pImageReslice[ViewLabel]->Update();
		this->UpdateImageMapToColors();
		m_pImageMapToColors[ViewLabel]->Update();
	}
}
// 设置显示属性
void ASPipelineArrayImage::SetShown(const bool c_Shown)
{
	if (m_isShown == c_Shown)
	{
		return;
	}
	m_isShown = c_Shown;
	if (m_isShown)
	{
		ms_NumOfShowingObj2D++;
		this->Update2DTransformMatrix();
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pImageStack2D[ViewLabel]->AddImage(m_pImageActor[ViewLabel]);
			m_pImageStack3D[ViewLabel]->AddImage(m_pImageActor[ViewLabel]);
		}
	}
	else
	{
		ms_NumOfShowingObj2D--;
		if (ms_NumOfShowingObj2D < 0)
		{
			ms_NumOfShowingObj2D = 0;
		}
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pImageStack2D[ViewLabel]->RemoveImage(m_pImageActor[ViewLabel]);
			m_pImageStack3D[ViewLabel]->RemoveImage(m_pImageActor[ViewLabel]);
		}
	}
}
// 重置显示
void ASPipelineArrayImage::ReinitDisplay()
{
	// 重置窗位窗宽

}

// 根据颜色表，窗位窗宽更新ImageMapToColors
void ASPipelineArrayImage::UpdateImageMapToColors()
{
	switch (m_ColorMap)
	{
	case ColorMap_Gray:
		ASColorMap::LookupTable_Gray->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		ASColorMap::LookupTable_Gray->Build();
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Gray);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Gray);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Gray);
		break;
	case ColorMap_Multilabel:
		ASColorMap::LookupTable_MultiLabel->SetRange(0, 255);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_MultiLabel);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_MultiLabel);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_MultiLabel);
		break;
	case ColorMap_Mask:
		ASColorMap::LookupTable_Mask->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Mask);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Mask);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Mask);
		break;
	case ColorMap_Cold:
		ASColorMap::LookupTable_Cold->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Cold);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Cold);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Cold);
		break;
	case ColorMap_Hot:
		ASColorMap::LookupTable_Hot->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Hot);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Hot);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Hot);
		break;
	case ColorMap_Cool:
		ASColorMap::LookupTable_Cool->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Cool);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Cool);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Cool);
		break;
	case ColorMap_Bronson:
		ASColorMap::LookupTable_Bronson->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Bronson);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Bronson);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Bronson);
		break;
	case ColorMap_Split_BBR:
		ASColorMap::LookupTable_Split_BBR->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Split_BBR);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Split_BBR);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Split_BBR);
		break;
	case ColorMap_UCLA:
		ASColorMap::LookupTable_UCLA->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_UCLA);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_UCLA);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_UCLA);
		break;
	case ColorMap_Red:
		ASColorMap::LookupTable_Red->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Red);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Red);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Red);
		break;
	case ColorMap_Green:
		ASColorMap::LookupTable_Green->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Green);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Green);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Green);
		break;
	case ColorMap_Blue:
		ASColorMap::LookupTable_Blue->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_Blue);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_Blue);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_Blue);
		break;
	case ColorMap_HeartVR:
		ASColorMap::LookupTable_HeartVR->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_HeartVR);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_HeartVR);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_HeartVR);
		break;
	case ColorMap_HeadVR:
		ASColorMap::LookupTable_HeadVR->SetRange(m_ColorLevel - m_ColorWindow, m_ColorLevel + m_ColorWindow);
		m_pImageMapToColors[0]->SetLookupTable(ASColorMap::LookupTable_HeadVR);
		m_pImageMapToColors[1]->SetLookupTable(ASColorMap::LookupTable_HeadVR);
		m_pImageMapToColors[2]->SetLookupTable(ASColorMap::LookupTable_HeadVR);
		break;
	default:
		break;
	}
}