#include "ASFModelImportDataFromDisk.h"
#include "ASThreadImportDataFromDisk.h"
#include "ASAbstractData.h"
#include "ASTransAddDeleteData.h"

ASFModelImportDataFromDisk::ASFModelImportDataFromDisk(QObject *parent)
	: ASFunctionModelBase(parent)
{
	
}

ASFModelImportDataFromDisk::~ASFModelImportDataFromDisk()
{
	if (m_ThreadImportDataFromDisk != nullptr)
	{
		delete m_ThreadImportDataFromDisk;
	}
}

void ASFModelImportDataFromDisk::LoadDataFromDisk(const QString c_FileName)
{
	if (m_ThreadImportDataFromDisk == nullptr)
	{
		m_ThreadImportDataFromDisk = new ASThreadImportDataFromDisk();
		connect(m_ThreadImportDataFromDisk, SIGNAL(signalDataLoadFinished(ASAbstractData*, int)), 
			this, SLOT(slotLoadDataFromDiskFinished(ASAbstractData*, int)));
	}
	m_ThreadImportDataFromDisk->wait();
	m_ThreadImportDataFromDisk->SetFileName(c_FileName);
	m_ThreadImportDataFromDisk->start();
}
// 读取被取消，放弃读取
void ASFModelImportDataFromDisk::CancelLoadData()
{
	m_ThreadImportDataFromDisk->terminate();
	m_ThreadImportDataFromDisk->wait();
	/*
	Since Qt 4.8, if deleteLater() is called on an object that lives in a thread with no running event loop,
	the object will be destroyed when the thread finishes.
	m_ThreadImportDataFromDisk->deleteLater();
	*/
	ASTransAddDeleteData::LoadDataStopManualyFinished();
}

// 数据读入完成
void ASFModelImportDataFromDisk::slotLoadDataFromDiskFinished(ASAbstractData* NewDataNode, int Type)
{
	//m_ThreadImportDataFromDisk->quit();
	m_ThreadImportDataFromDisk->wait();
	ASTransAddDeleteData::LoadDataFromDiskFinished(NewDataNode, Type);
}
