/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMConversions.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
  
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

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
    // FIXME If we check for NaN's here, we should check for it in the point data below
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
    // handle specific cases
    if(it->text == "normals")
    {
      const PM::Matrix normals = cloud.getDescriptorViewByName("normals");
      vtkIdType nr_normals = normals.cols();
      vtkSmartPointer<vtkFloatArray> pointNormalsArray = 
        vtkSmartPointer<vtkFloatArray>::New();
      pointNormalsArray->SetNumberOfComponents(3); //3d normals (ie x,y,z)
      pointNormalsArray->SetNumberOfTuples(nr_normals);
      // We must set the name to be able to select this data on paraview.
      pointNormalsArray->SetName("normals");
      for(vtkIdType i = 0; i < nr_normals; ++i)
      {
        float pN[3];
        pN[0] = normals.col(i).x();
        pN[1] = normals.col(i).y();
        pN[2] = normals.col(i).z(); 
        pointNormalsArray->SetTuple(i, pN) ;
      }
      polyData->GetPointData()->SetNormals(pointNormalsArray);
    }
    // else if(it->text == "eigVectors")
    // {
    //   buildTensorStream(stream, "eigVectors", data);
    // }
    // else if(it->text == "color")
    // {
    //   buildColorStream(stream, "color", data);
    // }
    // // handle generic cases
    // else if(it->span == 1)
    // {
    //   buildScalarStream(stream, it->text, data);
    // }
    // else if(it->span == 3 || it->span == 2)
    // {
    //   buildVectorStream(stream, it->text, data);
    // }
    // else
    // {
    //   LOG_WARNING_STREAM("Could not save label named " << it->text << " (dim=" << it->span << ").");
    // }
  }

  // polyData->GetPointData()->Print(cout);
  // cout << "polyData has normals? " << polyData->GetPointData()->HasArray("normals") << endl;

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

  // check for normals
  // FIXME make it generic on the type of Point Matcher
  vtkFloatArray* floatPointNormals = vtkFloatArray::SafeDownCast(polyData->GetPointData()->GetNormals());
  vtkDoubleArray* doublePointNormals = vtkDoubleArray::SafeDownCast(polyData->GetPointData()->GetNormals());
  if(floatPointNormals)
    {
    vtkIdType nr_normals = floatPointNormals->GetNumberOfTuples();
    PM::Matrix descriptor(3,nr_normals);
    for(vtkIdType i = 0; i < nr_normals; i++)
      {
      // This is weird, but "GetTuple" from vtkFloatArray has a double* as argument...
      double pN[3];
      floatPointNormals->GetTuple(i, pN);
      descriptor.col(i).x() = (float)pN[0];
      descriptor.col(i).y() = (float)pN[1];
      descriptor.col(i).z() = (float)pN[2];
      }
      cloud->addDescriptor("normals", descriptor);
    }
    else if(doublePointNormals)
    {
    vtkIdType nr_normals = doublePointNormals->GetNumberOfTuples();
    PM::Matrix descriptor(3,nr_normals);
    for(vtkIdType i = 0; i < nr_normals; i++)
      {
      double pN[3];
      doublePointNormals->GetTuple(i, pN);
      descriptor.col(i).x() = (float)pN[0];
      descriptor.col(i).y() = (float)pN[1];
      descriptor.col(i).z() = (float)pN[2];
      }
      cloud->addDescriptor("normals", descriptor);
    }

  return cloud;
}

















// namespace {

// template <typename T>
// vtkSmartPointer<vtkPolyData> TemplatedPolyDataFromPCDFile(const std::string& filename)
// {
//   typename pcl::PointCloud<T>::Ptr cloud(new pcl::PointCloud<T>);
//   if (pcl::io::loadPCDFile(filename, *cloud) == -1)
//     {
//     std::cout << "Error reading pcd file: " << filename;
//     return 0;
//     }

//   return vtkPMConversions::PolyDataFromPointCloud(cloud);
// }

// }


// //----------------------------------------------------------------------------
// vtkSmartPointer<vtkPolyData> vtkPMConversions::PolyDataFromPCDFile(const std::string& filename)
// {
//   int version;
//   int type;
//   unsigned int idx;

//   pcl::PointMatcherPointCloud2 cloud;
//   Eigen::Vector4f origin;
//   Eigen::Quaternionf orientation;
//   pcl::PCDReader reader;
//   reader.readHeader(filename, cloud, origin, orientation, version, type, idx);


//   if (pcl::getFieldIndex(cloud, "rgba") != -1) {
//     return TemplatedPolyDataFromPCDFile<pcl::PointXYZRGBA>(filename);
//   }
//   else if (pcl::getFieldIndex(cloud, "rgb") != -1) {
//     return TemplatedPolyDataFromPCDFile<pcl::PointXYZRGB>(filename);
//   }
//   else {
//     return TemplatedPolyDataFromPCDFile<pcl::PointXYZ>(filename);
//   }
// }


// //----------------------------------------------------------------------------
// vtkSmartPointer<vtkPolyData> vtkPMConversions::PolyDataFromPointCloud(pcl::PointCloud<pcl::PointXYZRGB>::ConstPtr cloud)
// {
//   vtkIdType nr_points = cloud->points.size();

//   vtkNew<vtkPoints> points;
//   points->SetDataTypeToFloat();
//   points->SetNumberOfPoints(nr_points);

//   vtkNew<vtkUnsignedCharArray> rgbArray;
//   rgbArray->SetName("rgb_colors");
//   rgbArray->SetNumberOfComponents(3);
//   rgbArray->SetNumberOfTuples(nr_points);


//   if (cloud->is_dense)
//   {
//     for (vtkIdType i = 0; i < nr_points; ++i) {
//       float point[3] = {cloud->points[i].x, cloud->points[i].y, cloud->points[i].z};
//       unsigned char color[3] = {cloud->points[i].r, cloud->points[i].g, cloud->points[i].b}; 
//       points->SetPoint(i, point);
//       rgbArray->SetTupleValue(i, color);
//     }
//   }
//   else
//   {
//     vtkIdType j = 0;    // true point index
//     for (vtkIdType i = 0; i < nr_points; ++i)
//     {
//       // Check if the point is invalid
//       if (!pcl_isfinite (cloud->points[i].x) || 
//           !pcl_isfinite (cloud->points[i].y) || 
//           !pcl_isfinite (cloud->points[i].z))
//         continue;

//       float point[3] = {cloud->points[i].x, cloud->points[i].y, cloud->points[i].z};
//       unsigned char color[3] = {cloud->points[i].r, cloud->points[i].g, cloud->points[i].b};
//       points->SetPoint(j, point);
//       rgbArray->SetTupleValue(j, color);
//       j++;
//     }
//     nr_points = j;
//     points->SetNumberOfPoints(nr_points);
//     rgbArray->SetNumberOfTuples(nr_points);
//   }

//   vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
//   polyData->SetPoints(points.GetPointer());
//   polyData->GetPointData()->AddArray(rgbArray.GetPointer());
//   polyData->SetVerts(NewVertexCells(nr_points));
//   return polyData;
// }

// //----------------------------------------------------------------------------
// vtkSmartPointer<vtkPolyData> vtkPMConversions::PolyDataFromPointCloud(pcl::PointCloud<pcl::PointXYZRGBA>::ConstPtr cloud)
// {
//   vtkIdType nr_points = cloud->points.size();

//   vtkNew<vtkPoints> points;
//   points->SetDataTypeToFloat();
//   points->SetNumberOfPoints(nr_points);

//   vtkNew<vtkUnsignedCharArray> rgbArray;
//   rgbArray->SetName("rgb_colors");
//   rgbArray->SetNumberOfComponents(3);
//   rgbArray->SetNumberOfTuples(nr_points);


//   if (cloud->is_dense)
//   {
//     for (vtkIdType i = 0; i < nr_points; ++i) {
//       float point[3] = {cloud->points[i].x, cloud->points[i].y, cloud->points[i].z};
//       unsigned char color[3] = {cloud->points[i].r, cloud->points[i].g, cloud->points[i].b}; 
//       points->SetPoint(i, point);
//       rgbArray->SetTupleValue(i, color);
//     }
//   }
//   else
//   {
//     vtkIdType j = 0;    // true point index
//     for (vtkIdType i = 0; i < nr_points; ++i)
//     {
//       // Check if the point is invalid
//       if (!pcl_isfinite (cloud->points[i].x) || 
//           !pcl_isfinite (cloud->points[i].y) || 
//           !pcl_isfinite (cloud->points[i].z))
//         continue;

//       float point[3] = {cloud->points[i].x, cloud->points[i].y, cloud->points[i].z};
//       unsigned char color[3] = {cloud->points[i].r, cloud->points[i].g, cloud->points[i].b};
//       points->SetPoint(j, point);
//       rgbArray->SetTupleValue(j, color);
//       j++;
//     }
//     nr_points = j;
//     points->SetNumberOfPoints(nr_points);
//     rgbArray->SetNumberOfTuples(nr_points);
//   }

//   vtkSmartPointer<vtkPolyData> polyData = vtkSmartPointer<vtkPolyData>::New();
//   polyData->SetPoints(points.GetPointer());
//   polyData->GetPointData()->AddArray(rgbArray.GetPointer());
//   polyData->SetVerts(NewVertexCells(nr_points));
//   return polyData;
// }


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

// //----------------------------------------------------------------------------
// void vtkPMConversions::PerformPointCloudConversionBenchmark(vtkPolyData* polyData)
// {
//   if (!polyData)
//     {
//     return;
//     }

//   double start;
//   double elapsed;
//   unsigned long kilobytes;

//   const vtkIdType numberOfPoints = polyData->GetNumberOfPoints();
//   std::cout << "Number of input points: " << numberOfPoints << std::endl;

//   start = vtkTimerLog::GetUniversalTime();
//   pcl::PointCloud<pcl::PointXYZ>::Ptr tempCloud = PointCloudFromPolyData(polyData);
//   elapsed = vtkTimerLog::GetUniversalTime() - start;

//   std::cout << "Conversion to pcl::PointCloud took " << elapsed << " seconds. "
//             << numberOfPoints / elapsed << " points per second." << std::endl;


//   start = vtkTimerLog::GetUniversalTime();
//   vtkSmartPointer<vtkPolyData> tempPolyData = PolyDataFromPointCloud(tempCloud);
//   elapsed = vtkTimerLog::GetUniversalTime() - start;

//   std::cout << "Conversion to vtkPolyData took " << elapsed << " seconds. "
//             << numberOfPoints / elapsed << " points per second." << std::endl;


//   start = vtkTimerLog::GetUniversalTime();
//   vtkSmartPointer<vtkCellArray> tempCells = NewVertexCells(numberOfPoints);
//   elapsed = vtkTimerLog::GetUniversalTime() - start;

//   std::cout << "Constructing vertex cells took " << elapsed << " seconds. "
//             << numberOfPoints / elapsed << " points per second." << std::endl;


//   kilobytes = tempPolyData->GetActualMemorySize();
//   std::cout << "vtkPolyData uses " << kilobytes/1024.0 << " MB. "
//             << kilobytes*1024 / numberOfPoints << " bytes per point." << std::endl;

//   kilobytes = tempPolyData->GetPoints()->GetActualMemorySize();
//   std::cout << "vtkPolyData's points use " << kilobytes/1024.0 << " MB. "
//             << kilobytes*1024 / numberOfPoints << " bytes per point." << std::endl;

//   kilobytes = tempPolyData->GetVerts()->GetActualMemorySize();
//   std::cout << "vtkPolyData's cells use " << kilobytes/1024.0 << " MB. "
//             << kilobytes*1024 / numberOfPoints << " bytes per point." << std::endl;
// }

//----------------------------------------------------------------------------
// namespace {

// vtkSmartPointer<vtkIntArray> NewLabelsArray(vtkIdType length)
// {
//   vtkSmartPointer<vtkIntArray> labels = vtkSmartPointer<vtkIntArray>::New();
//   labels->SetNumberOfComponents(1);
//   labels->SetNumberOfTuples(length);
//   labels->FillComponent(0, 0);
//   return labels;
// }

// void LabelIndices(const std::vector<int>& indices, vtkIntArray* labels, const int labelValue)
// {
//   const size_t numberOfIndices = indices.size();
//   for (size_t k = 0; k < numberOfIndices; ++k)
//     {
//     labels->SetValue(indices[k], labelValue);
//     }
// }

// }

// //----------------------------------------------------------------------------
// vtkSmartPointer<vtkIntArray> vtkPMConversions::NewLabelsArray(pcl::IndicesConstPtr indices, vtkIdType length)
// {
//   vtkSmartPointer<vtkIntArray> labels = ::NewLabelsArray(length);
//   if (indices)
//     {
//     LabelIndices(*indices, labels, 1);
//     }  
//   return labels;
// }

// //----------------------------------------------------------------------------
// vtkSmartPointer<vtkIntArray> vtkPMConversions::NewLabelsArray(pcl::PointIndices::ConstPtr indices, vtkIdType length)
// {
//   vtkSmartPointer<vtkIntArray> labels = ::NewLabelsArray(length);
//   if (indices)
//     {
//     LabelIndices(indices->indices, labels, 1);
//     }  
//   return labels;
// }

// //----------------------------------------------------------------------------
// vtkSmartPointer<vtkIntArray> vtkPMConversions::NewLabelsArray(const std::vector<pcl::PointIndices>& indices, vtkIdType length)
// {
//   vtkSmartPointer<vtkIntArray> labels = ::NewLabelsArray(length);

//   for (size_t i = 0; i < indices.size(); ++i)
//     {
//     const int labelValue = i + 1;
//     LabelIndices(indices[i].indices, labels, labelValue);
//     }

//   return labels;
// }

//----------------------------------------------------------------------------
void vtkPMConversions::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os, indent);
  //os << indent << "Property: " << endl;
}
