#pragma once

#include "ASDataManagerAction.h"
#include <QTreeView>
#include <QWidget>
#include <QStandardItemModel>

class ASDataManagerView;
class ASDataManagerModel;
class ASDataManagerDelegate;
class ASAbstractData;

class ASDataManagerWidget : public QWidget
{
	Q_OBJECT

public:
	ASDataManagerWidget(QWidget *parent);
	~ASDataManagerWidget();

private:

	QTreeView* m_pDataTreeView;
	QStandardItemModel* m_pDataItemModel;

	// MVC组成部分
	ASDataManagerView* m_pDataManagerView;
	ASDataManagerModel* m_pDataManagerModel;
	ASDataManagerDelegate* m_pDataManagerDelegate;

	// 辅助函数，刷新数据列表显示时先根遍历树的迭代函数
	void addNoteDisplay(ASAbstractData* note, int IDOfFather, QStandardItem* fatherItem);
	// 辅助函数，刷新Seed等数据的子条目
	void refreshMarkEleNode(ASAbstractData* note, QStandardItem* crntItem);
	// 辅助函数，取消某类MarkEle的选择
	void ResetMarkEleFocus(const int c_Type);
	// 辅助函数，从QKeyEvent中得到数字
	int GetNumFromKeyEvent(QKeyEvent* pKeyEvent);

signals:

private slots:

	// 刷新文件树显示
	void slotRefreshDataManagerWidget();
	// 拖放操作完成
	void slotDragDropFinished(int row, int column, const QModelIndex* parent);
	// 当前选中item发生改变
	void slotCrntItemSelectionChanged(const QItemSelection &selected, const QItemSelection &deselected);
	// 刷新当前数据显示，用于切换当前数据时
	void slotRefreshCrntData();
	// 单击切换当前工具
	void slotItemClicked(QStandardItem* item);
	// 数据是否显示主动改变
	void slotDataShownChanged(ASAbstractData* DataNode);
	// 接收全局键盘信号
	void slotGlobalKeyBoard(QKeyEvent* pKeyEvent);
};
