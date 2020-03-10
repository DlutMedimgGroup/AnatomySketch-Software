#pragma once

#include <QObject>
#include "ASDataManagerAction.h"

class ASDataManagerAction;
class ASAbstractData;

class ASMarkEleRClickMenu : public QObject
{
	Q_OBJECT

public:
	ASMarkEleRClickMenu(QObject *parent = nullptr);
	~ASMarkEleRClickMenu();

	void Generate(const QPoint c_pos, ASAbstractData* DataNode, const int c_MarkEleID);

private:

	// 右键菜单
	ASDataManagerAction* m_RARemove;
	ASDataManagerAction* m_RAFocusOn;
	// 新建数据
	ASDataManagerAction* m_RANewContour;
	ASDataManagerAction* m_RANewSeeds;
	ASDataManagerAction* m_RANewROI;

	// 右键菜单初始化
	void RightClickInit();
	// 右键菜单动作
	void RightClickMenuAction(ASAbstractData* DataNode, enumActionInContextMenu action, 
		const int c_MarkEleID);

signals:


private slots:


};
