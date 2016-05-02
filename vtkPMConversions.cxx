/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMConversions.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
#include "vtkPMConversions.h"

#include <vtkObjectFactory.h>
#include <vtkPolyData.h>
#include <vtkTimerLog.h>
#include <vtkNew.h>
#include <vtkIdList.h>
#include <vtkCellArray.h>
#include <vtkPoints.h>
#include <vtkDoubleArray.h>
#include <vtkFloatArray.h>
#include <vtkIntArray.h>
#include <vtkPointData.h>

#include <cassert>

using namespace std;

typedef vtkPMConversions::PM PM;

namespace
{

vtkSmartPointer<vtkDataArray> getVtkDataArrayFromDescriptor(const PM::DataPoints & cloud, string descriptorName)
{
  const PM::Matrix descriptor = cloud.getDescriptorViewByName(descriptorName);
  vtkIdType numDesc = descriptor.cols();
  vtkIdType sizeDesc = descriptor.rows();
  vtkSmartPointer<vtkFloatArray> floatArray = vtkSmartPointer<vtkFloatArray>::New();
  floatArray->SetNumberOfComponents(sizeDesc);
  floatArray->SetNumberOfTuples(numDesc);
  // We must set the name to be able to select this data on paraview.
  floatArray->SetName(descriptorName.c_str());
  for (vtkIdType i = 0; i < numDesc; ++i)
  {
    for (vtkIdType j = 0; j < sizeDesc; ++j)
      floatArray->SetComponent(i, j, descriptor.col(i)[j]);
  }

  return floatArray;
}

PM::Matrix getDescriptorMatrixFromVtkDataArray(vtkDataArray * dataArray)
{
  PM::Matrix descriptor;

  if (dataArray)
  {
    vtkIdType sizeDesc = dataArray->GetNumberOfComponents();
    vtkIdType numDesc = dataArray->GetNumberOfTuples();
    descriptor.resize(sizeDesc, numDesc);
    for (vtkIdType i = 0; i < numDesc; i++)
    {
      for (vtkIdType j = 0; j < sizeDesc; j++)
        descriptor.col(i)[j] = (float)dataArray->GetComponent(i, j); // FIXME make it generic on the type of Point Matcher
    }
  }
  else
    descriptor.resize(0, 0); // DataArray is not valid: no descriptor

  return descriptor;
}

}

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPMConversions);

//----------------------------------------------------------------------------
vtkPMConversions::vtkPMConversions()
{
}

//----------------------------------------------------------------------------
vtkPMConversions::~vtkPMConversions()
{
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkPolyData> vtkPMConversions::PolyDataFromDataPoints(const typename PM::DataPoints &cloud)
{
  vtkIdType nr_points = cloud.getNbPoints();

  // First we deal with the features
  // ###############################

  // Initialize vtkPoints structure
  vtkNew<vtkPoints> points;
  points->SetDataTypeToFloat();
  points->SetNumberOfPoints(nr_points);
  // Add points
  vtkIdType j = 0;    // true point index
  for (vtkIdType i = 0; i < nr_points; ++i)
  {
    // FIXME If we check for NaN's here, we should adapt the range of valid points in the point data taken from descriptors below
    // const BOOST_AUTO(colArray, cloud.features.col(i).array());
    // const BOOST_AUTO(hasNaN, !(colArray == colArray).all());
    // if (hasNaN)
    //   continue;

    T point[3] = {cloud.features.col(i)[0], cloud.features.col(i)[1], cloud.features.col(i)[2]}; 
    points->SetPoint(j, point);
    j++;
  }
  nr_points = j;
  points->SetNumberOfPoints(nr_points);

  // Add points to vtkPolyData
  vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
  polyData->SetPoints(points.GetPointer());
  polyData->SetVerts(NewVertexCells(nr_points));

  // And now we deal with the descriptors (if any)
  // #############################################

  for(BOOST_AUTO(it, cloud.descriptorLabels.begin()); it != cloud.descriptorLabels.end(); it++)
  {
    // Convert descriptor and add it to vtk object
    vtkSmartPointer<vtkDataArray> dataArray = getVtkDataArrayFromDescriptor(cloud, it->text);
    polyData->GetPointData()->AddArray(dataArray);

    // vtkPolyData has the ability to pick one of the arrays from the field as
    // the currently active array for each attribute type. For example, one of
    // the scalar arrays can be selected to be called "The Scalars", and can
    // be recovered using "GetScalars()" method. Here we're interested to
    // inform paraview which arrays should be pick as Normals and Tensors (for
    // eigenvalues).
    if(it->text == "normals")
    {
      polyData->GetPointData()->SetActiveNormals("normals");
    }
    else if(it->text == "eigVectors")
    {
      // FIXME not sure if we should set the eigen vectors as tensors.
      polyData->GetPointData()->SetActiveTensors("eigVectors");
    }
  }

  return polyData;
}

//----------------------------------------------------------------------------
boost::shared_ptr<PM::DataPoints> vtkPMConversions::DataPointsFromPolyData(vtkPolyData* polyData)
{
  const vtkIdType numberOfPoints = polyData->GetNumberOfPoints();

  boost::shared_ptr<PM::DataPoints> cloud(new PM::DataPoints);

  if (!numberOfPoints)
    {
    return cloud;
    }

  vtkFloatArray* floatPoints = vtkFloatArray::SafeDownCast(polyData->GetPoints()->GetData());
  vtkDoubleArray* doublePoints = vtkDoubleArray::SafeDownCast(polyData->GetPoints()->GetData());
  assert(floatPoints || doublePoints);

  PM::Matrix features(4, numberOfPoints);
  if (floatPoints)
    {
    float* data = floatPoints->GetPointer(0);
    for (vtkIdType i = 0; i < numberOfPoints; ++i)
      {
      features.col(i).x() = data[i*3];
      features.col(i).y() = data[i*3+1];
      features.col(i).z() = data[i*3+2];
      }
    }
  else if (doublePoints)
    {
    double* data = doublePoints->GetPointer(0);
    for (vtkIdType i = 0; i < numberOfPoints; ++i)
      {
      features.col(i).x() = data[i*3];
      features.col(i).y() = data[i*3+1];
      features.col(i).z() = data[i*3+2];
      features.col(i)(3) = 1.0; // padding
      }
    }

  cloud->addFeature("x", features.row(0));
  cloud->addFeature("y", features.row(1));
  cloud->addFeature("z", features.row(2));
  cloud->addFeature("pad", features.row(3));

  // check for descriptors
  vtkIdType numberOfPointArrays = polyData->GetPointData()->GetNumberOfArrays();
  for(vtkIdType i = 0; i < numberOfPointArrays; i++)
  {
    PM::Matrix descriptor = getDescriptorMatrixFromVtkDataArray(polyData->GetPointData()->GetArray(i));
    if (descriptor.rows())
      cloud->addDescriptor(polyData->GetPointData()->GetArrayName(i), descriptor);
  }

  return cloud;
}

//----------------------------------------------------------------------------
vtkSmartPointer<vtkCellArray> vtkPMConversions::NewVertexCells(vtkIdType numberOfVerts)
{
  vtkNew<vtkIdTypeArray> cells;
  cells->SetNumberOfValues(numberOfVerts*2);
  vtkIdType* ids = cells->GetPointer(0);
  for (vtkIdType i = 0; i < numberOfVerts; ++i)
    {
    ids[i*2] = 1;
    ids[i*2+1] = i;
    }

  vtkSmartPointer<vtkCellArray> cellArray = vtkSmartPointer<vtkCellArray>::New();
  cellArray->SetCells(numberOfVerts, cells.GetPointer());
  return cellArray;
}

//----------------------------------------------------------------------------
void vtkPMConversions::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  //os << indent << "Property: " << endl;
}
