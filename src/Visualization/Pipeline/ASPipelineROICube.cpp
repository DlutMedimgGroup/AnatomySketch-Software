#include "ASPipelineROICube.h"
#include "ASAbstractData.h"
#include "ASROIData.h"
#include "ASROITypeBase.h"
#include "ASROITypeCube.h"
#include "ASDataStructureWalker.h"
#include "ASVisualizationManager.h"
#include "ASVisualizationManager2D.h"
#include "GlobelDeclaration.h"

ASPipelineROICube* ASPipelineROICube::ms_SelfPointer = nullptr;

ASPipelineROICube::ASPipelineROICube(const int c_ID) : ASPipelineBase(c_ID)
{
	m_PipelineType = PipeLineType_ROICube;
	ms_SelfPointer = this;

	m_listCubePipes = new QList<structCubePipe *>;
}

ASPipelineROICube::~ASPipelineROICube()
{
	
}

// 管线类型判断
bool ASPipelineROICube::isTypeInherited(const int c_Type)
{
	if (c_Type == PipeLineType_ROICube || ASPipelineBase::isTypeInherited(c_Type))
	{
		return true;
	}
	return false;
}
bool ASPipelineROICube::isTypeAbsolute(const int c_Type)
{
	if (c_Type == PipeLineType_ROICube)
	{
		return true;
	}
	return false;
}
void ASPipelineROICube::DeleteNode()
{

}

// 虚函数，切图焦点改变
void ASPipelineROICube::Update2DTransformMatrix()
{
	vtkSmartPointer<vtkMatrix4x4> pMatrix[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToZero[3];
	vtkSmartPointer<vtkMatrix4x4> pMatrixToOne[3];
	ASVisualizationManager2D::GetTransformMatrix4x4(pMatrix, pMatrixToZero, pMatrixToOne);
	vtkSmartPointer<vtkRenderer> Renderer2D[3];
	ASVisualizationManager2D::GetRenderer2D(Renderer2D);
	// 0 清空原有显示
	int NumOfPipe = m_listCubePipes->size();
	for (int index = 0; index < NumOfPipe; index++)
	{
		structCubePipe* crntPipe = m_listCubePipes->at(index);
		Renderer2D[crntPipe->ViewLabel]->RemoveActor(crntPipe->pActor);
		crntPipe->pActor = nullptr;
		crntPipe->pMapper = nullptr;
		crntPipe->pProperty = nullptr;
		crntPipe->pTransPolyDataFilter = nullptr;
		crntPipe->pTransform = nullptr;
		crntPipe->pData = nullptr;
		crntPipe->pCells = nullptr;
		crntPipe->pCells = nullptr;
		delete crntPipe;
		crntPipe = nullptr;
	}
	m_listCubePipes->clear();

	// 1 添加新显示
	// 1.1 遍历ROI数据
	for (ASAbstractData* item = ASDataStructureWalker::bigen_inherit(DataType_ROI);
		item != nullptr; item = ASDataStructureWalker::next_inherit(DataType_ROI))
	{
		ASROIData* crntROIData = static_cast<ASROIData*>(item);
		if (crntROIData->isShown() == false)
		{
			continue;
		}
		// 1.2 遍历ROI
		int NumOfROI = crntROIData->GetNumOfROIs();
		for (int index = 0; index < NumOfROI; index++)
		{
			ASROITypeBase* roi = crntROIData->GetROIAt(index);
			if (roi->isTypeInherited(ROIType_Cube))
			{
				ASROITypeCube* crntROI = static_cast<ASROITypeCube*>(roi);
				if (crntROI->isShown() == false)
				{
					continue;
				}
				// 1.3 判断范围
				double range[6];
				crntROI->GetRange(range);
				double pos[3];
				ASVisualizationManager::GetDisplayPosition(pos);
				for (int ViewLabel = 0; ViewLabel < 3; ViewLabel++)
				{
					if (pos[ViewLabel] >= range[2 * ViewLabel] && 
						pos[ViewLabel] <= range[2 * ViewLabel + 1])
					{
						// 1.4 建立管线
						structCubePipe* crntPipe = new structCubePipe;
						m_listCubePipes->append(crntPipe);
						crntPipe->ROIDataID = crntROIData->getDataNodeID();
						crntPipe->ROIID = crntROI->GetROIID();
						crntPipe->ViewLabel = ViewLabel;
						// vtkPoints
						crntPipe->pPoints = vtkSmartPointer<vtkPoints>::New();
						switch (ViewLabel)
						{
						case ViewLabel_Sagittal:
							crntPipe->pPoints->InsertNextPoint(pos[0], range[2], range[4]);
							crntPipe->pPoints->InsertNextPoint(pos[0], range[2], range[5]);
							crntPipe->pPoints->InsertNextPoint(pos[0], range[3], range[5]);
							crntPipe->pPoints->InsertNextPoint(pos[0], range[3], range[4]);
							break;
						case ViewLabel_Coronal:
							crntPipe->pPoints->InsertNextPoint(range[0], pos[1], range[4]);
							crntPipe->pPoints->InsertNextPoint(range[0], pos[1], range[5]);
							crntPipe->pPoints->InsertNextPoint(range[1], pos[1], range[5]);
							crntPipe->pPoints->InsertNextPoint(range[1], pos[1], range[4]);
							break;
						case ViewLabel_Axial:
							crntPipe->pPoints->InsertNextPoint(range[0], range[2], pos[2]);
							crntPipe->pPoints->InsertNextPoint(range[0], range[3], pos[2]);
							crntPipe->pPoints->InsertNextPoint(range[1], range[3], pos[2]);
							crntPipe->pPoints->InsertNextPoint(range[1], range[2], pos[2]);
							break;
						default:
							break;
						}
						// vtkCellArray
						crntPipe->pCells = vtkSmartPointer<vtkCellArray>::New();
						vtkIdType  crntid[2];
						for (int i = 0; i < 4; i++)
						{
							crntid[0] = i;
							crntid[1] = (i + 1) % 4;
							crntPipe->pCells->InsertNextCell(2, crntid);
						}
						// vtkPolyData
						crntPipe->pData = vtkSmartPointer<vtkPolyData>::New();
						crntPipe->pData->SetPoints(crntPipe->pPoints);
						crntPipe->pData->SetLines(crntPipe->pCells);
						crntPipe->pData->Modified();
						// vtkTransform
						crntPipe->pTransform = vtkSmartPointer<vtkTransform>::New();
						crntPipe->pTransform->SetMatrix(pMatrixToOne[ViewLabel]);
						// vtkTransformPolyDataFilter
						crntPipe->pTransPolyDataFilter = 
							vtkSmartPointer<vtkTransformPolyDataFilter>::New();
						crntPipe->pTransPolyDataFilter->SetInputData(crntPipe->pData);
						crntPipe->pTransPolyDataFilter->SetTransform(crntPipe->pTransform);
						crntPipe->pTransPolyDataFilter->Update();
						// vtkPolyDataMapper
						crntPipe->pMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
						crntPipe->pMapper->SetInputData(
							crntPipe->pTransPolyDataFilter->GetOutput());
						// vtkProperty
						crntPipe->pProperty = vtkSmartPointer<vtkProperty>::New();
						crntPipe->pProperty->SetLineWidth(2);
						QColor color = crntROI->GetColor();
						crntPipe->pProperty->SetColor(((double)color.red())/255,
							((double)color.green()) / 255, ((double)color.blue()) / 255);
						// vtkActor
						crntPipe->pActor = vtkSmartPointer<vtkActor>::New();
						crntPipe->pActor->SetMapper(crntPipe->pMapper);
						crntPipe->pActor->SetPickable(false);
						crntPipe->pActor->SetProperty(crntPipe->pProperty);
						// vtkRender
						Renderer2D[ViewLabel]->AddActor(crntPipe->pActor);
					}
				}
			}
		}
	}
}