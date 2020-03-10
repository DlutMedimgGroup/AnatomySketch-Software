#include "ASDataManager.h"
#include "ASDataTree.h"
#include "ASDataStructureWalker.h"
#include <QFileInfo>
#include <QMessageBox>
#include <QStringList>
#include <QInputDialog>
#include <QFileDialog>

#include "ASAbstractData.h"
#include "ASArrayImageData.h"
#include "ASSeedData.h"
#include "ASDICOMData.h"
//#include "ASDICOMRTLoader.h"
//#include "ASPolyDeformation.h"
//#include "ASMask.h"


ASDataManager::ASDataManager(QObject *parent)
	: QObject(parent)
{
	// 数据存储树形数据结构
	m_DataTree = new ASDataTree(this);
	//// 面数据编辑模块
	//m_PolyDeformation = new ASPolyDeformation();
	//// 遮罩模块
	//m_pMask = new ASMask();
}

ASDataManager::~ASDataManager()
{

}

////由父对象输入连接器的指针
//void ASDataManager::setSignalLinker(ASSignalLinker* linker)
//{
//	m_SignalLinker = linker;
//
//	//连接器相关的信号槽
//	connect(this, SIGNAL(signalSetDatamanagerAddress(void*)), m_SignalLinker, SIGNAL(signalSetDatamanagerAddress(void*)));
//	connect(this, SIGNAL(signalRefreshDataManagerWidget()), m_SignalLinker, SIGNAL(signalRefreshDataManagerWidget()));
//	connect(this, SIGNAL(signalAddImageDataDisplay(int)), m_SignalLinker, SIGNAL(signalAddImageDataDisplay(int)));						//新增体数据显示
//	connect(this, SIGNAL(signalAddPolyDataDisplay(int)), m_SignalLinker, SIGNAL(signalAddPolyDataDisplay(int)));						//新增面数据显示
//	connect(this, SIGNAL(signalAddDICOMRTDisplay()), m_SignalLinker, SIGNAL(signalAddDICOMRTDisplay()));								//新增DICOM-RT文件显示，由于同时打开了多个RT文件，所以直接进行RT数据显示的刷新。
//	connect(this, SIGNAL(signalAddSeeds2DDisplay(int)), m_SignalLinker, SIGNAL(signalAddSeeds2DDisplay(int)));							//新增种子点显示，即新建相应标签文件的种子点渲染管线
//	
//	connect(this, SIGNAL(signalRemoveDataDisplay(int, enumDataType)), m_SignalLinker, SIGNAL(signalRemoveDataDisplay(int, enumDataType)));// 移除数据显示
//
//	connect(this, SIGNAL(signalUpdataLayerDisplay()), m_SignalLinker, SIGNAL(signalUpdataLayerDisplay()));								//体数据层更新后刷新层级的显示
//	connect(this, SIGNAL(signalRefreshFileComboBox()), m_SignalLinker, SIGNAL(signalRefreshFileComboBox()));							//刷新所有文件下拉选框
//	connect(this, SIGNAL(signalPolyDeformationRefresh()), m_SignalLinker, SIGNAL(signalPolyDeformationRefresh()));						//FFD操作过程中，generate之前，刷新显示
//}

////参数初始化，软件初始化第三阶段
//void ASDataManager::Parameterinitialization()
//{
//	emit signalSetDatamanagerAddress(this);
//}

//// 打开数据
//void ASDataManager::LoadData(QString filename)
//{
//	// 获取文件后缀，用于判断文件类型
//	QFileInfo pFileInfo = QFileInfo(filename);
//	QString qstrSuffix = pFileInfo.suffix();
//
//	//判断文件类型
//	if (strcmp(qstrSuffix.toStdString().c_str(), "mhd") == 0 || strcmp(qstrSuffix.toStdString().c_str(), "nii") == 0)
//	{
//		// 体数据
//		int newID = m_DataTree->LoadData(filename, EMArrayImageData);
//		if (newID >= 0)
//		{
//			//读取成功，更新界面和显示
//			emit signalAddImageDataDisplay(newID);
//		}
//	}
//	else if (strcmp(qstrSuffix.toStdString().c_str(), "stl") == 0 || strcmp(qstrSuffix.toStdString().c_str(), "ply") == 0 || strcmp(qstrSuffix.toStdString().c_str(), "obj") == 0)
//	{
//		// 面数据
//		int newID = m_DataTree->LoadData(filename, EMPolyImageData);
//		if (newID >= 0)
//		{
//			//读取成功，更新界面和显示
//			emit signalAddPolyDataDisplay(newID);
//		}
//	}
//	else if (strcmp(qstrSuffix.toStdString().c_str(), "dcm") == 0)
//	{
//		//DICOM-RT数据
//		QString fileName = QFileInfo(filename).fileName();
//		if (fileName.at(0) != 'R' || fileName.at(1) != 'S' || fileName.at(2) != '.')	//	检查文件名的前缀是否为"RS."
//		{
//			//若前缀名不是"RS."
//			int ret = QMessageBox::question(0, tr("message"), tr("The file name does not meet the DICOM-RT requirements. Whether to continue?"), QMessageBox::Yes, QMessageBox::Cancel);
//			if (ret == QMessageBox::Cancel)
//			{
//				return;
//			}
//		}
//		//输入一个体数据来确定RT的原点尺寸等相关信息
//		QStringList lst;
//		QList<int> IDofLst;
//		for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(EMArrayImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(EMArrayImageData))
//		{
//			lst << item->getName();
//			IDofLst << item->getDataNodeID();
//		}
//		//弹出询问对话框
//		bool ok = false;
//		QString rtnstring = QInputDialog::getItem(0, tr("message"), tr("Choose an array image data to set coordinate information: "), lst, 0, true, &ok);
//		if (ok)
//		{
//			vtkImageData* crntImageData = ((ASArrayImageData*)ASAbstractData::getDataNodeFromID(IDofLst.at(lst.indexOf(rtnstring))))->getArrayImageData();
//			ASDICOMRTLoader Loader;
//			Loader.setDataTree(m_DataTree);
//			Loader.setFatherID(IDofLst.at(lst.indexOf(rtnstring)));
//			Loader.setFileName(filename);
//			if (Loader.update())
//			{
//				emit signalAddDICOMRTDisplay();
//			}
//		}
//	}
//
//	emit signalRefreshDataManagerWidget();
//	emit signalRefreshFileComboBox();		//刷新文件下拉选框
//}
//// 读取DICOM数据
//void ASDataManager::LoadDICOMSeries(vtkDICOMDirectory* dir, QList<int>* serieslist)
//{
//	int numOfSeries = serieslist->size();
//	for (int it = 0; it < numOfSeries; it++)
//	{
//		int crntSeries = serieslist->at(it);
//
//		ASDICOMData* crntDataNode = (ASDICOMData*)m_DataTree->NewDataNode(EMDICOMData, nullptr);
//		crntDataNode->LoadDICOMDeries(dir, crntSeries);
//		emit signalAddImageDataDisplay(crntDataNode->getDataNodeID());
//	}
//	emit signalRefreshDataManagerWidget();
//	emit signalRefreshFileComboBox();		//刷新文件下拉选框
//}
//// 保存数据
//void ASDataManager::saveData(ASAbstractData* data)
//{
//	// 暂时只支持polydata
//	QFileDialog FileDialog;
//
//	QString setFilter = "OBJ(*.obj);;STL(*.stl);;PLY(*.ply)";
//	QString selectFilter, dirString;
//	QString crntFileName = data->getFileName();
//	if (crntFileName.isEmpty() == false)
//	{
//		dirString = QFileInfo(crntFileName).filePath();
//		if (QFileInfo(crntFileName).suffix() == "stl")
//		{
//			selectFilter = "STL(*." + QFileInfo(crntFileName).suffix() + ")";
//		}
//		else if (QFileInfo(crntFileName).suffix() == "obj")
//		{
//			selectFilter = "OBJ(*." + QFileInfo(crntFileName).suffix() + ")";
//		}
//		else if (QFileInfo(crntFileName).suffix() == "ply")
//		{
//			selectFilter = "PLY(*." + QFileInfo(crntFileName).suffix() + ")";
//		}
//	}
//	else
//	{
//		dirString = "./" + data->getName() + ".stl";
//		selectFilter = "PLY(*." + QFileInfo(crntFileName).suffix() + ")";
//	}
//	QString saveFileName = FileDialog.getSaveFileName(0, "save", dirString, setFilter, &selectFilter,
//		QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
//	if (data->getFileName() != saveFileName)
//	{
//		data->setFileName(saveFileName);
//	}
//	if (data->getName() != QFileInfo(saveFileName).baseName())
//	{
//		data->setName(QFileInfo(saveFileName).baseName());
//		emit signalRefreshDataManagerWidget();
//		emit signalRefreshFileComboBox();		//刷新文件下拉选框
//	}
//	data->SaveData(saveFileName);
//}
//// 移除数据
//void ASDataManager::removeData(ASAbstractData* data)
//{
//	if (data->isBeingProcess())
//	{
//		return;
//	}
//	if (data->isChildrenBeingProcess())
//	{
//		return;
//	}
//	//询问是否关闭文件
//	QMessageBox message(QMessageBox::Warning, "Information", "Really to remove data?", QMessageBox::Yes | QMessageBox::No, NULL);
//	if (message.exec() == QMessageBox::No)
//	{
//		return;
//	}
//
//	// 先记录数据ID和数据类型，并释放数据
//	int id = data->getDataNodeID();
//	enumDataType type = data->getDataType();
//	ASDataTree::RemoveNode(data);
//	// 后删除显示
//	signalRemoveDataDisplay(id, type);
//
//	// 刷新界面
//	emit signalRefreshDataManagerWidget();
//	emit signalRefreshFileComboBox();		//刷新文件下拉选框
//}
//// 新建标签文件，参数为名字，标签值和颜色
//void ASDataManager::NewLabelData(QString str, int label, int* nColor)
//{
//	ASSeedData* crntDataNode = (ASSeedData*)m_DataTree->NewDataNode(EMSeedData, nullptr);
//	crntDataNode->setBeingProcess(false);
//	crntDataNode->setName(str);
//	crntDataNode->setLabel(label);
//	crntDataNode->setColor(nColor[0], nColor[1], nColor[2]);
//	
//	emit signalAddSeeds2DDisplay(crntDataNode->getDataNodeID());
//	emit signalRefreshDataManagerWidget();
//	emit signalRefreshFileComboBox();		//刷新文件下拉选框
//}
//// 新建体数据
//ASArrayImageData* ASDataManager::NewArrayImageData(const int c_TemplateID, QString qname)
//{
//	ASArrayImageData* pTemplateData = (ASArrayImageData*)ASAbstractData::getDataNodeFromID(c_TemplateID);
//	ASArrayImageData* pNewData = (ASArrayImageData*)m_DataTree->NewDataNode(EMArrayImageData, pTemplateData);
//	pNewData->copyArrayImageDataFrom(c_TemplateID);
//	pNewData->setName(qname);
//	return pNewData;
//}

//----------------- Mask -----------------------
//
////得到储存遮罩数组的指针，只读
//unsigned char* ASDataManager::GetMaskArrayAddr()
//{
//	return m_pMask->GetMaskArrayAddr();
//}
////得到遮罩显示的vtkImageData
//vtkImageData* ASDataManager::GetMaskImageData()
//{
//	return m_pMask->GetMaskImageData();
//}
////设定遮罩图像的数据，包括数据相关属性和数据数组，需要输入参照图像的ID
//void ASDataManager::setMaskImageData(int ID)
//{
//	m_pMask->setMaskImageData(ID);
//}

//----------------- PolyDeformation -----------------------
//void ASDataManager::startPolyDeformation(double posx, double posy, double posz, enumPolyDeformaiton type, double ViewRange)
//{
//	m_PolyDeformation->startPolyDeformation(posx, posy, posz, type, ViewRange);
//}
//// 继续PolyDeformation操作
//void ASDataManager::PolyDeformationContinue(double posx, double posy, double posz, int viewlabel, enumPolyDeformaiton TransType)
//{
//	if (m_PolyDeformation->continuePolyDeformation(posx, posy, posz, viewlabel, TransType) == 0)
//	{
//		//刷新显示
//		emit signalPolyDeformationRefresh();
//	}
//}
////侧边栏输入旋转
//void ASDataManager::RotationChanging(double angle, int viewlabel)
//{
//	if (m_PolyDeformation->RotationDeformation(angle, viewlabel) == 0)
//	{
//		//刷新显示
//		emit signalPolyDeformationRefresh();
//	}
//}
////Zoom
//void ASDataManager::ZoomChanging(double multiple_x, double multiple_y, double multiple_z)
//{
//	m_PolyDeformation->setZoomMultiple(multiple_x, multiple_y, multiple_z);
//	if (m_PolyDeformation->ZoomDeformation() == 0)
//	{
//		//刷新显示
//		emit signalPolyDeformationRefresh();
//	}
//}
//void ASDataManager::ZoomChanging(double multiple)
//{
//	double* crntMultiple = m_PolyDeformation->getZoomMultiple();
//	for (int i = 0; i < 3; i++)
//	{
//		crntMultiple[i] *= multiple;
//	}
//	m_PolyDeformation->setZoomMultiple(crntMultiple[0], crntMultiple[1], crntMultiple[2]);
//	if (m_PolyDeformation->ZoomDeformation() == 0)
//	{
//		//刷新显示
//		emit signalPolyDeformationRefresh();
//	}
//}
//
////撤销
//void ASDataManager::PolyDeformationUndo()
//{
//	if (m_PolyDeformation->UndoDeformation() == 0)
//	{
//		emit signalPolyDeformationRefresh();
//	}
//}
////得到当前PolyDeformation操作的类型
//enumPolyDeformaiton ASDataManager::getCurrentPolyDeformationType()
//{
//	return m_PolyDeformation->getCurrentPolyDeformaitonType();
//}
////滚轮调整影响范围
//void ASDataManager::FFDWheel(int ForwardOrBackward)
//{
//	// 改变影响半径
//	if (m_PolyDeformation->FFDRadiusChange(ForwardOrBackward) == 0)
//	{
//		// 刷新显示
//		emit signalPolyDeformationRefresh();
//	}
//}

