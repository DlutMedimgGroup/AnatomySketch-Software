#pragma once

#include <QObject>
#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QStringListModel>
#include <QCompleter>
#include <QEvent>
#include <QKeyEvent>
#include <QAbstractItemView>

class ASDialogNewLM : public QDialog
{
	Q_OBJECT

public:
	ASDialogNewLM(QWidget *parent);
	~ASDialogNewLM();

	QString m_result;
private:
	QLineEdit* m_LineEdit;
	QPushButton* m_ok;
	QPushButton* m_cancel;
	QStringListModel listmodel;
	QCompleter completer;

private slots:

	// 确定
	void slotOK();
};

/*
QlineEdit 自动补全时，在有补全下拉菜单时按下回车会先触发LineEdit的editingFinished信号，
后触发QCompleter的activated信号。activated事件会将LineEdit中的内容补齐，所以此时在
editingFinished对应的槽中清空QlineEdit内容是无效的。
这是使用事件过滤器将activated事件无效（按下回车键不会触发activated信号），并发送信号给
清空QlineEdit内容的槽，完成清空操作。
*/

class EventFilterLM : public QObject
{
	Q_OBJECT
public:
	EventFilterLM(QLineEdit* lineEdit, QObject* parent = NULL)
		:QObject(parent)
		, mLineEdit(lineEdit)
	{ }
	virtual ~EventFilterLM()
	{ }

	bool eventFilter(QObject* watched, QEvent* event)
	{
		QAbstractItemView* view = qobject_cast<QAbstractItemView*>(watched);
		if (event->type() == QEvent::KeyPress)
		{
			QKeyEvent* keyEvent = dynamic_cast<QKeyEvent*>(event);
			if (keyEvent->key() == Qt::Key_Return ||
				keyEvent->key() == Qt::Key_Enter)
			{
				//mLineEdit->clear();
				view->hide();
				emit EnterPress();
				return true;
			}
		}
		return false;
	}

private:
	QLineEdit* mLineEdit;
signals:
	void EnterPress();
};