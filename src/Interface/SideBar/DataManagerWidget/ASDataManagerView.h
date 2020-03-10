#pragma once

#include "ASDataManagerAction.h"
#include <QTreeView>
#include <QStandardItem>
#include <QWheelEvent>
#include <QStandardItemModel>

class ASAbstractData;
class ASDataNodeRClickMenu;
class ASMarkEleRClickMenu;

class ASDataManagerView : public QTreeView
{
	Q_OBJECT

public:
	ASDataManagerView(QWidget *parent);
	~ASDataManagerView();

	QStandardItem* getDragDropStartItem();

protected:

	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);

private:
	
	ASDataNodeRClickMenu* m_DataNodeRClickMenu;
	ASMarkEleRClickMenu* m_MarkEleRClickMenu;
	QPoint m_DragStartPos;
	QStandardItem* m_pDragDropStartItem;

private slots:

	// 右键点击响应函数，实现右键菜单
	void slotShowContextMenu(const QPoint& pos);
	void slotDevTreeClicked(const QModelIndex &index);

signals:

	// 虚信号，Qt的虚信号需要重新声明才能生效
	void selectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	// 鼠标滚轮缩放刷新数据列表
	void signalRefreshDataManagerWidget();
	// 单击切换当前工具
	void signalItemClicked(QStandardItem* item);
};
