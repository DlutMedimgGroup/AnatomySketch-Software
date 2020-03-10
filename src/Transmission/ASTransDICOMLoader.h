#pragma once
#include "ASTransmissionBase.h"
#include <vtkDICOMDirectory.h>
#include <vtkSmartPointer.h>
#include <QString>
#include <QList>
#include <QMessageBox>

class ASDialogDICOMImporter;
class ASFModelDICOMImporter;
class ASAbstractData;

class ASTransDICOMLoader : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransDICOMLoader(QObject *parent = 0);
	~ASTransDICOMLoader();

	// 唯一对象
	static ASTransDICOMLoader* GetSelfPointer();

	// 打开DIOCOM扫描窗口
	static void OpenDICOMScanWidget();
	// 扫描硬盘
	static void ScanDisk(const QString c_path);
	// 扫描完毕
	static void ScanningDiskFinished(vtkDICOMDirectory* DICOMDirectory);
	// 扫描过程中被取消
	static void ScanningDiskStopManualy();
	// 读取DICOM序列
	static bool LoadDICOMData(vtkDICOMDirectory* dir, QList<int>* serieslist);
	// DICOM序列读取完毕，NewDataNode = nullptr表示读取失败
	static void LoadDICOMDataFinished(ASAbstractData* NewDataNode, int Type);

private:
	// 唯一对象
	static ASTransDICOMLoader* mc_TransDICOMLoader;

	// DICOM文件扫描的对话框
	ASDialogDICOMImporter* m_DialogDICOMImporter;
	// DICOM文件导入模块，该模块包含了扫描硬盘的功能，可以建立扫描或读取线程。
	ASFModelDICOMImporter* m_FModelDICOMImporter = nullptr;

	// 读数据进度对话框
	QMessageBox* m_MessageBoxImportDataFromDisk = nullptr;
	int NumOfSeriesToLoad;
	int crntSeriesToLoad;
	// 读入文件DIR和序列号
	vtkSmartPointer<vtkDICOMDirectory> m_DICOMDirectory;
	QList<int> m_SeriesList;

private slots:

	// 从硬盘读取数据过程中被取消
	void slotLoadDICOMStopManualy(QAbstractButton* button);

signals:

};
