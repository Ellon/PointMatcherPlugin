/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMMaxDistDataPointsFilter.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
  
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPMMaxDistDataPointsFilter -
// .SECTION Description
//

#ifndef __vtkPMMaxDistDataPointsFilter_h
#define __vtkPMMaxDistDataPointsFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>


class  VTKPMFILTERS_EXPORT vtkPMMaxDistDataPointsFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMMaxDistDataPointsFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMMaxDistDataPointsFilter *New();

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

  vtkPMMaxDistDataPointsFilter();
  virtual ~vtkPMMaxDistDataPointsFilter();

private:
  vtkPMMaxDistDataPointsFilter(const vtkPMMaxDistDataPointsFilter&);  // Not implemented.
  void operator=(const vtkPMMaxDistDataPointsFilter&);  // Not implemented.
};

#endif


