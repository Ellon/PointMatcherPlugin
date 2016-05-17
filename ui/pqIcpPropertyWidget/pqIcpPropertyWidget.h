#ifndef PQICPPROPERTYWIDGET_H
#define PQICPPROPERTYWIDGET_H

#include <QWidget>

namespace Ui {
class pqIcpPropertyWidget;
}

class pqIcpPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit pqIcpPropertyWidget(QWidget *parent = 0);
    ~pqIcpPropertyWidget();

private:
    Ui::pqIcpPropertyWidget *ui;
};

#endif // PQICPPROPERTYWIDGET_H
