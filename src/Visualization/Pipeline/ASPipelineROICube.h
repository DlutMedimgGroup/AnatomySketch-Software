#pragma once
#include "ASPipelineBase.h"
#include <vtkSmartPointer.h>
#include <vtkPoints.h>
#include <vtkCellArray.h>
#include <vtkActor.h>
#include <vtkPolyData.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>
#include <vtkProperty.h>
#include <vtkPolyDataMapper.h>
#include <QList>

typedef struct structCubePipe
{
	int ROIDataID;
	int ROIID;
	int ViewLabel;
	vtkSmartPointer<vtkPoints>					pPoints;
	vtkSmartPointer<vtkCellArray>				pCells;
	vtkSmartPointer<vtkActor>					pActor;
	vtkSmartPointer<vtkPolyData>				pData;
	vtkSmartPointer<vtkTransform>				pTransform;
	vtkSmartPointer<vtkTransformPolyDataFilter>	pTransPolyDataFilter;
	vtkSmartPointer<vtkProperty>				pProperty;
	vtkSmartPointer<vtkPolyDataMapper>			pMapper;
}structCubePipe;

class ASPipelineROICube : public ASPipelineBase
{
public:
	ASPipelineROICube(const int c_ID);
	~ASPipelineROICube();

	// 管线类型判断
	virtual bool isTypeInherited(const int c_Type);
	virtual bool isTypeAbsolute(const int c_Type);
	virtual void DeleteNode();	// 调用此函数来释放管线

	// 虚函数，切图焦点改变
	virtual void Update2DTransformMatrix();

private:

	// 该管线是唯一的，唯一对象
	static ASPipelineROICube* ms_SelfPointer;

	// 管线结构
	QList<structCubePipe*>* m_listCubePipes;

	
};
