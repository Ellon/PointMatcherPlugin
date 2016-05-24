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
  std::for_each(outlierFilterOptionVector.begin(), outlierFilterOptionVector.end(), [](const OutlierFilterOptions& opt)
  {
    std::cout << "filterName: " << opt.filterName << std::endl;
    std::cout << "    source: " << opt.source << std::endl;
    std::cout << "    descriptor: " << opt.descriptor << std::endl;
    std::cout << "    threshold: " << opt.threshold << std::endl;
    std::cout << "    useSoftThreshold: " << opt.useSoftThreshold << std::endl;
    std::cout << "    useLargerThan: " << opt.useLargerThan << std::endl;
    std::cout << "    maxDist: " << opt.maxDist << std::endl;
    std::cout << "    factor: " << opt.factor << std::endl;
    std::cout << "    minDist: " << opt.minDist << std::endl;
    std::cout << "    maxAngle: " << opt.maxAngle << std::endl;
    std::cout << "    ratio: " << opt.ratio << std::endl;
    std::cout << "    minRatio: " << opt.minRatio << std::endl;
    std::cout << "    maxRatio: " << opt.maxRatio << std::endl;
    std::cout << "    lambda: " << opt.lambda << std::endl;
  });

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

void vtkPMIcpFilter::AddOutlierFilter(
    const char* filterName, 
    const char* source, 
    const char* descriptor, 
    double threshold, 
    int useSoftThreshold, 
    int useLargerThan, 
    double maxDist, 
    double factor, 
    double minDist, 
    double maxAngle, 
    double ratio, 
    double minRatio, 
    double maxRatio, 
    double lambda)
{

  outlierFilterOptionVector.push_back(OutlierFilterOptions());
  OutlierFilterOptions &outlierFilterOptions = outlierFilterOptionVector.back();
  outlierFilterOptions.filterName = filterName;
  outlierFilterOptions.source = source;
  outlierFilterOptions.descriptor = descriptor;
  outlierFilterOptions.threshold = threshold;
  outlierFilterOptions.useSoftThreshold = useSoftThreshold;
  outlierFilterOptions.useLargerThan = useLargerThan;
  outlierFilterOptions.maxDist = maxDist;
  outlierFilterOptions.factor = factor;
  outlierFilterOptions.minDist = minDist;
  outlierFilterOptions.maxAngle = maxAngle;
  outlierFilterOptions.ratio = ratio;
  outlierFilterOptions.minRatio = minRatio;
  outlierFilterOptions.maxRatio = maxRatio;
  outlierFilterOptions.lambda = lambda;
}

void vtkPMIcpFilter::CleanOutlierFilters()
{
  outlierFilterOptionVector.clear();
}
