/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMMaxDistFilter.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
  
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPMMaxDistFilter -
// .SECTION Description
//

#ifndef __vtkPMMaxDistFilter_h
#define __vtkPMMaxDistFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>


class  VTKPMFILTERS_EXPORT vtkPMMaxDistFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMMaxDistFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMMaxDistFilter *New();

  vtkSetMacro(Dim, int);
  vtkGetMacro(Dim, int);

  vtkSetMacro(MaxDist, double);
  vtkGetMacro(MaxDist, double);

protected:

  int Dim;
  double MaxDist;

  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector);

  vtkPMMaxDistFilter();
  virtual ~vtkPMMaxDistFilter();

private:
  vtkPMMaxDistFilter(const vtkPMMaxDistFilter&);  // Not implemented.
  void operator=(const vtkPMMaxDistFilter&);  // Not implemented.
};

#endif


