#include "ASTransStatusBarGeneral.h"
#include "ASVisualizationManager.h"
#include "ASArrayImageData.h"
#include "ASTransGeneralInteractor.h"
#include <vtkImageData.h>

ASTransStatusBarGeneral* ASTransStatusBarGeneral::ms_TransStatusBarGeneral = nullptr;

ASTransStatusBarGeneral::ASTransStatusBarGeneral(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_TransStatusBarGeneral = this;
}

ASTransStatusBarGeneral::~ASTransStatusBarGeneral()
{

}

// 唯一对象
ASTransStatusBarGeneral* ASTransStatusBarGeneral::GetSelfPointer()
{
	return ms_TransStatusBarGeneral;
}

// 设置状态栏右侧焦点信息
void ASTransStatusBarGeneral::RefreshStatusBarFocusInfo()
{
	int crntindex[3];
	double crntPosition[3];
	double WindowLevel[2];
	WindowLevel[0] = ASTransGeneralInteractor::GetWindow();
	WindowLevel[1] = ASTransGeneralInteractor::GetLevel();
	ASVisualizationManager::GetDisplayPosition(crntPosition);
	double crntExtent[6];
	ASVisualizationManager::GetDisplayExtent(crntExtent);
	double crntSpacing[3];
	ASVisualizationManager::GetDisplaySpacing(crntSpacing);
	int crntFrame = ASVisualizationManager::GetDisplayFrame();
	for (size_t ViewLabel = 0; ViewLabel < 3; ViewLabel++)
	{
		crntindex[ViewLabel] = (crntPosition[ViewLabel] - crntExtent[ViewLabel * 2]) / crntSpacing[ViewLabel];
	}
	if (ASVisualizationManager::GetToplayerData() == nullptr)
	{
		emit ms_TransStatusBarGeneral->signalStatusBarFocusInformationRefresh(NULL, NULL, NULL, NULL, NULL, NULL, NULL);
		return;
	}
	double value = 0.0;
	vtkImageData* cronTopImageData = ASVisualizationManager::GetToplayerData()->getArrayImageData();
	int extent[6];
	double space[3];
	double origin[3];
	if (cronTopImageData != NULL)
	{
		cronTopImageData->GetExtent(extent);
		cronTopImageData->GetSpacing(space);
		cronTopImageData->GetOrigin(origin);
		int NumOfFrame = cronTopImageData->GetNumberOfScalarComponents();
		if (crntFrame >= NumOfFrame)
		{
			crntFrame = NumOfFrame - 1;
		}
		else if (crntFrame < 0)
		{
			crntFrame = 0;
		}
		int xyz[3];
		xyz[0] = (crntPosition[0] - origin[0]) / space[0];
		xyz[1] = (crntPosition[1] - origin[1]) / space[1];
		xyz[2] = (crntPosition[2] - origin[2]) / space[2];
		if (xyz[0] >= extent[0] && xyz[0] <= extent[1] &&
			xyz[1] >= extent[2] && xyz[1] <= extent[3] &&
			xyz[2] >= extent[4] && xyz[2] <= extent[5])
		{
			value = cronTopImageData->GetScalarComponentAsDouble(xyz[0], xyz[1], xyz[2], crntFrame);
		}
	}
	emit ms_TransStatusBarGeneral->signalStatusBarFocusInformationRefresh(crntPosition, crntindex, value, extent, space, origin, WindowLevel);
}