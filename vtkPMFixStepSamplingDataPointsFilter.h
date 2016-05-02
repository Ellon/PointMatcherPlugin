/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMFixStepSamplingDataPointsFilter.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
  
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPMFixStepSamplingDataPointsFilter -
// .SECTION Description
//

#ifndef __vtkPMFixStepSamplingDataPointsFilter_h
#define __vtkPMFixStepSamplingDataPointsFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>


class  VTKPMFILTERS_EXPORT vtkPMFixStepSamplingDataPointsFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMFixStepSamplingDataPointsFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMFixStepSamplingDataPointsFilter *New();

  vtkSetMacro(StartStep, unsigned);
  vtkGetMacro(StartStep, unsigned);

  vtkSetMacro(EndStep, unsigned);
  vtkGetMacro(EndStep, unsigned);

  vtkSetMacro(StepMult, double);
  vtkGetMacro(StepMult, double);

protected:

  unsigned StartStep;
  unsigned EndStep;
  double StepMult;

  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector);

  vtkPMFixStepSamplingDataPointsFilter();
  virtual ~vtkPMFixStepSamplingDataPointsFilter();

private:
  vtkPMFixStepSamplingDataPointsFilter(const vtkPMFixStepSamplingDataPointsFilter&);  // Not implemented.
  void operator=(const vtkPMFixStepSamplingDataPointsFilter&);  // Not implemented.
};

#endif


