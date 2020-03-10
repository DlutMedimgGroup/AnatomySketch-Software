#include "ASTransGlobalKeyBoard.h"

ASTransGlobalKeyBoard* ASTransGlobalKeyBoard::ms_GlobalKeyBoard = nullptr;

ASTransGlobalKeyBoard::ASTransGlobalKeyBoard(QObject *parent)
	: ASTransmissionBase(parent)
{
	ms_GlobalKeyBoard = this;
}

ASTransGlobalKeyBoard::~ASTransGlobalKeyBoard()
{

}

// 全局对象
ASTransGlobalKeyBoard* ASTransGlobalKeyBoard::GetSelfPointer()
{
	return ms_GlobalKeyBoard;
}
// 输入函数
void ASTransGlobalKeyBoard::GlobalKeyBoard(QKeyEvent* pKeyEvent)
{
	emit ms_GlobalKeyBoard->signalGlobalKeyBoard(pKeyEvent);
}
