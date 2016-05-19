#ifndef PQICPPROPERTYWIDGET_H
#define PQICPPROPERTYWIDGET_H

// Set this flag to allow building the widget from Qt Creator
#ifdef BUILD_FROM_QT_CREATOR
#include <QWidget>
#else
#include "pqPropertyWidget.h"
#endif // BUILD_FROM_QT_CREATOR

#include <outlierfilteroptions.h>

namespace Ui {
class pqIcpPropertyWidget;
}

class QListWidgetItem;

#ifdef BUILD_FROM_QT_CREATOR
class pqIcpPropertyWidget : public QWidget
#else
class pqIcpPropertyWidget : public pqPropertyWidget
#endif // BUILD_FROM_QT_CREATOR
{
    Q_OBJECT
#ifdef BUILD_FROM_QT_CREATOR
public:
    explicit pqIcpPropertyWidget(QWidget *parent = 0);
    ~pqIcpPropertyWidget();
#else
    typedef pqPropertyWidget Superclass;
public:
    pqIcpPropertyWidget(
        vtkSMProxy *smproxy, vtkSMProperty *smproperty, QWidget *parentObject=0);
    virtual ~pqIcpPropertyWidget();
#endif // BUILD_FROM_QT_CREATOR

private slots:
    // Matcher
    void on_matcherTypeComboBox_currentIndexChanged(const QString &arg1);

    // Error Minimizer
    void on_errorMinimizerTypeComboBox_currentIndexChanged(const QString &arg1);

    // Outlier Filter
    void on_outlierFilterAddPushButton_clicked();

    void on_outlierFilterRemovePushButton_clicked();

    void on_outlierFilterListWidget_itemSelectionChanged();

    void on_outlierFilterUpPushButton_clicked();

    void on_outlierFilterDownPushButton_clicked();

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

    // Transformation Checker
    void on_transCheckerListWidget_itemChanged(QListWidgetItem *item);

    void on_transCheckerUpPushButton_clicked();

    void on_transCheckerDownPushButton_clicked();

private:
    Ui::pqIcpPropertyWidget *ui;
    typedef std::vector<OutlierFilterOptions> OutlierFilterOptionVector;
    OutlierFilterOptionVector outlierFilterOptionVector;

    void updateOutlierFilterOptionWidgets();
#ifndef BUILD_FROM_QT_CREATOR
private:
  Q_DISABLE_COPY(pqIcpPropertyWidget);
#endif // BUILD_FROM_QT_CREATOR
};

#endif // PQICPPROPERTYWIDGET_H
