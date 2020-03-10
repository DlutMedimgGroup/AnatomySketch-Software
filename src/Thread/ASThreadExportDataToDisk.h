#pragma once
#include "ASThreadBase.h"
#include <vtkSmartPointer.h>
#include <vtkMetaImageWriter.h>
#include <vtkSTLWriter.h>
#include <vtkOBJWriter.h>
#include <vtkPLYWriter.h>

class ASAbstractData;

class ASThreadExportDataToDisk : public ASThreadBase
{
	Q_OBJECT

public:
	ASThreadExportDataToDisk(QObject *parent = 0);
	~ASThreadExportDataToDisk();

	void SetFileName(const QString c_FileName);
	void SetDataNode(ASAbstractData* DataNode);

protected:
	void run();

private:
	QString m_FileName;
	ASAbstractData* m_DataNode;

	vtkSmartPointer<vtkMetaImageWriter> m_MetaImageWriter = nullptr;
	vtkSmartPointer<vtkSTLWriter> m_stlWriter = nullptr;
	vtkSmartPointer<vtkOBJWriter> m_objWriter = nullptr;
	vtkSmartPointer<vtkPLYWriter> m_plyWriter = nullptr;

	void WriteSeedData();
	void WriteContourData();
	void WriteLandmarkData();
	void WriteROIData();

signals:
	// 数据读入完成
	void signalSaveDataFromDiskFinished();

};
