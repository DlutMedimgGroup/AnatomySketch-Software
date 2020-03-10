#pragma once
#include <vtkSmartPointer.h>
#include <vtkBalloonWidget.h>

class ASQVTKWidget;
class ASInteractorStyle2DGeneral;
class ASInteractorStyle2DWindowLevel;
class ASInteractorStyle2DPaintSeeds;
class ASInteractorStyle2DPaintContour;
class ASInteractorStyle2DPolyDeformation;
class ASInteractorStyle2DMask;
class ASInteractorStyle2DPaintLandmark;
class ASInteractorStyle2DROICube;

class ASInteractorStyle3DGeneral;

// 交互方式
#define InterStyle2D_General			0
#define InterStyle2D_WindowLevel		1
#define InterStyle2D_PaintingSeeds		2
#define InterStyle2D_PaintingContour	3
#define InterStyle2D_PolyDeformation	4
#define InterStyle2D_Mask				5
#define InterStyle2D_PaintingLandmarks	6
#define InterStyle2D_ROICube			7

#define InterStyle3D_General			0

class ASInteractorStyleManager
{
public:
	ASInteractorStyleManager();
	~ASInteractorStyleManager();

	// 得到当前二维交互模式
	static int GetInteractorStyle2D();
	// 设置二维交互模式
	static void setInteractorStyle2D(const int c_style);
	// 得到当前二维交互模式
	static ASInteractorStyle2DGeneral* GetInteractorStyle2D(const int c_ViewLabel);
	// 得到当前三维交互模式
	static int GetInteractorStyle3DType();
	static ASInteractorStyle3DGeneral* GetInteractorStyle3D();
	// 设置三维交互模式
	static void setInteractorStyle3D(const int c_style);


private:
	// 唯一对象
	static ASInteractorStyleManager* ms_SelfPointer;
	// ASQVTKWidget
	ASQVTKWidget* m_pQVTKWiget[4];
	// 交互方式 2D:
	vtkSmartPointer<ASInteractorStyle2DGeneral> m_Style2DGeneral[3];
	vtkSmartPointer<ASInteractorStyle2DWindowLevel> m_Style2DWindowLevel[3];
	vtkSmartPointer<ASInteractorStyle2DPaintSeeds> m_Style2DPaintSeeds[3];
	vtkSmartPointer<ASInteractorStyle2DPaintContour> m_Style2DPaintContour[3];
	vtkSmartPointer<ASInteractorStyle2DPolyDeformation> m_Style2DPolyDeformation[3];
	vtkSmartPointer<ASInteractorStyle2DMask> m_Style2DMask[3];
	vtkSmartPointer<ASInteractorStyle2DPaintLandmark> m_Style2DLandmark[3];
	vtkSmartPointer<ASInteractorStyle2DROICube> m_Style2DROICube[3];

	// 交互方式 3D
	vtkSmartPointer<ASInteractorStyle3DGeneral> m_Style3DGeneral;

	// 当前交互方式
	int m_InteractorStyle2D = -1;
	int m_InteractorStyle3D = -1;

};
