/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMMaxDensityDataPointsFilter.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
// .NAME vtkPMMaxDensityDataPointsFilter -
// .SECTION Description
//

#ifndef __vtkPMMaxDensityDataPointsFilter_h
#define __vtkPMMaxDensityDataPointsFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>


class  VTKPMFILTERS_EXPORT vtkPMMaxDensityDataPointsFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMMaxDensityDataPointsFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMMaxDensityDataPointsFilter *New();

  vtkSetMacro(MaxDensity, double);
  vtkGetMacro(MaxDensity, double);

protected:

  double MaxDensity;

  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector);

  vtkPMMaxDensityDataPointsFilter();
  virtual ~vtkPMMaxDensityDataPointsFilter();

private:
  vtkPMMaxDensityDataPointsFilter(const vtkPMMaxDensityDataPointsFilter&);  // Not implemented.
  void operator=(const vtkPMMaxDensityDataPointsFilter&);  // Not implemented.
};

#endif


