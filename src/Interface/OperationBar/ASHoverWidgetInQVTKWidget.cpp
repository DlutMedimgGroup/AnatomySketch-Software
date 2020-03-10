#include "ASHoverWidgetInQVTKWidget.h"
#include "GlobelDeclaration.h"
#include <QEvent>
#include <QPainter>
#include <QHBoxLayout>

ASHoverWidgetInQVTKWidget::ASHoverWidgetInQVTKWidget(QWidget *parent, int c_ViewLabel)
	: QWidget(parent)
{
	QHBoxLayout* mainlayout = new QHBoxLayout(this);
	mainlayout->addStretch();

	m_pButtonLayout = new QPushButton(this);
	m_pButtonLayout->setFixedSize(15, 15);
	m_pButtonLayout->setToolTip(tr("Layout"));
	m_pButtonLayout->setIcon(QIcon(":/Icon/LayoutMode31"));
	connect(m_pButtonLayout, SIGNAL(clicked()), this, SIGNAL(signalLayoutChanged()));

	if (c_ViewLabel == ViewLabel_3D)
	{
		m_pButtonCrossShair = new QPushButton(this);
		m_pButtonCrossShair->setFixedSize(15, 15);
		m_pButtonCrossShair->setToolTip(tr("CrossShair"));
		m_pButtonCrossShair->setIcon(QIcon(":/Icon/CrossShair"));
		connect(m_pButtonCrossShair, SIGNAL(clicked()), this, SIGNAL(signalSetCrossShairShown()));
		mainlayout->addWidget(m_pButtonCrossShair);
	}

	mainlayout->addWidget(m_pButtonLayout);
	mainlayout->setMargin(2);
	this->setLayout(mainlayout);
}

ASHoverWidgetInQVTKWidget::~ASHoverWidgetInQVTKWidget()
{

}

void ASHoverWidgetInQVTKWidget::setExpanded(const bool c_expanded)
{
	if (c_expanded)
	{
		m_pButtonLayout->setIcon(QIcon(":/Icon/LayoutMode22"));
	}
	else
	{
		m_pButtonLayout->setIcon(QIcon(":/Icon/LayoutMode31"));
	}
}

void ASHoverWidgetInQVTKWidget::paintEvent(QPaintEvent *event)
{
	QPainter p(this);
	p.setPen(Qt::NoPen);
	p.setBrush(QColor(0, 0, 0));
	p.drawRect(rect());
}

//void ASHoverWidgetInQVTKWidget::slotButtonLayoutClicked()
//{
//	emit signalLayoutChanged();
//}
