#include "ASWidgetInSideBarBase.h"

ASWidgetInSideBarBase::ASWidgetInSideBarBase(QWidget *parent)
	: QWidget(parent)
{
	//在鼠标没有点击是跟踪鼠标移动，用于窗口边界缩放
	setMouseTracking(true);
}

ASWidgetInSideBarBase::~ASWidgetInSideBarBase()
{
}

void ASWidgetInSideBarBase::SetEnabled(const bool c_bEnabled)
{

}