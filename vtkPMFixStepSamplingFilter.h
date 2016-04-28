/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkPMFixStepSamplingFilter.h

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPMFixStepSamplingFilter -
// .SECTION Description
//

#ifndef __vtkPMFixStepSamplingFilter_h
#define __vtkPMFixStepSamplingFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>


class  VTKPMFILTERS_EXPORT vtkPMFixStepSamplingFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMFixStepSamplingFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMFixStepSamplingFilter *New();

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

  vtkPMFixStepSamplingFilter();
  virtual ~vtkPMFixStepSamplingFilter();

private:
  vtkPMFixStepSamplingFilter(const vtkPMFixStepSamplingFilter&);  // Not implemented.
  void operator=(const vtkPMFixStepSamplingFilter&);  // Not implemented.
};

#endif


