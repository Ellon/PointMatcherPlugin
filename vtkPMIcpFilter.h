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

