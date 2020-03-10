#pragma once
#include "ASTransmissionBase.h"

class ASTransTheme : public ASTransmissionBase
{
Q_OBJECT

public:
	ASTransTheme(QObject* parent = nullptr);
	~ASTransTheme();

	// 唯一对象
	static ASTransTheme* GetSelfPointer();
	// 默认主题
	static bool GetTheme();
	static int GetToolBtnDown();
	static void SetToolBtnDown(int crntBtnDown);
	static void SetTheme(bool checked);

	// 主题更换
	static void QSSChange(bool checked);
	static void BackgroundChange(bool checked);
	static void OperationbarThemeChange(bool checked);
	static void SidebarChange(bool checked);


private:

	// 唯一对象
	static ASTransTheme* ms_ThemeChange;
	// 默认主题
	static bool default_theme;
	// 按下按钮
	static int toolBtnDown;


signals:

	// 发送主题更换信号
	void signalQSSChange(bool checked);
	void signalOperationbarThemeChange(bool checked);
	void signalBackgroundChange(bool checked);
	void signalSidebarChange(bool checked);
};
