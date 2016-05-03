/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMObservationDirectionDataPointsFilter.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
// .NAME vtkPMObservationDirectionDataPointsFilter -
// .SECTION Description
//

#ifndef __vtkPMObservationDirectionDataPointsFilter_h
#define __vtkPMObservationDirectionDataPointsFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>


class  VTKPMFILTERS_EXPORT vtkPMObservationDirectionDataPointsFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMObservationDirectionDataPointsFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMObservationDirectionDataPointsFilter *New();

  vtkSetMacro(X, double);
  vtkGetMacro(X, double);

  vtkSetMacro(Y, double);
  vtkGetMacro(Y, double);

  vtkSetMacro(Z, double);
  vtkGetMacro(Z, double);

protected:

  double X;
  double Y;
  double Z;

  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector);

  vtkPMObservationDirectionDataPointsFilter();
  virtual ~vtkPMObservationDirectionDataPointsFilter();

private:
  vtkPMObservationDirectionDataPointsFilter(const vtkPMObservationDirectionDataPointsFilter&);  // Not implemented.
  void operator=(const vtkPMObservationDirectionDataPointsFilter&);  // Not implemented.
};

#endif


