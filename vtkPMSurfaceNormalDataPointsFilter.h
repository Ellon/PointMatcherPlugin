/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMSurfaceNormalDataPointsFilter.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
  
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
// .NAME vtkPMSurfaceNormalDataPointsFilter -
// .SECTION Description
//

#ifndef __vtkPMSurfaceNormalDataPointsFilter_h
#define __vtkPMSurfaceNormalDataPointsFilter_h

#include <vtkPolyDataAlgorithm.h>
#include <vtkPMFiltersModule.h>


class  VTKPMFILTERS_EXPORT vtkPMSurfaceNormalDataPointsFilter : public vtkPolyDataAlgorithm
{
public:
  vtkTypeMacro(vtkPMSurfaceNormalDataPointsFilter, vtkPolyDataAlgorithm);
  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkPMSurfaceNormalDataPointsFilter *New();

  vtkSetMacro(Knn, unsigned);
  vtkGetMacro(Knn, unsigned);

  vtkSetMacro(Epsilon, double);
  vtkGetMacro(Epsilon, double);

  vtkSetMacro(KeepNormals, bool);
  vtkGetMacro(KeepNormals, bool);

  vtkSetMacro(KeepDensities, bool);
  vtkGetMacro(KeepDensities, bool);

  vtkSetMacro(KeepEigenValues, bool);
  vtkGetMacro(KeepEigenValues, bool);

  vtkSetMacro(KeepEigenVectors, bool);
  vtkGetMacro(KeepEigenVectors, bool);

  vtkSetMacro(KeepMatchedIds, bool);
  vtkGetMacro(KeepMatchedIds, bool);

protected:

  unsigned Knn;
  double Epsilon;
  bool KeepNormals;
  bool KeepDensities;
  bool KeepEigenValues;
  bool KeepEigenVectors;
  bool KeepMatchedIds;

  virtual int RequestData(vtkInformation *request,
                          vtkInformationVector **inputVector,
                          vtkInformationVector *outputVector);

  vtkPMSurfaceNormalDataPointsFilter();
  virtual ~vtkPMSurfaceNormalDataPointsFilter();

private:
  vtkPMSurfaceNormalDataPointsFilter(const vtkPMSurfaceNormalDataPointsFilter&);  // Not implemented.
  void operator=(const vtkPMSurfaceNormalDataPointsFilter&);  // Not implemented.
};

#endif


