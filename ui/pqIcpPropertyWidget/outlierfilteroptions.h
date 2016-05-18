#ifndef OUTLIERFILTEROPTIONS_H
#define OUTLIERFILTEROPTIONS_H

#include <string>

class OutlierFilterOptions
{
public:
    OutlierFilterOptions();

    // All possible options from all filters
    // GenericDescriptorOutlierFilter
    unsigned source;
    std::string descName;
    bool useSoftThreshold;
    bool useLargerThan;
    double threshold;
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

#endif // OUTLIERFILTEROPTIONS_H
