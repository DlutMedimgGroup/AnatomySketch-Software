#pragma once
#include "ASFunctionModelBase.h"

class ASOperation;

typedef struct structAddin
{
	int type;
	QString path;
	QString name;
}structAddin;

class ASFModelAddins : public ASFunctionModelBase
{
	Q_OBJECT

public:
	ASFModelAddins(QObject *parent = 0);
	~ASFModelAddins();

	// addin 访问
	static int GetNumOfAddins();
	static structAddin* GetAddinAt(const int c_index);
	// 当前操作
	void SetCrntOperation(ASOperation* opera);
	ASOperation* GetCrntOperation();


private:
	// 唯一对象
	static ASFModelAddins* ms_SelfPointer;
	ASOperation* m_CrntOperation = nullptr;

	QList<structAddin*>* m_listAddins;

	// 读取配置文件初始化
	void InitConfigurationFile();

private slots:


};
