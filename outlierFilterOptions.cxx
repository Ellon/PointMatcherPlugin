#include <outlierFilterOptions.h>

OutlierFilterOptions::OutlierFilterOptions() :
    filterName(""),
    // GenericDescriptorOutlierFilter
    source("reference"),
    descriptor(""),
    threshold(0.1),
    useSoftThreshold(false),
    useLargerThan(true),
    // MaxDistOutlierFilter
    maxDist(1),
    // MedianDistOutlierFilter
    factor(3),
    // MinDistOutlierFilter
    minDist(1),
    // SurfaceNormalOutlierFilter
    maxAngle(1.57),
    // TrimmedDistOutlierFilter
    ratio(0.85),
    // VarTrimmedDistOutlierFilter
    minRatio(0.05),
    maxRatio(0.99),
    lambda(0.95)
{
}
