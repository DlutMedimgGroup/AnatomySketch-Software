#include "ASTransSetting.h"
#include <QSettings>

ASTransSetting* ASTransSetting::ms_Setting = nullptr;
QSettings* ASTransSetting::SettingConfig = nullptr;

ASTransSetting::ASTransSetting(QObject* parent)
	: ASTransmissionBase(parent)
{
	ms_Setting = this;
	SettingConfig = new QSettings("./Configurations/Setting.ini", QSettings::IniFormat);
}

ASTransSetting::~ASTransSetting()
{
}

// Œ®“ª∂‘œÛ
ASTransSetting* ASTransSetting::GetSelfPointer()
{
	return ms_Setting;
}

QSettings* ASTransSetting::GetSetting()
{
	return SettingConfig;
}
