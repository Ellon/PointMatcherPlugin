#ifndef PQICPPROPERTYWIDGET_H
#define PQICPPROPERTYWIDGET_H

#include <QWidget>
#include <outlierfilteroptions.h>

namespace Ui {
class pqIcpPropertyWidget;
}

class QListWidgetItem;

class pqIcpPropertyWidget : public QWidget
{
    Q_OBJECT

public:
    explicit pqIcpPropertyWidget(QWidget *parent = 0);
    ~pqIcpPropertyWidget();

private slots:
    void on_matcherTypeComboBox_currentIndexChanged(const QString &arg1);

    void on_errorMinimizerTypeComboBox_currentIndexChanged(const QString &arg1);

    void on_outlierFilterAddPushButton_clicked();

    void on_outlierFilterRemovePushButton_clicked();

    void on_outlierFilterListWidget_itemSelectionChanged();

    void on_transCheckerListWidget_itemChanged(QListWidgetItem *item);

    void on_outlierFilterUpPushButton_clicked();

    void on_outlierFilterDownPushButton_clicked();

    void on_transCheckerUpPushButton_clicked();

    void on_transCheckerDownPushButton_clicked();

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
    Ui::pqIcpPropertyWidget *ui;
    typedef std::vector<OutlierFilterOptions> OutlierFilterOptionVector;
    OutlierFilterOptionVector outlierFilterOptionVector;

    void updateOutlierFilterOptionWidgets();
};

#endif // PQICPPROPERTYWIDGET_H
