#include "pqOutlierFilterListWidget.h"
#include "ui_pqOutlierFilterListWidget.h"

#ifndef BUILD_FROM_QT_CREATOR
#include <vtkSMPropertyGroup.h>
#endif

#ifdef BUILD_FROM_QT_CREATOR
pqOutlierFilterListWidget::pqOutlierFilterListWidget(QWidget *parent) :
    QWidget(parent),
#else
pqOutlierFilterListWidget::pqOutlierFilterListWidget(
  vtkSMProxy *smproxy, vtkSMPropertyGroup *smgroup, QWidget *parentObject)
  : Superclass(smproxy, parentObject),
#endif // BUILD_FROM_QT_CREATOR
    ui(new Ui::pqOutlierFilterListWidget)
{
    ui->setupUi(this);

    vtkSMProperty* smproperty = smgroup->GetProperty("OutlierFilters");
    if (smproperty)
    {
        this->addPropertyLink(
            this, "outlierFilters", SIGNAL(outlierFiltersChanged(const QStringList &)), smproperty);
    }
    else
    {
        qCritical("Missing 'OutlierFilterList' property. Widget may not function correctly.");
    }

    // To initialize with right widgets enabled
    on_outlierFilterListWidget_itemSelectionChanged();
}

pqOutlierFilterListWidget::~pqOutlierFilterListWidget()
{
    delete ui;
}

void pqOutlierFilterListWidget::updateOutlierFilterOptionWidgets()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        OutlierFilterOptions &currentOptions = outlierFilterOptionVector[currentRow];
        // GenericDescriptorOutlierFilter
        ui->outlierFilterGenericDescriptorSourceComboBox->setCurrentIndex( (currentOptions.source == "reference"? 0 : 1) );
        ui->outlierFilterGenericDescriptorDescriptorLineEdit->setText(QString(currentOptions.descriptor.c_str()));
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

void pqOutlierFilterListWidget::on_outlierFilterAddPushButton_clicked()
{
    QString itemName = ui->outlierFilterTypeComboBox->currentText();
    new QListWidgetItem(itemName, ui->outlierFilterListWidget);
    this->outlierFilterOptionVector.push_back(OutlierFilterOptions());
    this->outlierFilterOptionVector.back().filterName = itemName.toStdString();
#ifndef BUILD_FROM_QT_CREATOR
    pqPropertyWidget::changeAvailable();
#endif
    emit this->itemAdded(itemName);
    emit this->outlierFiltersChanged(outlierFilters());
}

void pqOutlierFilterListWidget::on_outlierFilterRemovePushButton_clicked()
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        QListWidgetItem* currentItem = ui->outlierFilterListWidget->takeItem(currentRow);
        delete currentItem;

        OutlierFilterOptionVector::iterator it = this->outlierFilterOptionVector.begin();
        it+= currentRow;
        this->outlierFilterOptionVector.erase(it);

#ifndef BUILD_FROM_QT_CREATOR
    pqPropertyWidget::changeAvailable();
#endif
        emit this->itemRemoved(currentRow);
        emit this->outlierFiltersChanged(outlierFilters());
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

        std::swap(this->outlierFilterOptionVector[currentRow-1],this->outlierFilterOptionVector[currentRow]);

#ifndef BUILD_FROM_QT_CREATOR
    pqPropertyWidget::changeAvailable();
#endif
        updateOutlierFilterOptionWidgets();

        emit this->itemMovedUp(currentRow);
        emit this->outlierFiltersChanged(outlierFilters());
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

        std::swap(this->outlierFilterOptionVector[currentRow+1],this->outlierFilterOptionVector[currentRow]);

#ifndef BUILD_FROM_QT_CREATOR
    pqPropertyWidget::changeAvailable();
#endif
        updateOutlierFilterOptionWidgets();

        emit this->itemMovedDown(currentRow);
        emit this->outlierFiltersChanged(outlierFilters());
    }
}

void pqOutlierFilterListWidget::on_outlierFilterListWidget_itemSelectionChanged()
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
            ui->outlierFilterOptionStackedWidget->hide();
//            ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterNullPage);
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

        this->updateOutlierFilterOptionWidgets();

        emit this->selectedFilterChanged(currentItemName);
    }
    else
    {
        ui->outlierFilterOptionStackedWidget->hide();
//        ui->outlierFilterOptionStackedWidget->setCurrentWidget(ui->outlierFilterNullPage);
    }
}

void pqOutlierFilterListWidget::on_outlierFilterGenericDescriptorSourceComboBox_currentIndexChanged(int index)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].source = (index == 0? "reference" : "reading");
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterGenericDescriptorDescriptorLineEdit_textChanged(const QString &arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].descriptor = arg1.toStdString();
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterGenericDescriptorThresholdDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].threshold = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterGenericDescriptorSoftThresholdCheckBox_clicked(bool checked)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].useSoftThreshold = checked;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterGenericDescriptorLargerThanCheckBox_clicked(bool checked)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].useLargerThan = checked;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterMaxDistMaxDistDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].maxDist = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterMedianDistFactorDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].factor = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterMinDistMinDistDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].minDist = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterSurfaceNormalMaxAngleDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].maxAngle = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterTrimmedDistRatioDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].ratio = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterVarTrimmedDistMinRatioDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].minRatio = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterVarTrimmedDistMaxRatioDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].maxRatio = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}

void pqOutlierFilterListWidget::on_outlierFilterVarTrimmedDistLambdaDoubleSpinBox_valueChanged(double arg1)
{
    int currentRow = ui->outlierFilterListWidget->currentRow();
    if(currentRow >= 0)
    {
        Q_ASSERT(currentRow < (int)outlierFilterOptionVector.size());
        outlierFilterOptionVector[currentRow].lambda = arg1;
#ifndef BUILD_FROM_QT_CREATOR
        pqPropertyWidget::changeAvailable();
#endif
    }
}


QString pqOutlierFilterListWidget::selectedFilter()
{
    QListWidgetItem* currentItem = ui->outlierFilterListWidget->currentItem();
    if(currentItem)
        return currentItem->text();

    // Return empty if no item is selected.
    return QString("");
}

QStringList pqOutlierFilterListWidget::outlierFilters()
{
    QStringList stringList;
    Q_FOREACH(const OutlierFilterOptions &opt, this->outlierFilterOptionVector)
    {
        stringList.push_back( QString(opt.filterName.c_str())       );
        stringList.push_back( QString(opt.source.c_str())           );
        stringList.push_back( QString(opt.descriptor.c_str())       );
        stringList.push_back( QString::number(opt.threshold)        );
        stringList.push_back( QString::number(opt.useSoftThreshold) );
        stringList.push_back( QString::number(opt.useLargerThan)    );
        stringList.push_back( QString::number(opt.maxDist)          );
        stringList.push_back( QString::number(opt.factor)           );
        stringList.push_back( QString::number(opt.minDist)          );
        stringList.push_back( QString::number(opt.maxAngle)         );
        stringList.push_back( QString::number(opt.ratio)            );
        stringList.push_back( QString::number(opt.minRatio)         );
        stringList.push_back( QString::number(opt.maxRatio)         );
        stringList.push_back( QString::number(opt.lambda)           );
    }

    return stringList;
}

void pqOutlierFilterListWidget::setOutlierFilters(const QStringList & stringList)
{
    QString filterNameStr, sourceStr, descriptorStr, thresholdStr, useSoftThresholdStr, 
        useLargerThanStr, maxDistStr, factorStr, minDistStr, maxAngleStr, ratioStr, 
        minRatioStr, maxRatioStr, lambdaStr;

    for(int i=0; i< stringList.size(); i+=14)
    {
        filterNameStr = stringList[i+0];
        sourceStr = stringList[i+1];
        descriptorStr = stringList[i+2];
        thresholdStr = stringList[i+3];
        useSoftThresholdStr = stringList[i+4];
        useLargerThanStr = stringList[i+5];
        maxDistStr = stringList[i+6];
        factorStr = stringList[i+7];
        minDistStr = stringList[i+8];
        maxAngleStr = stringList[i+9];
        ratioStr = stringList[i+10];
        minRatioStr = stringList[i+11];
        maxRatioStr = stringList[i+12];
        lambdaStr = stringList[i+13];

        this->outlierFilterOptionVector.push_back(OutlierFilterOptions());
        OutlierFilterOptions& opt = this->outlierFilterOptionVector.back();
        bool ok = true;
        opt.filterName = filterNameStr.toStdString(); 
        opt.source = sourceStr.toStdString(); 
        opt.descriptor = descriptorStr.toStdString(); 
        opt.threshold = thresholdStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of threshold to double failed!"); }
        opt.useSoftThreshold = useSoftThresholdStr.toInt(&ok); 
        if(ok == false) { qCritical("Conversion of useSoftThreshold to int failed!"); }
        opt.useLargerThan = useLargerThanStr.toInt(&ok); 
        if(ok == false) { qCritical("Conversion of useLargerThan to int failed!"); }
        opt.maxDist = maxDistStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of maxDist to int failed!"); }
        opt.factor = factorStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of factor to double failed!"); }
        opt.minDist = minDistStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of minDist to double failed!"); }
        opt.maxAngle = maxAngleStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of maxAngle to double failed!"); }
        opt.ratio = ratioStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of ratio to double failed!"); }
        opt.minRatio = minRatioStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of minRatio to double failed!"); }
        opt.maxRatio = maxRatioStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of maxRatio to double failed!"); }
        opt.lambda = lambdaStr.toDouble(&ok); 
        if(ok == false) { qCritical("Conversion of lambda to double failed!"); }

        new QListWidgetItem(filterNameStr, ui->outlierFilterListWidget);
    }

    updateOutlierFilterOptionWidgets();
}

void pqOutlierFilterListWidget::resetOutlierFilters()
{
    ui->outlierFilterListWidget->clear();
    this->outlierFilterOptionVector.clear();
}
