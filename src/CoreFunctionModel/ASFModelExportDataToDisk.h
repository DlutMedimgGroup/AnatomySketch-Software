#pragma once
#include "ASFunctionModelBase.h"

class ASThreadExportDataToDisk;
class ASAbstractData;

class ASFModelExportDataToDisk : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelExportDataToDisk(QObject *parent = 0);
	~ASFModelExportDataToDisk();

	void WriteDataToDisk(const QString c_FileName, ASAbstractData* DataNode);
	void CancelWriteData(); // 写入被取消，放弃写入

private:
	ASThreadExportDataToDisk* m_ThreadExportDataToDisk = nullptr;

	QString m_FileName;

private slots:
	
	// 数据保存完成
	void slotSaveDataFromDiskFinished();
};
