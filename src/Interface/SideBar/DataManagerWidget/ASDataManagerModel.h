#pragma once

#include <QStandardItemModel>
#include <QStandardItem>

class ASAbstructItemWidget;
class ASDataNodeItemWidget;

class ASDataManagerModel : public QStandardItemModel
{
	Q_OBJECT

public:
	ASDataManagerModel(QObject *parent);
	~ASDataManagerModel();

	// 清空列表
	void RemoveAllItem();
	// 在列表中添加一个结构体
	void addItemWidget(ASAbstructItemWidget* structitem);
	// 由结构体中QStandardItem的值得遍历得到结构体本身的指针
	ASAbstructItemWidget* getItemWidgetFromItem(QStandardItem* item);
	// 由ID值遍历得到结构体本身的指针
	ASAbstructItemWidget* getItemWidgetFromID(int ID);
	// 得到所有结构体来遍历
	QList<ASAbstructItemWidget*>* getListItemInListWidget();

private:

	//储存列表中元素的链表
	QList<ASAbstructItemWidget*>* m_plistItemWidget;
	
	Qt::DropActions ASDataManagerModel::supportedDropActions() const
	{
		return Qt::MoveAction;
	}

	bool dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent);

signals:

	//拖放操作完成时进行相应操作
	void signalDragDropFinished(int row, int column, const QModelIndex* parent);
};
