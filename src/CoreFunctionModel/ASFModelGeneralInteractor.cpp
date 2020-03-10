#include "ASFModelGeneralInteractor.h"
#include "ASTransGeneralInteractor.h"

ASFModelGeneralInteractor* ASFModelGeneralInteractor::ms_SelfPointer = nullptr;

ASFModelGeneralInteractor::ASFModelGeneralInteractor(QObject *parent)
	: ASFunctionModelBase(parent)
{
	ms_SelfPointer = this;

	m_HoverActorList = new QList<vtkActor*>;
	m_HoverInforList = new QList<QString>;
	m_HoverTimer = new QTimer();
	connect(m_HoverTimer, SIGNAL(timeout()), this, SLOT(slotHoverTimeOut()));
}

ASFModelGeneralInteractor::~ASFModelGeneralInteractor()
{

}

// 悬停信息
void ASFModelGeneralInteractor::HoverActorPicked(vtkActor* actor, const int c_x, const int c_y, const int c_ViewLabel)
{
	m_HoverFlag = 0;
	m_HoveringActor = nullptr;
	m_HoverTimer->stop();
	if (m_HoveringTextID >= 0)
	{
		ASTransGeneralInteractor::DeleteHoverText(m_HoveringTextID, m_HoveringViewlabel);
		m_HoveringTextID = -1;
	}
	if (m_HoverActorList->indexOf(actor) >= 0)
	{
		m_HoverFlag = 1;
		m_HoveringActor = actor;
		m_HoveringPos[0] = c_x;
		m_HoveringPos[1] = c_y;
		m_HoveringViewlabel = c_ViewLabel;
		m_HoverTimer->start(500);
	}
}
void ASFModelGeneralInteractor::AddHoverInfor(vtkActor* actor, QString infor)
{
	m_HoverActorList->append(actor);
	m_HoverInforList->append(infor);
}
void ASFModelGeneralInteractor::DeleteHoverInfor(vtkActor* actor)
{
	int i = m_HoverActorList->indexOf(actor);
	if (i >= 0)
	{
		m_HoverActorList->removeAt(i);
		m_HoverInforList->removeAt(i);
	}
}

void ASFModelGeneralInteractor::slotHoverTimeOut()
{
	m_HoverTimer->stop();
	int n = m_HoverActorList->indexOf(m_HoveringActor);
	double pos1[2];
	double pos2[2];
	pos1[0] = m_HoveringPos[0] + 3;
	pos1[1] = m_HoveringPos[1] - 10;
	pos1[0] = m_HoveringPos[0] + 13;
	pos1[1] = m_HoveringPos[1] + 10;
	if (n >= 0)
	{
		// 添加文字显示
		m_HoveringTextID = ASTransGeneralInteractor::ShowHoverText(m_HoveringViewlabel, m_HoverInforList->at(n), pos1, pos2);
	}

}