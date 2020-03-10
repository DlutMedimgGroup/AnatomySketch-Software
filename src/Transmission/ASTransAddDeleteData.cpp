#include "ASTransAddDeleteData.h"
#include "ASTransGeneralInteractor.h"
#include "ASFModelImportDataFromDisk.h"
#include "ASFModelExportDataToDisk.h"
#include "ASArrayImageData.h"
#include "ASPolyImageData.h"
#include "ASOperation.h"
#include "ASPipelineManager.h"
#include "ASPipelineArrayImage.h"
#include "ASPipelinePolyImage.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager2D.h"
#include "ASDataTree.h"
#include "ASDataStructureWalker.h"
#include "ASTransSidebarGeneral.h"
#include "ASSeedData.h"
#include "ASContourData.h"
#include "ASLandmarkData.h"
#include "ASTransSeeds.h"
#include "ASTransContour.h"
#include "ASTransLandmark.h"
#include "ASTransAddins.h"
#include "ASMainWidget.h"
#include "ASTransDataGeneral.h"
#include "ASROIData.h"
#include "ASTransROI.h"
#include "ASTransSetting.h"
#include <QFileInfo>
#include <QFileDialog>
#include <QInputDialog>
#include <QSettings>

ASTransAddDeleteData* ASTransAddDeleteData::ms_SelfPointer = nullptr;
QStringList* ASTransAddDeleteData::m_FileListToLoad = new QStringList();
ASFModelImportDataFromDisk* ASTransAddDeleteData::m_FModelImportDataFromDisk = nullptr;
ASFModelExportDataToDisk* ASTransAddDeleteData::m_FModelExportDataToDisk = nullptr;
bool ASTransAddDeleteData::ms_bResetCarmera = true;

ASTransAddDeleteData::ASTransAddDeleteData(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;
	m_LoadWaitingSeed = new QStringList;
	m_LoadWaitingCtr = new QStringList;
	m_LoadWaitingLdm = new QStringList;
	m_LoadWaitingRoi = new QStringList;
}

ASTransAddDeleteData::~ASTransAddDeleteData()
{

}

// 唯一对象
ASTransAddDeleteData* ASTransAddDeleteData::GetSelfPointer()
{
	return ms_SelfPointer;
}

// 从硬盘读取数据
void ASTransAddDeleteData::LoadDataFromDisk(const QStringList c_FileNameList)
{
	ms_bResetCarmera = (ASPipelineBase::GetNumOfObj2D() == 0);
	if (c_FileNameList.size() == 0)
	{
		// 读入等待区数据
		if (ms_SelfPointer->m_LoadWaitingSeed->size() > 0)
		{
			ASTransSeeds::LoadseedDataFromDisk(*ms_SelfPointer->m_LoadWaitingSeed);
			ms_SelfPointer->m_LoadWaitingSeed->clear();
		}
		if (ms_SelfPointer->m_LoadWaitingCtr->size() > 0)
		{
			ASTransContour::LoadContourDataFromDisk(*ms_SelfPointer->m_LoadWaitingCtr);
			ms_SelfPointer->m_LoadWaitingCtr->clear();
		}
		if (ms_SelfPointer->m_LoadWaitingLdm->size() > 0)
		{
			ASTransLandmark::LoadLandMDataFromDisk(*ms_SelfPointer->m_LoadWaitingLdm);
			ms_SelfPointer->m_LoadWaitingLdm->clear();
		}
		if (ms_SelfPointer->m_LoadWaitingRoi->size() > 0)
		{
			ASTransROI::LoadROIDataFromDisk(*ms_SelfPointer->m_LoadWaitingRoi);
			ms_SelfPointer->m_LoadWaitingRoi->clear();
		}
		qDebug("Update Visualization.");
		ASTransAddDeleteData::UpdateDispleyAfterAddDelete();
		qDebug("Update Lists.");
		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_AbstractData);
		return;
	}
	if (m_FModelImportDataFromDisk == nullptr)
	{
		m_FModelImportDataFromDisk = new ASFModelImportDataFromDisk();
	}

	*m_FileListToLoad = c_FileNameList;

	// 0 弹出进度对话框
	ms_SelfPointer->m_MessageBoxImportDataFromDisk = new QMessageBox(QMessageBox::NoIcon, 
		"Anatomy Sketch", "Loading " + c_FileNameList.first(), QMessageBox::Cancel, NULL);
	ms_SelfPointer->m_MessageBoxImportDataFromDisk->setModal(true);
	ms_SelfPointer->m_MessageBoxImportDataFromDisk->show();
	connect(ms_SelfPointer->m_MessageBoxImportDataFromDisk, SIGNAL(buttonClicked(QAbstractButton*)), 
		ms_SelfPointer, SLOT(slotLoadDataStopManualy(QAbstractButton*)));

	qDebug(qPrintable("start Loading Data: " + c_FileNameList.first()));

	// 读入数据，多线程，非阻塞，成功后会发signal回来
	m_FModelImportDataFromDisk->LoadDataFromDisk(c_FileNameList.first());
}
void ASTransAddDeleteData::LoadDataFromDiskFinished(ASAbstractData* NewDataNode, int Type)
{
	m_FileListToLoad->removeFirst();
	// 1 将新读入的数据加入数据树
	ASDataTree::NewDataNode(NewDataNode, nullptr);
	qDebug("Build new data node.");
	if (NewDataNode->isTypeInherited(DataType_ArrayImageData))
	{
		if (ASTransDataGeneral::GetGeneratingData() == nullptr)
		{
			ASTransDataGeneral::SetGeneratingData(NewDataNode);
		}
	}
	// 2 建立新管线
	ASTransAddDeleteData::AddNewPipeline(NewDataNode, Type);
	qDebug("Build new pipeline.");
	// 3 判断是否还有要读入的数据
	if (m_FileListToLoad->size() == 0)
	{
		// 没有要读的数据了
		disconnect(ms_SelfPointer->m_MessageBoxImportDataFromDisk, SIGNAL(buttonClicked(QAbstractButton*)), ms_SelfPointer, SLOT(slotLoadDataStopManualy(QAbstractButton*)));
		delete ms_SelfPointer->m_MessageBoxImportDataFromDisk;
		// 读入等待区数据
		if (ms_SelfPointer->m_LoadWaitingSeed->size() > 0)
		{
			ASTransSeeds::LoadseedDataFromDisk(*ms_SelfPointer->m_LoadWaitingSeed);
			ms_SelfPointer->m_LoadWaitingSeed->clear();
		}
		if (ms_SelfPointer->m_LoadWaitingCtr->size() > 0)
		{
			ASTransContour::LoadContourDataFromDisk(*ms_SelfPointer->m_LoadWaitingCtr);
			ms_SelfPointer->m_LoadWaitingCtr->clear();
		}
		if (ms_SelfPointer->m_LoadWaitingLdm->size() > 0)
		{
			ASTransLandmark::LoadLandMDataFromDisk(*ms_SelfPointer->m_LoadWaitingLdm);
			ms_SelfPointer->m_LoadWaitingLdm->clear();
		}
		if (ms_SelfPointer->m_LoadWaitingRoi->size() > 0)
		{
			ASTransROI::LoadROIDataFromDisk(*ms_SelfPointer->m_LoadWaitingRoi);
			ms_SelfPointer->m_LoadWaitingRoi->clear();
		}
		ASTransAddDeleteData::UpdateDispleyAfterAddDelete(ms_bResetCarmera);
		ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_AbstractData);
	}
	else
	{
		// 继续读数据
		ms_SelfPointer->m_MessageBoxImportDataFromDisk->setText(m_FileListToLoad->first());
		m_FModelImportDataFromDisk->LoadDataFromDisk(m_FileListToLoad->first());
	}
}
void ASTransAddDeleteData::LoadDataStopManualyFinished()
{
	m_FileListToLoad->clear();
	// 没有要读的数据了
	disconnect(ms_SelfPointer->m_MessageBoxImportDataFromDisk, SIGNAL(buttonClicked(QAbstractButton*)), ms_SelfPointer, SLOT(slotLoadDataStopManualy(QAbstractButton*)));
	//delete ms_SelfPointer->m_MessageBoxImportDataFromDisk;
	qDebug("Update Visualization.");
	ASTransAddDeleteData::UpdateDispleyAfterAddDelete();
	qDebug("Update Lists.");
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_AbstractData);
}
void ASTransAddDeleteData::AddLoadWaitingArea(const QStringList c_strlist, const int c_type)
{
	switch (c_type)
	{
	case 0:
		// seed
		ms_SelfPointer->m_LoadWaitingSeed->append(c_strlist);
		break;
	case 1:
		// ctr
		ms_SelfPointer->m_LoadWaitingCtr->append(c_strlist);
		break;
	case 2:
		// ldm
		ms_SelfPointer->m_LoadWaitingLdm->append(c_strlist);
		break;
	case 3:
		// roi
		ms_SelfPointer->m_LoadWaitingRoi->append(c_strlist);
		break;
	default:
		break;
	}
}
// 保存数据到硬盘
void ASTransAddDeleteData::SaveOneDataToDisk(ASAbstractData* SaveDataNode)
{
	ms_SelfPointer->m_NodeListToWrite.clear();
	ms_SelfPointer->m_PathListToWrite.clear();
	//为了能记住上次打开的路径
	QString lastPath = ASTransSetting::GetSetting()->value("LastFilePath").toString();
	// 0 设置文件名
	QString WritingFileName;
	QFileDialog FileDialog;
	switch (SaveDataNode->getDataType())
	{
	case DataType_ArrayImageData:
	case DataType_DICOMData:
	{
		ASArrayImageData* crntDataNode = (ASArrayImageData*)SaveDataNode;
		QString selectFilter = "mhd(*.mhd)";
		//WritingFileName = FileDialog.getSaveFileName(0, "save", "./" + crntDataNode->getName() + ".mhd", "mhd(*.mhd)", &selectFilter,
		//	QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		WritingFileName = FileDialog.getSaveFileName(0, "save", lastPath + "/" +
			crntDataNode->getName(), "mhd(*.mhd)", &selectFilter,
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (WritingFileName == "")
		{
			return;
		}
		break;
	}
	case DataType_PolyImageData:
	{
		ASPolyImageData* crntDataNode = (ASPolyImageData*)SaveDataNode;
		QString setFilter = "OBJ(*.obj);;STL(*.stl);;PLY(*.ply)";
		QString selectFilter, dirString;
		QString crntFileName = crntDataNode->getFileName();
		if (crntFileName.isEmpty() == false)
		{
			dirString = QFileInfo(crntFileName).filePath();
			if (QFileInfo(crntFileName).suffix() == "stl")
			{
				selectFilter = "STL(*.stl)";
			}
			else if (QFileInfo(crntFileName).suffix() == "obj")
			{
				selectFilter = "OBJ(*.obj)";
			}
			else if (QFileInfo(crntFileName).suffix() == "ply")
			{
				selectFilter = "PLY(*.ply)";
			}
		}
		else
		{
			dirString = "./" + crntDataNode->getName() + ".stl";
			selectFilter = "STL(*.stl)";
		}
		//WritingFileName = FileDialog.getSaveFileName(0, "save", dirString, setFilter, &selectFilter,
		//	QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		WritingFileName = FileDialog.getSaveFileName(0, "save", lastPath + "/" + crntDataNode->getName(), 
			setFilter, &selectFilter,
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (WritingFileName == "")
		{
			return;
		}
		break;
	}
	case DataType_SeedData:
	{
		ASSeedData* crntDataNode = static_cast<ASSeedData*>(SaveDataNode);
		QString selectFilter = "seed(*.seed)";
		QString crntName = "";
		if (crntDataNode->getFather()->getDataNodeID() >= 0)
		{
			crntName = lastPath + "/" + crntDataNode->getFather()->getName();
		}
		WritingFileName = FileDialog.getSaveFileName(0, "save", crntName, "seed(*.seed)", &selectFilter,
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		break;
	}
	case DataType_ContourData:
	{
		ASContourData* crntDataNode = static_cast<ASContourData*>(SaveDataNode);
		QString selectFilter = "ctr(*.ctr)";
		QString crntName = "";
		if (crntDataNode->getFather()->getDataNodeID() >= 0)
		{
			crntName = lastPath + "/" + crntDataNode->getFather()->getName();
		}
		WritingFileName = FileDialog.getSaveFileName(0, "save", crntName, "ctr(*.ctr)", &selectFilter,
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (WritingFileName == "")
		{
			return;
		}
		break;
	}
	case DataType_LandmarkData:
	{
		ASLandmarkData* crntDataNode = static_cast<ASLandmarkData*>(SaveDataNode);
		QString selectFilter = "ldm(*.ldm)";
		QString crntName = "";
		if (crntDataNode->getFather()->getDataNodeID() >= 0)
		{
			crntName = lastPath + "/" + crntDataNode->getFather()->getName();
		}
		WritingFileName = FileDialog.getSaveFileName(0, "save", crntName, "ldm(*.ldm)", &selectFilter,
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (WritingFileName == "")
		{
			return;
		}
		break;
	}
	case DataType_ROI:
	{
		ASROIData* crntDataNode = static_cast<ASROIData*>(SaveDataNode);
		QString selectFilter = "roi(*.roi)";
		QString crntName = "";
		if (crntDataNode->getFather()->getDataNodeID() >= 0)
		{
			crntName = lastPath + "/" + crntDataNode->getFather()->getName();
		}
		WritingFileName = FileDialog.getSaveFileName(0, "save", crntName, "roi(*.roi)", &selectFilter,
			QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
		if (WritingFileName == "")
		{
			return;
		}
		break;
	}
	default:
		break;
	}
	QFileInfo fi(WritingFileName);
	ASTransSetting::GetSetting()->setValue("LastFilePath", QVariant(fi.path()));
	ms_SelfPointer->m_PathListToWrite.append(WritingFileName);
	ms_SelfPointer->m_NodeListToWrite.append(SaveDataNode);
	ASTransAddDeleteData::StartSaveDataToDisk();
}
void ASTransAddDeleteData::SaveAllPolyDataToDisk()
{
	ms_SelfPointer->m_NodeListToWrite.clear();
	ms_SelfPointer->m_PathListToWrite.clear();
	//打开文件标准对话框
	QString OperatingDirectoryName = QFileDialog::getExistingDirectory(
		0,						// 指定父类
		"Directory",				// 文件对话框标题栏名称
		"");					// 指定打开文件目录
								//判断是否打开文件
	if (OperatingDirectoryName.size() == 0)
	{
		return;
	}
	// 遍历曲面数据
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_PolyImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_PolyImageData))
	{
		ASPolyImageData* crntDataNode = static_cast<ASPolyImageData*>(item);
		QString filename = OperatingDirectoryName + "/" + crntDataNode->getName() + ".stl";
		ms_SelfPointer->m_NodeListToWrite.append(item);
		ms_SelfPointer->m_PathListToWrite.append(filename);
	}
	ASTransAddDeleteData::StartSaveDataToDisk();
}
void ASTransAddDeleteData::StartSaveDataToDisk()
{
	// 1 弹出进度对话框
	ms_SelfPointer->m_MessageBoxExportDataToDisk = new QMessageBox(QMessageBox::NoIcon, "Anatomy Sketch", "Writing " + QFileInfo(ms_SelfPointer->m_PathListToWrite.first()).baseName(), QMessageBox::Cancel, NULL);
	ms_SelfPointer->m_MessageBoxExportDataToDisk->setModal(true);
	ms_SelfPointer->m_MessageBoxExportDataToDisk->show();
	connect(ms_SelfPointer->m_MessageBoxExportDataToDisk, SIGNAL(buttonClicked(QAbstractButton*)), ms_SelfPointer, SLOT(slotWriteDataStopManualy(QAbstractButton*)));

	// 2 实例一个数据保存模块
	m_FModelExportDataToDisk = new ASFModelExportDataToDisk();
	// 保存数据，多线程，非阻塞，成功后会发signal回来
	m_FModelExportDataToDisk->WriteDataToDisk(ms_SelfPointer->m_PathListToWrite.first(), 
		ms_SelfPointer->m_NodeListToWrite.first());
}
void ASTransAddDeleteData::SaveDataToDiskFinished()
{
	ms_SelfPointer->m_PathListToWrite.removeFirst();
	ms_SelfPointer->m_NodeListToWrite.removeFirst();
	// 判断是否还有要写的数据
	if (ms_SelfPointer->m_NodeListToWrite.size() == 0)
	{
		// 没有要写的数据了
		disconnect(ms_SelfPointer->m_MessageBoxExportDataToDisk, SIGNAL(buttonClicked(QAbstractButton*)), 
			ms_SelfPointer, SLOT(slotWriteDataStopManualy(QAbstractButton*)));
		delete ms_SelfPointer->m_MessageBoxExportDataToDisk;
		delete m_FModelExportDataToDisk;
		m_FModelExportDataToDisk = nullptr;
	} 
	else
	{
		// 继续写数据
		ms_SelfPointer->m_MessageBoxExportDataToDisk->setText("Writing " + QFileInfo(ms_SelfPointer->m_PathListToWrite.first()).baseName());
		m_FModelExportDataToDisk->WriteDataToDisk(ms_SelfPointer->m_PathListToWrite.first(), ms_SelfPointer->m_NodeListToWrite.first());
	}
}
void ASTransAddDeleteData::SaveDataStopManualyFinished()
{
	delete m_FModelExportDataToDisk;
	m_FModelExportDataToDisk = nullptr;
}

// 移除数据
void ASTransAddDeleteData::RemoveDataNode(ASAbstractData* RemoveDataNode)
{
	//询问是否关闭文件
	QMessageBox message(QMessageBox::Warning, "Anatomy Sketch", tr("Remove this and its child data?"), QMessageBox::Yes | QMessageBox::No, NULL);
	if (message.exec() == QMessageBox::No)
	{
		return;
	}
	int DataType = RemoveDataNode->getDataType();
	qDebug(qPrintable("start removing Data: " + RemoveDataNode->getName()));
	ms_SelfPointer->RemoveDataNodeAndChildren(RemoveDataNode);
	ms_bResetCarmera = (ASPipelineBase::GetNumOfObj2D() == 0);
	ASTransAddDeleteData::UpdateDispleyAfterAddDelete(ms_bResetCarmera);
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_AbstractData);
}
void ASTransAddDeleteData::RemoveAllDataNode()
{
	//询问是否关闭文件
	QMessageBox::StandardButton rb = QMessageBox::question(NULL, "Anatomy Sketch", tr("Close all data?"), QMessageBox::Yes | QMessageBox::No, QMessageBox::Yes);
	if (rb == QMessageBox::No)
	{
		return;
	}
	// 移除数据
	ms_SelfPointer->RemoveDataNodeAndChildren(ASDataTree::getRootDataNote());
	// 刷新显示
	ASTransAddDeleteData::UpdateDispleyAfterAddDelete();
	ASTransAddDeleteData::UpdateListsAfterAddDelete(DataType_AbstractData);
}
// 在数据树中移动数据
// 输入被移动的数据，弹出对话框选择新位置
void ASTransAddDeleteData::MoveDataNode(ASAbstractData* DataNode)
{
	int crntID = DataNode->getDataNodeID();
	// 0 弹出对话框选择新的父亲节点
	bool ok;
	QStringList items;
	QList<int> ListID;
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_AbstractData); 
		item != nullptr; 
		item = ASDataStructureWalker::next_inherit(DataType_AbstractData))
	{
		bool a = true;
		if (item->getDataNodeID() != crntID && DataNode->getFather() != item)
		{
			ASAbstractData* f = item->getFather();
			while (f != nullptr)
			{
				if (f->getDataNodeID() == crntID)
				{
					a = false;
					break;
				}
				f = f->getFather();
			}
		}
		else
		{
			a = false;
		}
		if (a)
		{
			items << item->getName();
			ListID << item->getDataNodeID();
		}
	}
	
	QString item = QInputDialog::getItem(ASMainWidget::GetMainWidget(), "Input Item Dialog",
		"please choose a item:", items, 0, true, &ok);
	if (ok)
	{
		int index = items.indexOf(item);
		// 0 在数据树中移动节点
		ASAbstractData* FatherDataNode = ASAbstractData::getDataNodeFromID(ListID.at(index));
		ASAbstractData* startIDDataNode = DataNode;
		startIDDataNode->getFather()->removeChild(startIDDataNode);
		int numOfChild = FatherDataNode->getChild()->size();
		FatherDataNode->addChild(startIDDataNode, numOfChild);
		startIDDataNode->setFather(FatherDataNode);
		// 1 重新计算数据的层级
		ASDataTree::RefreshArrayDataLayer();
		// 2 刷新管线层级
		for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ArrayImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_ArrayImageData))
		{
			ASArrayImageData* CrntDataNode = (ASArrayImageData*)item;
			QList<ASPipelineBase*>* listPipeline = new QList<ASPipelineBase*>;
			ASPipelineManager::GetPipelineForDataID(listPipeline, CrntDataNode->getDataNodeID());
			for (int ncrntPipe = 0; ncrntPipe < listPipeline->size(); ncrntPipe++)
			{
				if (listPipeline->at(ncrntPipe)->GetPipeLineType() == PipeLineType_ArrayImage)
				{
					((ASPipelineArrayImage*)listPipeline->at(ncrntPipe))->SetLayer(CrntDataNode->getLayer());
				}
			}
		}
		// 3 重新计算顶层体数据和像素尺寸
		ASVisualizationManager::ResetTopArrayImage();
		// 4 渲染
		ASVisualizationManager::Render();
		// 5 其他界面更新
		ASTransDataGeneral::ResetGeneratingData();
		// 6 刷新侧边栏文件列表显示，所有类型变化都需要刷新
		emit ms_SelfPointer->signalUpdateDataNodeLists(DataType_AbstractData);
	}
}
// 1 startID 被移动节点的ID 
// 2 FaterID 移动后位置的父节点的ID，-1为根节点
// 3 endPos  移动后位置为父节点的第几个孩子，该序号为原节点删除之前的序号
void ASTransAddDeleteData::MoveDataNode(const int startID, const int FaterID, const int endPos)
{
	// 0 在数据树中移动节点
	int orderToInsert = endPos;
	ASAbstractData* FatherDataNode = ASAbstractData::getDataNodeFromID(FaterID);
	ASAbstractData* startIDDataNode = ASAbstractData::getDataNodeFromID(startID);
	if (startIDDataNode->getFather() == FatherDataNode)
	{
		// 在相同根节点下移动
		if (startIDDataNode->getFather()->getOrderInBrother(startIDDataNode) < endPos)
		{
			// 向下移动
			orderToInsert -= 1;
		}
	}
	startIDDataNode->getFather()->removeChild(startIDDataNode);
	FatherDataNode->addChild(startIDDataNode, orderToInsert);
	startIDDataNode->setFather(FatherDataNode);
	// 1 重新计算数据的层级
	ASDataTree::RefreshArrayDataLayer();
	// 2 刷新管线层级
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ArrayImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_ArrayImageData))
	{
		ASArrayImageData* CrntDataNode = (ASArrayImageData*)item;
		QList<ASPipelineBase*>* listPipeline = new QList<ASPipelineBase*>;
		ASPipelineManager::GetPipelineForDataID(listPipeline, CrntDataNode->getDataNodeID());
		for (int ncrntPipe = 0; ncrntPipe < listPipeline->size(); ncrntPipe++)
		{
			if (listPipeline->at(ncrntPipe)->GetPipeLineType() == PipeLineType_ArrayImage)
			{
				((ASPipelineArrayImage*)listPipeline->at(ncrntPipe))->SetLayer(CrntDataNode->getLayer());
			}
		}
	}
	// 3 重新计算顶层体数据和像素尺寸
	ASVisualizationManager::ResetTopArrayImage();
	// 4 渲染
	ASVisualizationManager::Render();
}
// 建立新管线
void ASTransAddDeleteData::AddNewPipeline(ASAbstractData* NewDataNode, int Type)
{
	switch (Type)
	{
	case DataType_ArrayImageData:
	case DataType_DICOMData:
	{
		ASArrayImageData* crntDataNode = (ASArrayImageData*)NewDataNode;
		ASPipelineArrayImage* newPipeLine = (ASPipelineArrayImage*)ASPipelineManager::AddPipeline(PipeLineType_ArrayImage);
		newPipeLine->SetInputData(crntDataNode->getArrayImageData());
		newPipeLine->SetDataID(NewDataNode->getDataNodeID());
		newPipeLine->ArrayPipeInit(crntDataNode);
		break;
	}
	case DataType_PolyImageData:
	{
		ASPolyImageData* crntDataNode = (ASPolyImageData*)NewDataNode;
		ASPipelinePolyImage* newPipeLine = (ASPipelinePolyImage*)ASPipelineManager::AddPipeline(PipeLineType_PolyImage);
		newPipeLine->SetInputData(crntDataNode->getPolyImageData());
		newPipeLine->SetDataID(NewDataNode->getDataNodeID());
		newPipeLine->SetShown(true);
		newPipeLine->SetColor(crntDataNode->getColor());
		newPipeLine->SetOpacity(1);
		newPipeLine->SetContourLineWidth();
		newPipeLine->SetContourLinePickable(ASVisualizationManager2D::GetContoursPickable());
		break;
	}
	default:
		break;
	}
}
// 文件数量变更后刷新显示
void ASTransAddDeleteData::UpdateDispleyAfterAddDelete(const bool c_bResetCamera)
{
	// 更新数据范围	
	ASVisualizationManager::ResetDisplayExtent();
	// 根据当前各管线的数量更改辅助显示元素
	ASVisualizationManager::UpdateAuxiliaryPipeline(); // 在ResetDisplayExtent()之后，刷新种子点需要用到。
	// 重新计算顶层体数据和像素尺寸
	ASVisualizationManager::ResetTopArrayImage();
	if (c_bResetCamera)
	{
		// 设置焦点
		double Extent[6];
		ASVisualizationManager::GetDisplayExtent(Extent);
		ASTransGeneralInteractor::fSetVisualizationPosition((Extent[0] + Extent[1]) / 2, (Extent[2] + Extent[3]) / 2, (Extent[4] + Extent[5]) / 2);
		// 重置相机
		ASVisualizationManager::ResetCamera();
	}
	// 渲染
	ASVisualizationManager::Render();
}
// 文件数量变更
void ASTransAddDeleteData::UpdateListsAfterAddDelete(const int c_AffectType)
{
	// 刷新侧边栏文件列表显示，所有类型变化都需要刷新
	emit ms_SelfPointer->signalUpdateDataNodeLists(c_AffectType);
	// 刷新侧边栏文件下拉选框
	ASTransSidebarGeneral::RefreshDataComboBox(c_AffectType); 
	// 如果是种子点数据刷新种子点列表
	if (ASSeedData::isTypeInheritedStatic(c_AffectType))
	{
		ASTransSeeds::RefreshSeedsList();
		ASTransSeeds::AdjustUndoList();
	}
	// 如果是轮廓线数据刷新种子点列表	
	if (ASContourData::isTypeInheritedStatic(c_AffectType))
	{
		ASTransContour::RefreshContourList();
	}
	// 如果是ROI
	if (ASROIData::isTypeInheritedStatic(c_AffectType))
	{
		ASTransROI::RefreshROIList();
	}
}
// 递归函数，移除子数据
void ASTransAddDeleteData::RemoveDataNodeAndChildren(ASAbstractData* DataNode)
{	
	// 0 释放子节点
	QList<ASAbstractData*>* listChildren = DataNode->getChild();
	for (int ncrntChild = listChildren->size() - 1; ncrntChild >= 0; ncrntChild--)
	{
		RemoveDataNodeAndChildren(listChildren->at(ncrntChild));
	}
	// 1 记录数据ID和数据类型，并释放数据
	if (DataNode == ASDataTree::getRootDataNote())
	{
		return;
	}
	if (DataNode->getDataType() == DataType_Operation)
	{
		ASTransAddins::DataRemoved(DataNode);
	}
	int nID = DataNode->getDataNodeID();
	// 3 当前数据
	if (DataNode == ASTransDataGeneral::GetGeneratingData())
	{
		ASTransDataGeneral::SetGeneratingData(nullptr);
	}
	emit ms_SelfPointer->signalRemoveNode(DataNode);
	// 2 后删除显示
	QList<ASPipelineBase*>* crntListPipeline = new QList<ASPipelineBase*>;
	ASPipelineManager::GetPipelineForDataID(crntListPipeline, nID);
	for (int crntPipe = 0; crntPipe < crntListPipeline->size(); crntPipe++)
	{
		if (crntListPipeline->at(crntPipe)->isTypeInherited(PipeLineType_ArrayImage)
			|| crntListPipeline->at(crntPipe)->isTypeInherited(PipeLineType_PolyImage))
		{
			ASPipelineManager::RemovePipeline(crntListPipeline->at(crntPipe));
		}		
	}
}
void ASTransAddDeleteData::slotLoadDataStopManualy(QAbstractButton* button)
{
	m_FModelImportDataFromDisk->CancelLoadData();
}
// 向硬盘写数据过程中被取消
void ASTransAddDeleteData::slotWriteDataStopManualy(QAbstractButton* button)
{
	m_FModelExportDataToDisk->CancelWriteData();
	disconnect(ms_SelfPointer->m_MessageBoxExportDataToDisk, SIGNAL(buttonClicked(QAbstractButton*)), ms_SelfPointer, SLOT(slotWriteDataStopManualy(QAbstractButton*)));
	delete ms_SelfPointer->m_MessageBoxExportDataToDisk;
}