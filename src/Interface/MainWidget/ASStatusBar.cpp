#include "ASStatusBar.h"
#include "ASTransStatusBarGeneral.h"

ASStatusBar::ASStatusBar(QWidget *parent)
	: QStatusBar(parent)
{
	
	setFixedHeight(STATUSBAR_HEIGHT);
	this->setObjectName("Statusbar");
	m_PermanentWidget = new QLabel(this);
	m_Origin = new QLabel(this);
	m_PermanentWidget->setObjectName("Status");
	m_Origin->setObjectName("Status");
	this->insertWidget(0, m_Origin);
	this->addPermanentWidget(m_PermanentWidget);

	//在鼠标没有点击是跟踪鼠标移动，用于窗口边界缩放
	setMouseTracking(true);

	connect(ASTransStatusBarGeneral::GetSelfPointer(), 
		SIGNAL(signalStatusBarFocusInformationRefresh(double*, int*, double, int*, double*, double*, double*)),
		this, SLOT(slotStatusBarFocusInformationRefresh(double*, int*, double, int*, double*, double*, double*)));
}

ASStatusBar::~ASStatusBar()
{
}

//焦点改变更新状态点焦点信息显示
void ASStatusBar::slotStatusBarFocusInformationRefresh(double* Pos, int* Index, double PixValue, int* extent, double* space, double* origin, double* windowlevel)
{
	QString strDisplay;
	QString strOrigin;
	if (Pos == NULL && Index == NULL)
	{
		m_PermanentWidget->setText(tr(""));
		return;
	}
	strDisplay = "WindowLevel: (" + QString::number(QString::number(windowlevel[0], 'f', 2).toDouble())
		+ ", " + QString::number(QString::number(windowlevel[1], 'f', 2).toDouble())
		//// Extent
		//+ "> Extent: <" + QString::number(extent[0], 'f', 2) + ", " + QString::number(extent[1], 'f', 2) 
		//+ ", " + QString::number(extent[2], 'f', 2) + ", " + QString::number(extent[3], 'f', 2) 
		//+ ", " + QString::number(extent[4], 'f', 2) + ", " + QString::number(extent[5], 'f', 2)
		// Spacing
		+ ")   Spacing: (" + QString::number(QString::number(space[0], 'f', 2).toDouble())
		+ ", " + QString::number(QString::number(space[1], 'f', 2).toDouble()) 
		+ ", " + QString::number(QString::number(space[2], 'f', 2).toDouble())
		//// Origin
		//+ "> Origin: <" + QString::number(origin[0], 'f', 2) +
		//", " + QString::number(origin[1], 'f', 2) + ", " + QString::number(origin[2], 'f', 2)
		// Position
		+")   Position: (" + QString::number(QString::number(Pos[0], 'f', 2).toDouble())
		+ ", " + QString::number(QString::number(Pos[1], 'f', 2).toDouble()) 
		+ ", " + QString::number(QString::number(Pos[2], 'f', 2).toDouble())
		+ ") mm   Index: (" + QString::number(QString::number(Index[0]).toDouble()) 
		+ ", " + QString::number(QString::number(Index[1]).toDouble()) 
		+ ", " + QString::number(QString::number(Index[2]).toDouble()) 
		+ ")   Pixel value: " + QString::number(QString::number(PixValue, 'f', 5).toDouble()) + "  ";
	m_PermanentWidget->setText(strDisplay);
}