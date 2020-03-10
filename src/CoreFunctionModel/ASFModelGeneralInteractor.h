#pragma once
#include "ASFunctionModelBase.h"
#include <vtkActor.h>
#include <QTimer>

class ASFModelGeneralInteractor : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelGeneralInteractor(QObject *parent = 0);
	~ASFModelGeneralInteractor();

	// 悬停信息
	void HoverActorPicked(vtkActor* actor, const int c_x, const int c_y, const int c_ViewLabel);
	void AddHoverInfor(vtkActor* actor, QString infor);
	void DeleteHoverInfor(vtkActor* actor);

private:
	// 唯一对象
	static ASFModelGeneralInteractor* ms_SelfPointer;

	int m_HoverFlag = 0;
	QList<vtkActor*>* m_HoverActorList;
	QList<QString>* m_HoverInforList;
	vtkActor* m_HoveringActor = nullptr;
	QTimer* m_HoverTimer;
	int m_HoveringPos[2];
	int m_HoveringViewlabel;
	int m_HoveringTextID = -1;

private slots:
	
	void slotHoverTimeOut();
};
