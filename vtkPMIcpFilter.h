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

