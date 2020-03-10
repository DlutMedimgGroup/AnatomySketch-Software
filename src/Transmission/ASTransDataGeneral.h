#pragma once
#include "ASTransmissionBase.h"

class ASAbstractData;
class ASArrayImageData;
class ASAnnotation;

class ASTransDataGeneral : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransDataGeneral(QObject *parent = 0);
	~ASTransDataGeneral();

	// 唯一对象
	static ASTransDataGeneral* GetSelfPointer();

	// 设置某体数据为当前数据
	static void SetGeneratingData(ASAbstractData* crntData);
	static ASAbstractData* GetGeneratingData();
	static void ResetGeneratingData(); // 更新和当前数据相关

	// 标记数据
	static ASAnnotation* NewAnnotationData(ASAbstractData* parent); // 新建标记数据


private:
	// 唯一对象
	static ASTransDataGeneral* ms_SelfPointer;

	static ASArrayImageData* m_GeneratingData;


signals :
	void signalRefreshCrntData();
};
