/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMSurfaceNormalDataPointsFilter.cxx

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
#include "vtkPMSurfaceNormalDataPointsFilter.h"
#include "vtkPMConversions.h"

#include "vtkPolyData.h"
#include "vtkPointData.h"
#include "vtkInformation.h"
#include "vtkInformationVector.h"
#include "vtkObjectFactory.h"
#include "vtkStreamingDemandDrivenPipeline.h"
#include "vtkSmartPointer.h"
#include "vtkNew.h"

typedef vtkPMConversions::PM PM;

//----------------------------------------------------------------------------
vtkStandardNewMacro(vtkPMSurfaceNormalDataPointsFilter);

//----------------------------------------------------------------------------
vtkPMSurfaceNormalDataPointsFilter::vtkPMSurfaceNormalDataPointsFilter()
{
  this->Knn = 5;
  this->Epsilon = 0;
  this->KeepNormals = 1;
  this->KeepDensities = 0;
  this->KeepEigenValues = 0;
  this->KeepEigenVectors = 0;
  this->KeepMatchedIds = 0;
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
}

//----------------------------------------------------------------------------
vtkPMSurfaceNormalDataPointsFilter::~vtkPMSurfaceNormalDataPointsFilter()
{
}

//----------------------------------------------------------------------------
int vtkPMSurfaceNormalDataPointsFilter::RequestData(
  vtkInformation* vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get input and output data objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // configure the filter
  PointMatcherSupport::Parametrizable::Parameters params;
  std::string name = "SurfaceNormalDataPointsFilter";
  params["knn"] = boost::lexical_cast<std::string>(this->Knn);
  params["epsilon"] = boost::lexical_cast<std::string>(this->Epsilon);
  params["keepNormals"] = boost::lexical_cast<std::string>(this->KeepNormals);
  params["keepDensities"] = boost::lexical_cast<std::string>(this->KeepDensities);
  params["keepEigenValues"] = boost::lexical_cast<std::string>(this->KeepEigenValues);
  params["keepEigenVectors"] = boost::lexical_cast<std::string>(this->KeepEigenVectors);
  params["keepMatchedIds"] = boost::lexical_cast<std::string>(this->KeepMatchedIds);
  PM::DataPointsFilter* PM_filter = 
    PM::get().DataPointsFilterRegistrar.create(name, params);

  // perform fix step sampling
  boost::shared_ptr<PM::DataPoints> cloud = vtkPMConversions::DataPointsFromPolyData(input);
  PM_filter->inPlaceFilter(*cloud);

  // store output
  output->ShallowCopy(vtkPMConversions::PolyDataFromDataPoints(*cloud));

  return 1;
}

//----------------------------------------------------------------------------
void vtkPMSurfaceNormalDataPointsFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
