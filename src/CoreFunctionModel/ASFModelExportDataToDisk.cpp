#include "ASFModelExportDataToDisk.h"
#include "ASThreadExportDataToDisk.h"
#include "ASTransAddDeleteData.h"
#include <QFile>
#include <QFileInfo>

ASFModelExportDataToDisk::ASFModelExportDataToDisk(QObject *parent)
	: ASFunctionModelBase(parent)
{
	
}

ASFModelExportDataToDisk::~ASFModelExportDataToDisk()
{

}

void ASFModelExportDataToDisk::WriteDataToDisk(const QString c_FileName, ASAbstractData* DataNode)
{
	if (m_ThreadExportDataToDisk != nullptr)
	{
		return;
	}
	m_FileName = c_FileName;
	m_ThreadExportDataToDisk = new ASThreadExportDataToDisk(this);
	m_ThreadExportDataToDisk->SetFileName(c_FileName);
	m_ThreadExportDataToDisk->SetDataNode(DataNode);
	connect(m_ThreadExportDataToDisk, SIGNAL(signalSaveDataFromDiskFinished()), this, SLOT(slotSaveDataFromDiskFinished()));

	m_ThreadExportDataToDisk->start();
}
void ASFModelExportDataToDisk::CancelWriteData()
{
	m_ThreadExportDataToDisk->terminate();
	m_ThreadExportDataToDisk->wait();
	disconnect(m_ThreadExportDataToDisk, SIGNAL(signalSaveDataFromDiskFinished()), this, SLOT(slotSaveDataFromDiskFinished()));
	/*
	Since Qt 4.8, if deleteLater() is called on an object that lives in a thread with no running event loop,
	the object will be destroyed when the thread finishes.
	m_ThreadExportDataToDisk->deleteLater();
	*/
	delete m_ThreadExportDataToDisk;
	m_ThreadExportDataToDisk = nullptr;

	// 删除保存一半的文件
	QFile file(m_FileName);
	file.setPermissions(QFile::WriteOwner);
	if (file.exists())
	{
		file.remove();
	}
	// 如果是mhd文件
	if (QFileInfo(m_FileName).suffix() == "mhd")
	{
		QFileInfo fileInfo = QFileInfo(m_FileName);
		file.setFileName(fileInfo.path() + "//" + fileInfo.completeBaseName() + ".raw");
		file.setPermissions(QFile::WriteOwner);
		if (file.exists())
		{
			file.remove();
		}
		file.setFileName(fileInfo.path() + "//" + fileInfo.completeBaseName() + ".zraw");
		file.setPermissions(QFile::WriteOwner);
		if (file.exists())
		{
			file.remove();
		}
	}

	ASTransAddDeleteData::SaveDataStopManualyFinished();
}

// 数据保存完成
void ASFModelExportDataToDisk::slotSaveDataFromDiskFinished()
{
	disconnect(m_ThreadExportDataToDisk, SIGNAL(signalSaveDataFromDiskFinished()), this, SLOT(slotSaveDataFromDiskFinished()));
	m_ThreadExportDataToDisk->deleteLater();
	m_ThreadExportDataToDisk = nullptr;
	ASTransAddDeleteData::SaveDataToDiskFinished();
}
