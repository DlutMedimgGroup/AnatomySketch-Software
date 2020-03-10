#include "ASVisualizationManager.h"
#include "ASPipelineManager.h"
#include "ASVisualizationManager2D.h"
#include "ASVisualizationManager3D.h"
#include "ASQVTKWidget.h"
#include "ASOperationBar.h"
#include "ASDataStructureWalker.h"
#include "ASAbstractData.h"
#include "ASColorMap.h"
#include "ASInteractorStyleManager.h"
#include "ASInteractorStyle2DGeneral.h"
#include "GlobelDeclaration.h"
#include "ASDataStructureWalker.h"
#include "ASArrayImageData.h"
#include "ASPipelineBase.h"
#include "ASPipelineCrossLine2D.h"
#include "ASPipelineOuterBorder3D.h"
#include "ASTransSidebarGeneral.h"
#include "ASTransSeeds.h"
#include "ASTransContour.h"
#include "ASTransLandmark.h"
#include "ASTransROI.h"
#include <vtkCoordinate.h>

ASVisualizationManager* ASVisualizationManager::ms_SelfPointer = nullptr;

ASVisualizationManager::ASVisualizationManager()
{
	ms_SelfPointer = this;

	m_PipeLineManager = new ASPipelineManager;
	m_ColorMap = new ASColorMap;
	m_VisualizationManager2D = new ASVisualizationManager2D;
	m_VisualizationManager3D = new ASVisualizationManager3D;
	ASQVTKWidget* pQVTKWiget[4];
	ASOperationBar::GetQVTKWidget(pQVTKWiget);
	m_VisualizationManager2D->SetQVTKWidget(pQVTKWiget);
	m_VisualizationManager2D->setImagePosition(m_ImagePosition);
	m_VisualizationManager3D->SetQVTKWidget(pQVTKWiget[3]);
	ASVisualizationManager::Render();
}

ASVisualizationManager::~ASVisualizationManager()
{

}

// 获得当前显示参数
void ASVisualizationManager::GetDisplayPosition(double* position)
{
	position[0] = ms_SelfPointer->m_ImagePosition[0];
	position[1] = ms_SelfPointer->m_ImagePosition[1];
	position[2] = ms_SelfPointer->m_ImagePosition[2];
}
void ASVisualizationManager::GetDisplayExtent(double* extent)
{
	extent[0] = ms_SelfPointer->m_ImageExtent[0];
	extent[1] = ms_SelfPointer->m_ImageExtent[1];
	extent[2] = ms_SelfPointer->m_ImageExtent[2];
	extent[3] = ms_SelfPointer->m_ImageExtent[3];
	extent[4] = ms_SelfPointer->m_ImageExtent[4];
	extent[5] = ms_SelfPointer->m_ImageExtent[5];
}
void ASVisualizationManager::GetDisplaySpacing(double* spacing)
{
	spacing[0] = ms_SelfPointer->m_ImageSpacing[0];
	spacing[1] = ms_SelfPointer->m_ImageSpacing[1];
	spacing[2] = ms_SelfPointer->m_ImageSpacing[2];
}
ASArrayImageData* ASVisualizationManager::GetToplayerData()
{
	return ms_SelfPointer->m_TopLayerData;
}
int ASVisualizationManager::GetDisplayFrame()
{
	return ms_SelfPointer->m_ImageFrame;
}

// 渲染
void ASVisualizationManager::Render()
{
	ASVisualizationManager2D::Render();
	ASVisualizationManager3D::Render();
}
// 根据当前各管线的数量更改辅助显示元素
void ASVisualizationManager::UpdateAuxiliaryPipeline()
{
	if (ASPipelineBase::GetNumOfShowingObj2D() > 0)
	{
		// 有2d内容显示
		ASPipelineCrossLine2D::setShown(true);
		ASPipelineOuterBorder3D::setShown(true);
		ASTransSeeds::SetSeedShown(true);
		ASTransContour::SetContourShown(true);
		ASTransLandmark::SetLandmarksShown(true);
	} 
	else
	{
		// 无2d内容显示
		//ASPipelineCrossLine2D::setShown(false);
		//ASPipelineOuterBorder3D::setShown(false);
		ASTransSeeds::SetSeedShown(false);
		ASTransContour::SetContourShown(false);
		ASTransLandmark::SetLandmarksShown(false);
	}
	ASTransROI::RefreshROICubeDisplay();
}
// 重置显示范围
void ASVisualizationManager::ResetDisplayExtent()
{
	bool firstNode = true;
	ms_SelfPointer->m_ImageFrame = 0;
	ms_SelfPointer->m_ImageNumOfFrame = 1;
	ms_SelfPointer->m_ImageExtent[0] = 0;
	ms_SelfPointer->m_ImageExtent[1] = 0;
	ms_SelfPointer->m_ImageExtent[2] = 0;
	ms_SelfPointer->m_ImageExtent[3] = 0;
	ms_SelfPointer->m_ImageExtent[4] = 0;
	ms_SelfPointer->m_ImageExtent[5] = 0;
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_AbstractData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_AbstractData))
	{
		if (item->isVoidNode())
		{
			continue;
		}
		double* crntExtent = item->getDataExtent();
		if (crntExtent != nullptr)
		{
			if (firstNode)
			{
				ms_SelfPointer->m_ImageExtent[0] = crntExtent[0];
				ms_SelfPointer->m_ImageExtent[1] = crntExtent[1];
				ms_SelfPointer->m_ImageExtent[2] = crntExtent[2];
				ms_SelfPointer->m_ImageExtent[3] = crntExtent[3];
				ms_SelfPointer->m_ImageExtent[4] = crntExtent[4];
				ms_SelfPointer->m_ImageExtent[5] = crntExtent[5];
				firstNode = false;
			}
			else
			{
				for (int i = 0; i < 6; i += 2)
				{
					if (ms_SelfPointer->m_ImageExtent[i] > crntExtent[i])
					{
						ms_SelfPointer->m_ImageExtent[i] = crntExtent[i];
					}
				}
				for (int i = 1; i < 6; i += 2)
				{
					if (ms_SelfPointer->m_ImageExtent[i] < crntExtent[i])
					{
						ms_SelfPointer->m_ImageExtent[i] = crntExtent[i];
					}
				}
			}
		}
		if (item->getNumberOfFrame() > ms_SelfPointer->m_ImageNumOfFrame)
		{
			ms_SelfPointer->m_ImageNumOfFrame = item->getNumberOfFrame();
		}
	}
	ASTransSidebarGeneral::SetDisplayExtent(ms_SelfPointer->m_ImageExtent, ms_SelfPointer->m_ImageNumOfFrame);
}
// 重置判断顶层体数据
void ASVisualizationManager::ResetTopArrayImage()
{
	// 0 寻找显示的顶层体数据
	int crntTopLayer = -1;
	ms_SelfPointer->m_TopLayerData = nullptr;
	// 遍历数据寻找顶层图像
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ArrayImageData); item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_ArrayImageData))
	{
		ASArrayImageData* crntDataNode = (ASArrayImageData*)item;
		if (crntDataNode->isShown() && crntDataNode->isVoidNode() == false && crntDataNode->getLayer() > crntTopLayer)
		{
			crntTopLayer = crntDataNode->getLayer();
			ms_SelfPointer->m_TopLayerData = crntDataNode;
		}
	}
	// 1 根据顶层数据重新设定Spacing
	if (ms_SelfPointer->m_TopLayerData == nullptr)
	{
		ms_SelfPointer->m_ImageSpacing[0] = 1;
		ms_SelfPointer->m_ImageSpacing[1] = 1;
		ms_SelfPointer->m_ImageSpacing[2] = 1;
	} 
	else
	{
		ms_SelfPointer->m_TopLayerData->getArrayImageData()->GetSpacing(ms_SelfPointer->m_ImageSpacing);
	}
	// 2 设置侧边栏
	ASTransSidebarGeneral::SetImageSpacing(ms_SelfPointer->m_ImageSpacing);	
	if (ms_SelfPointer->m_TopLayerData == nullptr)
	{
		double WindowLevel[2];
		WindowLevel[0] = -1;
		WindowLevel[1] = 0;
		ASTransSidebarGeneral::SetWindowLevel(WindowLevel);
	}
	else
	{
		ASTransSidebarGeneral::SetWindowLevel(ms_SelfPointer->m_TopLayerData->getWindowLevel());
	}
	// 3 刷新种子点显示
	ASTransSeeds::RefreshSeedsDisplay();
}
// 重置相机
void ASVisualizationManager::ResetCamera()
{
	ms_SelfPointer->m_VisualizationManager2D->ResetCamera();
	ms_SelfPointer->m_VisualizationManager3D->ResetCamera();
}
// 设置焦点
void ASVisualizationManager::SetDisplayPostion(const double c_x, const double c_y, const double c_z)
{
	// 0 判断焦点是否超限
	double NewPos[3];
	NewPos[ViewLabel_Sagittal] = c_x;
	NewPos[ViewLabel_Coronal] = c_y;
	NewPos[ViewLabel_Axial] = c_z;
	for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		if (NewPos[ViewLabel] < ms_SelfPointer->m_ImageExtent[ViewLabel * 2])
		{
			NewPos[ViewLabel] = ms_SelfPointer->m_ImageExtent[ViewLabel * 2];
		}
		if (NewPos[ViewLabel] > ms_SelfPointer->m_ImageExtent[ViewLabel * 2 + 1])
		{
			NewPos[ViewLabel] = ms_SelfPointer->m_ImageExtent[ViewLabel * 2 + 1];
		}
	}
	// 1 更新焦点
	ms_SelfPointer->m_ImagePosition[ViewLabel_Sagittal] = NewPos[ViewLabel_Sagittal];
	ms_SelfPointer->m_ImagePosition[ViewLabel_Coronal] = NewPos[ViewLabel_Coronal];
	ms_SelfPointer->m_ImagePosition[ViewLabel_Axial] = NewPos[ViewLabel_Axial];
	// 2 更新变换矩阵
	ms_SelfPointer->m_VisualizationManager2D->UpdateTransformMatrix();
	ms_SelfPointer->m_VisualizationManager3D->UpdateImageStack3DTransform();
	// 3 更新管线
	ASPipelineManager::UpdatePipeline2DTransformMatrix();
}
// 设置帧
void ASVisualizationManager::SetDisplayFrame(const int c_Frame)
{
	if (c_Frame < ms_SelfPointer->m_ImageNumOfFrame)
	{
		ms_SelfPointer->m_ImageFrame = c_Frame;
	}
	else
	{
		ms_SelfPointer->m_ImageFrame = ms_SelfPointer->m_ImageNumOfFrame;
	}
}
// 缩放指数
void ASVisualizationManager::RefreshZoomPara(const int c_View)
{
	vtkSmartPointer<vtkCoordinate> Coordinate = vtkSmartPointer<vtkCoordinate>::New();
	Coordinate->SetCoordinateSystemToDisplay();
	Coordinate->SetValue(ZoomPara, 0, 0);
	vtkSmartPointer<vtkRenderer> render2D[3];
	ms_SelfPointer->m_VisualizationManager2D->GetRenderer2D(render2D);
	double radius1 = Coordinate->GetComputedWorldValue(render2D[c_View])[0];
	Coordinate->SetValue(0, 0, 0);
	double radius2 = Coordinate->GetComputedWorldValue(render2D[c_View])[0];
	ms_SelfPointer->m_ZoomPara[c_View] = abs(radius2 - radius1);
}
double ASVisualizationManager::GetZoomPara(const int c_View)
{
	return ms_SelfPointer->m_ZoomPara[c_View];
}