#include "ASPaintingLandmarkWidget.h"
#include "ASTransLandmark.h"
#include "ASLandMarkItemWidget.h"
#include "ASDataStructureWalker.h"
#include "ASAbstractData.h"
#include "ASLandmarkData.h"
#include "ASInteractorStyleManager.h"
#include "ASTransInteractorStyle.h"
#include "ASTransGlobalKeyBoard.h"
#include "ASTransGeneralInteractor.h"
#include <QVBoxLayout>

ASPaintingLandmarkWidget::ASPaintingLandmarkWidget(QWidget *parent)
	: ASWidgetInSideBarBase(parent)
{
	this->UIInit();
	this->setStyleSheet("padding: 2px;");

	// 接收transmission信号
	connect(ASTransInteractorStyle::GetSelfPointer(), SIGNAL(signalInteractorStyle2DChanged(int)), 
		this, SLOT(slotInteractorStyle2DChanged(int)));
}

ASPaintingLandmarkWidget::~ASPaintingLandmarkWidget()
{

}

void ASPaintingLandmarkWidget::SetEnabled(const bool c_bEnabled)
{
	if (c_bEnabled == ASFModelLandmark::GetLandmarkModleEnabled())
	{
		return;
	}
	ASTransLandmark::SetModelEnabled(c_bEnabled);
}

// 界面初始化
void ASPaintingLandmarkWidget::UIInit()
{

}
// 软件当前交互方式改变
void ASPaintingLandmarkWidget::slotInteractorStyle2DChanged(int interactor)
{
	if (interactor == InterStyle2D_PaintingLandmarks)
	{
		this->SetEnabled(true);
	}
	else
	{
		this->SetEnabled(false);
	}
}