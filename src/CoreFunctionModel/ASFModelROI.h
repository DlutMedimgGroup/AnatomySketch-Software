#pragma once
#include "ASFunctionModelBase.h"
#include "ASPipelineROICube.h"

class ASFModelROI : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelROI(QObject *parent = 0);
	~ASFModelROI();

	// 没建立管线就建立管线
	void InitROICubePipeline();
	// 刷新ROICube显示
	void RefreshROICubeDisplay();
	// 读取ROI数据
	void LoadROIData(const QString c_FileName);

private:
	// 唯一对象
	static ASFModelROI* ms_SelfPointer;

	// ROICube显示管线
	ASPipelineROICube* m_ROICubePipeLine = nullptr;
	


private slots:
	

};
