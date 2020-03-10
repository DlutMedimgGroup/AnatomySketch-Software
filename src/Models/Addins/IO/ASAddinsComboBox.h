#pragma once

#include <QComboBox>

class ASAddinsComboBox;

typedef std::function<void(ASAddinsComboBox*)> Fun;

class ASAddinsComboBox : public QComboBox
{
	Q_OBJECT
public:
	ASAddinsComboBox(QWidget* parent = nullptr);
	~ASAddinsComboBox();

	void SetCallBack(Fun f);

private:

	Fun m_f = nullptr;

private slots:

	void slotCrntIndexChanged(int i);
};
