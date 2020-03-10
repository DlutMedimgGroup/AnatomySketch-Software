#include "ASThreadGraphCut.h"
#include "ASArrayImageData.h"
#include "ASSeedData.h"
#include "ASVisualizationManager.h"
#include <vtkImageIterator.h>
#include <vtkImageShiftScale.h>
#include <vtkImageClip.h>
#include <vtkImageExtractComponents.h>
#include <QStringList>
#include "stdio.h"
#include "math.h"
#include "graph.h"
#include "graph.cpp"
#include "maxflow.cpp"

#include <itkImage.h>
#include <itkVTKImageToImageFilter.h>
#include <itkImageToVTKImageFilter.h>
#include <itkBinaryBallStructuringElement.h>
#include <itkBinaryMorphologicalClosingImageFilter.h>
#include <itkBinaryMorphologicalOpeningImageFilter.h>
#include <itkConnectedComponentImageFilter.h>

#include <vtkMetaImageWriter.h>

int GetOffset(const unsigned char c_Flag, const int c_OffsetX, const int c_OffsetY, const int c_OffsetZ)
{
	return  (((c_Flag & 0x02) != 0) - ((c_Flag & 0x01) != 0)) * c_OffsetX +
		(((c_Flag & 0x08) != 0) - ((c_Flag & 0x04) != 0)) * c_OffsetY +
		(((c_Flag & 0x20) != 0) - ((c_Flag & 0x10) != 0)) * c_OffsetZ;
}
//smooth and normalize the histogram
void SmoothNorm_Hist(float *Hist0, float *Hist1, int l)
{
	const unsigned int radius = 15;
	int start, end;
	int sum;

	//smoothing
	for (int ii = 0; ii < l; ii++)
	{
		start = ii - radius; if (start < 0) start = 0;
		end = ii + radius; if (end >= l) end = l - 1;
		sum = 0;
		for (int jj = start; jj <= end; jj++) sum += Hist0[jj];
		Hist1[ii] = sum / (end - start + 1);
	}

	//normalization
	sum = 1e-5;
	for (int ii = 0; ii < l; ii++) sum += Hist1[ii];
	for (int ii = 0; ii < l; ii++) Hist1[ii] = Hist1[ii] / sum;
}

ASThreadGraphCut::ASThreadGraphCut(QObject *parent)
	: ASThreadBase(parent)
{

}

ASThreadGraphCut::~ASThreadGraphCut()
{

}
void ASThreadGraphCut::SetInputData(ASArrayImageData* input)
{
	m_InputData = input;
}
void ASThreadGraphCut::SetMaskImage(vtkImageData* mask)
{
	m_MaskImage = mask;
}
void ASThreadGraphCut::SetSeedsID(QList<int> SeedsID)
{
	m_listSeedsID = SeedsID;
}
void ASThreadGraphCut::SetOutputImage(ASArrayImageData* output)
{
	m_OutputData = output;
}
void ASThreadGraphCut::SetVOIExtent(double* Extent)
{
	double adOrigin[3];
	m_InputData->getArrayImageData()->GetOrigin(adOrigin);
	double adSpacing[3];
	m_InputData->getArrayImageData()->GetSpacing(adSpacing);
	for (int i = 0; i < 3; i++)
	{
		m_VOIExtent[i * 2] = (Extent[i * 2] - adOrigin[i]) / adSpacing[i];
		m_VOIExtent[i * 2 + 1] = (Extent[i * 2 + 1] - adOrigin[i]) / adSpacing[i] - 1;
	}
}
void ASThreadGraphCut::SetParameter(const double c_Lambda, const double c_Delta, const int c_Neighbor)
{
	m_Lambda = c_Lambda;
	m_Delta = c_Delta;
	m_nNeighbor = c_Neighbor;
}
void ASThreadGraphCut::SetPostProcessing(const QString c_PostProcessing)
{
	m_PostProcessing = c_PostProcessing;
}

void ASThreadGraphCut::run()
{
	// 0 输入图像
	vtkSmartPointer<vtkImageClip> InputClip = vtkSmartPointer<vtkImageClip>::New();	
	int NumOfComponents = m_InputData->getArrayImageData()->GetNumberOfScalarComponents();
	if (NumOfComponents > 1)
	{
		// 如果输入数据有多帧，取当前帧
		vtkSmartPointer<vtkImageExtractComponents> ImageExtractComponents = vtkSmartPointer<vtkImageExtractComponents>::New();
		int crntTime = ASVisualizationManager::GetDisplayFrame();
		if (crntTime >= NumOfComponents)
		{
			crntTime = NumOfComponents - 1;
		}
		ImageExtractComponents->SetInputData(m_InputData->getArrayImageData());
		ImageExtractComponents->SetComponents(crntTime);
		ImageExtractComponents->Update();
		InputClip->SetInputData(ImageExtractComponents->GetOutput());
	} 
	else
	{
		InputClip->SetInputData(m_InputData->getArrayImageData());
	}	
	InputClip->SetOutputWholeExtent(m_VOIExtent);
	InputClip->ClipDataOn();
	InputClip->Update();
	double* WindowLevel = m_InputData->getWindowLevel();
	vtkSmartPointer<vtkImageShiftScale> ImageShiftScale = vtkSmartPointer<vtkImageShiftScale>::New();
	ImageShiftScale->SetInputData(InputClip->GetOutput());
	ImageShiftScale->SetShift(WindowLevel[0] - WindowLevel[1]);
	ImageShiftScale->SetScale(127.5 / WindowLevel[0]);
	ImageShiftScale->ClampOverflowOn();
	ImageShiftScale->SetOutputScalarTypeToUnsignedChar();
	ImageShiftScale->Update();
	vtkImageData* InputImage = ImageShiftScale->GetOutput();

	// 1 遮罩图像
	vtkSmartPointer<vtkImageClip> MaskClip = vtkSmartPointer<vtkImageClip>::New();
	MaskClip->SetInputData(m_MaskImage);
	MaskClip->SetOutputWholeExtent(m_VOIExtent);
	MaskClip->ClipDataOn();
	MaskClip->Update();
	vtkImageData* MaskImage = MaskClip->GetOutput();

	// 2 种子点图像
	vtkSmartPointer<vtkImageData> SeedImage = vtkSmartPointer<vtkImageData>::New();
	SeedImage->ShallowCopy(InputImage);
	SeedImage->AllocateScalars(VTK_INT, 1);
	int *ptrSeedpre;
	double SeedOri[3];
	SeedImage->GetOrigin(SeedOri);
	double SeedSpa[3];
	SeedImage->GetSpacing(SeedSpa);
	int SeedExt[6];
	SeedImage->GetExtent(SeedExt);
	// 使用迭代器将元素舒初始化为0
	vtkImageIterator<int> it(SeedImage, SeedExt);
	while (!it.IsAtEnd())
	{
		int *inSI = it.BeginSpan();
		int *inSIEnd = it.EndSpan();
		while (inSI != inSIEnd)
		{
			*inSI = 0;
			++inSI;
		}
		it.NextSpan();
	}
	QList<int> LabelList;
	int Pos[3];
	for (int ncrntSeed = 0; ncrntSeed < m_listSeedsID.size(); ncrntSeed++)
	{
		ASSeedData* cnrtSeed = (ASSeedData*)ASAbstractData::getDataNodeFromID(m_listSeedsID.at(ncrntSeed));
		//int Label = cnrtSeed->getLabel();
		//LabelList.append(Label);
		for (int ncrntLine = 0; ncrntLine < cnrtSeed->getNumberOfSeeds(); ncrntLine++)
		{
			structLine* crntLine;// = cnrtSeed->getLineOfSeeds(ncrntLine);
			//for (int ncrntPoint = 0; ncrntPoint < crntLine->pvectorPoint.size(); ncrntPoint++)
			//{
			//	Pos[0] = (crntLine->pvectorPoint.at(ncrntPoint)->dX - SeedOri[0]) / SeedSpa[0];
			//	Pos[1] = (crntLine->pvectorPoint.at(ncrntPoint)->dY - SeedOri[1]) / SeedSpa[1];
			//	Pos[2] = (crntLine->pvectorPoint.at(ncrntPoint)->dZ - SeedOri[2]) / SeedSpa[2];
			//	if (Pos[0] >= SeedExt[0] && Pos[0] <= SeedExt[1] &&
			//		Pos[1] >= SeedExt[2] && Pos[1] <= SeedExt[3] &&
			//		Pos[2] >= SeedExt[4] && Pos[2] <= SeedExt[5])
			//	{
			//		ptrSeedpre = static_cast<int*>(SeedImage->GetScalarPointer(Pos));
			//		//*ptrSeedpre = Label;
			//	}
			//}
		}
	}
	SeedImage->Modified();
	qSort(LabelList.begin(), LabelList.end());

	// 3 输出图像
	vtkSmartPointer<vtkImageData> OutputImage = vtkSmartPointer<vtkImageData>::New();
	OutputImage->ShallowCopy(m_InputData->getArrayImageData());
	OutputImage->AllocateScalars(VTK_INT, 1);

	// 使用迭代器将元素舒初始化为背景的label
	vtkImageIterator<int> it164(OutputImage, OutputImage->GetExtent());
	int BackLabel = LabelList.first();
	while (!it164.IsAtEnd())
	{
		int *inSI = it164.BeginSpan();
		int *inSIEnd = it164.EndSpan();
		while (inSI != inSIEnd)
		{
			*inSI = BackLabel;
			++inSI;
		}
		it164.NextSpan();
	}

	// 4 运算
	for (int i = 1; i < LabelList.size(); i++)
	{
		GenerateGraphCut(InputImage, MaskImage, SeedImage, OutputImage, LabelList.at(i));
	}

	// 5 后处理
	m_PostProcessing = m_PostProcessing.remove(QRegExp("\\s"));
	QStringList OperationList = m_PostProcessing.split("+");
	for (int it = 0; it < OperationList.size(); it++)
	{
		QStringList CrntOpera = OperationList.at(it).split("-");
		if (CrntOpera.first() == 'o' || CrntOpera.first() == 'O')
		{
			double radius = CrntOpera.at(1).toDouble();
			if (radius > 0)
			{
				// 多标签开运算
				this->GenerateCloseOpen(OutputImage, MaskImage, LabelList, radius, Operation_Open);
			}
		}
		else if (CrntOpera.first() == 'c' || CrntOpera.first() == 'C')
		{
			double radius = CrntOpera.at(1).toDouble();
			if (radius > 0)
			{
				// 多标签闭运算
				this->GenerateCloseOpen(OutputImage, MaskImage, LabelList, radius, Operation_Close);
			}
		}
		else if (CrntOpera.first() == 'd' || CrntOpera.first() == 'D')
		{
			double ratio = CrntOpera.at(1).toDouble();
			if (ratio > 0)
			{
				if (ratio > 0.99999)
				{
					ratio = 0.99999;
				}
				// 多标连通域
				this->GenerateConnected(OutputImage, MaskImage, LabelList, ratio);
			}
		}
	}
	m_OutputData->setArrayImageData(OutputImage);
	emit signalDataLoadFinished();
}

// 计算一次graph cut
void ASThreadGraphCut::GenerateGraphCut(vtkImageData* InputImage, vtkImageData* MaskImage, vtkImageData* SeedImage, vtkImageData* OutputImage, const int c_Label)
{
	unsigned int ind;
	int Gray;

	long idNode, idNode0, idNode1;
	int numEdges = 0;
	int numNodes = 0;

	float totalNlink, K = 0.f;
	float nlink, p_src, p_snk;
	float Rp_obj, Rp_bkg, minRp;

	int DimSmall[3];
	InputImage->GetDimensions(DimSmall);
	int AreaSmall = DimSmall[0] * DimSmall[1];
	long *idV = new long[DimSmall[0] * DimSmall[1] * DimSmall[2]]; //The volume of node index
	float *Hist_S = new float[256]; //the histogram of the sources
	float *Hist_T = new float[256]; //the histogram of the sources
	float *Hist_S1 = new float[256]; //the histogram of the sources after smoothing
	float *Hist_T1 = new float[256]; //the histogram of the sources after smoothing
	for (int ii = 0; ii < 256; ii++) { Hist_S[ii] = 0; Hist_T[ii] = 0; }

	//get numNodes and numEdges
	unsigned char* ptrInput;
	unsigned char* ptrMask;
	int* ptrSeed;
	ind = 0;
	unsigned char isEdge = 0x10;
	for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
		if (z == m_VOIExtent[5]) isEdge |= 0x20;
		isEdge |= 0x04;
		for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
			if (y == m_VOIExtent[3]) isEdge |= 0x08;
			isEdge |= 0x01;

			ptrInput = static_cast<unsigned char *>(InputImage->GetScalarPointer(m_VOIExtent[0], y, z));
			ptrMask = static_cast<unsigned char *>(MaskImage->GetScalarPointer(m_VOIExtent[0], y, z));
			ptrSeed = static_cast<int *>(SeedImage->GetScalarPointer(m_VOIExtent[0], y, z));
			for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
				if (x == m_VOIExtent[1]) isEdge |= 0x02;

				if (*ptrMask == 0)
					idV[ind] = -1;
				else
				{
					//add to histogram
					if (*ptrSeed != 0)
						if (*ptrSeed == c_Label)
						{
							Hist_S[*ptrInput]++;
						} 
						else
						{
							Hist_T[*ptrInput]++;
						}						

					idV[ind] = numNodes;
					numNodes++;
					if (m_nNeighbor != 6) {
						for (int i = 0; i < 14; i++)
						{
							if ((isEdge & NeiborFlag26[i]) == false)
								if (*(ptrMask + GetOffset(NeiborFlag26[i], 1, DimSmall[0], AreaSmall)) != 0) numEdges++;
						}
					}
					else {
						for (int i = 0; i < 3; i++)
						{
							if ((isEdge & NeiborFlag6[i]) == false)
								if (*(ptrMask + GetOffset(NeiborFlag6[i], 1, DimSmall[0], AreaSmall)) != 0) numEdges++;
						}
					}
				}
				ptrInput++;
				ptrMask++;
				ptrSeed++;
				ind++;
				isEdge &= 0x3c;
			}
			isEdge &= 0x33;
		}
		isEdge &= 0x0f;
	}

	//smooth and normalize the histograms
	SmoothNorm_Hist(Hist_S, Hist_S1, 256);
	SmoothNorm_Hist(Hist_T, Hist_T1, 256);

	typedef Graph<float, float, float> GraphType;
	GraphType *g = new GraphType(numNodes, numEdges);

	//Construct nodes
	for (int inode = 0; inode < numNodes; inode++)
		g->add_node();


	//Add n-link weights
	ind = 0;
	isEdge = 0x10;
	for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
		if (z == m_VOIExtent[5]) isEdge |= 0x20;
		isEdge |= 0x04;
		for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
			if (y == m_VOIExtent[3]) isEdge |= 0x08;
			isEdge |= 0x01;
			ptrInput = static_cast<unsigned char *>(InputImage->GetScalarPointer(m_VOIExtent[0], y, z));
			ptrMask = static_cast<unsigned char *>(MaskImage->GetScalarPointer(m_VOIExtent[0], y, z));
			for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
				if (x == m_VOIExtent[1]) isEdge |= 0x02;
				if (*ptrMask != 0)
				{
					idNode0 = idV[ind];
					totalNlink = 0;
					int OffsetSmall;
					if (m_nNeighbor != 6)
					{
						for (int i = 0; i < 14; i++)
						{
							if ((isEdge & NeiborFlag26[i]) == false)
							{
								OffsetSmall = GetOffset(NeiborFlag26[i], 1, DimSmall[0], AreaSmall);
								if (*(ptrMask + OffsetSmall) != 0) {
									idNode1 = idV[ind + OffsetSmall];
									nlink = exp(-(*(ptrInput + OffsetSmall) - *ptrInput)*(*(ptrInput + OffsetSmall) - *ptrInput) / (2 * m_Delta* m_Delta));
									totalNlink += nlink;
									g->add_edge(idNode0, idNode1, nlink, nlink);
								}
							}
						}
					}
					else {
						for (int i = 0; i < 3; i++)
						{
							if ((isEdge & NeiborFlag6[i]) == false)
							{
								OffsetSmall = GetOffset(NeiborFlag6[i], 1, DimSmall[0], AreaSmall);
								if (*(ptrMask + OffsetSmall) != 0) {
									idNode1 = idV[ind + OffsetSmall];
									nlink = exp(-(*(ptrInput + OffsetSmall) - *ptrInput)*(*(ptrInput + OffsetSmall) - *ptrInput) / (2 * m_Delta* m_Delta));
									totalNlink += nlink;
									g->add_edge(idNode0, idNode1, nlink, nlink);
								}
							}
						}
					}
					//get the maximum capacity
					if (K < totalNlink) K = totalNlink;
				}
				ptrInput++;
				ptrMask++;
				ind++;
				isEdge &= 0x3c;
			}
			isEdge &= 0x33;
		}
		isEdge &= 0x0f;
	}
	//get the maximum capacity
	K += 100;

	//Add t-link weights
	ind = 0;
	for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
		for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
			ptrInput = static_cast<unsigned char *>(InputImage->GetScalarPointer(m_VOIExtent[0], y, z));
			ptrMask = static_cast<unsigned char *>(MaskImage->GetScalarPointer(m_VOIExtent[0], y, z));
			ptrSeed = static_cast<int *>(SeedImage->GetScalarPointer(m_VOIExtent[0], y, z));
			for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
				//ind = x + y*Dimx + z*area;
				if (*ptrMask != 0) {
					idNode = idV[ind];
					if (*ptrSeed == 0) {
						Gray = *ptrInput;
						p_src = Hist_S1[Gray];
						p_snk = Hist_T1[Gray];
						Rp_obj = -log(p_src);
						Rp_bkg = -log(p_snk);
						minRp = (Rp_obj < Rp_bkg) ? Rp_obj : Rp_bkg;
						g->add_tweights(idNode, m_Lambda*(Rp_bkg - minRp), m_Lambda*(Rp_obj - minRp));
					}
					else
					{
						if (*ptrSeed == c_Label)
						{
							g->add_tweights(idNode, K, 0);
						}
						else
						{
							g->add_tweights(idNode, 0, K);
						}
					}
				}
				ptrInput++;
				ptrMask++;
				ptrSeed++;
				ind++;
			}
		}
	}
	//Do segmentation
	float flow = g->maxflow();

	//------------ Get the segmented image --------------------//
	int* ptrOutput;
	ind = 0;
	for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
		for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
			ptrMask = static_cast<unsigned char *>(MaskImage->GetScalarPointer(m_VOIExtent[0], y, z));
			ptrOutput = static_cast<int*>(OutputImage->GetScalarPointer(m_VOIExtent[0], y, z));
			for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
				//ind = x + y*Dimx + z*area;
				if (*ptrMask != 0) {
					idNode = idV[ind];
					if (g->what_segment(idNode) == GraphType::SOURCE)
						*ptrOutput = c_Label;
				}
				ptrMask++;
				ptrOutput++;
				ind++;
			}
		}
	}

	delete g;
	delete[] idV;
	delete[] Hist_S;
	delete[] Hist_T;
	delete[] Hist_S1;
	delete[] Hist_T1;
}
// 开闭运算
void ASThreadGraphCut::GenerateCloseOpen(vtkImageData* InputImage, vtkImageData* MaskImage, 
	QList<int> list_Label, const double c_Radius, int c_Opration)
{
	// 0 建立图像
	itk::Index<3> corner = { {0,0,0} };
	itk::Size<3> size;
	size[0] = m_VOIExtent[1] - m_VOIExtent[0] + 1;
	size[1] = m_VOIExtent[3] - m_VOIExtent[2] + 1;
	size[2] = m_VOIExtent[5] - m_VOIExtent[4] + 1;
	itk::ImageRegion<3> region(corner, size);

	typedef itk::Image<int, 3>  SigImageType;
	SigImageType::Pointer SigInImage = SigImageType::New();
	SigInImage->SetRegions(region);
	SigInImage->SetSpacing(InputImage->GetSpacing());
	SigInImage->Allocate();
	SigImageType::Pointer SigOutImage = SigImageType::New();
	SigOutImage->SetRegions(region);
	SigOutImage->SetSpacing(InputImage->GetSpacing());
	SigOutImage->Allocate();

	// 1 初始化输出图像
	SigImageType::IndexType  point_temp;
	int OriLabel = list_Label.at(0);
	for (int k = 0; k < size[2]; k++)
	{
		point_temp[2] = k;
		for (int j = 0; j < size[1]; j++)
		{
			point_temp[1] = j;
			for (int i = 0; i < size[0]; i++)
			{				
				point_temp[0] = i;				
				SigOutImage->SetPixel(point_temp, OriLabel);
			}
		}
	}

	// 2 依次运算各个标签值
	for (int i = 1; i < list_Label.size(); i++)
	{
		int crntLabel = list_Label.at(i);
		
		// 2.1 填充输入图像
		int* ptrInputOri;
		point_temp[2] = 0;
		for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
			point_temp[1] = 0;
			for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
				ptrInputOri = static_cast<int *>(InputImage->GetScalarPointer(m_VOIExtent[0], y, z));
				point_temp[0] = 0;
				for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
					if (*ptrInputOri++ == crntLabel)
					{
						SigInImage->SetPixel(point_temp, 255);
					}
					else
					{
						SigInImage->SetPixel(point_temp, 0);
					}
					point_temp[0]++;
				}
				point_temp[1]++;
			}
			point_temp[2]++;
		}

		// 2.2 运算
		typedef itk::BinaryBallStructuringElement<SigImageType::PixelType, 3> StructuringElementType;
		StructuringElementType structuringElement;
		StructuringElementType::SizeType ballSize;
		ballSize.Fill(c_Radius);
		structuringElement.SetRadius(ballSize);
		structuringElement.CreateStructuringElement();

		SigImageType::Pointer TimImage;
		if (c_Opration == Operation_Open)
		{
			typedef itk::BinaryMorphologicalOpeningImageFilter <SigImageType, SigImageType, StructuringElementType>
				BinaryErodeImageFilterType;
			BinaryErodeImageFilterType::Pointer erodeFilter = BinaryErodeImageFilterType::New();
			erodeFilter->SetInput(SigInImage);
			erodeFilter->SetKernel(structuringElement);
			erodeFilter->SetForegroundValue(255);
			erodeFilter->Update();
			TimImage = erodeFilter->GetOutput();
		}
		else if (c_Opration == Operation_Close)
		{
			typedef itk::BinaryMorphologicalClosingImageFilter <SigImageType, SigImageType, StructuringElementType>
				BinaryErodeImageFilterType;
			BinaryErodeImageFilterType::Pointer erodeFilter = BinaryErodeImageFilterType::New();
			erodeFilter->SetInput(SigInImage);
			erodeFilter->SetKernel(structuringElement);
			erodeFilter->SetSafeBorder(true);
			erodeFilter->SetForegroundValue(255);
			erodeFilter->Update();
			TimImage = erodeFilter->GetOutput();
		}
		else
		{
			return;
		}

		// 2.3 根据结果填充输出
		for (int k = 0; k < size[2]; k++)
		{
			point_temp[2] = k;
			for (int j = 0; j < size[1]; j++)
			{
				point_temp[1] = j;
				for (int i = 0; i < size[0]; i++)
				{
					point_temp[0] = i;
					if (TimImage->GetPixel(point_temp) == 255)
					{
						SigOutImage->SetPixel(point_temp, crntLabel);
					}
				}
			}
		}
	}
	// 3 根据结果覆盖输入数据
	int* ptrInputOri;
	unsigned char* ptrMask;
	point_temp[2] = 0;
	for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
		point_temp[1] = 0;
		for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
			ptrInputOri = static_cast<int *>(InputImage->GetScalarPointer(m_VOIExtent[0], y, z));
			ptrMask = static_cast<unsigned char *>(MaskImage->GetScalarPointer(m_VOIExtent[0], y, z));
			point_temp[0] = 0;
			for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
				if (*ptrMask != 0)
				{
					*ptrInputOri++ = SigOutImage->GetPixel(point_temp);
				}
				point_temp[0]++;
			}
			point_temp[1]++;
		}
		point_temp[2]++;
	}
}
// 连通域分析
void ASThreadGraphCut::GenerateConnected(vtkImageData* InputImage, vtkImageData* MaskImage,
	QList<int> list_Label, const double c_Radio)
{
	// 0 建立图像
	itk::Index<3> corner = { { 0,0,0 } };
	itk::Size<3> size;
	size[0] = m_VOIExtent[1] - m_VOIExtent[0] + 1;
	size[1] = m_VOIExtent[3] - m_VOIExtent[2] + 1;
	size[2] = m_VOIExtent[5] - m_VOIExtent[4] + 1;
	itk::ImageRegion<3> region(corner, size);

	typedef itk::Image<int, 3>  SigImageType;
	SigImageType::Pointer SigInImage = SigImageType::New();
	SigInImage->SetRegions(region);
	SigInImage->SetSpacing(InputImage->GetSpacing());
	SigInImage->Allocate();
	SigImageType::Pointer SigOutImage = SigImageType::New();
	SigOutImage->SetRegions(region);
	SigOutImage->SetSpacing(InputImage->GetSpacing());
	SigOutImage->Allocate();

	// 1 初始化输出图像
	SigImageType::IndexType  point_temp;
	int OriLabel = list_Label.at(0);
	for (int k = 0; k < size[2]; k++)
	{
		point_temp[2] = k;
		for (int j = 0; j < size[1]; j++)
		{
			point_temp[1] = j;
			for (int i = 0; i < size[0]; i++)
			{
				point_temp[0] = i;
				SigOutImage->SetPixel(point_temp, OriLabel);
			}
		}
	}

	// 2 依次运算各个标签值
	for (int i = 1; i < list_Label.size(); i++)
	{
		int crntLabel = list_Label.at(i);

		// 2.1 填充输入图像
		int* ptrInputOri;
		point_temp[2] = 0;
		for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
			point_temp[1] = 0;
			for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
				ptrInputOri = static_cast<int *>(InputImage->GetScalarPointer(m_VOIExtent[0], y, z));
				point_temp[0] = 0;
				for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
					if (*ptrInputOri++ == crntLabel)
					{
						SigInImage->SetPixel(point_temp, 255);
					}
					else
					{
						SigInImage->SetPixel(point_temp, 0);
					}
					point_temp[0]++;
				}
				point_temp[1]++;
			}
			point_temp[2]++;
		}

		// 2.2 运算
		// 2.2.1 连通域计算
		typedef itk::ConnectedComponentImageFilter <SigImageType, SigImageType > ConnectedComponentImageFilterType;
		ConnectedComponentImageFilterType::Pointer connected = ConnectedComponentImageFilterType::New();
		connected->SetInput(SigInImage);
		connected->Update();
		int NumOfConnected = connected->GetObjectCount()+1;
		SigImageType::Pointer TimImage = connected->GetOutput();

		int a1 = 0;
		int a2 = 0;
		point_temp[2] = 0;
		for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
			point_temp[1] = 0;
			for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
				point_temp[0] = 0;
				for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
					int aaaaaa = TimImage->GetPixel(point_temp);
					if (aaaaaa == 0)
					{
						a1++;
					}
					else
					{
						a2++;
					}
					
					point_temp[0]++;
				}
				point_temp[1]++;
			}
			point_temp[2]++;
		}


		
		// 2.2.2 直方图
		int *Hist = new int[NumOfConnected];
		for (int ii = 0; ii < NumOfConnected; ii++) { Hist[ii] = 0;}

		point_temp[2] = 0;
		for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
			point_temp[1] = 0;
			for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
				point_temp[0] = 0;
				for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
					if (SigInImage->GetPixel(point_temp) == 255)
					{
						Hist[TimImage->GetPixel(point_temp)]++;
					}
					point_temp[0]++;
				}
				point_temp[1]++;
			}
			point_temp[2]++;
		}

		// 2.2.3 根据像素数计算保留标签
		int max = 0;
		for (int ii = 0; ii < NumOfConnected; ii++) 
		{ 
			if (Hist[ii] > max) max = Hist[ii]; 
		}
		max *= c_Radio;
		for (int ii = 0; ii < NumOfConnected; ii++)
		{
			if (Hist[ii] >= max) 
				Hist[ii] = 255;
			else
				Hist[ii] = 0;
		}

		// 2.3 根据结果填充输出
		for (int k = 0; k < size[2]; k++)
		{
			point_temp[2] = k;
			for (int j = 0; j < size[1]; j++)
			{
				point_temp[1] = j;
				for (int i = 0; i < size[0]; i++)
				{
					point_temp[0] = i;
					if (Hist[TimImage->GetPixel(point_temp)] == 255)
					{
						SigOutImage->SetPixel(point_temp, crntLabel);
					}
				}
			}
		}
	}
	// 3 根据结果覆盖输入数据
	int* ptrInputOri;
	unsigned char* ptrMask;
	point_temp[2] = 0;
	for (int z = m_VOIExtent[4]; z <= m_VOIExtent[5]; z++) {
		point_temp[1] = 0;
		for (int y = m_VOIExtent[2]; y <= m_VOIExtent[3]; y++) {
			ptrInputOri = static_cast<int *>(InputImage->GetScalarPointer(m_VOIExtent[0], y, z));
			ptrMask = static_cast<unsigned char *>(MaskImage->GetScalarPointer(m_VOIExtent[0], y, z));
			point_temp[0] = 0;
			for (int x = m_VOIExtent[0]; x <= m_VOIExtent[1]; x++) {
				if (*ptrMask != 0)
				{
					*ptrInputOri++ = SigOutImage->GetPixel(point_temp);
				}
				point_temp[0]++;
			}
			point_temp[1]++;
		}
		point_temp[2]++;
	}
}
