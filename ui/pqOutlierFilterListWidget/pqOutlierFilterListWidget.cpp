#include "pqOutlierFilterListWidget.h"
#include "ui_pqOutlierFilterListWidget.h"


#ifdef BUILD_FROM_QT_CREATOR
pqOutlierFilterListWidget::pqOutlierFilterListWidget(QWidget *parent) :
    QWidget(parent),
#else
pqOutlierFilterListWidget::pqOutlierFilterListWidget(
    vtkSMProxy *smproxy, vtkSMProperty *smproperty, QWidget *parentObject)
    : Superclass(smproxy, parentObject),
#endif // BUILD_FROM_QT_CREATOR
    ui(new Ui::pqOutlierFilterListWidget)
{
#ifndef BUILD_FROM_QT_CREATOR
    this->setShowLabel(false);
#endif // BUILD_FROM_QT_CREATOR

    ui->setupUi(this);
}

pqOutlierFilterListWidget::~pqOutlierFilterListWidget()
{
    delete ui;
}

void pqOutlierFilterListWidget::on_outlierFilterAddPushButton_clicked()
{
    QString itemName = ui->outlierFilterTypeComboBox->currentText();
    new QListWidgetItem(itemName, ui->outlierFilterListWidget);
#ifndef BUILD_FROM_QT_CREATOR
    pqPropertyWidget::changeAvailable();
#endif
    emit this->itemAdded(itemName);
}

void pqOutlierFilterListWidget::on_outlierFilterRemovePushButton_clicked()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        QListWidgetItem* currentItem = ui->outlierFilterListWidget->takeItem(currentRow);
        delete currentItem;
#ifndef BUILD_FROM_QT_CREATOR
    pqPropertyWidget::changeAvailable();
#endif
        emit this->itemRemoved(currentRow);
    }
}

void pqOutlierFilterListWidget::on_outlierFilterUpPushButton_clicked()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow > 0)
    {
        QListWidgetItem* currentItem = ui->outlierFilterListWidget->takeItem(currentRow);
        ui->outlierFilterListWidget->insertItem(currentRow-1,currentItem);
        ui->outlierFilterListWidget->setCurrentItem(currentItem);
#ifndef BUILD_FROM_QT_CREATOR
    pqPropertyWidget::changeAvailable();
#endif
        emit this->itemMovedUp(currentRow);
    }
}

void pqOutlierFilterListWidget::on_outlierFilterDownPushButton_clicked()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    int numItems = ui->outlierFilterListWidget->count();
    if(currentRow >= 0 && currentRow < numItems-1)
    {
        QListWidgetItem* currentItem = ui->outlierFilterListWidget->takeItem(currentRow);
        ui->outlierFilterListWidget->insertItem(currentRow+1,currentItem);
        ui->outlierFilterListWidget->setCurrentItem(currentItem);
#ifndef BUILD_FROM_QT_CREATOR
    pqPropertyWidget::changeAvailable();
#endif
        emit this->itemMovedDown(currentRow);
    }
}

void pqOutlierFilterListWidget::on_outlierFilterListWidget_itemSelectionChanged()
{
    QListWidgetItem* currentItem = ui->outlierFilterListWidget->currentItem();
    if(currentItem)
    {
        QString currentItemName = currentItem->text();
        emit this->currentSelectionChanged(currentItemName);
    }
}
