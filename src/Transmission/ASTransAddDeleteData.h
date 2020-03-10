#pragma once
#include "ASTransmissionBase.h"
#include <QStringList>
#include <QMessageBox>
#include <QAbstractButton>

class ASFModelImportDataFromDisk;
class ASFModelExportDataToDisk;
class ASAbstractData;
class ASArrayImageData;

class ASTransAddDeleteData : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransAddDeleteData(QObject *parent = 0);
	~ASTransAddDeleteData();

	// 唯一对象
	static ASTransAddDeleteData* GetSelfPointer();

	// 从硬盘读取数据
	static void LoadDataFromDisk(const QStringList c_FileNameList); // 开始读取
	static void LoadDataFromDiskFinished(ASAbstractData* NewDataNode, int Type); // 完成一例																				 
	static void LoadDataStopManualyFinished(); // 从硬盘读取数据过程中被取消，线程退出完成
	static void AddLoadWaitingArea(const QStringList c_strlist, const int c_type);
	// 保存数据到硬盘
	static void SaveOneDataToDisk(ASAbstractData* SaveDataNode);
	static void SaveAllPolyDataToDisk();
	static void StartSaveDataToDisk();
	static void SaveDataToDiskFinished(); // 保存完成
	static void SaveDataStopManualyFinished(); // 保存数据过程中被取消，线程退出完成
	// 移除数据
	static void RemoveDataNode(ASAbstractData* RemoveDataNode);
	static void RemoveAllDataNode();
	// 在数据树中移动数据
	// 输入被移动的数据，弹出对话框选择新位置
	static void MoveDataNode(ASAbstractData* DataNode);
	// 1 startID 被移动节点的ID 
	// 2 FaterID 移动后位置的父节点的ID，-1为根节点
	// 3 endPos  移动后位置为父节点的第几个孩子，该序号为原节点删除之前的序号
	static void MoveDataNode(const int startID, const int FaterID, const int endPos);
	// 建立新管线
	static void AddNewPipeline(ASAbstractData* NewDataNode, int Type);
	// 文件数量变更后刷新显示
	static void UpdateDispleyAfterAddDelete(const bool c_bResetCamera = true);
	// 文件数量变更，参数为发生变化的类型，如果对所有类型都有影响则输入DataType_AbstractData，以此类推
	static void UpdateListsAfterAddDelete(const int c_AffectType);
	
private:
	// 唯一对象
	static ASTransAddDeleteData* ms_SelfPointer;

	// 0 从硬盘读入数据
	// 待读入的路径列表
	static QStringList* m_FileListToLoad;
	// 数据读取模块
	static ASFModelImportDataFromDisk* m_FModelImportDataFromDisk;
	// 读数据进度对话框
	QMessageBox* m_MessageBoxImportDataFromDisk = nullptr;
	// 读入等待区，这个地方写的太繁琐。有时间要改。
	QStringList* m_LoadWaitingSeed;
	QStringList* m_LoadWaitingCtr;
	QStringList* m_LoadWaitingLdm;
	QStringList* m_LoadWaitingRoi;
	// 1 向硬盘中写入数据
	// 待写入数据节点列表和路径列表
	QList<ASAbstractData*> m_NodeListToWrite;
	QStringList m_PathListToWrite;
	// 数据写入模块
	static ASFModelExportDataToDisk* m_FModelExportDataToDisk;
	// 写数据进度对话框
	QMessageBox* m_MessageBoxExportDataToDisk = nullptr;

	// 2 移除数据
	// 递归函数，移除子数据
	void RemoveDataNodeAndChildren(ASAbstractData* DataNode);

	// 进行数据操作时是否要重置相机
	static bool ms_bResetCarmera;

private slots:

	// 从硬盘读取数据过程中被取消
	void slotLoadDataStopManualy(QAbstractButton* button);
	// 向硬盘写数据过程中被取消
	void slotWriteDataStopManualy(QAbstractButton* button);

signals:

	// 移除数据后在数据存储树上删除节点
	int signalRemoveNode(ASAbstractData* DataNode);
	// 刷新数据列表
	void signalUpdateDataNodeLists(int Type);
};
