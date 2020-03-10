#pragma once
#include "ASTransmissionBase.h"
#include <functional>

class ASAbstractData;
class ASOperation;
class ASFModelAddins;

typedef std::function<void()> Fun1;

class ASTransAddins : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransAddins(QObject *parent = 0);
	~ASTransAddins();

	// 唯一对象
	static ASTransAddins* GetSelfPointer();

	// 新建插件操作
	static void NewAddinOperation(ASAbstractData* pParent, const int c_Type);
	// 当前分割
	static void SetCrntOperation(ASAbstractData* opera);
	static ASOperation* GetCrntOperation();
	// 有数据被移除
	static void DataRemoved(ASAbstractData* opera);

	// 新建遮罩
	static void NewMask(ASAbstractData* pParent, QString name, Fun1 f);
	static void NewMaskFinish();

private:

	// 唯一对象
	static ASTransAddins* ms_SelfPointer;
	// 新建遮罩的分割
	ASOperation* m_NewMaskGenerating = nullptr;
	// 新建遮罩完成回调函数
	Fun1 m_f = nullptr;
	// Model
	ASFModelAddins* m_Addins;

signals:

	// 刷新分割界面，如进行删除之后
	void signalRefreshWidget();
	// 当前分割发生变化
	void signalCurrentSegChanged();
	// 设置当前方法
	void signalSetCurrentMethod(QString s);

};
