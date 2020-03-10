#include "ASQVTKWidget.h"
#include "ASTransInteractorStyle.h"
#include <vtkRenderWindow.h>
#include <QTouchEvent>
#include <QLineF>

ASQVTKWidget::ASQVTKWidget(QWidget *parent)
	: QVTKWidget(parent)
{
	//在鼠标没有点击是跟踪鼠标移动，用于窗口边界缩放
	setMouseTracking(true);
	this->GetRenderWindow()->GlobalWarningDisplayOff();
	this->setAttribute(Qt::WA_AcceptTouchEvents, true);
	this->setAttribute(Qt::WA_TouchPadAcceptSingleTouchEvents, true);
	this->setAttribute(Qt::WA_WState_AcceptedTouchBeginEvent, true);
}

ASQVTKWidget::~ASQVTKWidget()
{
}

void ASQVTKWidget::setViewSelection(int viewselection)
{
	m_ViewSelection = viewselection;
}

bool ASQVTKWidget::event(QEvent *pEvent)
{
	static bool touching = false;
	switch (pEvent->type())
	{
	case QEvent::TouchBegin:
	case QEvent::TouchUpdate:
	case QEvent::TouchEnd:
	{
		QTouchEvent* pTouchEvent = static_cast<QTouchEvent*>(pEvent);
		QList<QTouchEvent::TouchPoint> qlistTouchPoints = pTouchEvent->touchPoints();
		if (qlistTouchPoints.count() >= 2)
		{
			touching = true;
		}
		else
		{
			touching = false;
		}
		if (qlistTouchPoints.count() == 2)
		{
			const QTouchEvent::TouchPoint &pTouchPoint0 = qlistTouchPoints.first();
			const QTouchEvent::TouchPoint &pTouchPoint1 = qlistTouchPoints.last();
			// 平移
			QPointF a = QLineF(pTouchPoint0.pos(), pTouchPoint1.pos()).center();
			QPointF b = QLineF(pTouchPoint0.lastPos(), pTouchPoint1.lastPos()).center();
			double x = a.x() - b.x();
			double y = a.y() - b.y();
			if (a.x() - b.x() != 0 || a.x() - b.x() != 0)
			{
				double pos[4];
				pos[0] = a.x();
				pos[1] = a.y();
				pos[2] = b.x();
				pos[3] = b.y();
				ASTransInteractorStyle::TouchPan(m_ViewSelection, pos);
			}
			// 缩放
			double factor = QLineF(pTouchPoint0.pos(), pTouchPoint1.pos()).length() /
				QLineF(pTouchPoint0.lastPos(), pTouchPoint1.lastPos()).length();
			if (factor != 1)
			{
				ASTransInteractorStyle::TouchZoom(m_ViewSelection, factor);
			}
		}
		else if (qlistTouchPoints.count() == 3)
		{
			const QTouchEvent::TouchPoint &pTouchPoint = qlistTouchPoints.first();
			double dis = pTouchPoint.pos().y() - pTouchPoint.lastPos().y();
			if (dis != 0)
			{
				ASTransInteractorStyle::TouchWheel(m_ViewSelection, dis);
			}			
		}
		break;
	}
	case QEvent::MouseButtonPress:
	case QEvent::MouseMove:
	case QEvent::MouseButtonRelease:
	{
		if (touching)
		{
			pEvent->accept();
			return false;
		}
	}
	default:
		break;
	}
	return QVTKWidget::event(pEvent);
}
