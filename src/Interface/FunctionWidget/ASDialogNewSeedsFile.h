#pragma once

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QPushButton>
#include <QColor>

class ASDataManager;

class ASDialogNewSeedsFile : public QDialog
{
	Q_OBJECT

public:
	ASDialogNewSeedsFile(QWidget *parent);
	~ASDialogNewSeedsFile();

	QString m_NameOfNewSeeds;
	int m_nLabel = 0;
	QColor m_Color;

private:

	//界面元素
	QLabel*				m_LabelName;				// "Name:"
	QLabel*				m_LabelValue;				// "Value:"
	QLabel*				m_LabelColor;				// "Color:"
	QLineEdit*			m_LineEditNameInput;		// 输入标签名
	QSpinBox*			m_SpinBoxValueInput;		// 输入标签值
	QLabel*				m_LabelColorDisplay;		// 显示当前标签所对应的颜色
	QPushButton*		m_PushButtonCancel;			// 取消
	QPushButton*		m_PushButtonOK;				// 导入

	//GUI init
	void UIInit();
	//刷新颜色显示label
	void refreshColorDisplay();

private slots:

	void slotLineEdittextChanged();
	void slotSpinBoxValueChanged();

};
