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

private:
    Ui::pqIcpPropertyWidget *ui;
    typedef std::vector<OutlierFilterOptions> OutlierFilterOptionVector;
    OutlierFilterOptionVector outlierFilterOptionVector;

    void updateOutlierFilterOptionWidgets();
};

#endif // PQICPPROPERTYWIDGET_H
