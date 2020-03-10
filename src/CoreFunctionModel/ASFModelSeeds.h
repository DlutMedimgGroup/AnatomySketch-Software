#pragma once
#include "ASFunctionModelBase.h"

class ASPipelineSeeds;

class ASFModelSeeds : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelSeeds(QObject *parent = 0);
	~ASFModelSeeds();

	// 当前绘制的种子点的ID
	void SetIDOfPaintingData(const int c_ID);
	static int GetIDOfPaintingData();
	// 模块使能状态
	void SetSeedsModleEnabled(const bool c_enabled);
	static bool GetSeedsModleEnabled();
	// 没建立管线就建立管线
	void InitSeedsPipeline();
	// 绘制种子点
	void StartPaintingData(double* pPosition);
	void ContinuePaintingData(double* pPosition);
	// 删除种子点
	void DeleteSeeds(const int c_DataID, const int c_SeedID);
	// 刷新种子点显示
	void RefreshSeedsDisplay();
	// 种子点管线是否显示，没有2D内容时不显示种子点
	void SetSeedShown(const bool c_Shown);
	// 种子点数据发生变化时，如有需要调整撤销列表。就是说删除种子点文件时，将撤销列表中的对应ID 剔除
	void AdjustUndoList();

	// 读取种子点数据
	void LoadSeedData(const QString c_FileName);

private:
	// 唯一对象
	static ASFModelSeeds* ms_FModelSeeds;

	// 当前绘制的种子点的ID
	int m_IDOfPaintingData = -1;
	// 模块使能状态
	bool m_SeedModelEnabled = false;
	// 种子点模块是否显示
	bool m_SeedsShown = false;

	// 种子点显示管线
	ASPipelineSeeds* m_SeedPipeLine = nullptr;

	// 储存操作顺序
	QList<int>* m_OperationSequence;

private slots:
	

};
