#include "ASTransSidebarGeneral.h"
#include "ASFModelMask.h"
#include "ASVisualizationManager.h"
#include "ASTransAddins.h"
#include "ASOperation.h"
#include "ASTransContour.h"
#include "ASTransSeeds.h"
#include "ASTransROI.h"
#include "ASTransLandmark.h"

ASTransSidebarGeneral* ASTransSidebarGeneral::ms_SelfPointer = nullptr;
ASAbstractData* ASTransSidebarGeneral::ms_crntDataNode = nullptr;

ASTransSidebarGeneral::ASTransSidebarGeneral(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_SelfPointer = this;

	// 功能模块初始化
	m_FModelMask = new ASFModelMask();
}

ASTransSidebarGeneral::~ASTransSidebarGeneral()
{

}

// 唯一对象
ASTransSidebarGeneral* ASTransSidebarGeneral::GetSelfPointer()
{
	return ms_SelfPointer;
}

// 设置侧边栏下方widget
void ASTransSidebarGeneral::SetSidebarBelowWidget(const int c_WidgetType, const bool c_isShown)
{
	emit ms_SelfPointer->signalSetSideBarBelowWidget(c_WidgetType, c_isShown);
}
// 刷新侧边栏文件下拉选款
void ASTransSidebarGeneral::RefreshDataComboBox(const int c_AffectType)
{
	emit ms_SelfPointer->signalRefreshDataComboBox(c_AffectType);
}

// 0 当前选中的数据节点
void ASTransSidebarGeneral::SetCrntDataNode(ASAbstractData* datanode)
{
	ms_crntDataNode = datanode;
}
ASAbstractData* ASTransSidebarGeneral::GetCrntDataNode()
{
	return ms_crntDataNode;
}

// 1 ImageNavigator
// 设置当前焦点
void ASTransSidebarGeneral::SetDisplayPosition(const double c_x, const double c_y, const double c_z)
{
	emit ms_SelfPointer->signalSetDisplayPosition(c_x, c_y, c_z);
}
// 设置当前帧
void ASTransSidebarGeneral::SetDisplayFrame(const int c_Frame)
{
	emit ms_SelfPointer->signalSetDisplayFrame(c_Frame);
}
// 设置像素尺寸
void ASTransSidebarGeneral::SetImageSpacing(double* Spacing)
{
	emit ms_SelfPointer->signalSetImageSpacing(Spacing);
}
// 设置图像范围
void ASTransSidebarGeneral::SetDisplayExtent(double* Extent, const int c_NumOfFrames)
{
	emit ms_SelfPointer->signalSetDisplayExtent(Extent, c_NumOfFrames);
}
// 设置窗位窗宽
void ASTransSidebarGeneral::SetWindowLevel(double* WindowLevel)
{
	emit ms_SelfPointer->signalSetWindowLevel(WindowLevel[0], WindowLevel[1]);
}
// 2 Mask
// 设置遮罩使能状态
void ASTransSidebarGeneral::SetMaskModleEnabled(const bool c_Enable)
{
	// 设置模块状态
	ms_SelfPointer->m_FModelMask->SetMaskEnable(c_Enable);
	ASVisualizationManager::Render();
	emit ms_SelfPointer->signalMaskEnabledChanged();
}
// 设置参照图像ID
void ASTransSidebarGeneral::SetMaskWorkingImageID(const int c_ID)
{
	ms_SelfPointer->m_FModelMask->SetMaskImageID(c_ID);
}
// 设置遮罩参数
void ASTransSidebarGeneral::SetMaskParameter(double* Extent, double* ScaleRange)
{
	ms_SelfPointer->m_FModelMask->SetMaskParameter(Extent, ScaleRange);
	ASVisualizationManager::Render();
}
// 设置遮罩模式
void ASTransSidebarGeneral::SetMaskModel(const int c_model)
{
	ms_SelfPointer->m_FModelMask->SetMaskModel(c_model);
}
int ASTransSidebarGeneral::GetMaskModel()
{
	return ms_SelfPointer->m_FModelMask->GetMaskModel();
}
// 交互设置遮罩位置
void ASTransSidebarGeneral::SetMaskInteractorly(const double c_x, const double c_y, const double c_z)
{
	emit ms_SelfPointer->signalSetMaskInteractorly(c_x, c_y, c_z);
}
// 设置输入遮罩图像的ID
void ASTransSidebarGeneral::SetMaskMData(const int c_ID)
{
	ms_SelfPointer->m_FModelMask->SetMDataID(c_ID);
}

// 3 Addins
void ASTransSidebarGeneral::NewFromAddin(int c_Type, QString name, int label, QColor color)
{
	ASOperation* crntSeg = ASTransAddins::GetCrntOperation();
	switch (c_Type)
	{
	case DataType_ContourData:
		ASTransContour::NewContourData(crntSeg, name, label, color);
		break;
	case DataType_SeedData:
		ASTransSeeds::NewSeedsData(crntSeg, name, label, color);
		break;
	case DataType_ROI:
		ASTransROI::NewROIData(crntSeg, name, label, color);
		break;
	case DataType_LandmarkData:
		ASTransLandmark::NewLandmarkData(crntSeg);
		break;
	default:
		break;
	}
}

void ASTransSidebarGeneral::BtnRst()
{
	emit ms_SelfPointer->signalBtnRst();
}
