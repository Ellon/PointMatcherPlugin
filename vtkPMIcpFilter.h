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

class  VTKPMFILTERS_EXPORT vtkPMIcpFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMIcpFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMIcpFilter *New();

protected:

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

