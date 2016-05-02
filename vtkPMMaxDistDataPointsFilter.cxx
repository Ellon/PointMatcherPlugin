/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMMaxDistDataPointsFilter.cxx

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
#include "vtkPMMaxDistDataPointsFilter.h"
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
vtkStandardNewMacro(vtkPMMaxDistDataPointsFilter);

//----------------------------------------------------------------------------
vtkPMMaxDistDataPointsFilter::vtkPMMaxDistDataPointsFilter()
{

  this->Dim = -1;
  this->MaxDist = 1;
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
}

//----------------------------------------------------------------------------
vtkPMMaxDistDataPointsFilter::~vtkPMMaxDistDataPointsFilter()
{
}

//----------------------------------------------------------------------------
int vtkPMMaxDistDataPointsFilter::RequestData(
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
  std::string name = "MaxDistDataPointsFilter";
  params["dim"] = boost::lexical_cast<std::string>(this->Dim);
  params["maxDist"] = boost::lexical_cast<std::string>(this->MaxDist);
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
void vtkPMMaxDistDataPointsFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
