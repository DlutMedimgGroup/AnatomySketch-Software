#pragma once
#include "ASFunctionModelBase.h"
#include <vtkSmartPointer.h>
#include <vtkDICOMDirectory.h>
#include <QMutex>
#include <QList>

class ASThreadDICOMScanner;
class ASThreadDICOMLoader;
class ASAbstractData;

class ASFModelDICOMImporter : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelDICOMImporter(QObject *parent = 0);
	~ASFModelDICOMImporter();

	// 设置扫描路径
	void SetScanningPath(const QString c_ScanPath);
	// 开始扫描，建立线程并传回信息
	void StartScanning();
	// 扫描被取消，放弃扫描
	void CancelScanning();
	// 读取DICOM序列
	void LoadDICOMData(vtkDICOMDirectory* dir, const int c_Series);
	// 读取DICOM序列被取消，放弃读取
	void CancelLoadDICOMData();

private:

	// 扫描路径
	QString m_ScanningPath;
	
	// 子模块
	ASThreadDICOMScanner* m_ThreadDICOMScanner = nullptr;
	ASThreadDICOMLoader* m_ThreadDICOMLoader = nullptr;

	// 扫描结果存储
	vtkSmartPointer<vtkDICOMDirectory> m_DICOMDirectory;
	QMutex* m_MutexForDICOMDirectory;

private slots:

	// 扫描完毕
	void slotScanningFinished();
	// 数据读取成功，NewDataNode = nullptr表示读取失败
	void slotDataLoadFinished(ASAbstractData* NewDataNode, int Type);

signals:

	// 扫描完毕
	void signalScanningFinished(vtkDICOMDirectory* m_DICOMDirectory);

};
