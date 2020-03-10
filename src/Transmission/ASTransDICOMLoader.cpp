#include "ASTransDICOMLoader.h"
#include "ASDialogDICOMImporter.h"
#include "ASFModelDICOMImporter.h"
#include "ASTransAddDeleteData.h"
#include "ASAbstractData.h"
#include "ASMainWidget.h"
#include "ASDataTree.h"
#include "ASTransDataGeneral.h"

ASTransDICOMLoader* ASTransDICOMLoader::mc_TransDICOMLoader = nullptr;

ASTransDICOMLoader::ASTransDICOMLoader(QObject *parent)
	: ASTransmissionBase(parent)
{
	mc_TransDICOMLoader = this;

	m_DialogDICOMImporter = new ASDialogDICOMImporter(ASMainWidget::GetMainWidget());
	// 扫描读取模块
	m_FModelDICOMImporter = new ASFModelDICOMImporter(this);
}

ASTransDICOMLoader::~ASTransDICOMLoader()
{

}
// 唯一对象
ASTransDICOMLoader* ASTransDICOMLoader::GetSelfPointer()
{
	return mc_TransDICOMLoader;
}

// 打开DIOCOM扫描窗口
void ASTransDICOMLoader::OpenDICOMScanWidget()
{
	mc_TransDICOMLoader->m_DialogDICOMImporter->show();
	mc_TransDICOMLoader->m_DialogDICOMImporter->CheckVoid();
}
// 扫描硬盘
void ASTransDICOMLoader::ScanDisk(const QString c_path)
{
	mc_TransDICOMLoader->m_FModelDICOMImporter->SetScanningPath(c_path);
	mc_TransDICOMLoader->m_FModelDICOMImporter->StartScanning(); // 开始扫描，多线程，非阻塞，会发signal回来
}
// 扫描完毕
void ASTransDICOMLoader::ScanningDiskFinished(vtkDICOMDirectory* DICOMDirectory)
{
	mc_TransDICOMLoader->m_DialogDICOMImporter->DiskScanningFinished(DICOMDirectory);
}
// 扫描过程中被取消
void ASTransDICOMLoader::ScanningDiskStopManualy()
{
	mc_TransDICOMLoader->m_FModelDICOMImporter->CancelScanning();
}
// 读取DICOM序列
bool ASTransDICOMLoader::LoadDICOMData(vtkDICOMDirectory* dir, QList<int>* serieslist)
{
	if (serieslist->size() == 0)
	{
		return false;
	}
	mc_TransDICOMLoader->m_DICOMDirectory = dir;
	mc_TransDICOMLoader->m_SeriesList = *serieslist;
	mc_TransDICOMLoader->NumOfSeriesToLoad = serieslist->size();
	mc_TransDICOMLoader->crntSeriesToLoad = 1;
	// 0 弹出进度对话框
	mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk = new QMessageBox(QMessageBox::NoIcon, 
		"Anatomy Sketch", 
		tr("Loading the ") + QString::number(mc_TransDICOMLoader->crntSeriesToLoad) + tr("th sequence, ") + QString::number(mc_TransDICOMLoader->NumOfSeriesToLoad) + tr("in total."),
		QMessageBox::Cancel, NULL);
	connect(mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk, SIGNAL(buttonClicked(QAbstractButton*)), mc_TransDICOMLoader, SLOT(slotLoadDICOMStopManualy(QAbstractButton*)));
	mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk->setModal(true);
	mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk->show();
	// 1 读入序列
	mc_TransDICOMLoader->m_FModelDICOMImporter->LoadDICOMData(dir, serieslist->first()); // 开始读数据，多线程，非阻塞，会发signal回来
	return true;
}
// DICOM序列读取完毕，NewDataNode = nullptr表示读取失败
void ASTransDICOMLoader::LoadDICOMDataFinished(ASAbstractData* NewDataNode, int Type)
{
	if (NewDataNode == nullptr)
	{
		// 读取失败
		QMessageBox::critical(NULL, "Anatomy Sketch", tr("The") + QString::number(mc_TransDICOMLoader->crntSeriesToLoad) + tr("th sequence failed to load."), QMessageBox::Yes, QMessageBox::Yes);
		mc_TransDICOMLoader->m_SeriesList.removeFirst();
		mc_TransDICOMLoader->crntSeriesToLoad++;
	} 
	else
	{
		mc_TransDICOMLoader->m_SeriesList.removeFirst();
		mc_TransDICOMLoader->crntSeriesToLoad++;
		// 1 将新读入的数据加入数据树
		ASDataTree::NewDataNode(NewDataNode, nullptr);
		// 2 建立新管线
		ASTransAddDeleteData::AddNewPipeline(NewDataNode, Type);
		// 3 设置当前数据
		if (NewDataNode->isTypeInherited(DataType_ArrayImageData))
		{
			if (ASTransDataGeneral::GetGeneratingData() == nullptr)
			{
				ASTransDataGeneral::SetGeneratingData(NewDataNode);
			}
		}
	}
	// 3 判断是否还有要读入的数据
	if (mc_TransDICOMLoader->m_SeriesList.size() == 0)
	{
		// 没有要读的数据了
		disconnect(mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk, SIGNAL(buttonClicked(QAbstractButton*)), mc_TransDICOMLoader, SLOT(slotLoadDICOMStopManualy(QAbstractButton*)));
		delete mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk;
		ASTransAddDeleteData::UpdateDispleyAfterAddDelete();
		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ArrayImageData);
	}
	else
	{
		// 继续读数据
		mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk->setText(
			tr("Loading the ") + QString::number(mc_TransDICOMLoader->crntSeriesToLoad) + tr("th sequence, ") + QString::number(mc_TransDICOMLoader->NumOfSeriesToLoad) + tr("in total."));
		mc_TransDICOMLoader->m_FModelDICOMImporter->LoadDICOMData(mc_TransDICOMLoader->m_DICOMDirectory, mc_TransDICOMLoader->m_SeriesList.first()); // 开始读数据，多线程，非阻塞，会发signal回来
	}
}

// 从硬盘读取数据过程中被取消
void ASTransDICOMLoader::slotLoadDICOMStopManualy(QAbstractButton* button)
{
	m_FModelDICOMImporter->CancelLoadDICOMData();
	// 没有要读的数据了	
	disconnect(mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk, SIGNAL(buttonClicked(QAbstractButton*)), mc_TransDICOMLoader, SLOT(slotLoadDICOMStopManualy(QAbstractButton*)));
	mc_TransDICOMLoader->m_MessageBoxImportDataFromDisk->deleteLater();
	ASTransAddDeleteData::UpdateDispleyAfterAddDelete();
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_ArrayImageData);
}