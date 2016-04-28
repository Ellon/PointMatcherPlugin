/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMFixStepSamplingFilter.cxx

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See Copyright.txt or http://www.kitware.com/Copyright.htm for details.
  
     This software is distributed WITHOUT ANY WARRANTY; without even
     the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
     PURPOSE.  See the above copyright notice for more information.

=========================================================================*/
#include "vtkPMFixStepSamplingFilter.h"
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
vtkStandardNewMacro(vtkPMFixStepSamplingFilter);

//----------------------------------------------------------------------------
vtkPMFixStepSamplingFilter::vtkPMFixStepSamplingFilter()
{

  this->StartStep = 10;
  this->EndStep = 10;
  this->StepMult = 1;
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
}

//----------------------------------------------------------------------------
vtkPMFixStepSamplingFilter::~vtkPMFixStepSamplingFilter()
{
}

//----------------------------------------------------------------------------
int vtkPMFixStepSamplingFilter::RequestData(
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
  std::string name = "FixStepSamplingDataPointsFilter";
  params["startStep"] = boost::lexical_cast<std::string>(this->StartStep);
  params["endStep"] = boost::lexical_cast<std::string>(this->EndStep);
  params["stepMult"] = boost::lexical_cast<std::string>(this->StepMult);
  PM::DataPointsFilter* fixStepFilter = 
    PM::get().DataPointsFilterRegistrar.create(name, params);

  // perform fix step sampling
  boost::shared_ptr<PM::DataPoints> cloud = vtkPMConversions::DataPointsFromPolyData(input);
  fixStepFilter->inPlaceFilter(*cloud);

  // store output
  output->ShallowCopy(vtkPMConversions::PolyDataFromDataPoints(*cloud));

  return 1;
}

//----------------------------------------------------------------------------
void vtkPMFixStepSamplingFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}
