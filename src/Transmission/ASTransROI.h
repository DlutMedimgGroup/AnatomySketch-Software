#pragma once
#include "ASTransmissionBase.h"
#include <QString>

class ASROIData;
class ASROITypeBase;
class ASFModelROI;
class ASAbstractData;

class ASTransROI : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransROI(QObject *parent = 0);
	~ASTransROI();

	// 唯一对象
	static ASTransROI* GetSelfPointer();
	//当前ROIID
	static void SetCrntROIID(const int c_id);
	static int GetCrntROIID();
	// 得到当前ROI文件
	static ASROIData* GetCurrentROIData();
	// 刷新轮廓线相关列表
	static void UpdateListsOfROIData();
	// 新建ROI数据
	static void NewROIData(ASAbstractData* pParent);
	static void NewROIData(ASAbstractData* pParent, QString name, int label, QColor color);
	// 读取ROI数据
	static void LoadROIDataFromDisk(const QStringList c_FileNameList);
	// 新建ROI
	static ASROITypeBase* NewROI(const int c_ROIId, const QString c_Name, const int c_Type);
	// 移除ROI
	static void RemoveROI(const int c_ROIDataId, const int c_ROIId);
	// 刷新ROI列表
	static void RefreshROIList();
	// 交互设置Cube范围
	static void SetROICubeRange(const int c_Index, const double c_Value);
	// 刷新ROICube显示
	static void RefreshROICubeDisplay();

private:

	// 唯一对象
	static ASTransROI* ms_SelfPointer;
	// 模块
	ASFModelROI* m_FModelROI;
	// 当前ROIID
	int m_CurrentROIID = -1;

	//void fNewROIData(QString name, int label, QColor color);

signals:

	// 刷新Cube Roi界面参数
	void signalRefreshROICube();
	// 刷新ROI列表
	void signalRefreshList();
};
