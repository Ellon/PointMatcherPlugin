#include "pqIcpPropertyWidget.h"
#include "ui_pqIcpPropertyWidget.h"
#include "impossibleoptionexception.h"

#include <iostream>

using namespace std;

pqIcpPropertyWidget::pqIcpPropertyWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::pqIcpPropertyWidget)
{
    ui->setupUi(this);

    // Setup initial conditions
    on_matcherTypeComboBox_currentIndexChanged(ui->matcherTypeComboBox->currentText());
    on_errorMinimizerTypeComboBox_currentIndexChanged(ui->errorMinimizerTypeComboBox->currentText());
    on_outlierFilterListWidget_itemSelectionChanged();
}

pqIcpPropertyWidget::~pqIcpPropertyWidget()
{
    delete ui;
}

void pqIcpPropertyWidget::on_matcherTypeComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "KDTreeMatcher")
    {
        ui->matcherKnnLabel->setVisible(true);
        ui->matcherKnnSpinBox->setVisible(true);
        ui->matcherEpsilonLabel->setVisible(true);
        ui->matcherEpsilonDoubleSpinBox->setVisible(true);
        ui->matcherSearchTypeLabel->setVisible(true);
        ui->matcherSearchTypeComboBox->setVisible(true);
        ui->matcherMaxDistLabel->setVisible(true);
        ui->matcherMaxDistDoubleSpinBox->setVisible(true);
        ui->matcherMaxDistFieldLabel->setVisible(false);
        ui->matcherMaxDistFieldComboBox->setVisible(false);
    }
    else if(arg1 == "KDTreeVarDistMatcher")
    {
        ui->matcherKnnLabel->setVisible(true);
        ui->matcherKnnSpinBox->setVisible(true);
        ui->matcherEpsilonLabel->setVisible(true);
        ui->matcherEpsilonDoubleSpinBox->setVisible(true);
        ui->matcherSearchTypeLabel->setVisible(true);
        ui->matcherSearchTypeComboBox->setVisible(true);
        ui->matcherMaxDistLabel->setVisible(false);
        ui->matcherMaxDistDoubleSpinBox->setVisible(false);
        ui->matcherMaxDistFieldLabel->setVisible(true);
        ui->matcherMaxDistFieldComboBox->setVisible(true);
    }
    else if(arg1 == "NullMatcher")
    {
        ui->matcherKnnLabel->setVisible(false);
        ui->matcherKnnSpinBox->setVisible(false);
        ui->matcherEpsilonLabel->setVisible(false);
        ui->matcherEpsilonDoubleSpinBox->setVisible(false);
        ui->matcherSearchTypeLabel->setVisible(false);
        ui->matcherSearchTypeComboBox->setVisible(false);
        ui->matcherMaxDistLabel->setVisible(false);
        ui->matcherMaxDistDoubleSpinBox->setVisible(false);
        ui->matcherMaxDistFieldLabel->setVisible(false);
        ui->matcherMaxDistFieldComboBox->setVisible(false);
    }
    else
    {
        // This should never happen...
        throw ImpossibleOptionException();
    }
}

void pqIcpPropertyWidget::on_errorMinimizerTypeComboBox_currentIndexChanged(const QString &arg1)
{
    if(arg1 == "PointToPlaneErrorMinimizer" || arg1 == "PointToPlaneWithCovErrorMinimizer")
        ui->errorMinimizerForce2DCheckBox->setVisible(true);
    else
        ui->errorMinimizerForce2DCheckBox->setVisible(false);

    if(arg1 == "PointToPlaneWithCovErrorMinimizer" || arg1 == "PointToPointWithCovErrorMinimizer")
    {
        ui->errorMinimizerSensorStdDoubleSpinBox->setVisible(true);
        ui->errorMinimizerSensorStdLabel->setVisible(true);
    }
    else
    {
        ui->errorMinimizerSensorStdDoubleSpinBox->setVisible(false);
        ui->errorMinimizerSensorStdLabel->setVisible(false);
    }
}

void pqIcpPropertyWidget::on_outlierFilterAddPushButton_clicked()
{
    new QListWidgetItem(ui->outlierFilterTypeComboBox->currentText(), ui->outlierFilterListWidget);
    outlierFilterOptionVector.push_back(OutlierFilterOptions());
    cout << "outlierFilterOptionVector size is " << outlierFilterOptionVector.size() << endl;
}

void pqIcpPropertyWidget::on_outlierFilterRemovePushButton_clicked()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        QListWidgetItem* currentItem = ui->outlierFilterListWidget->takeItem(currentRow);
        delete currentItem;

        OutlierFilterOptionVector::iterator it = outlierFilterOptionVector.begin();
        it+= currentRow;
        outlierFilterOptionVector.erase(it);
        cout << "outlierFilterOptionVector size is " << outlierFilterOptionVector.size() << endl;
    }
}

void pqIcpPropertyWidget::on_outlierFilterListWidget_itemSelectionChanged()
{
    QListWidgetItem* currentItem = ui->outlierFilterListWidget->currentItem();
    if(currentItem)
    {
        ui->outlierFilterOptionStackedWidget->show();

        QString currentItemName = currentItem->text();
        if(currentItemName == "GenericDescriptorOutlierFilter")
        {
            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterGenericDescriptorPage);
        }
        else if(currentItemName == "MaxDistOutlierFilter")
        {
            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterMaxDistPage);
        }
        else if(currentItemName == "MedianDistOutlierFilter")
        {
            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterMedianDistPage);
        }
        else if(currentItemName == "MinDistOutlierFilter")
        {
            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterMinDistPage);
        }
        else if(currentItemName == "NullOutlierFilter")
        {
//            ui->outlierFilterOptionStackedWidget->hide();
            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterNullPage);
        }
        else if(currentItemName == "SurfaceNormalOutlierFilter")
        {
            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterSurfaceNormalPage);
        }
        else if(currentItemName == "TrimmedDistOutlierFilter")
        {
            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterTrimmedDistPage);
        }
        else if(currentItemName == "VarTrimmedDistOutlierFilter")
        {
            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterVarTrimmedDistPage);
        }
        else
        {
            // This should never happen...
            throw ImpossibleOptionException();
        }

        updateOutlierFilterOptionWidgets();
    }
    else
    {
//        ui->outlierFilterOptionStackedWidget->hide();
        ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterNullPage);
    }
}

void pqIcpPropertyWidget::updateOutlierFilterOptionWidgets()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    cout << "updating outlier filter option widgets..." << endl;
    if(currentRow >= 0)
    {
        OutlierFilterOptions &currentOptions = outlierFilterOptionVector[currentRow];
        // GenericDescriptorOutlierFilter
        ui->outlierFilterGenericDescriptorSourceComboBox->setCurrentIndex(currentOptions.source);
        ui->outlierFilterGenericDescriptorDescriptorLineEdit->setText(QString(currentOptions.descName.c_str()));
        ui->outlierFilterGenericDescriptorSoftThresholdCheckBox->setChecked(currentOptions.useSoftThreshold);
        ui->outlierFilterGenericDescriptorLargerThanCheckBox->setChecked(currentOptions.useLargerThan);
        ui->outlierFilterGenericDescriptorThresholdDoubleSpinBox->setValue(currentOptions.threshold);
        // MaxDistOutlierFilter
        ui->outlierFilterMaxDistMaxDistDoubleSpinBox->setValue(currentOptions.maxDist);
        // MedianDistOutlierFilter
        ui->outlierFilterMedianDistFactorDoubleSpinBox->setValue(currentOptions.factor);
        // MinDistOutlierFilter
        ui->outlierFilterMinDistMinDistDoubleSpinBox->setValue(currentOptions.minDist);
        // SurfaceNormalOutlierFilter
        ui->outlierFilterSurfaceNormalMaxAngleDoubleSpinBox->setValue(currentOptions.maxAngle);
        // TrimmedDistOutlierFilter
        ui->outlierFilterTrimmedDistRatioDoubleSpinBox->setValue(currentOptions.ratio);
        // VarTrimmedDistOutlierFilter
        ui->outlierFilterVarTrimmedDistMinRatioDoubleSpinBox->setValue(currentOptions.minRatio);
        ui->outlierFilterVarTrimmedDistMaxRatioDoubleSpinBox->setValue(currentOptions.maxRatio);
        ui->outlierFilterVarTrimmedDistLambdaDoubleSpinBox->setValue(currentOptions.lambda);
    }
}

void pqIcpPropertyWidget::on_transCheckerListWidget_itemChanged(QListWidgetItem *item)
{
    bool enabled = item->checkState() == Qt::Checked;
    QString itemName = item->text();
    if(itemName == "BoundTransformationChecker")
    {
        ui->transCheckerMaxRotNormLabel->setEnabled(enabled);
        ui->transCheckerMaxRotNormDoubleSpinBox->setEnabled(enabled);
        ui->transCheckerMaxTransNormLabel->setEnabled(enabled);
        ui->transCheckerMaxTransNormDoubleSpinBox->setEnabled(enabled);
    }
    else if(itemName == "CounterTransformationChecker")
    {
        ui->transCheckerMaxIterCountLabel->setEnabled(enabled);
        ui->transCheckerMaxIterCountSpinBox->setEnabled(enabled);
    }
    else if(itemName == "DifferentialTransformationChecker")
    {
        ui->transCheckerMinDiffRotErrorLabel->setEnabled(enabled);
        ui->transCheckerMinDiffRotErrorDoubleSpinBox->setEnabled(enabled);
        ui->transCheckerMinDiffTransErrorLabel->setEnabled(enabled);
        ui->transCheckerMinDiffTransErrorDoubleSpinBox->setEnabled(enabled);
        ui->transCheckerSmoothLengthLabel->setEnabled(enabled);
        ui->transCheckerSmoothLengthSpinBox->setEnabled(enabled);
    }
    else
    {
        // This should never happen...
        throw ImpossibleOptionException();
    }
}

void pqIcpPropertyWidget::on_outlierFilterUpPushButton_clicked()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow > 0)
    {
        QListWidgetItem* currentItem = ui->outlierFilterListWidget->takeItem(currentRow);
        ui->outlierFilterListWidget->insertItem(currentRow-1,currentItem);
        ui->outlierFilterListWidget->setCurrentItem(currentItem);

        std::swap(outlierFilterOptionVector[currentRow-1],outlierFilterOptionVector[currentRow]);

        updateOutlierFilterOptionWidgets();
    }
}

void pqIcpPropertyWidget::on_outlierFilterDownPushButton_clicked()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    int numItems = ui->outlierFilterListWidget->count();
    if(currentRow >= 0 && currentRow < numItems-1)
    {
        QListWidgetItem* currentItem = ui->outlierFilterListWidget->takeItem(currentRow);
        ui->outlierFilterListWidget->insertItem(currentRow+1,currentItem);
        ui->outlierFilterListWidget->setCurrentItem(currentItem);

        std::swap(outlierFilterOptionVector[currentRow+1],outlierFilterOptionVector[currentRow]);

        updateOutlierFilterOptionWidgets();
    }
}

void pqIcpPropertyWidget::on_transCheckerUpPushButton_clicked()
{
    int currentRow = ui->transCheckerListWidget->currentRow();
    if(currentRow > 0)
    {
        QListWidgetItem* currentItem = ui->transCheckerListWidget->takeItem(currentRow);
        ui->transCheckerListWidget->insertItem(currentRow-1,currentItem);
        ui->transCheckerListWidget->setCurrentItem(currentItem);
    }
}

void pqIcpPropertyWidget::on_transCheckerDownPushButton_clicked()
{
    int currentRow = ui->transCheckerListWidget->currentRow();
    int numItems = ui->transCheckerListWidget->count();
    if(currentRow >= 0 && currentRow < numItems-1)
    {
        QListWidgetItem* currentItem = ui->transCheckerListWidget->takeItem(currentRow);
        ui->transCheckerListWidget->insertItem(currentRow+1,currentItem);
        ui->transCheckerListWidget->setCurrentItem(currentItem);
    }
}

void pqIcpPropertyWidget::on_outlierFilterGenericDescriptorSourceComboBox_currentIndexChanged(int index)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].source = index;
    }
}

// TODO connect signals from other outlier filter widgets
