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

enum DialogType
{
	enContour,
	enSeed,
	enROI,
	enLandmark,
	enOperation
};

class ASDialogNewElement : public QDialog
{
	Q_OBJECT

public:
	ASDialogNewElement(QWidget *parent, DialogType diatype);
	~ASDialogNewElement();

	QString m_result;
private:
	QLineEdit* m_LineEdit;
	QPushButton* m_ok;
	QPushButton* m_cancel;
	QStringListModel listmodel;
	QCompleter completer;

private slots:

	// ȷ��
	void slotOK();
};

/*
QlineEdit �Զ���ȫʱ�����в�ȫ�����˵�ʱ���»س����ȴ���LineEdit��editingFinished�źţ�
�󴥷�QCompleter��activated�źš�activated�¼��ὫLineEdit�е����ݲ��룬���Դ�ʱ��
editingFinished��Ӧ�Ĳ������QlineEdit��������Ч�ġ�
����ʹ���¼���������activated�¼���Ч�����»س������ᴥ��activated�źţ����������źŸ�
���QlineEdit���ݵĲۣ������ղ�����
*/

class EventFilterElement : public QObject
{
	Q_OBJECT
public:
	EventFilterElement(QLineEdit* lineEdit, QObject* parent = NULL)
		:QObject(parent)
		, mLineEdit(lineEdit)
	{ }
	virtual ~EventFilterElement()
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