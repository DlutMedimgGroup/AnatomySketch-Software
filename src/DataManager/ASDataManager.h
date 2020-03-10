#pragma once

//#include "common.h"
//#include "ASSignalLinker.h"
#include <vtkDICOMDirectory.h>
#include <vtkActor.h>
#include <vtkImageData.h>
#include "QObject"
#include "QString"
#include "QList"

class ASDataTree;
class ASAbstractData;
class ASArrayImageData;
class ASPolyDeformation;
class ASMask;

class ASDataManager : public QObject
{
	Q_OBJECT

public:
	ASDataManager(QObject *parent = 0);
	~ASDataManager();
	
	//由父对象输入连接器的指针
	//void setSignalLinker(ASSignalLinker* linker);
	//参数初始化，软件初始化第三阶段
	//void Parameterinitialization();

	// 打开数据
	//void LoadData(QString filename);
	// 读取DICOM数据
	//void LoadDICOMSeries(vtkDICOMDirectory* dir, QList<int>* serieslist);
	// 保存数据
	//void saveData(ASAbstractData* data);
	// 移除数据
	//void removeData(ASAbstractData* data);
	// 新建标签文件，参数为名字，标签值和颜色
	//void NewLabelData(QString str, int label, int* nColor);
	// 新建体数据
	//ASArrayImageData* NewArrayImageData(const int c_TemplateID, QString qname);

	//----------------- Mask -----------------------

	//得到储存遮罩数组的指针，只读
	//unsigned char* GetMaskArrayAddr();
	//得到遮罩显示的vtkImageData
	//vtkImageData* GetMaskImageData();
	//设定遮罩图像的数据，包括数据相关属性和数据数组，需要输入参照图像的ID
	//void setMaskImageData(int ID);

	//----------------- PolyDeformation -----------------------

	//// 开始PolyDeformation操作
	//void startPolyDeformation(double posx, double posy, double posz, enumPolyDeformaiton type, double ViewRange);
	//// 继续PolyDeformation操作
	//void PolyDeformationContinue(double posx, double posy, double posz, int viewlabel, enumPolyDeformaiton TransType);
	////撤销
	//void PolyDeformationUndo();
	////得到当前PolyDeformation操作的类型
	//enumPolyDeformaiton getCurrentPolyDeformationType();
	//// 侧边栏旋转
	//void RotationChanging(double angle, int viewlabel);
	//// Zoom
	//void ZoomChanging(double multiple_x, double multiple_y, double multiple_z);
	//void ZoomChanging(double multiple);
	//// FFD滚轮调整影响范围
	//void FFDWheel(int ForwardOrBackward);

private:

	// 数据存储树形数据结构
	ASDataTree* m_DataTree;
	// 面数据编辑模块
	//ASPolyDeformation* m_PolyDeformation;
	// 遮罩模块
	//ASMask* m_pMask;
	// 软件中通用信号连接器
	//ASSignalLinker* m_SignalLinker;

signals:

	////---------------------------显示操作--------------------------------------
	////基本显示操作
	//void signalAddImageDataDisplay(int ID);					//新增体数据文件显示
	//void signalAddPolyDataDisplay(int ID);					//新增面数据文件显示
	//void signalAddDICOMRTDisplay();							//新增DICOM-RT文件显示，由于同时打开了多个RT文件，所以直接进行RT数据显示的刷新。
	//void signalAddSeeds2DDisplay(int ID);					//新增种子点显示，即新建相应标签文件的种子点渲染管线
	//void signalRemoveDataDisplay(int ID, enumDataType type);// 移除数据显示
	//void signalUpdataLayerDisplay();						//体数据层更新后刷新层级的显示

	////其他显示操作
	//void signalPolyDeformationRefresh();					//PolyDeformation刷新显示
	////向需要数据管理器指针的类发送地址，在软件初始化阶段，避免互相调用，使用空指针
	//void signalSetDatamanagerAddress(void* p);
	////刷新文件显示列表
	//void signalRefreshDataManagerWidget();
	////刷新所有文件下拉选框
	//void signalRefreshFileComboBox();
};
