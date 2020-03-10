#ifndef ASHELPWIDGET_H
#define ASHELPWIDGET_H

#include <QWidget>
#include "QtWebEngineWidgets/QWebEngineView"

namespace Ui {
class ASHelpWidget;
}

class ASHelpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ASHelpWidget(QWidget *parent = 0);
    ~ASHelpWidget();

private:
    Ui::ASHelpWidget *ui;

};

#endif // ASHELPWIDGET_H
