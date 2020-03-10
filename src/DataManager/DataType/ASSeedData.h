#pragma once

#include "ASMarkData.h"
#include "GlobelDeclaration.h"

// 继承时需要在构造函数中设定 m_Datatype， 并重写walkerCheck函数

typedef struct structLine {
	QList<structPoint*> pvectorPoint;
	int ViewLabel; //0:sagittal  1 : coronal   2 : axial DimensionsOfImage
	double dLayer;
	int nSerial;//保存画线的先后顺序，用于撤销等
}structLine;
typedef struct structSeeds
{
	int seedId;
	QColor color;
	QString name;
	int label;
	bool isShown;
	QList<structLine*>* pListSeedsData;
}structSeeds;

class ASSeedData : public ASMarkData
{
public:
	ASSeedData();
	~ASSeedData();
	virtual void DeleteNode();	// 调用此函数来释放节点

	// 数据类型判断
	virtual bool isTypeInherited(const int c_Type); // 判断类型，可继承的，如：ASDICOMData->isTypeInherited(DataType_ArrayImageData)为真
	virtual bool isTypeAbsolute(const int c_Type);
	static bool isTypeInheritedStatic(const int c_Type);

	// 新建种子点
	int NewSeed(const QString c_name, const int c_lable, const QColor c_color);

	// 种子点数据数量
	static int GetNumOfSeedsData();
	// 标签值
	int getLabel(); // 弃用
	void setLabel(const int label); // 弃用
	// 种子点数据的访问
	int getNumberOfSeeds() {
		return m_listSeedsData->size();
	}
	void AppandLine(structLine* NewLine);
	// 输入这条线在QList中的位置，返回线的结构体
	structSeeds* getSeedsAt(int atN) {
		return m_listSeedsData->at(atN);
	}
	structSeeds* getSeedsFromID(const int c_SeedsID);
	// 撤销
	void AddOperaSequence(const int c_SeedsID);
	bool UndoALine();
	// 属性
	virtual double* getDataExtent(); // 得到数据范围，double[6]

	// 画种子点
	void StartPaintingSeeds(double* pPosition, const int ViewLabel, const int c_SeedID);
	void ContinuePaintingSeeds(double* pPosition, const int ViewLabel);
	void EndPaintingSeeds();

	// 删除种子点
	void DeleteWholeSeeds(const int c_SeedID);

protected:

	int m_nLabel; // 标签值 // 弃用

private:

	// 储存种子点数据
	QList<structSeeds*>* m_listSeedsData;
	QList<int>* m_listOperaSequence;
	structLine* m_pWorkingSeedsLine; // 辅助画线，正在画的线
	int m_crntSerialOfSeeds = 0; // 操作序号，用于撤销

	static int ms_nNumOfSeedsData;				// 当前打开的种子点数据的数量

	// 得到一个新的不重复ID
	int GetNewSeedsID();
};
