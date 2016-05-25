/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMIcpFilter.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
// .NAME vtkPMIcpFilter -
// .SECTION Description
//

#ifndef __vtkPMIcpFilter_h
#define __vtkPMIcpFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>

#include <outlierFilterOptions.h>

#include <algorithm>
#include <iostream>

class  VTKPMFILTERS_EXPORT vtkPMIcpFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMIcpFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMIcpFilter *New();

  enum MatcherType {
    kKDTreeMatcher=0,
    kKDTreeVarDistMatcher,
    kNullMatcher
  };

  enum ErrorMinimizerType {
    kIdentityErrorMinimizer=0,
    kPointToPlaneErrorMinimizer,
    kPointToPlaneWithCovErrorMinimizer,
    kPointToPointErrorMinimizer,
    kPointToPointSimilarityErrorMinimizer,
    kPointToPointWithCovErrorMinimizer
  };

  // Matcher get/set methods
  vtkSetMacro(Matcher, int);
  vtkGetMacro(Matcher, int);

  vtkSetMacro(Knn, int);
  vtkGetMacro(Knn, int);

  vtkSetMacro(Epsilon, double);
  vtkGetMacro(Epsilon, double);

  vtkSetMacro(SearchType, int);
  vtkGetMacro(SearchType, int);

  vtkSetMacro(MaxDist, double);
  vtkGetMacro(MaxDist, double);

  /// FIXME make this function receive only 'name' parameter.
  ///       Somehow paraview calls with the extra arguments when the name comes from input arrays.
  virtual void SetMaxDistField(int idx, int port, int connection, int fieldAssociation, const char *name);
  vtkGetStringMacro(MaxDistField);

  // Outlier Filter get/clean methods
  void AddOutlierFilter(
    const char* filterName, 
    const char* source, 
    const char* descriptor, 
    double threshold, 
    int useSoftThreshold, 
    int useLargerThan, 
    double maxDist, 
    double factor, 
    double minDist, 
    double maxAngle, 
    double ratio, 
    double minRatio, 
    double maxRatio, 
    double lambda);

  void CleanOutlierFilters();

  // Error Minimizer get/set methods
  vtkSetMacro(ErrorMinimizer, int);
  vtkGetMacro(ErrorMinimizer, int);

  vtkSetMacro(SensorStdDev, double);
  vtkGetMacro(SensorStdDev, double);

  vtkSetMacro(Force2D, int);
  vtkGetMacro(Force2D, int);

  // Transformation Checkers get/set methods
  vtkSetMacro(UseBoundTransformationChecker, int);
  vtkGetMacro(UseBoundTransformationChecker, int);

  vtkSetMacro(UseCounterTransformationChecker, int);
  vtkGetMacro(UseCounterTransformationChecker, int);

  vtkSetMacro(UseDifferentialTransformationChecker, int);
  vtkGetMacro(UseDifferentialTransformationChecker, int);

  vtkSetMacro(MaxRotationNorm, double);
  vtkGetMacro(MaxRotationNorm, double);

  vtkSetMacro(MaxTranslationNorm, double);
  vtkGetMacro(MaxTranslationNorm, double);

  vtkSetMacro(MaxIterationCount, double);
  vtkGetMacro(MaxIterationCount, double);

  vtkSetMacro(MinDiffRotationError, double);
  vtkGetMacro(MinDiffRotationError, double);

  vtkSetMacro(MinDiffTranslationError, double);
  vtkGetMacro(MinDiffTranslationError, double);

  vtkSetMacro(SmoothLength, int);
  vtkGetMacro(SmoothLength, int);

protected:
  // Matcher
  int Matcher;
  int Knn;
  double Epsilon;
  int SearchType;
  double MaxDist;
  char* MaxDistField;
  // Outlier Filter
  typedef std::vector<OutlierFilterOptions> OutlierFilterOptionVector;
  OutlierFilterOptionVector outlierFilterOptionVector;
  // Error Minimizer
  int ErrorMinimizer;
  double SensorStdDev;
  int Force2D;
  // Transformation Checkers
  int UseBoundTransformationChecker;
  int UseCounterTransformationChecker;
  int UseDifferentialTransformationChecker;
  double MaxRotationNorm;
  double MaxTranslationNorm;
  double MaxIterationCount;
  double MinDiffRotationError;
  double MinDiffTranslationError;
  int SmoothLength;

  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector);

  vtkPMIcpFilter();
  virtual ~vtkPMIcpFilter();

private:
  vtkPMIcpFilter(const vtkPMIcpFilter&);  // Not implemented.
  void operator=(const vtkPMIcpFilter&);  // Not implemented.
};

#endif

