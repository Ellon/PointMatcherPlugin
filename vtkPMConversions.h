/*=========================================================================

  Program:   Visualization Toolkit
  Module:    vtkAnnotateOBBs.cxx

  Copyright (c) Ken Martin, Will Schroeder, Bill Lorensen
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.

     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

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

  // static vtkSmartPointer<vtkPolyData> PolyDataFromDataPointsFile(const std::string& filename);

  static vtkSmartPointer<vtkPolyData> PolyDataFromDataPoints(
    const PM::DataPoints &cloud);

  static boost::shared_ptr<PM::DataPoints> DataPointsFromPolyData(
    vtkPolyData* polyData);

  static vtkSmartPointer<vtkCellArray> NewVertexCells(vtkIdType numberOfVerts);

  // static vtkSmartPointer<vtkIntArray> NewLabelsArray(pcl::IndicesConstPtr indices, vtkIdType length);
  // static vtkSmartPointer<vtkIntArray> NewLabelsArray(pcl::PointIndices::ConstPtr indices, vtkIdType length);
  // static vtkSmartPointer<vtkIntArray> NewLabelsArray(const std::vector<pcl::PointIndices>& indices, vtkIdType length);

  // static void PerformDataPointsConversionBenchmark(vtkPolyData* polyData);

protected:

  vtkPMConversions();
  ~vtkPMConversions();

private:

  vtkPMConversions(const vtkPMConversions&); // Not implemented
  void operator=(const vtkPMConversions&); // Not implemented
};

#endif
