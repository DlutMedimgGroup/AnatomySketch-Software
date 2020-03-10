#include "ASPipelineMask.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager2D.h"
#include "ASVisualizationManager3D.h"
#include "vtkImageProperty.h"

ASPipelineMask* ASPipelineMask::ms_PipelineMask = nullptr;

template<typename TIn>
void TransScaleToColor(vtkImageData* pInput, vtkImageData* pOutput)
{
	unsigned char *ptrOutput;
	TIn* ptrInput;
	int nExtent[6];
	pInput->GetExtent(nExtent);
	for (int k = nExtent[4]; k <= nExtent[5]; k++)
	{
		for (int j = nExtent[2]; j <= nExtent[3]; j++)
		{
			ptrOutput = static_cast<unsigned char *>(pOutput->GetScalarPointer(nExtent[0], j, k));
			ptrInput = static_cast<TIn*>(pInput->GetScalarPointer(nExtent[0], j, k));
			for (int i = nExtent[0]; i <= nExtent[1]; i++)
			{
				if (*ptrInput > 0)
				{
					*ptrOutput++ = 0; // R
					*ptrOutput++ = 0; // G
					*ptrOutput++ = 0; // B
					*ptrOutput++ = 0; // A
				}
				else
				{
					*ptrOutput++ = 163; // R
					*ptrOutput++ = 214; // G
					*ptrOutput++ = 112; // B
					*ptrOutput++ = 127; // A
				}
				ptrInput++;
			}
		}
	}
}

ASPipelineMask::ASPipelineMask(const int c_ID) : ASPipelineBase(c_ID)
{
	m_PipelineType = PipeLineType_Mask;

	ms_PipelineMask = this;

	// 管线初始化
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		//ImageReslice
		m_ImageReslice[ViewLabel] = vtkSmartPointer<vtkImageReslice>::New();
		m_ImageReslice[ViewLabel]->SetOutputDimensionality(2);
		m_ImageReslice[ViewLabel]->SetInterpolationModeToLinear();

		m_ImageMapped[ViewLabel] = vtkSmartPointer<vtkImageData>::New();

		//vtkImageActor
		m_ImageActor[ViewLabel] = vtkSmartPointer<vtkImageActor>::New();
		m_ImageActor[ViewLabel]->SetInputData(m_ImageMapped[ViewLabel]);
		m_ImageActor[ViewLabel]->GetProperty()->SetLayerNumber(Layer_MaskModel);
	}
	//ImageStack
	ASVisualizationManager2D::GetImageStack2D(m_pImageStack2D);
	ASVisualizationManager3D::GetImageStack3D(m_pImageStack3D);
}

ASPipelineMask::~ASPipelineMask()
{

}

// 管线类型判断
bool ASPipelineMask::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_Mask || ASPipelineBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPipelineMask::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_Mask)
	{
		return true;
	}
	return false;
}

// 设置显示属性
void ASPipelineMask::SetShown(const bool c_Shown)
{
	if (m_isShown == c_Shown)
	{
		return;
	}
	m_isShown = c_Shown;
	if (m_isShown)
	{
		this->Update2DTransformMatrix();
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pImageStack2D[ViewLabel]->AddImage(m_ImageActor[ViewLabel]);
			m_pImageStack3D[ViewLabel]->AddImage(m_ImageActor[ViewLabel]);
		}
	}
	else
	{
		for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
		{
			m_pImageStack2D[ViewLabel]->RemoveImage(m_ImageActor[ViewLabel]);
			m_pImageStack3D[ViewLabel]->RemoveImage(m_ImageActor[ViewLabel]);
		}
	}

}
// 设置遮罩数据
void ASPipelineMask::SetMaskImageData(vtkImageData* InputData)
{
	this->m_InputData = InputData;
	this->m_ImageReslice[0]->SetInputData(InputData);
	this->m_ImageReslice[1]->SetInputData(InputData);
	this->m_ImageReslice[2]->SetInputData(InputData);
}
// 更新
void ASPipelineMask::Update()
{
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		m_ImageReslice[ViewLabel]->Update();
		UpdateImageMapped();
	}
}

// 虚函数，切图焦点改变
void ASPipelineMask::Update2DTransformMatrix()
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
		m_ImageReslice[ViewLabel]->SetResliceAxes(pMatrix[ViewLabel]);
		m_ImageReslice[ViewLabel]->Update();
		UpdateImageMapped();
	}

}

// 更新m_ImageMapped
void ASPipelineMask::UpdateImageMapped()
{
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		//m_ImageMapped[ViewLabel] = vtkSmartPointer<vtkImageData>::New();
		//m_ImageActor[ViewLabel]->SetInputData(m_ImageMapped[ViewLabel]);
		m_ImageMapped[ViewLabel]->ShallowCopy(m_ImageReslice[ViewLabel]->GetOutput());
		m_ImageMapped[ViewLabel]->AllocateScalars(VTK_UNSIGNED_CHAR, 4);
		switch (m_ImageReslice[ViewLabel]->GetOutput()->GetScalarType())
		{
			vtkTemplateMacro(TransScaleToColor<VTK_TT>(m_ImageReslice[ViewLabel]->GetOutput(), m_ImageMapped[ViewLabel]));
		default:
			break;
		}
		m_ImageMapped[ViewLabel]->Modified();
		m_ImageActor[ViewLabel]->Update();
	}

}