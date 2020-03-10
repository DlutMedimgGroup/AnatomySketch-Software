#include "ASColorMap.h"

vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Gray = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_MultiLabel = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Mask = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Cold = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Hot = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Cool = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Bronson = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Split_BBR = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_UCLA = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Red = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Green = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_Blue = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_HeartVR = vtkSmartPointer<vtkLookupTable>::New();
vtkSmartPointer<vtkLookupTable> ASColorMap::LookupTable_HeadVR = vtkSmartPointer<vtkLookupTable>::New();

ASColorMap::ASColorMap()
{
	ASColorMap::initialization();
}

ASColorMap::~ASColorMap()
{
}

void ASColorMap::initialization()
{
	int numOfPoint;
	// Gray
	LookupTable_Gray->SetValueRange(0.0, 1.0); // from black to white
	LookupTable_Gray->SetSaturationRange(0.0, 0.0); // no color saturation
	LookupTable_Gray->SetRampToLinear();
	//LookupTable_Gray->Build();
	// MultiLabel
	LookupTable_MultiLabel->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Multilabel_Data) / sizeof(ColorMap_Multilabel_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_MultiLabel->SetTableValue(
			ColorMap_Multilabel_Data[crntPoint][0],
			ColorMap_Multilabel_Data[crntPoint] + 1);
	}
	LookupTable_MultiLabel->SetNumberOfTableValues(256);
	//LookupTable_MultiLabel->Build();
	// Mask
	LookupTable_Mask->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Mask_Data) / sizeof(ColorMap_Mask_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Mask->SetTableValue(
			ColorMap_Mask_Data[crntPoint][0],
			ColorMap_Mask_Data[crntPoint] + 1);
	}
	LookupTable_Mask->SetNumberOfTableValues(256);
	//LookupTable_Mask->Build();
	// Cold
	LookupTable_Cold->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Cold_Data) / sizeof(ColorMap_Cold_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Cold->SetTableValue(
			ColorMap_Cold_Data[crntPoint][0],
			ColorMap_Cold_Data[crntPoint] + 1);
	}
	LookupTable_Cold->SetNumberOfTableValues(255);
	//LookupTable_Cold->Build();
	// Hot
	LookupTable_Hot->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Hot_Data) / sizeof(ColorMap_Hot_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Hot->SetTableValue(
			ColorMap_Hot_Data[crntPoint][0],
			ColorMap_Hot_Data[crntPoint] + 1);
	}
	LookupTable_Hot->SetNumberOfTableValues(255);
	//LookupTable_Hot->Build();
	// Cool
	LookupTable_Cool->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Cool_Data) / sizeof(ColorMap_Cool_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Cool->SetTableValue(
			ColorMap_Cool_Data[crntPoint][0],
			ColorMap_Cool_Data[crntPoint] + 1);
	}
	LookupTable_Cool ->SetNumberOfTableValues(255);
	//LookupTable_Cool->Build();
	// Bronson
	LookupTable_Bronson->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Bronson_Data) / sizeof(ColorMap_Bronson_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Bronson->SetTableValue(
			ColorMap_Bronson_Data[crntPoint][0],
			ColorMap_Bronson_Data[crntPoint] + 1);
	}
	LookupTable_Bronson->SetNumberOfTableValues(255);
	//LookupTable_Bronson->Build();
	// Split_BBR
	LookupTable_Split_BBR->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Split_BBR_Data) / sizeof(ColorMap_Split_BBR_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Split_BBR->SetTableValue(
			ColorMap_Split_BBR_Data[crntPoint][0],
			ColorMap_Split_BBR_Data[crntPoint] + 1);
	}
	LookupTable_Split_BBR->SetNumberOfTableValues(255);
	//LookupTable_Split_BBR->Build();
	// UCLA
	LookupTable_UCLA->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_UCLA_Data) / sizeof(ColorMap_UCLA_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_UCLA->SetTableValue(
			ColorMap_UCLA_Data[crntPoint][0],
			ColorMap_UCLA_Data[crntPoint] + 1);
	}
	LookupTable_UCLA->SetNumberOfTableValues(255);
	//LookupTable_UCLA->Build();
	// Red
	LookupTable_Red->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Red_Data) / sizeof(ColorMap_Red_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Red->SetTableValue(
			ColorMap_Red_Data[crntPoint][0],
			ColorMap_Red_Data[crntPoint] + 1);
	}
	LookupTable_Red->SetNumberOfTableValues(255);
	//LookupTable_Red->Build();
	// Green
	LookupTable_Green->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Green_Data) / sizeof(ColorMap_Green_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Green->SetTableValue(
			ColorMap_Green_Data[crntPoint][0],
			ColorMap_Green_Data[crntPoint] + 1);
	}
	LookupTable_Green->SetNumberOfTableValues(255);
	//LookupTable_Green->Build();
	// Blue
	LookupTable_Blue->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_Blue_Data) / sizeof(ColorMap_Blue_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_Blue->SetTableValue(
			ColorMap_Blue_Data[crntPoint][0],
			ColorMap_Blue_Data[crntPoint] + 1);
	}
	LookupTable_Blue->SetNumberOfTableValues(255);
	//LookupTable_Blue->Build();
	// HeartVR
	LookupTable_HeartVR->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_HeartVR_Data) / sizeof(ColorMap_HeartVR_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_HeartVR->SetTableValue(
			ColorMap_HeartVR_Data[crntPoint][0],
			ColorMap_HeartVR_Data[crntPoint] + 1);
	}
	LookupTable_HeartVR->SetNumberOfTableValues(256);
	//LookupTable_HeartVR->Build();
	// HeadVR
	LookupTable_HeadVR->SetNumberOfColors(256);
	numOfPoint = sizeof(ColorMap_HeadVR_Data) / sizeof(ColorMap_HeadVR_Data[0]);
	for (size_t crntPoint = 0; crntPoint < numOfPoint; crntPoint++)
	{
		LookupTable_HeadVR->SetTableValue(
			ColorMap_HeadVR_Data[crntPoint][0],
			ColorMap_HeadVR_Data[crntPoint] + 1);
	}
	LookupTable_HeadVR->SetNumberOfTableValues(255);
	//LookupTable_HeadVR->Build();
}