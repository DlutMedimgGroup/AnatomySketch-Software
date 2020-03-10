#pragma once
#include "ASFunctionModelBase.h"
#include <QString>

class ASThreadImportDataFromDisk;
class ASAbstractData;

class ASFModelImportDataFromDisk : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelImportDataFromDisk(QObject *parent = 0);
	~ASFModelImportDataFromDisk();

	void LoadDataFromDisk(const QString c_FileName);
	void CancelLoadData(); // 读取被取消，放弃读取

private:
	ASThreadImportDataFromDisk* m_ThreadImportDataFromDisk = nullptr;

private slots:
	
	// 数据读入完成
	void slotLoadDataFromDiskFinished(ASAbstractData* NewDataNode, int Type);
};
