#include "ASFModelDICOMImporter.h"
#include "ASThreadDICOMScanner.h"
#include "ASThreadDICOMLoader.h"
#include "ASTransDICOMLoader.h"

ASFModelDICOMImporter::ASFModelDICOMImporter(QObject *parent)
	: ASFunctionModelBase(parent)
{
	m_DICOMDirectory = vtkSmartPointer<vtkDICOMDirectory>::New();

	m_MutexForDICOMDirectory = new QMutex();
}

ASFModelDICOMImporter::~ASFModelDICOMImporter()
{

}

// 设置扫描路径
void ASFModelDICOMImporter::SetScanningPath(const QString c_ScanPath)
{
	m_ScanningPath = c_ScanPath;
}
// 开始扫描，建立线程并传回信息
void ASFModelDICOMImporter::StartScanning()
{
	if (m_ThreadDICOMScanner != nullptr)
	{
		return;
	}	
	m_ThreadDICOMScanner = new ASThreadDICOMScanner(this);
	connect(m_ThreadDICOMScanner, SIGNAL(signalScanningFinished()), this, SLOT(slotScanningFinished()));
	m_ThreadDICOMScanner->SetScanPath(m_ScanningPath);
	m_ThreadDICOMScanner->SetDICOMDirectory(m_DICOMDirectory);
	m_ThreadDICOMScanner->SetMutex(m_MutexForDICOMDirectory);
	m_ThreadDICOMScanner->start();
}
// 扫描被取消，放弃扫描
void ASFModelDICOMImporter::CancelScanning()
{
	m_ThreadDICOMScanner->terminate();
	m_ThreadDICOMScanner->wait();
	m_MutexForDICOMDirectory->unlock();
	disconnect(m_ThreadDICOMScanner, SIGNAL(signalScanningFinished()), this, SLOT(slotScanningFinished()));
	/*
	Since Qt 4.8, if deleteLater() is called on an object that lives in a thread with no running event loop,
	the object will be destroyed when the thread finishes.
	m_ThreadExportDataToDisk->deleteLater();
	*/
	delete m_ThreadDICOMScanner;
	m_ThreadDICOMScanner = nullptr;
}
// 扫描完毕
void ASFModelDICOMImporter::slotScanningFinished()
{
	m_ThreadDICOMScanner->terminate();
	m_ThreadDICOMScanner->wait();
	// 释放线程
	disconnect(m_ThreadDICOMScanner, SIGNAL(signalScanningFinished()), this, SLOT(slotScanningFinished()));
	delete m_ThreadDICOMScanner;
	m_ThreadDICOMScanner = nullptr;
	// 刷新窗口数据列表显示
	ASTransDICOMLoader::ScanningDiskFinished(m_DICOMDirectory);
}
// 读取DICOM序列
void ASFModelDICOMImporter::LoadDICOMData(vtkDICOMDirectory* dir, const int c_Series)
{
	if (m_ThreadDICOMLoader != nullptr)
	{
		return;
	}
	m_ThreadDICOMLoader = new ASThreadDICOMLoader(this);
	connect(m_ThreadDICOMLoader, SIGNAL(signalDataLoadFinished(ASAbstractData*, int)), this, SLOT(slotDataLoadFinished(ASAbstractData*, int)));
	m_ThreadDICOMLoader->SetDICOMDirSeries(dir, c_Series);
	m_ThreadDICOMLoader->start();
}
// 读取DICOM序列被取消，放弃读取
void ASFModelDICOMImporter::CancelLoadDICOMData()
{
	m_ThreadDICOMLoader->terminate();
	m_ThreadDICOMLoader->wait();
	disconnect(m_ThreadDICOMLoader, SIGNAL(signalDataLoadFinished(ASAbstractData*, int)), this, SLOT(slotDataLoadFinished(ASAbstractData*, int)));
	delete m_ThreadDICOMLoader; // 这里可能会造成内存泄漏	
	m_ThreadDICOMLoader = nullptr;
}
// 数据读取成功，NewDataNode = nullptr表示读取失败
void ASFModelDICOMImporter::slotDataLoadFinished(ASAbstractData* NewDataNode, int Type)
{
	disconnect(m_ThreadDICOMLoader, SIGNAL(signalDataLoadFinished(ASAbstractData*, int)), this, SLOT(slotDataLoadFinished(ASAbstractData*, int)));
	delete m_ThreadDICOMLoader;
	m_ThreadDICOMLoader = nullptr;
	ASTransDICOMLoader::LoadDICOMDataFinished(NewDataNode, Type);
}