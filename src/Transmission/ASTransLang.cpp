#include "ASTransLang.h"
#include "ASTransSetting.h"
#include <QSettings>

ASTransLang* ASTransLang::ms_LangChange = nullptr;
bool ASTransLang::default_lang = NULL;

ASTransLang::ASTransLang(QObject* parent)
	: ASTransmissionBase(parent)
{
	ms_LangChange = this;
	// ¶ÁÈ¡ÓïÑÔÅäÖÃ
	default_lang = ASTransSetting::GetSetting()->value("DefaultLang").toBool();
}

ASTransLang::~ASTransLang()
{
}

// Î¨Ò»¶ÔÏó
ASTransLang* ASTransLang::GetSelfPointer()
{
	return ms_LangChange;
}

bool ASTransLang::GetLang()
{
	return default_lang;
}

void ASTransLang::SetLang(bool checked)
{
	default_lang = checked;
}

// ·¢ËÍÓïÑÔ¸ü»»ÐÅºÅ
void ASTransLang::LanguageChange(bool checked)
{
	emit ms_LangChange->signalLanguageChange(checked);
}

