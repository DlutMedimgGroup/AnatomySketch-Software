#pragma once

#include <QVTKWidget.h>
#include <QEvent>

class ASQVTKWidget : public QVTKWidget
{
	Q_OBJECT

public:
	ASQVTKWidget(QWidget *parent = 0);
	~ASQVTKWidget();

	void setViewSelection(int viewselection);

private:

	int m_ViewSelection = 0;

protected:

	bool event(QEvent* pEvent)  Q_DECL_OVERRIDE;
};
