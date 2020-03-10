#pragma once
#include "ASTransmissionBase.h"
#include <QProgressDialog>

class ASAbstractData;
class ASArrayImageData;
class ASFModelGraphCut;

class ASTransGraphCut : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransGraphCut(QObject *parent = 0);
	~ASTransGraphCut();

	// 唯一对象
	static ASTransGraphCut* GetSelfPointer();

	// Marching cubs
	static void SetMarchingCubs(const bool c_Marching, const int c_SmoothIterations);
	// 调用Graph Cut功能
	static void GraphCutGenerate(const int c_ImageDataID, const QList<int> SeedsID, 
		const QString c_OutputName, const double c_Lambda, const double c_Delta, 
		const int c_nNeighbor, const QString c_PostProcessing, const bool c_bQuery);
	// 计算完成
	static void GraphCutsFinished(ASArrayImageData* DataNode);

private:
	// 唯一对象
	static ASTransGraphCut* ms_Selfpointer;
	// 读数据进度对话框
	QProgressDialog* m_ProgressDialog = nullptr;
	// 运算模块
	ASFModelGraphCut* m_FModelGraphCut;
	// Marching Cubs
	bool m_NeedMarchingCubs = false;
	int m_SmoothIterations = 0;

	QList<int> m_SeedsID;

signals:


};
