#pragma once
#include "ASTransmissionBase.h"

class ASAbstractData;
class ASFModelSeeds;
class ASSeedData;

class ASTransSeeds : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransSeeds(QObject *parent = 0);
	~ASTransSeeds();

	// 唯一对象
	static ASTransSeeds* GetSelfPointer();
	//当前SeedsID
	static void SetCrntSeedsID(const int c_id);
	static int GetCrntSeedsID();
	// 得到当前种子点文件
	static ASSeedData* GetCurrentSeedData();
	// 新建种子点文件
	static void NewSeedsData(ASAbstractData* pParent);
	static void NewSeedsData(ASAbstractData* pParent, QString name, int label, QColor color);
	// 刷新种子点相关列表
	static void UpdateListsOfSeedData();
	// 设置模块是否使能
	static void SetModelEnabled(const bool c_enable);
	// 刷新种子点列表
	static void RefreshSeedsList();
	// 设置当前正在绘制的种子点数据的ID
	//static void SetPaintingSeedsID(const int c_ID);
	// 绘制种子点
	static void StartPaintSeeds(double* pPosition, const int c_ViewLabel);
	static void ContinuePaintSeeds(double* pPosition, const int c_ViewLabel);
	static void EndPaintSeeds();
	// 刷新种子点显示
	static void RefreshSeedsDisplay();
	// 种子点是否显示
	static void SetSeedShown(const bool c_Shown);
	// 撤销一次
	static void UndoPaintingSedds();
	// 种子点数据发生变化时，如有需要调整撤销列表。就是说删除种子点文件时，将撤销列表中的对应ID 剔除
	static void AdjustUndoList();
	// 从硬盘读文件
	static void LoadseedDataFromDisk(const QStringList c_FileNameList);
	// 移除轮廓线
	static void RemoveSeeds(const int c_SeedDataID, const int c_SeedID);

private:
	// 唯一对象
	static ASTransSeeds* ms_SelfPointer;
	// 种子点模块
	ASFModelSeeds* m_FModelSeeds;
	// 绘制中的节点
	ASSeedData* m_WorkingSeedData = nullptr;
	// 当前SeedID
	int m_CurrentSeedID = -1;

	//void fNewSeedData(QString name, int label, QColor color);

signals :

	// 设置界面的模块使能状态
	void signalSetModelEnabled(bool enabled);
};
