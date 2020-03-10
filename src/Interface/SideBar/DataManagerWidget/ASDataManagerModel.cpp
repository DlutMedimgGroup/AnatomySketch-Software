#include "ASDataManagerModel.h"
#include "ASDataNodeItemWidget.h"
#include "ASAbstructItemWidget.h"
#include <QDebug>

ASDataManagerModel::ASDataManagerModel(QObject *parent)
	: QStandardItemModel(parent)
{
	m_plistItemWidget = new QList<ASAbstructItemWidget*>;

}

ASDataManagerModel::~ASDataManagerModel()
{
}

// 清空列表
void ASDataManagerModel::RemoveAllItem()
{
	this->clear();
	while (m_plistItemWidget->size() > 0)
	{
		//QStandardItemModel会自动管理已经加入其中的各个Item的内存，所以当前面调用clear()时，Item的内存已经被释放
		delete m_plistItemWidget->first();
		m_plistItemWidget->removeFirst();
	}
}
// 在列表中添加一个结构体
void ASDataManagerModel::addItemWidget(ASAbstructItemWidget* structitem)
{
	m_plistItemWidget->append(structitem);
}

ASAbstructItemWidget* ASDataManagerModel::getItemWidgetFromItem(QStandardItem* item)
{
	int numOfItem = m_plistItemWidget->size();
	for (int crntItem = 0; crntItem < numOfItem; crntItem++)
	{
		ASAbstructItemWidget* crntItemWidget = m_plistItemWidget->at(crntItem);
		if (crntItemWidget->GetListWidgetItem() == item)
		{
			return crntItemWidget;
		}
	}
	return nullptr;
}
// 由ID值遍历得到结构体本身的指针
ASAbstructItemWidget* ASDataManagerModel::getItemWidgetFromID(int ID)
{
	int numOfItem = m_plistItemWidget->size();
	for (int crntItem = 0; crntItem < numOfItem; crntItem++)
	{
		ASAbstructItemWidget* crntItemWidget = m_plistItemWidget->at(crntItem);
		if (crntItemWidget->IsMarkEle())
		{
			continue;
		}
		if (crntItemWidget->GetID() == ID)
		{
			return crntItemWidget;
		}
	}
	return nullptr;
}
// 得到所有结构体来遍历
QList<ASAbstructItemWidget*>* ASDataManagerModel::getListItemInListWidget()
{
	return m_plistItemWidget;
}

bool ASDataManagerModel::dropMimeData(const QMimeData * data, Qt::DropAction action, int row, int column, const QModelIndex & parent)
{
	bool a = QStandardItemModel::dropMimeData(data, action, row, column, parent);
	emit signalDragDropFinished(row, column, &parent);
	return a;
}