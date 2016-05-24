#ifndef PQOUTLIERFILTERLISTWIDGET_H
#define PQOUTLIERFILTERLISTWIDGET_H

// Set this flag to allow building the widget from Qt Creator
#ifdef BUILD_FROM_QT_CREATOR
#include <QWidget>
#else
#include "pqPropertyWidget.h"
#endif // BUILD_FROM_QT_CREATOR

#include <outlierFilterOptions.h>

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
    Q_PROPERTY(QStringList outlierFilters READ outlierFilters WRITE setOutlierFilters RESET resetOutlierFilters NOTIFY outlierFiltersChanged)

#ifdef BUILD_FROM_QT_CREATOR
public:
    explicit pqOutlierFilterListWidget(QWidget *parent = 0);
    ~pqOutlierFilterListWidget();
#else
    typedef pqPropertyWidget Superclass;
public:
    pqOutlierFilterListWidget(
        vtkSMProxy *smproxy, vtkSMPropertyGroup *smgroup, QWidget *parentObject=0);
    virtual ~pqOutlierFilterListWidget();
#endif // BUILD_FROM_QT_CREATOR

public:
    QStringList outlierFilters();
    void setOutlierFilters(const QStringList &);
    void resetOutlierFilters();
    QString selectedFilter();

signals:
    void itemAdded(QString itemName);
    void itemMovedUp(int oldPos);
    void itemMovedDown(int oldPos);
    void itemRemoved(int old);
    void selectedFilterChanged(const QString &newFilterName);
    void outlierFiltersChanged(const QStringList &outlierFilters);

private slots:
    void on_outlierFilterAddPushButton_clicked();

    void on_outlierFilterUpPushButton_clicked();

    void on_outlierFilterDownPushButton_clicked();

    void on_outlierFilterRemovePushButton_clicked();

    void on_outlierFilterListWidget_itemSelectionChanged();

    void on_outlierFilterGenericDescriptorSourceComboBox_currentIndexChanged(int index);

    void on_outlierFilterGenericDescriptorDescriptorLineEdit_textChanged(const QString &arg1);

    void on_outlierFilterGenericDescriptorThresholdDoubleSpinBox_valueChanged(double arg1);

    void on_outlierFilterGenericDescriptorSoftThresholdCheckBox_clicked(bool checked);

    void on_outlierFilterGenericDescriptorLargerThanCheckBox_clicked(bool checked);

    void on_outlierFilterMaxDistMaxDistDoubleSpinBox_valueChanged(double arg1);

    void on_outlierFilterMedianDistFactorDoubleSpinBox_valueChanged(double arg1);

    void on_outlierFilterMinDistMinDistDoubleSpinBox_valueChanged(double arg1);

    void on_outlierFilterSurfaceNormalMaxAngleDoubleSpinBox_valueChanged(double arg1);

    void on_outlierFilterTrimmedDistRatioDoubleSpinBox_valueChanged(double arg1);

    void on_outlierFilterVarTrimmedDistMinRatioDoubleSpinBox_valueChanged(double arg1);

    void on_outlierFilterVarTrimmedDistMaxRatioDoubleSpinBox_valueChanged(double arg1);

    void on_outlierFilterVarTrimmedDistLambdaDoubleSpinBox_valueChanged(double arg1);

private:
    Ui::pqOutlierFilterListWidget *ui;
    typedef std::vector<OutlierFilterOptions> OutlierFilterOptionVector;
    OutlierFilterOptionVector outlierFilterOptionVector;

    void updateOutlierFilterOptionWidgets();

#ifndef BUILD_FROM_QT_CREATOR
private:
  Q_DISABLE_COPY(pqOutlierFilterListWidget);
#endif // BUILD_FROM_QT_CREATOR
};

#endif // PQOUTLIERFILTERLISTWIDGET_H
