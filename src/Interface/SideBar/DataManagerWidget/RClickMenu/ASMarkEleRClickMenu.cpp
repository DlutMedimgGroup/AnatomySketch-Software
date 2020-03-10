#include "ASMarkEleRClickMenu.h"
#include "ASAbstractData.h"
#include "ASTransContour.h"
#include "ASTransSeeds.h"
#include "ASTransROI.h"
#include "ASTransLandmark.h"
#include "ASROIData.h"
#include "ASROITypeBase.h"
#include "ASROITypeCube.h"
#include "ASLandmarkData.h"
#include "ASTransGeneralInteractor.h"
#include <QMenu>

ASMarkEleRClickMenu::ASMarkEleRClickMenu(QObject *parent)
	: QObject(parent)
{
	this->RightClickInit();
}

ASMarkEleRClickMenu::~ASMarkEleRClickMenu()
{

}

void ASMarkEleRClickMenu::Generate(const QPoint c_pos, ASAbstractData* DataNode, const int c_MarkEleID)
{
	QMenu* crntMenu = new QMenu();
	if (DataNode->isTypeInherited(DataType_ContourData))	// 轮廓线
	{
		// 添加菜单
		crntMenu->addAction(m_RARemove);
		crntMenu->addAction(m_RANewContour);
	}
	else if (DataNode->isTypeInherited(DataType_SeedData))		// 种子点
	{
		// 添加菜单
		crntMenu->addAction(m_RARemove);
		crntMenu->addAction(m_RANewSeeds);
	}
	else if (DataNode->isTypeInherited(DataType_ROI))
	{
		// 添加菜单
		crntMenu->addAction(m_RARemove);
		crntMenu->addAction(m_RAFocusOn);
		crntMenu->addAction(m_RANewROI);
	}
	else if (DataNode->isTypeInherited(DataType_LandmarkData))	// 标定点
	{
		// 添加菜单
		crntMenu->addAction(m_RARemove);
		crntMenu->addAction(m_RAFocusOn);
	}
	// 弹出右键菜单
	ASDataManagerAction* selectedItem = (ASDataManagerAction*)crntMenu->exec(c_pos);
	delete crntMenu;
	if (selectedItem)
	{
		enumActionInContextMenu action = selectedItem->getAction();
		this->RightClickMenuAction(DataNode, action, c_MarkEleID);
	}
}

// 右键菜单初始化
void ASMarkEleRClickMenu::RightClickInit()
{
	m_RARemove = new ASDataManagerAction(tr("Remove"), this, EMActRemove);
	m_RAFocusOn = new ASDataManagerAction(tr("Focus On"), this, EMActFocusOn);
	// 新建数据
	m_RANewContour = new ASDataManagerAction(tr("New Contour"), this, EMActNewContour);
	m_RANewSeeds = new ASDataManagerAction(tr("New Seeds"), this, EMActNewSeed);
	m_RANewROI = new ASDataManagerAction(tr("New Bounding Box"), this, EMActNewROI);
}
// 右键菜单动作
void ASMarkEleRClickMenu::RightClickMenuAction(ASAbstractData* DataNode, enumActionInContextMenu action, 
	const int c_MarkEleID)
{
	if (DataNode->isTypeInherited(DataType_ContourData))	// 轮廓线
	{
		if (action == EMActRemove)
		{
			ASTransContour::RemoveContour(DataNode->getDataNodeID(), c_MarkEleID);
		}
		else if (action == EMActNewContour)
		{
			ASTransContour::NewContourData(DataNode);
		}
	}
	else if (DataNode->isTypeInherited(DataType_SeedData))
	{
		if (action == EMActRemove)
		{
			ASTransSeeds::RemoveSeeds(DataNode->getDataNodeID(), c_MarkEleID);
		}
		else if (action == EMActNewSeed)
		{
			ASTransSeeds::NewSeedsData(DataNode);
		}
	}
	else if (DataNode->isTypeInherited(DataType_ROI))
	{
		if (action == EMActRemove)
		{
			ASTransROI::RemoveROI(DataNode->getDataNodeID(), c_MarkEleID);
		}
		else if (action == EMActFocusOn)
		{
			ASROIData* crntData = static_cast<ASROIData*>(DataNode);
			ASROITypeBase* roib =  crntData->GetROIWithID(c_MarkEleID);
			if (roib->isTypeAbsolute(ROIType_Cube))
			{
				ASROITypeCube* cube = static_cast<ASROITypeCube*>(roib);
				double center[3];
				cube->GetCenter(center);
				ASTransGeneralInteractor::SetVisualizationPosition(center[0], center[1], center[2]);
			}
		}
		else if (action == EMActNewROI)
		{
			ASTransROI::NewROIData(DataNode);
		}
	}
	else if (DataNode->isTypeInherited(DataType_LandmarkData))
	{
		if (action == EMActRemove)
		{
			ASTransLandmark::RemoveLandMark(DataNode->getDataNodeID(), c_MarkEleID);
		}
		else if (action == EMActFocusOn)
		{
			ASLandmarkData* crntData = static_cast<ASLandmarkData*>(DataNode);
			structLandmark* crntLandmark = crntData->GetLandmarkWithID(c_MarkEleID);
			if (!crntLandmark)
			{
				return;
			}
			ASTransGeneralInteractor::SetVisualizationPosition(crntLandmark->dPosition[0],
				crntLandmark->dPosition[1], crntLandmark->dPosition[2]);
		}
	}
}