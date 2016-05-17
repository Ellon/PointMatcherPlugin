#include "pqIcpPropertyWidget.h"
#include "ui_pqIcpPropertyWidget.h"

pqIcpPropertyWidget::pqIcpPropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pqIcpPropertyWidget)
{
    ui->setupUi(this);
}

pqIcpPropertyWidget::~pqIcpPropertyWidget()
{
    delete ui;
}
