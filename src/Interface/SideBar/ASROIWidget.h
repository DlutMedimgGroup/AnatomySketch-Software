#pragma once

#include "ASWidgetInSideBarBase.h"
#include <QComboBox>
#include <QListWidget>
#include <QTabWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QDoubleSpinBox>
#include <QCompleter>
#include <QStringListModel>
#include <QEvent>
#include <QKeyEvent>
#include <QPoint>
#include <QMenu>
#include <QAction>

class ASROIData;
class ASROIItemWidget;

class ASROIWidget : public ASWidgetInSideBarBase
{
	Q_OBJECT

public:
	ASROIWidget(QWidget *parent);
	~ASROIWidget();

	virtual void SetEnabled(const bool c_bEnabled);

private:

	// 界面元素
	// 0
	QTabWidget*		m_TabWTools;
	// 1 方形ROI
	QLineEdit*		m_CubeName;
	QCompleter*		m_CubeCmlt;
	QStringListModel* m_CubeStrLiMo;
	QDoubleSpinBox*	m_CubeRange[6]; // 0: XMin 1: XMax 2: YMin 3: YMax 4: ZMin 5: ZMax
	QPushButton* m_BtnFinish;

	bool m_FlagFromAddin = false;
	// 初始化函数
	void UIInit();
	void CompleterInit();
	void NewROI(QString name, int label, QColor color);
	

private slots:

	// Cube ROI 新建ROI
	void slotCubeNameReturn();
	// 刷新Cube页
	void slotRefreshCubeWidget();
	// Cube页输入参数
	void slotCubeValueChanged();
	// 从插件界面中新建数据
	//void slotNewFromAddin(int c_Type, QString name, int label, QColor color);
	// Finish
	void slotFinish();

signals:

};

/*
QlineEdit 自动补全时，在有补全下拉菜单时按下回车会先触发LineEdit的editingFinished信号，
后触发QCompleter的activated信号。activated事件会将LineEdit中的内容补齐，所以此时在
editingFinished对应的槽中清空QlineEdit内容是无效的。
这是使用事件过滤器将activated事件无效（按下回车键不会触发activated信号），并发送信号给
清空QlineEdit内容的槽，完成清空操作。
*/

class EventFilterROI : public QObject
{
	Q_OBJECT
public:
	EventFilterROI(QLineEdit* lineEdit, QObject* parent = NULL)
		:QObject(parent)
		, mLineEdit(lineEdit)
	{ }
	virtual ~EventFilterROI()
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