#pragma once
#include "ASTransmissionBase.h"
#include <vtkActor.h>

class ASPolyImageData;
class ASFModelPolyDeformation;

class ASTransPolyDeformation : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransPolyDeformation(QObject *parent = 0);
	~ASTransPolyDeformation();

	// 唯一对象
	static ASTransPolyDeformation* GetSelfPointer();
	// 数据是否被选中
	static void ResetPolyDataChoosen(QList<int> listChoosenID);
	static void SetPolyDataChoosen(const int c_DataNodeID, const bool c_Choosen);
	static bool ConourPicked(vtkActor* ActorPicked); // 通过交互拾取到了轮廓线
	// 使能
	static void SetEnabled(const bool c_enable);
	static bool GetEnabled();

	// 变形;)
	static void PolyDefStart(double* Pos, const int c_ViewLabel, const int c_TransType);
	static void PolyDefContinue(double* Pos, const int c_ViewLabel, const int c_TransType);
	static void PolyDefZoom(int InOrOut); 	//滚轮缩放。 0: zoom in 1: zoom out
	static void PolyDefRotate(int ClockwiseOrCounter, int viewlabel); //滚轮旋转
	static void PolyDefWheelFFD(int ForwardOrBackward); 	//PolyDeformation, FFD。 0:Forward 1:Backward
	static void PolyDefUndo();

private:

	// PolyDeformation模块
	ASFModelPolyDeformation* m_FModelPoly;
	// 唯一对象
	static ASTransPolyDeformation* ms_SelfPointer;
	static bool ms_Enabled;

signals:

	// 数据选择状态改变
	void signalChoosnChanged(int ID);

};
