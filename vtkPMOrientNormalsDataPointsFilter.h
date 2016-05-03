/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMOrientNormalsDataPointsFilter.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
// .NAME vtkPMOrientNormalsDataPointsFilter -
// .SECTION Description
//

#ifndef __vtkPMOrientNormalsDataPointsFilter_h
#define __vtkPMOrientNormalsDataPointsFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>


class  VTKPMFILTERS_EXPORT vtkPMOrientNormalsDataPointsFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMOrientNormalsDataPointsFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMOrientNormalsDataPointsFilter *New();

  vtkSetMacro(TowardCenter, bool);
  vtkGetMacro(TowardCenter, bool);

protected:

  bool TowardCenter;

  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector);

  vtkPMOrientNormalsDataPointsFilter();
  virtual ~vtkPMOrientNormalsDataPointsFilter();

private:
  vtkPMOrientNormalsDataPointsFilter(const vtkPMOrientNormalsDataPointsFilter&);  // Not implemented.
  void operator=(const vtkPMOrientNormalsDataPointsFilter&);  // Not implemented.
};

#endif


