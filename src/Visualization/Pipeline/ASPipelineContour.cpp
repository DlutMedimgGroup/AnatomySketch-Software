#include "ASPipelineContour.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager2D.h"
#include "ASVisualizationManager3D.h"
#include "ASDataStructureWalker.h"
#include "ASContourData.h"
#include <QDebug>

ASPipelineContour::ASPipelineContour(const int c_ID) : ASPipelineBase(c_ID)
{
	m_PipelineType = PipeLineType_Contour;

	m_plistContourSplinePipeline = new QList<structCTRSPipe*>;

	// Renderer
	ASVisualizationManager2D::GetRenderer2D(m_pRenderer2D);
}

ASPipelineContour::~ASPipelineContour()
{

}

// 管线类型判断
bool ASPipelineContour::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_Contour || ASPipelineBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPipelineContour::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_Contour)
	{
		return true;
	}
	return false;
}
                                                    
// 虚函数
// 调用此函数来释放管线
void ASPipelineContour::DeleteNode()
{
	// 释放数据时，不释放轮廓线管线
	// delete this;	
}
// 切图焦点改变
void ASPipelineContour::Update2DTransformMatrix()
{
	RefreshSplineContourDisplay(0);
	RefreshSplineContourDisplay(1);
	RefreshSplineContourDisplay(2);
}
void ASPipelineContour::SetShown(const bool c_Shown)
{
	m_isShown = c_Shown;
	if (m_isShown == false)
	{
		RefreshSplineContourDisplay(0);
		RefreshSplineContourDisplay(1);
		RefreshSplineContourDisplay(2);
	}
}

// 是否可以拾取
void ASPipelineContour::SetPickable(const bool c_isPickable)
{
	if (m_Pickable == c_isPickable)
	{
		return;
	}
	m_Pickable = c_isPickable;
	for (int ncrntPipeline = 0; ncrntPipeline < m_plistContourSplinePipeline->size(); ncrntPipeline++)
	{
		structCTRSPipe* crntPipeline = m_plistContourSplinePipeline->at(ncrntPipeline);
		int numOfCtrlPoints = crntPipeline->plistContourSplinSphereHandle->size();
		for (int ncrntPoint = 0; ncrntPoint < numOfCtrlPoints; ncrntPoint++)
		{
			vtkSphereHandleRepresentation* crntHandle = crntPipeline->plistContourSplinSphereHandle->at(ncrntPoint);
			crntHandle->SetPickable(m_Pickable);
		}
	}
}
// 刷新样条轮廓线显示
void ASPipelineContour::RefreshSplineContourDisplay(const int c_ViewLabel)
{
	// 清空该方向原有数据显示
	for (int ncrntPipeline = 0; ncrntPipeline < m_plistContourSplinePipeline->size();)
	{
		structCTRSPipe* crntPipeline = m_plistContourSplinePipeline->at(ncrntPipeline);
		if (crntPipeline->ViewLabel == c_ViewLabel)
		{
			m_pRenderer2D[crntPipeline->ViewLabel]->RemoveActor(crntPipeline->pCTRSActor);
			crntPipeline->pCTRSPoints = NULL;
			crntPipeline->pCTRSParaSpline = NULL;
			crntPipeline->pCTRSParaFuncSource = NULL;
			crntPipeline->pCTRSTransform = NULL;
			crntPipeline->pCTRSTransPolyDataFilter = NULL;
			crntPipeline->pCTRSProperty = NULL;
			crntPipeline->pCTRSPolyDataMapper = NULL;
			crntPipeline->pCTRSActor = NULL;

			while (crntPipeline->plistContourSplinSphereHandle->size() > 0)
			{
				vtkSphereHandleRepresentation* crntHandle = crntPipeline->plistContourSplinSphereHandle->at(0);
				vtkSmartPointer<vtkPropCollection> PropCollection = vtkSmartPointer<vtkPropCollection>::New();
				crntHandle->GetActors(PropCollection);
				PropCollection->InitTraversal();
				vtkProp *prop = PropCollection->GetNextProp();
				m_pRenderer2D[crntPipeline->ViewLabel]->RemoveActor(prop);
				crntHandle->Delete();

				crntPipeline->plistContourSplinSphereHandle->removeOne(crntHandle);
			}
			delete(crntPipeline->plistContourSplinSphereHandle);

			m_plistContourSplinePipeline->removeOne(crntPipeline);
			delete(crntPipeline);
		}
		else
		{
			ncrntPipeline++;
		}
	}
	if (m_isShown == false)
	{
		return;
	}
	
	//计算当前层的范围，对该范围内数据进行显示
	double ImagePosition[3];
	ASVisualizationManager::GetDisplayPosition(ImagePosition);
	double ImageSpacing[3];
	ASVisualizationManager::GetDisplaySpacing(ImageSpacing);
	double layermin = ImagePosition[c_ViewLabel] - 0.5 * ImageSpacing[c_ViewLabel];
	double layermax = ImagePosition[c_ViewLabel] + 0.5 * ImageSpacing[c_ViewLabel];
	vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
	ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);

	// 遍历所有样条轮廓线数据
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ContourData); 
		item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_ContourData))
	{
		ASContourData* crntDataNode = static_cast<ASContourData*>(item);
		// 遍历轮廓
		int NumOfContours = crntDataNode->GetNumberOfContour();
		for (int ncrntContour = 0; ncrntContour < NumOfContours; ncrntContour++)
		{
			structContour* crntContour = crntDataNode->GetContourAt(ncrntContour);
			if (crntContour->isShown == false)
			{
				continue;
			}
			double dr = ((double)crntContour->color.red()) / 255;
			double dg = ((double)crntContour->color.green()) / 255;
			double db = ((double)crntContour->color.blue()) / 255;
			// 遍历curve
			int numOfCurves = crntContour->pListCurves->size();
			for (int ncrntCurve = 0; ncrntCurve < numOfCurves; ncrntCurve++)
			{
				structCurve* crntCurve = crntContour->pListCurves->at(ncrntCurve);
				if (crntCurve->ViewLabel != c_ViewLabel)
				{
					continue;
				}
				// 判断该轮廓是否在当前层面
				if (crntCurve->dLayer >= layermin && crntCurve->dLayer < layermax)
				{
					// 建立新管线
					structCTRSPipe* crntPipeline = new structCTRSPipe;
					crntPipeline->ID = crntDataNode->getDataNodeID();
					crntPipeline->ContourID = crntContour->contourID;
					crntPipeline->ViewLabel = crntCurve->ViewLabel;
					crntPipeline->ItemOfContour = ncrntCurve;
					m_plistContourSplinePipeline->append(crntPipeline);
					// 变换矩阵
					vtkSmartPointer<vtkMatrix4x4> pactMatrixToOne = vtkSmartPointer<vtkMatrix4x4>::New();
					pactMatrixToOne->DeepCopy(pMatrixToOne[crntCurve->ViewLabel]);
					//pactMatrixToOne->SetElement(2, 3, crntCurve->dLayer + 1);
					// handle
					crntPipeline->plistContourSplinSphereHandle = new QList<vtkSphereHandleRepresentation*>;
					int numOfCtrlPoints = crntCurve->pvectorPoint.size();
					for (int ncrntPoint = 0; ncrntPoint < numOfCtrlPoints; ncrntPoint++)
					{
						structPoint* crntPoint = crntCurve->pvectorPoint.at(ncrntPoint);
						// vtkSphereHandleRepresentation
						vtkSphereHandleRepresentation* crntHandle = vtkSphereHandleRepresentation::New();
						double posTransInput[4];
						posTransInput[0] = crntPoint->dX;
						posTransInput[1] = crntPoint->dY;
						posTransInput[2] = crntPoint->dZ;
						posTransInput[3] = 1;
						double posDisplay[4];

						pactMatrixToOne->MultiplyPoint(posTransInput, posDisplay);
						crntHandle->SetWorldPosition(posDisplay);
						crntHandle->SetSphereRadius(m_HandleRadius[c_ViewLabel]);
						crntHandle->SetPickable(m_Pickable);
						if (crntCurve->haveTemporary == true && ncrntPoint == numOfCtrlPoints - 1)
						{
							// 临时控制点不可被拾取
							crntHandle->SetPickable(false);
						}
						// vtkActor
						vtkSmartPointer<vtkPropCollection> PropCollection = vtkSmartPointer<vtkPropCollection>::New();
						crntHandle->GetActors(PropCollection);
						PropCollection->InitTraversal();
						vtkActor* actor = (vtkActor*)(PropCollection->GetNextProp());
						if (crntDataNode->isShown())
						{
							m_pRenderer2D[crntCurve->ViewLabel]->AddActor(actor);
							actor->SetPickable(m_Pickable);
							if (crntCurve->haveTemporary == true && ncrntPoint == numOfCtrlPoints - 1)
							{
								// 临时控制点不可被拾取
								actor->SetPickable(false);
							}
						}
						crntPipeline->plistContourSplinSphereHandle->append(crntHandle);
					}
					if (numOfCtrlPoints < 2)
					{
						continue;;
					}

					// vtkPoints
					crntPipeline->pCTRSPoints = vtkSmartPointer<vtkPoints>::New();
					for (int ncrntPoint = 0; ncrntPoint < numOfCtrlPoints; ncrntPoint++)
					{
						structPoint* crntPoint = crntCurve->pvectorPoint.at(ncrntPoint);
						crntPipeline->pCTRSPoints->InsertPoint(ncrntPoint, crntPoint->dX, crntPoint->dY, crntPoint->dZ);
					}
					crntPipeline->pCTRSPoints->Modified();
					// vtkParametricSpline
					crntPipeline->pCTRSParaSpline = vtkSmartPointer<vtkParametricSpline>::New();
					crntPipeline->pCTRSParaSpline->SetPoints(crntPipeline->pCTRSPoints);
					if (crntCurve->isClosure)
					{
						crntPipeline->pCTRSParaSpline->ClosedOn();
					}
					else
					{
						crntPipeline->pCTRSParaSpline->ClosedOff();
					}
					crntPipeline->pCTRSParaSpline->Modified();
					// vtkParametricFunctionSource
					crntPipeline->pCTRSParaFuncSource = vtkSmartPointer<vtkParametricFunctionSource>::New();
					crntPipeline->pCTRSParaFuncSource->SetParametricFunction(crntPipeline->pCTRSParaSpline);
					crntPipeline->pCTRSParaFuncSource->SetUResolution(numOfCtrlPoints * 8);
					crntPipeline->pCTRSParaFuncSource->Update();
					// vtkPolyData
					crntPipeline->pCTRSPolyData = vtkSmartPointer<vtkPolyData>::New();
					crntPipeline->pCTRSPolyData = crntPipeline->pCTRSParaFuncSource->GetOutput();
					// vtkTransform
					crntPipeline->pCTRSTransform = vtkSmartPointer<vtkTransform>::New();
					crntPipeline->pCTRSTransform->SetMatrix(pactMatrixToOne);
					// vtkTransformPolyDataFilter
					crntPipeline->pCTRSTransPolyDataFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
					crntPipeline->pCTRSTransPolyDataFilter->SetInputData(crntPipeline->pCTRSPolyData);
					crntPipeline->pCTRSTransPolyDataFilter->SetTransform(crntPipeline->pCTRSTransform);
					crntPipeline->pCTRSTransPolyDataFilter->Update();
					// vtkPolyDataMapper
					crntPipeline->pCTRSPolyDataMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
					crntPipeline->pCTRSPolyDataMapper->SetInputData(crntPipeline->pCTRSTransPolyDataFilter->GetOutput());
					// vtkProperty
					crntPipeline->pCTRSProperty = vtkSmartPointer<vtkProperty>::New();
					crntPipeline->pCTRSProperty->SetLineWidth(2);
					crntPipeline->pCTRSProperty->SetColor(dr, dg, db);
					// vtkActor
					crntPipeline->pCTRSActor = vtkSmartPointer<vtkActor>::New();
					crntPipeline->pCTRSActor->SetMapper(crntPipeline->pCTRSPolyDataMapper);
					crntPipeline->pCTRSActor->SetProperty(crntPipeline->pCTRSProperty);
					crntPipeline->pCTRSActor->SetPickable(false);
					//renderer
					if (crntDataNode->isShown())
					{
						m_pRenderer2D[crntCurve->ViewLabel]->AddActor(crntPipeline->pCTRSActor);
					}
				}
			}
		}
	}
}
// 设置控制点直径
void ASPipelineContour::ReSetCtrlRadius(const int c_ViewLabel)
{
	m_HandleRadius[c_ViewLabel] = this->GetCtrlPointRadiusSplineContour(c_ViewLabel);
	if (m_HandleRadius[c_ViewLabel] > 0)
	{
		for (int ncrntPipeline = 0; ncrntPipeline < m_plistContourSplinePipeline->size(); ncrntPipeline++)
		{
			structCTRSPipe* crntPipeline = m_plistContourSplinePipeline->at(ncrntPipeline);
			if (crntPipeline->ViewLabel == c_ViewLabel)
			{
				int numOfCtrlPoints = crntPipeline->plistContourSplinSphereHandle->size();
				for (int ncrntPoint = 0; ncrntPoint < numOfCtrlPoints; ncrntPoint++)
				{
					vtkSphereHandleRepresentation* crntHandle = crntPipeline->plistContourSplinSphereHandle->at(ncrntPoint);
					crntHandle->SetSphereRadius(m_HandleRadius[c_ViewLabel]);
				}
			}
		}
	}
	else
	{
		m_HandleRadius[c_ViewLabel] = 1;
	}

}
// 根据actor地址查询控制点信息
bool ASPipelineContour::reserchHandleSplineContour(vtkActor* pActor, int* ID, int* ContourID, 
	int* ViewLabel, int* ItemOfContour, int* ItemOfHandle)
{
	// 遍历所有structContourSplinepipeline
	foreach(structCTRSPipe* crntContourSplinepipeline, *m_plistContourSplinePipeline)
	{
		// 遍历handle
		foreach(vtkSphereHandleRepresentation* crntHandle, *(crntContourSplinepipeline->plistContourSplinSphereHandle))
		{
			vtkSmartPointer<vtkPropCollection> PropCollection = vtkSmartPointer<vtkPropCollection>::New();
			crntHandle->GetActors(PropCollection);
			PropCollection->InitTraversal();
			vtkActor* actor = (vtkActor*)(PropCollection->GetNextProp());
			if (actor == pActor)
			{
				*ID = crntContourSplinepipeline->ID;
				*ContourID = crntContourSplinepipeline->ContourID;
				*ViewLabel = crntContourSplinepipeline->ViewLabel;
				*ItemOfContour = crntContourSplinepipeline->ItemOfContour;
				*ItemOfHandle = crntContourSplinepipeline->plistContourSplinSphereHandle->indexOf(crntHandle);
				return true;
			}
		}
	}
	return false;
}
// 插入控制点时根据坐标判断控制点位置
bool ASPipelineContour::getInsertHandleIndex(double* pos, const int c_viewLabel, int* ID, int* ItemOfContour, int* ItemOfHandle)
{
	vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
	ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);
	
	double minDistance = -1;

	// 遍历轮廓
	foreach(structCTRSPipe* crntPipeline, *m_plistContourSplinePipeline)
	{
		if (crntPipeline->ViewLabel == c_viewLabel)
		{
			// 遍历控制点，找到距离各个控制点最近的样条顶点
			int numOfHandles = crntPipeline->plistContourSplinSphereHandle->size();
			QList<int> indexOfHandlesInspline;

			double* minDistanceOfCtrlPoints = (double*)malloc(sizeof(double) * numOfHandles);
			for (int i = 0; i < numOfHandles; i++)
			{
				minDistanceOfCtrlPoints[i] = -1;
			}
			int* indexOfCtrlPoints = (int*)malloc(sizeof(int) * numOfHandles);
			vtkSmartPointer<vtkMatrix4x4> oneToWorld = vtkSmartPointer<vtkMatrix4x4>::New();
			vtkMatrix4x4::Invert(pMatrixToOne[c_viewLabel], oneToWorld);

			// 遍历样条顶点
			vtkPolyData* crntPolyData = crntPipeline->pCTRSParaFuncSource->GetOutput();
			int numOfPoints = crntPolyData->GetNumberOfPoints();
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				double crntPoint[3];
				crntPolyData->GetPoint(ncrntPoint, crntPoint);

				// 寻找控制点对应点
				for (int ncrntHandle = 0; ncrntHandle < numOfHandles; ncrntHandle++)
				{
					vtkSphereHandleRepresentation* crntSphereHandle = crntPipeline->plistContourSplinSphereHandle->at(ncrntHandle);
					double* crntWorldPosition = crntSphereHandle->GetWorldPosition();
					double posTransInput[4];
					posTransInput[0] = crntWorldPosition[0];
					posTransInput[1] = crntWorldPosition[1];
					posTransInput[2] = crntWorldPosition[2];
					posTransInput[3] = 1;
					double posInWorld[4];

					oneToWorld->MultiplyPoint(posTransInput, posInWorld);

					double crntDistance = sqrt(pow(posInWorld[0] - crntPoint[0], 2) + pow(posInWorld[1] - crntPoint[1], 2) + pow(posInWorld[2] - crntPoint[2], 2));

					if (minDistanceOfCtrlPoints[ncrntHandle] < 0)
					{
						minDistanceOfCtrlPoints[ncrntHandle] = crntDistance;
						indexOfCtrlPoints[ncrntHandle] = ncrntPoint;
					}
					else
					{
						if (minDistanceOfCtrlPoints[ncrntHandle] > crntDistance)
						{
							minDistanceOfCtrlPoints[ncrntHandle] = crntDistance;
							indexOfCtrlPoints[ncrntHandle] = ncrntPoint;
						}
					}
				}
			}
			for (int ncrntPoint = 0; ncrntPoint < numOfPoints; ncrntPoint++)
			{
				double crntPoint[3];
				crntPolyData->GetPoint(ncrntPoint, crntPoint);
				// 寻找最近点
				double crntDistance = sqrt(pow(pos[0] - crntPoint[0], 2) + pow(pos[1] - crntPoint[1], 2) + pow(pos[2] - crntPoint[2], 2));
				if (minDistance < 0)
				{
					minDistance = crntDistance;
				}
				else
				{
					if (crntDistance < minDistance)
					{
						minDistance = crntDistance;
						*ID = crntPipeline->ID;
						*ItemOfContour = crntPipeline->ItemOfContour;
						*ItemOfHandle = numOfHandles - 1;
						for (int i = 0; i < numOfHandles; i++)
						{
							if (indexOfCtrlPoints[i] > ncrntPoint)
							{
								*ItemOfHandle = i - 1;
								break;
							}
						}
					}
				}
			}
		}
	}

	if (minDistance < 0)
	{
		return false;
	}
	else
	{
		return true;
	}
}

// 控制点半径
double ASPipelineContour::GetCtrlPointRadiusSplineContour(const int c_viewLabel)
{
	return ASVisualizationManager::GetZoomPara(c_viewLabel) / CTRLPOINTRADIUS;
}