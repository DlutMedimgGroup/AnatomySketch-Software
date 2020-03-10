#pragma once

#include <QWidget>
#include <QPushButton>

class ASHoverWidgetInQVTKWidget : public QWidget
{
	Q_OBJECT

public:
	ASHoverWidgetInQVTKWidget(QWidget *parent, const int c_ViewLabel);
	~ASHoverWidgetInQVTKWidget();
	void setExpanded(const bool c_expanded);

private:
	QPushButton* m_pButtonCrossShair;
	QPushButton* m_pButtonLayout;
	int m_ViewLabel = 0;

protected:
	void paintEvent(QPaintEvent *event);

private slots:
	
	//void slotButtonLayoutClicked();

signals:

	void signalLayoutChanged();
	void signalSetCrossShairShown();

};
