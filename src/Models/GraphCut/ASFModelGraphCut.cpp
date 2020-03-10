#include "ASFModelGraphCut.h"
#include "ASThreadGraphCut.h"
#include "ASArrayImageData.h"
#include "ASSeedData.h"
#include "ASPolyImageData.h"
#include "ASFModelMask.h"
#include "ASTransAddDeleteData.h"
#include "ASTransGraphCut.h"
#include "ASDataTree.h"
#include <vtkImageIterator.h>
#include <vtkMarchingCubes.h>
#include <vtkWindowedSincPolyDataFilter.h>
#include <vtkImageClip.h>

ASFModelGraphCut::ASFModelGraphCut(QObject *parent)
	: ASFunctionModelBase(parent)
{

}

ASFModelGraphCut::~ASFModelGraphCut()
{

}

void ASFModelGraphCut::SetInputData(ASArrayImageData* input)
{
	m_InputImage = input;
}
void ASFModelGraphCut::SetSeedsID(QList<int> SeedsID)
{
	m_listSeedsID = SeedsID;
}
void ASFModelGraphCut::SetOutputImage(ASArrayImageData* output)
{
	m_OutputData = output;
}
void ASFModelGraphCut::SetParameter(const double c_Lambda, const double c_Delta, const int c_Neighbor)
{
	m_Lambda = c_Lambda;
	m_Delta = c_Delta;
	m_nNeighbor = c_Neighbor;
}
void ASFModelGraphCut::SetPostProcessing(const QString c_PostProcessing)
{
	m_PostProcessing = c_PostProcessing;
}
// 开始运算
void ASFModelGraphCut::GraphCutsStart()
{
	if (m_ThreadGraphCut != nullptr)
	{
		return;
	}
	m_ThreadGraphCut = new ASThreadGraphCut(this);
	m_ThreadGraphCut->SetInputData(m_InputImage);
	m_ThreadGraphCut->SetMaskImage(ASFModelMask::GetMaskImageData());
	m_ThreadGraphCut->SetSeedsID(m_listSeedsID);
	m_ThreadGraphCut->SetOutputImage(m_OutputData);
	m_ThreadGraphCut->SetVOIExtent(ASFModelMask::GetMaskExtent()); // 在SetInputImage之后
	m_ThreadGraphCut->SetParameter(m_Lambda, m_Delta, m_nNeighbor);
	m_ThreadGraphCut->SetPostProcessing(m_PostProcessing);
	connect(m_ThreadGraphCut, SIGNAL(signalDataLoadFinished()), this, SLOT(signalDataLoadFinished()));
	m_ThreadGraphCut->start();	
}
// Marching cubs
void ASFModelGraphCut::MarchingCubs(ASArrayImageData* MarInput, QList<ASPolyImageData*>* listMarOutput, 
	QList<int> SeedsID, int c_SmoothIterations)
{
	// 2.1 计算标签列表
	QList<int> LabelList;
	for (int ncrntSeed = 0; ncrntSeed < SeedsID.size(); ncrntSeed++)
	{
		ASSeedData* cnrtSeed = (ASSeedData*)ASAbstractData::getDataNodeFromID(SeedsID.at(ncrntSeed));
		int Label = cnrtSeed->getLabel();
		LabelList.append(Label);
	}
	qSort(LabelList.begin(), LabelList.end());
	int BackGroundLabel = LabelList.first();

	// 2.2 预处理
	double adOrigin[3];
	MarInput->getArrayImageData()->GetOrigin(adOrigin);
	double adSpacing[3];
	MarInput->getArrayImageData()->GetSpacing(adSpacing);
	double* Extent = ASFModelMask::GetMaskExtent();
	int VOIExtent[6];
	for (int i = 0; i < 3; i++)
	{
		VOIExtent[i * 2] = (Extent[i * 2] - adOrigin[i]) / adSpacing[i];
		VOIExtent[i * 2 + 1] = (Extent[i * 2 + 1] - adOrigin[i]) / adSpacing[i] - 1;
	}
	// 2.3 建立图像
	vtkSmartPointer<vtkImageClip> InputClip = vtkSmartPointer<vtkImageClip>::New();
	InputClip->SetInputData(MarInput->getArrayImageData());
	InputClip->SetOutputWholeExtent(VOIExtent);
	InputClip->ClipDataOn();
	InputClip->Update();
	vtkImageData* InputImage = InputClip->GetOutput();
	vtkSmartPointer<vtkImageData> input = vtkSmartPointer<vtkImageData>::New();
	input->ShallowCopy(InputImage);
	input->AllocateScalars(VTK_INT, 1);
	
	// 2.4 依次处理各个标签数据
	for (int ncrntSeed = 0; ncrntSeed < SeedsID.size(); ncrntSeed++)
	{
		ASSeedData* cnrtSeed = (ASSeedData*)ASAbstractData::getDataNodeFromID(SeedsID.at(ncrntSeed));
		int Label = cnrtSeed->getLabel();
		if (Label != BackGroundLabel)
		{
			// 2.4.1 输出数据
			ASPolyImageData* OutputPoly;
			int OutputPolyID = -1;
			QList<ASAbstractData*>* Children = MarInput->getChild();
			for (int ncChild = 0; ncChild < Children->size(); ncChild++)
			{
				ASAbstractData* crntChild = Children->at(ncChild);
				if (crntChild->getName() == cnrtSeed->getName() && crntChild->isTypeInherited(DataType_PolyImageData))
				{
					OutputPolyID = crntChild->getDataNodeID();
				}
			}
			if (OutputPolyID == -1)
			{
				// 新建数据
				OutputPoly = new ASPolyImageData();
				OutputPoly->setName(cnrtSeed->getName());
				OutputPoly->setVoidNode(true);
				OutputPoly->setShown(true);
				ASDataTree::NewDataNode(OutputPoly, MarInput); // 将数据加入数据树
				ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_AbstractData);
			}
			else
			{
				OutputPoly = static_cast<ASPolyImageData*>(ASAbstractData::getDataNodeFromID(OutputPolyID));
				if (OutputPoly->isTypeInherited(DataType_PolyImageData) == false)
				{
					return;
				}
			}
			// 2.4.2 填充输入数据
			int* pInput = static_cast<int*>(input->GetScalarPointer());
			int* pOriInput = static_cast<int*>(InputImage->GetScalarPointer());
			for (int z = VOIExtent[4]; z <= VOIExtent[5]; z++) {
				for (int y = VOIExtent[2]; y <= VOIExtent[3]; y++) {
					pInput = static_cast<int *>(input->GetScalarPointer(VOIExtent[0], y, z));
					pOriInput = static_cast<int *>(InputImage->GetScalarPointer(VOIExtent[0], y, z));
					for (int x = VOIExtent[0]; x <= VOIExtent[1]; x++) {
						if (*pOriInput++ == Label)
						{
							*pInput++ = 255;
						}
						else
						{
							*pInput++ = 0;
						}
					}
				}
			}
			vtkSmartPointer<vtkMarchingCubes> pMarchingCubes = vtkSmartPointer<vtkMarchingCubes>::New();
			pMarchingCubes->SetInputData(input);
			pMarchingCubes->ComputeNormalsOff();
			pMarchingCubes->SetValue(0, 128);
			pMarchingCubes->Update();

			if (c_SmoothIterations > 0)
			{
				// Smooth
				vtkSmartPointer<vtkWindowedSincPolyDataFilter> pSmoothFilter = vtkSmartPointer<vtkWindowedSincPolyDataFilter>::New();
				pSmoothFilter->SetInputConnection(pMarchingCubes->GetOutputPort());
				pSmoothFilter->SetNumberOfIterations(c_SmoothIterations);
				pSmoothFilter->Update();
				OutputPoly->setPolyData(pSmoothFilter->GetOutput());
			} 
			else
			{
				OutputPoly->setPolyData(pMarchingCubes->GetOutput());
			}			
			listMarOutput->append(OutputPoly);
		}
	}
}
// 计算成功
void ASFModelGraphCut::signalDataLoadFinished()
{
	disconnect(m_ThreadGraphCut, SIGNAL(signalDataLoadFinished()), this, SLOT(signalDataLoadFinished()));
	m_ThreadGraphCut->wait();
	m_ThreadGraphCut->deleteLater();
	m_ThreadGraphCut = nullptr;
	ASTransGraphCut::GraphCutsFinished(m_OutputData);
}