#pragma once

class ASVisualizationManager2D;
class ASVisualizationManager3D;
class ASPipelineManager;
class ASAbstractData;
class ASArrayImageData;
class ASColorMap;

#define Layer_MaskModel			1000
#define Layer_SeedsModel		1001

#define ZoomPara 50

class ASVisualizationManager
{
public:
	ASVisualizationManager();
	~ASVisualizationManager();

	// 获得当前显示参数
	static void GetDisplayPosition(double* position);
	static void GetDisplayExtent(double* extent);
	static void GetDisplaySpacing(double* spacing);
	static ASArrayImageData* GetToplayerData();
	static int GetDisplayFrame();

	// 渲染
	static void Render();
	// 根据当前各管线的数量更改辅助显示元素
	static void UpdateAuxiliaryPipeline();
	// 重置显示范围
	static void ResetDisplayExtent();
	// 重置判断顶层体数据
	static void ResetTopArrayImage();
	// 重置相机
	static void ResetCamera();
	// 设置焦点
	static void SetDisplayPostion(const double c_x, const double c_y, const double c_z);
	// 设置帧
	static void SetDisplayFrame(const int c_Frame);
	// 缩放指数
	static void RefreshZoomPara(const int c_View);
	static double GetZoomPara(const int c_View);

private:
	// 显示参数
	double m_ImagePosition[3];			// 焦点，世界坐标系
	double m_ImageExtent[6];			// 范围，世界坐标系
	int m_ImageFrame;					// 当前显示时刻
	int m_ImageNumOfFrame;				// 图像显示帧范围
	double m_ImageSpacing[3];			// 像素尺寸
	double m_ZoomPara[3];				// 缩放指数
	ASArrayImageData* m_TopLayerData = nullptr;	// 顶层体数据
	// 当前存在的对象数量，包括显示和没显示，不包括十字线等辅助显示。
	int m_NumberOfObject = 0;
	// 子模块
	ASVisualizationManager2D* m_VisualizationManager2D;
	ASVisualizationManager3D* m_VisualizationManager3D;
	ASPipelineManager* m_PipeLineManager;
	ASColorMap* m_ColorMap;

	// 唯一对象
	static ASVisualizationManager* ms_SelfPointer;
};
