#pragma once

#include "ASWidgetInSideBarBase.h"
#include <QLineEdit>
#include <QStringListModel>
#include <QCompleter>
#include <QEvent>
#include <QKeyEvent>
#include <QComboBox>
#include <QAbstractItemView>
#include <QPushButton>

class ASContourItemWidget;

class ASPaintingContourWidget : public ASWidgetInSideBarBase
{
	Q_OBJECT

public:
	ASPaintingContourWidget(QWidget *parent);
	~ASPaintingContourWidget();

	virtual void SetEnabled(const bool c_bEnabled);

private:
	// 界面元素
	QLineEdit* m_LineENewData;
	QComboBox* m_ComboTouchModel;
	QCompleter*	m_Cmlt;
	QStringListModel* m_StrLiMo;
	QPushButton* m_BtnFinish;

	bool m_FlagFromAddin = false;
	// 初始化
	void UIInit();
	void CompleterInit();
	void NewContour(QString name, int label, QColor color);

signals:

private slots :

	// 新建文件
	void slotContourNameReturn();
	// touch model
	void slotTouchModelChanged(int state);
	// 软件当前交互方式改变
	void slotInteractorStyle2DChanged(int interactor);
	//// 从插件界面中新建数据
	//void slotNewFromAddin(int c_Type, QString name, int label, QColor color);
	// Finish
	void slotFinish();
};

/*
QlineEdit 自动补全时，在有补全下拉菜单时按下回车会先触发LineEdit的editingFinished信号，
后触发QCompleter的activated信号。activated事件会将LineEdit中的内容补齐，所以此时在
editingFinished对应的槽中清空QlineEdit内容是无效的。
这是使用事件过滤器将activated事件无效（按下回车键不会触发activated信号），并发送信号给
清空QlineEdit内容的槽，完成清空操作。
*/

class EventFilterContour : public QObject
{
	Q_OBJECT
public:
	EventFilterContour(QLineEdit* lineEdit, QObject* parent = NULL)
		:QObject(parent)
		, mLineEdit(lineEdit)
	{ }
	virtual ~EventFilterContour()
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