#pragma once
#include "ASTransmissionBase.h"

class ASTransLang : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransLang(QObject* parent = nullptr);
	~ASTransLang();

	// 唯一对象
	static ASTransLang* GetSelfPointer();
	// 默认主题
	static bool GetLang();
	static void SetLang(bool checked);

	// 语言更换
	static void LanguageChange(bool checked);


private:

	// 唯一对象
	static ASTransLang* ms_LangChange;
	// 默认语言
	static bool default_lang;


signals:

	// 发送语言更换信号
	void signalLanguageChange(bool checked);

};
