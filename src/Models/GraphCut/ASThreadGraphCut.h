#pragma once
#include "ASThreadBase.h"
#include <vtkImageData.h>
#include <vtkSmartPointer.h>
#include <QList>

class ASArrayImageData;

#define Operation_Open	0
#define Operation_Close	1

class ASThreadGraphCut : public ASThreadBase
{
	Q_OBJECT

public:
	ASThreadGraphCut(QObject *parent = 0);
	~ASThreadGraphCut();

	void SetInputData(ASArrayImageData* input);
	void SetMaskImage(vtkImageData* mask);
	void SetSeedsID(QList<int> SeedsID);
	void SetOutputImage(ASArrayImageData* output);
	void SetVOIExtent(double* Extent);
	void SetParameter(const double c_Lambda, const double c_Delta, const int c_Neighbor);
	void SetPostProcessing(const QString c_PostProcessing);

protected:
	void run();

private:

	ASArrayImageData* m_InputData;
	vtkSmartPointer<vtkImageData> m_MaskImage;
	QList<int> m_listSeedsID;
	ASArrayImageData* m_OutputData;
	double m_Lambda;
	double m_Delta;
	int m_nNeighbor;
	int m_VOIExtent[6];
	QString m_PostProcessing;

	unsigned char NeiborFlag26[14] = { 0x25, 0x21, 0x29, 0x24, 0x20, 0x28, 0x26, 0x22, 0x2a, 0x01, 0x08, 0x0a, 0x01, 0x06 };
	unsigned char NeiborFlag6[3] = { 0x02, 0x08, 0x20};

	// 计算Graph cuts
	void GenerateGraphCut(vtkImageData* InputImage, vtkImageData* MaskImage, vtkImageData* SeedImage, vtkImageData* OutputImage, const int c_Label);
	// 开运算 闭运算
	void GenerateCloseOpen(vtkImageData* InputImage, vtkImageData* MaskImage, 
		QList<int> list_Label, const double c_Radius, const int c_Opration);
	// 连通域分析
	void GenerateConnected(vtkImageData* InputImage, vtkImageData* MaskImage, 
		QList<int> list_Label, const double c_Radio);


signals:
	// 计算成功
	void signalDataLoadFinished();
};
