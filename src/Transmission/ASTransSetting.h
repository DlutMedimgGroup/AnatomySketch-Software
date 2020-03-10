#pragma once
#include "ASTransmissionBase.h"
class QSettings;

class ASTransSetting : public ASTransmissionBase
{
Q_OBJECT

public:
	ASTransSetting(QObject* parent = nullptr);
	~ASTransSetting();

	// 唯一对象
	static ASTransSetting* GetSelfPointer();

	// 读取和写入配置
	static QSettings* GetSetting();

private:
	// 唯一对象
	static ASTransSetting* ms_Setting;

	static QSettings* SettingConfig;
};
