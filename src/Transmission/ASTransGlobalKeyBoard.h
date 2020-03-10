#pragma once
#include "ASTransmissionBase.h"
#include <QKeyEvent>

class ASTransGlobalKeyBoard : public ASTransmissionBase
{
	Q_OBJECT

public:
	ASTransGlobalKeyBoard(QObject *parent = 0);
	~ASTransGlobalKeyBoard();

	// 全局对象
	static ASTransGlobalKeyBoard* GetSelfPointer();
	// 输入函数
	static void GlobalKeyBoard(QKeyEvent* pKeyEvent);

private:
	// 唯一对象
	static ASTransGlobalKeyBoard* ms_GlobalKeyBoard;

signals:
	// 发送全局键盘信号
	void signalGlobalKeyBoard(QKeyEvent* pKeyEvent);
};
