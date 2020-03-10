#include <QLineEdit>
#include <QDoubleSpinBox>
#include <QApplication>
#include "ASDiplayFrame.h"
#include "ASHoverWidgetInQVTKWidget.h"
#include "ASQVTKWidget.h"
#include "GlobelDeclaration.h"
#include "ASTransContour.h"
#include "ASContourData.h"
#include "ASPipelineBase.h"
#include "ASTransGeneralInteractor.h"
#include <QApplication>
#include <QLineEdit>

ASDiplayFrame::ASDiplayFrame(QWidget *parent)
	: QFrame(parent)
{
	//在鼠标没有点击是跟踪鼠标移动，用于窗口边界缩放
	setMouseTracking(true);
	
	m_QVTKWidget = new ASQVTKWidget(this);
	m_HBoxLayout = new QHBoxLayout();

	m_HBoxLayout->addWidget(m_QVTKWidget);
	m_HBoxLayout->setContentsMargins(0, 0, 0, 0);

	this->setLayout(m_HBoxLayout);
}

ASDiplayFrame::~ASDiplayFrame()
{
}

void ASDiplayFrame::setViewSelection(int viewselection)
{
	m_ViewSelection = viewselection;
	switch (viewselection)
	{
	case ViewLabel_Sagittal:
		this->setStyleSheet("QFrame{  border: 4px solid rgb(0,194,15);   }");
		break;
	case ViewLabel_Coronal:
		this->setStyleSheet("QFrame{  border: 4px solid rgb(0,23,188);   }");
		break;
	case ViewLabel_Axial:
		this->setStyleSheet("QFrame{  border: 4px solid rgb(183,33,0);   }");
		break;
	case ViewLabel_3D:
		this->setStyleSheet("QFrame{  border: 4px solid rgb(113,116,0);   }");
		break;
	default:
		break;
	}
	m_QVTKWidget->setViewSelection(viewselection);
	
	m_HoverWidget = new ASHoverWidgetInQVTKWidget(this, viewselection);
	connect(m_HoverWidget, SIGNAL(signalLayoutChanged()), this, SLOT(slotButtonLayoutClicked()));
	connect(m_HoverWidget, SIGNAL(signalSetCrossShairShown()), this, SLOT(slotButtonCrossShairClicked()));
	m_HoverWidget->hide();
}

ASQVTKWidget* ASDiplayFrame::getASQVTKWidget()
{
	return m_QVTKWidget;
}

void ASDiplayFrame::setExpanded(const bool c_expanded)
{
	if (m_Expanded == c_expanded)
	{
		return;
	} 
	else
	{
		m_Expanded = c_expanded;
		m_HoverWidget->setExpanded(m_Expanded);
	}
}

void ASDiplayFrame::resizeEvent(QResizeEvent *event)
{
	int szx = this->width();
	if (m_ViewSelection == ViewLabel_3D)
	{
		m_HoverWidget->setGeometry(szx - 50, 10, 40, 20);
	} 
	else
	{
		m_HoverWidget->setGeometry(szx - 30, 10, 20, 20);
		if (ASPipelineBase::GetNumOfShowingObj2D() > 0)
		{
			ASTransGeneralInteractor::GenerateViewZoom(this->m_ViewSelection);
		}
	}
}

void ASDiplayFrame::enterEvent(QEvent *event)
{
	m_HoverWidget->show();
	QWidget *current_focus_widget = QApplication::focusWidget();
	QLineEdit *lineEditFocused = qobject_cast<QLineEdit*>(current_focus_widget);
	QSpinBox *spinBoxFocused = qobject_cast<QSpinBox*>(current_focus_widget);
	QDoubleSpinBox *doubleSpinBoxFocused = qobject_cast<QDoubleSpinBox*>(current_focus_widget);
	if(!lineEditFocused && !spinBoxFocused && !doubleSpinBoxFocused)
	{
		m_QVTKWidget->setFocus();
	}
}
void ASDiplayFrame::leaveEvent(QEvent *event)
{
	m_HoverWidget->hide();
}

void ASDiplayFrame::slotButtonLayoutClicked()
{
	emit signalButtonLayoutClicked(m_ViewSelection);
}
void ASDiplayFrame::slotButtonCrossShairClicked()
{
	emit signalButtonCrossShairClicked(m_ViewSelection);
}