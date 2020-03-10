#pragma once
#include "ASThreadBase.h"
#include "ASAbstractData.h"
#include "ASPolyImageData.h"
#include "ASArrayImageData.h"
#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPLYReader.h>
#include <vtkOBJReader.h>

class ASThreadImportDataFromDisk : public ASThreadBase
{
	Q_OBJECT

public:
	ASThreadImportDataFromDisk(QObject *parent = 0);
	~ASThreadImportDataFromDisk();

	void SetFileName(const QString c_FileName);

protected:
	void run();

private:
	// 文件路径
	QString m_FileName;

	vtkSmartPointer<vtkPLYReader> m_pPLYReader = nullptr;
	vtkSmartPointer<vtkOBJReader> m_pOBJReader = nullptr;

	// 读入体数据
	ASArrayImageData* LoadArrayImageData(const QString c_FileName);
	// 读入曲面数据
	ASPolyImageData* LoadPolyImageDate(const QString c_FileName);
	// 读入二维数据
	ASArrayImageData* Load2DImageData(const QString c_FileName);

signals:

	// 数据读取成功
	void signalDataLoadFinished(ASAbstractData* NewDataNode, int Type);

};
