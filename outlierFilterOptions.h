#ifndef OUTLIERFILTEROPTIONS_H
#define OUTLIERFILTEROPTIONS_H

#include <string>

class OutlierFilterOptions
{
public:
    OutlierFilterOptions();

    std::string filterName;
    // All possible options from all filters
    // GenericDescriptorOutlierFilter
    std::string source;
    std::string descriptor;
    double threshold;
    bool useSoftThreshold;
    bool useLargerThan;
    // MaxDistOutlierFilter
    double maxDist;
    // MedianDistOutlierFilter
    double factor;
    // MinDistOutlierFilter
    double minDist;
    // SurfaceNormalOutlierFilter
    double maxAngle;
    // TrimmedDistOutlierFilter
    double ratio;
    // VarTrimmedDistOutlierFilter
    double minRatio;
    double maxRatio;
    double lambda;
};

#endif