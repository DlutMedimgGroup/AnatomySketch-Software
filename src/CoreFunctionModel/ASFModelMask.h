#pragma once
#include "ASFunctionModelBase.h"
#include <vtkSmartPointer.h>
#include <vtkImageData.h>

class ASPipelineMask;

class ASFModelMask : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelMask(QObject *parent = 0);
	~ASFModelMask();

	// 当前遮罩模块使能状态
	static bool IsMaskEnable();
	void SetMaskEnable(const bool c_enable);
	// 参照图像ID
	void SetMaskImageID(const int c_ID);
	// 遮罩数据
	static vtkImageData* GetMaskImageData();
	// 设置遮罩参数
	void SetMaskParameter(double* Extent, double* ScaleRange);
	static double* GetMaskExtent();
	static double* GetMaskScaleRange();
	static double* GetMDataScaleRange();
	// 额外输入遮罩图像
	void SetMaskModel(const int c_model);
	int GetMaskModel();
	void SetMDataID(const int c_ID);

private:
	// 唯一对象
	static ASFModelMask* ms_FModelMask;
	// 遮罩对象ID
	int m_WorkingDataID = -1;
	int m_maskMoudle = 0;
	int m_MDataID = -1;
	// 遮罩参数
	double m_MaskExtentOri[6];
	int m_MaskExtent[6];
	double m_ScaleRange[2];
	double m_ScaleRangeMData[2];
	// 遮罩模块使能状态
	static bool ms_isEnable;
	// 存储遮罩的ImageData
	vtkSmartPointer<vtkImageData> m_pMaskImageData = nullptr;
	// 遮罩显示管线
	ASPipelineMask* m_MaskPipeLine = nullptr;

private slots:
	

};
