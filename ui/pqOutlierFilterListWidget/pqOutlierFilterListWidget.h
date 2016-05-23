#ifndef PQOUTLIERFILTERLISTWIDGET_H
#define PQOUTLIERFILTERLISTWIDGET_H

// Set this flag to allow building the widget from Qt Creator
#ifdef BUILD_FROM_QT_CREATOR
#include <QWidget>
#else
#include "pqPropertyWidget.h"
#endif // BUILD_FROM_QT_CREATOR

namespace Ui {
class pqOutlierFilterListWidget;
}

#ifdef BUILD_FROM_QT_CREATOR
class pqOutlierFilterListWidget : public QWidget
#else
class pqOutlierFilterListWidget : public pqPropertyWidget
#endif // BUILD_FROM_QT_CREATOR
{
    Q_OBJECT

#ifdef BUILD_FROM_QT_CREATOR
public:
    explicit pqOutlierFilterListWidget(QWidget *parent = 0);
    ~pqOutlierFilterListWidget();
#else
    typedef pqPropertyWidget Superclass;
public:
    pqOutlierFilterListWidget(
        vtkSMProxy *smproxy, vtkSMProperty *smproperty, QWidget *parentObject=0);
    virtual ~pqOutlierFilterListWidget();
#endif // BUILD_FROM_QT_CREATOR

signals:
    void itemAdded(QString itemName);
    void itemMovedUp(int oldPos);
    void itemMovedDown(int oldPos);
    void itemRemoved(int old);
    void currentSelectionChanged(QString currentItemName);

private slots:
    void on_outlierFilterAddPushButton_clicked();

    void on_outlierFilterUpPushButton_clicked();

    void on_outlierFilterDownPushButton_clicked();

    void on_outlierFilterRemovePushButton_clicked();

    void on_outlierFilterListWidget_itemSelectionChanged();

private:
    Ui::pqOutlierFilterListWidget *ui;

#ifndef BUILD_FROM_QT_CREATOR
private:
  Q_DISABLE_COPY(pqOutlierFilterListWidget);
#endif // BUILD_FROM_QT_CREATOR
};

#endif // PQOUTLIERFILTERLISTWIDGET_H
