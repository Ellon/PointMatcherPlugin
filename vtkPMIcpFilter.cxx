/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    vtkPMIcpFilter.cxx

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
#include "vtkPMIcpFilter.h"
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
vtkStandardNewMacro(vtkPMIcpFilter);

//----------------------------------------------------------------------------
vtkPMIcpFilter::vtkPMIcpFilter()
{
  this->SetNumberOfInputPorts(1);
  this->SetNumberOfOutputPorts(1);
}

//----------------------------------------------------------------------------
vtkPMIcpFilter::~vtkPMIcpFilter()
{
}

//----------------------------------------------------------------------------
int vtkPMIcpFilter::RequestData(
  vtkInformation* vtkNotUsed(request),
  vtkInformationVector **inputVector,
  vtkInformationVector *outputVector)
{
  // get input and output data objects
  vtkInformation *inInfo = inputVector[0]->GetInformationObject(0);
  vtkPolyData *input = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // TODO How to access the values set in the widgets from here?

  // FIXME For now just pass the input to the output to allow using the filter on ParaView
  output->DeepCopy(input);

  // OK
  return 1;
}

//----------------------------------------------------------------------------
void vtkPMIcpFilter::PrintSelf(ostream& os, vtkIndent indent)
{
  this->Superclass::PrintSelf(os,indent);
}

