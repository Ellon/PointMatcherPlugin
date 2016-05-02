/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMConversions.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
//
// .NAME vtkPMConversions - collection of routines to convert PointMatcher::DataPoints to/from vtkPolyData
//
// .SECTION Description
//

#ifndef __vtkPMConversions_h
#define __vtkPMConversions_h

#include <vtkObject.h>
#include <vtkSmartPointer.h>
#include <vtkPMFiltersModule.h>

#include <pointmatcher/PointMatcher.h>

class vtkPolyData;
class vtkCellArray;
class vtkIntArray;

class VTKPMFILTERS_EXPORT vtkPMConversions : public vtkObject
{
public:
  typedef float T;
  // TODO Make the type be defined from a template 
  typedef PointMatcher<T> PM;
        
  static vtkPMConversions* New();
     
  vtkTypeMacro(vtkPMConversions, vtkObject);

  void PrintSelf(ostream& os, vtkIndent indent);

  static vtkSmartPointer<vtkPolyData> PolyDataFromDataPoints(
    const PM::DataPoints &cloud);

  static boost::shared_ptr<PM::DataPoints> DataPointsFromPolyData(
    vtkPolyData* polyData);

  static vtkSmartPointer<vtkCellArray> NewVertexCells(vtkIdType numberOfVerts);

protected:

  vtkPMConversions();
  ~vtkPMConversions();

private:

  vtkPMConversions(const vtkPMConversions&); // Not implemented
  void operator=(const vtkPMConversions&); // Not implemented
};

#endif
