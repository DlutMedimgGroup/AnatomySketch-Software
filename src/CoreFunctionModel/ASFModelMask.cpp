#include "ASFModelMask.h"
#include "ASArrayImageData.h"
#include "ASTransSidebarGeneral.h"
#include "ASPipelineManager.h"
#include "ASPipelineBase.h"
#include "ASPipelineMask.h"
#include <vtkImageIterator.h>

bool ASFModelMask::ms_isEnable = false;
ASFModelMask* ASFModelMask::ms_FModelMask = nullptr;

template<typename TIn>
void ValueRangeCheck(int* nExtent, vtkImageData* MaskImageData, vtkImageData* WorkingData, double* ScaleRange)
{
	unsigned char *ptrMask;
	TIn* ptrImage;
	for (int k = nExtent[4]; k <= nExtent[5]; k++)
	{
		for (int j = nExtent[2]; j <= nExtent[3]; j++)
		{
			ptrMask = static_cast<unsigned char *>(MaskImageData->GetScalarPointer(nExtent[0], j, k));
			ptrImage = static_cast<TIn*>(WorkingData->GetScalarPointer(nExtent[0], j, k));
			for (int i = nExtent[0]; i <= nExtent[1]; i++)
			{
				if (*ptrImage >= ScaleRange[0] && *ptrImage <= ScaleRange[1])
				{
					*ptrMask++ = 1;
				}
				else
				{
					*ptrMask++ = 0;
				}
				ptrImage++;
			}
		}
	}
}

template<typename TIn>
void PosCheckWithValue(int* nExtent, vtkImageData* MaskImageData, vtkImageData* WorkingData, double* ScaleRange)
{
	unsigned char *ptrMask;
	TIn* ptrImage;
	for (int k = nExtent[4]; k <= nExtent[5]; k++)
	{
		for (int j = nExtent[2]; j <= nExtent[3]; j++)
		{
			ptrMask = static_cast<unsigned char *>(MaskImageData->GetScalarPointer(nExtent[0], j, k));
			ptrImage = static_cast<TIn*>(WorkingData->GetScalarPointer(nExtent[0], j, k));
			for (int i = nExtent[0]; i <= nExtent[1]; i++)
			{				
				if (*ptrImage >= ScaleRange[0] && *ptrImage <= ScaleRange[1])
				{
					*ptrMask = 1;
				}
				ptrImage++;
				ptrMask++;
			}
		}
	}
}
template<typename TIn>
void PosCheckWithoutValue(int* nExtent, vtkImageData* MaskImageData)
{
	unsigned char *ptrMask;
	for (int k = nExtent[4]; k <= nExtent[5]; k++)
	{
		for (int j = nExtent[2]; j <= nExtent[3]; j++)
		{
			ptrMask = static_cast<unsigned char *>(MaskImageData->GetScalarPointer(nExtent[0], j, k));
			for (int i = nExtent[0]; i <= nExtent[1]; i++)
			{
				*ptrMask++ = 0;
			}			
		}
	}
}

ASFModelMask::ASFModelMask(QObject *parent)
	: ASFunctionModelBase(parent)
{
	ms_FModelMask = this;
}

ASFModelMask::~ASFModelMask()
{

}

// 当前遮罩模块使能状态
bool ASFModelMask::IsMaskEnable()
{
	return ms_isEnable;
}
void ASFModelMask::SetMaskEnable(const bool c_enable)
{
	ms_isEnable = c_enable;
	if (ms_isEnable)
	{
		m_MaskPipeLine->SetShown(true);
		m_MaskPipeLine->Update();
	}
	else
	{
		m_MaskPipeLine->SetShown(false);
	}
}
// 参照图像ID
void ASFModelMask::SetMaskImageID(const int c_ID)
{
	m_WorkingDataID = c_ID;
	if (m_WorkingDataID == -1)
	{
		for (int i = 0; i < 6; i++)
		{
			m_MaskExtentOri[i] = 0;
			m_MaskExtent[i] = 0;
		}
		m_ScaleRange[0] = 0;
		m_ScaleRange[1] = 0;

		m_pMaskImageData = nullptr;
	}
	else
	{
		// 初始化范围
		ASArrayImageData* crntDataNode = (ASArrayImageData*)ASAbstractData::getDataNodeFromID(m_WorkingDataID);
		crntDataNode->getArrayImageData()->GetExtent(m_MaskExtent);
		crntDataNode->getArrayImageData()->GetScalarRange(m_ScaleRange);
		double adOrigin[3];
		crntDataNode->getArrayImageData()->GetOrigin(adOrigin);
		double adSpacing[3];
		crntDataNode->getArrayImageData()->GetSpacing(adSpacing);
		for (int i = 0; i < 3; i++)
		{
			m_MaskExtentOri[2 * i] = adOrigin[i];
			m_MaskExtentOri[2 * i + 1] = adOrigin[i] + adSpacing[i] * (m_MaskExtent[2 * i + 1] + 1);
		}

		if (m_MaskPipeLine == nullptr)
		{
			// 新建遮罩管线
			m_MaskPipeLine = (ASPipelineMask*)ASPipelineManager::AddPipeline(PipeLineType_Mask);
		}
		//更新存储遮罩数据
		if (m_pMaskImageData == nullptr)
		{
			m_pMaskImageData = vtkSmartPointer<vtkImageData>::New();
		}
		m_pMaskImageData->ShallowCopy(crntDataNode->getArrayImageData());
		m_pMaskImageData->AllocateScalars(VTK_UNSIGNED_CHAR, 1);
		m_MaskPipeLine->SetMaskImageData(m_pMaskImageData);
		// 使用迭代器将元素舒初始化为1
		vtkImageIterator<unsigned char> it(m_pMaskImageData, m_pMaskImageData->GetExtent());
		while (!it.IsAtEnd())
		{
			unsigned char *inSI = it.BeginSpan();
			unsigned char *inSIEnd = it.EndSpan();
			while (inSI != inSIEnd)
			{
				*inSI = 1;
				++inSI;
			}
			it.NextSpan();
		}
	}
}
// 遮罩数据
vtkImageData* ASFModelMask::GetMaskImageData()
{
	return ms_FModelMask->m_pMaskImageData;
}
// 设置遮罩参数
void ASFModelMask::SetMaskParameter(double* Extent, double* ScaleRange)
{
	m_MaskExtentOri[0] = Extent[0];
	m_MaskExtentOri[1] = Extent[1];
	m_MaskExtentOri[2] = Extent[2];
	m_MaskExtentOri[3] = Extent[3];
	m_MaskExtentOri[4] = Extent[4];
	m_MaskExtentOri[5] = Extent[5];
	vtkImageData* pWorkingImageData = ((ASArrayImageData*)ASAbstractData::getDataNodeFromID(m_WorkingDataID))->getArrayImageData();
	// 1 计算新的范围
	double adOrigin[3];
	m_pMaskImageData->GetOrigin(adOrigin);
	double adSpacing[3];
	m_pMaskImageData->GetSpacing(adSpacing);
	int TargetExtent[6];
	for (int i = 0; i < 3; i++)
	{
		TargetExtent[i * 2] = (Extent[i * 2] - adOrigin[i]) / adSpacing[i];
		TargetExtent[i * 2 + 1] = (Extent[i * 2 + 1] - adOrigin[i]) / adSpacing[i] - 1;
	}	
	// 2 更新数据
	// 2.1 ScalrValue	
	if (ScaleRange[0] != m_ScaleRange[0] || ScaleRange[1] != m_ScaleRange[1])
	{
		if (m_maskMoudle == 0)
		{
			switch (pWorkingImageData->GetScalarType())
			{
				vtkTemplateMacro(ValueRangeCheck<VTK_TT>(m_MaskExtent, m_pMaskImageData, pWorkingImageData, ScaleRange));
			default:
				break;
			}
			m_ScaleRange[0] = ScaleRange[0];
			m_ScaleRange[1] = ScaleRange[1];
		} 
		else
		{
			vtkImageData* pMData = ((ASArrayImageData*)ASAbstractData::getDataNodeFromID(m_MDataID))->getArrayImageData();
			switch (pMData->GetScalarType())
			{
				vtkTemplateMacro(ValueRangeCheck<VTK_TT>(m_MaskExtent, m_pMaskImageData, pMData, ScaleRange));
			default:
				break;
			}
			m_ScaleRangeMData[0] = ScaleRange[0];
			m_ScaleRangeMData[1] = ScaleRange[1];
		}
	}
	
	int crntExtent[6];
	// 2.3 x min
	if (TargetExtent[0] < m_MaskExtent[0])
	{
		int actualboundary = m_MaskExtent[0] - 1;
		if (m_MaskExtent[0] > m_MaskExtent[1] +1)
		{
			actualboundary = m_MaskExtent[1];
		}		
		crntExtent[0] = TargetExtent[0];
		crntExtent[1] = actualboundary;
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithValue<VTK_TT>(crntExtent, m_pMaskImageData, pWorkingImageData, ScaleRange));
		default:
			break;
		}
	}
	else if (TargetExtent[0] > m_MaskExtent[0])
	{
		int actualboundary = TargetExtent[0] - 1;
		if (TargetExtent[0] > m_MaskExtent[1] + 1)
		{
			actualboundary = m_MaskExtent[1];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = actualboundary;
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithoutValue<VTK_TT>(crntExtent, m_pMaskImageData));
		default:
			break;
		}
	}
	m_MaskExtent[0] = TargetExtent[0];
	// 2.4 x max
	if (TargetExtent[1] < m_MaskExtent[1])
	{
		int actualboundary = TargetExtent[1] + 1;
		if (TargetExtent[1] < m_MaskExtent[0] - 1)
		{
			actualboundary = m_MaskExtent[0];
		}
		crntExtent[0] = actualboundary;
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithoutValue<VTK_TT>(crntExtent, m_pMaskImageData));
		default:
			break;
		}
	}
	else if (TargetExtent[1] > m_MaskExtent[1])
	{
		int actualboundary = m_MaskExtent[1] + 1;
		if (m_MaskExtent[1] < m_MaskExtent[0] - 1)
		{
			actualboundary = m_MaskExtent[0];
		}
		crntExtent[0] = actualboundary;
		crntExtent[1] = TargetExtent[1];
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithValue<VTK_TT>(crntExtent, m_pMaskImageData, pWorkingImageData, ScaleRange));
		default:
			break;
		}
	}
	m_MaskExtent[1] = TargetExtent[1];
	// 2.5 y min
	if (TargetExtent[2] < m_MaskExtent[2])
	{
		int actualboundary = m_MaskExtent[2] - 1;
		if (m_MaskExtent[2] > m_MaskExtent[3] + 1)
		{
			actualboundary = m_MaskExtent[3];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = TargetExtent[2];
		crntExtent[3] = actualboundary;
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithValue<VTK_TT>(crntExtent, m_pMaskImageData, pWorkingImageData, ScaleRange));
		default:
			break;
		}
	}
	else if (TargetExtent[2] > m_MaskExtent[2])
	{
		int actualboundary = TargetExtent[2] - 1;
		if (TargetExtent[2] > m_MaskExtent[3] + 1)
		{
			actualboundary = m_MaskExtent[3];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = actualboundary;
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithoutValue<VTK_TT>(crntExtent, m_pMaskImageData));
		default:
			break;
		}
	}
	m_MaskExtent[2] = TargetExtent[2];
	// 2.6 y max
	if (TargetExtent[3] < m_MaskExtent[3])
	{
		int actualboundary = TargetExtent[3] + 1;
		if (TargetExtent[3] < m_MaskExtent[2] - 1)
		{
			actualboundary = m_MaskExtent[2];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = actualboundary;
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithoutValue<VTK_TT>(crntExtent, m_pMaskImageData));
		default:
			break;
		}
	}
	else if (TargetExtent[3] > m_MaskExtent[3])
	{
		int actualboundary = m_MaskExtent[3] + 1;
		if (m_MaskExtent[3] < m_MaskExtent[2] - 1)
		{
			actualboundary = m_MaskExtent[2];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = actualboundary;
		crntExtent[3] = TargetExtent[3];
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithValue<VTK_TT>(crntExtent, m_pMaskImageData, pWorkingImageData, ScaleRange));
		default:
			break;
		}
	}
	m_MaskExtent[3] = TargetExtent[3];
	// 2.7 z min
	if (TargetExtent[4] < m_MaskExtent[4])
	{
		int actualboundary = m_MaskExtent[4] - 1;
		if (m_MaskExtent[4] > m_MaskExtent[5] + 1)
		{
			actualboundary = m_MaskExtent[5];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = TargetExtent[4];
		crntExtent[5] = actualboundary;
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithValue<VTK_TT>(crntExtent, m_pMaskImageData, pWorkingImageData, ScaleRange));
		default:
			break;
		}
	}
	else if (TargetExtent[4] > m_MaskExtent[4])
	{
		int actualboundary = TargetExtent[4] - 1;
		if (TargetExtent[4] > m_MaskExtent[5] + 1)
		{
			actualboundary = m_MaskExtent[5];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = m_MaskExtent[4];
		crntExtent[5] = actualboundary;
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithoutValue<VTK_TT>(crntExtent, m_pMaskImageData));
		default:
			break;
		}
	}
	m_MaskExtent[4] = TargetExtent[4];
	// 2.8 z max
	if (TargetExtent[5] < m_MaskExtent[5])
	{
		int actualboundary = TargetExtent[5] + 1;
		if (TargetExtent[5] < m_MaskExtent[4] - 1)
		{
			actualboundary = m_MaskExtent[4];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = actualboundary;
		crntExtent[5] = m_MaskExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithoutValue<VTK_TT>(crntExtent, m_pMaskImageData));
		default:
			break;
		}
	}
	else if (TargetExtent[5] > m_MaskExtent[5])
	{
		int actualboundary = m_MaskExtent[5] + 1;
		if (m_MaskExtent[5] < m_MaskExtent[4] - 1)
		{
			actualboundary = m_MaskExtent[4];
		}
		crntExtent[0] = m_MaskExtent[0];
		crntExtent[1] = m_MaskExtent[1];
		crntExtent[2] = m_MaskExtent[2];
		crntExtent[3] = m_MaskExtent[3];
		crntExtent[4] = actualboundary;
		crntExtent[5] = TargetExtent[5];
		switch (pWorkingImageData->GetScalarType())
		{
			vtkTemplateMacro(PosCheckWithValue<VTK_TT>(crntExtent, m_pMaskImageData, pWorkingImageData, ScaleRange));
		default:
			break;
		}
	}
	m_MaskExtent[5] = TargetExtent[5];

	m_pMaskImageData->Modified();
	m_MaskPipeLine->Update();
}

double* ASFModelMask::GetMaskExtent()
{
	return ms_FModelMask->m_MaskExtentOri;
}
double* ASFModelMask::GetMaskScaleRange()
{
	return ms_FModelMask->m_ScaleRange;
}
double* ASFModelMask::GetMDataScaleRange()
{
	return ms_FModelMask->m_ScaleRangeMData;
}
void ASFModelMask::SetMaskModel(const int c_model)
{
	m_maskMoudle = c_model;
}
int ASFModelMask::GetMaskModel()
{
	return m_maskMoudle;
}
void ASFModelMask::SetMDataID(const int c_ID)
{
	m_MDataID = c_ID;
	if (m_MDataID == -1)
	{
		m_ScaleRangeMData[0] = 0;
		m_ScaleRangeMData[1] = 0;
	}
	else
	{
		// 初始化范围
		ASArrayImageData* crntMData = (ASArrayImageData*)ASAbstractData::getDataNodeFromID(m_MDataID);
		crntMData->getArrayImageData()->GetScalarRange(m_ScaleRangeMData);
	}
}
