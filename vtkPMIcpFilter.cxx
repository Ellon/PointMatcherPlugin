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
vtkPMIcpFilter::vtkPMIcpFilter() :
  Matcher(0),
  Knn(1),
  Epsilon(0),
  SearchType(1),
  MaxDist(std::numeric_limits<double>::infinity()),
  MaxDistField(NULL),
  SensorStdDev(0.01),
  Force2D(0),
  UseBoundTransformationChecker(0),
  UseCounterTransformationChecker(0),
  UseDifferentialTransformationChecker(0),
  MaxRotationNorm(1),
  MaxTranslationNorm(1),
  MaxIterationCount(40),
  MinDiffRotationError(0.001),
  MinDiffTranslationError(0.001),
  SmoothLength(3)
{
  this->SetNumberOfInputPorts(2);
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
  vtkPolyData *referenceInput = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  inInfo = inputVector[1]->GetInformationObject(0);
  vtkPolyData *readingInput = vtkPolyData::SafeDownCast(inInfo->Get(vtkDataObject::DATA_OBJECT()));
  
  vtkInformation *outInfo = outputVector->GetInformationObject(0);
  vtkPolyData *output = vtkPolyData::SafeDownCast(outInfo->Get(vtkDataObject::DATA_OBJECT()));

  // Convert point clouds from vtkPolyData to PM::DataPoints
  boost::shared_ptr<PM::DataPoints> referenceCloud = vtkPMConversions::DataPointsFromPolyData(referenceInput);
  boost::shared_ptr<PM::DataPoints> readingCloud = vtkPMConversions::DataPointsFromPolyData(readingInput);

  // Create the default ICP algorithm
  PM::ICP icp;
  PointMatcherSupport::Parametrizable::Parameters params;
  std::string name;

  try
  {
    // Prepare matching function
    name =
      (this->Matcher == kKDTreeMatcher ? "KDTreeMatcher" :
      (this->Matcher == kKDTreeVarDistMatcher ? "KDTreeVarDistMatcher" :
      (this->Matcher == kNullMatcher ? "kNullMatcher" :
      "")));
    switch (this->Matcher)
    {
    case kKDTreeMatcher:
      params["knn"] = boost::lexical_cast<std::string>(this->Knn);
      params["epsilon"] = boost::lexical_cast<std::string>(this->Epsilon);
      params["searchType"] = boost::lexical_cast<std::string>(this->SearchType);
      params["maxDist"] = boost::lexical_cast<std::string>(this->MaxDist);
      break;
    case kKDTreeVarDistMatcher:
      params["knn"] = boost::lexical_cast<std::string>(this->Knn);
      params["epsilon"] = boost::lexical_cast<std::string>(this->Epsilon);
      params["searchType"] = boost::lexical_cast<std::string>(this->SearchType);
      if(this->MaxDistField)
        params["maxDistField"] = this->MaxDistField;
      break;
    case kNullMatcher:
      break;
    }
    PM::Matcher* pm_matcher = 
      PM::get().MatcherRegistrar.create(name, params);
    params.clear();

    // Prepare outlier filters
    std::vector<PM::OutlierFilter*> pm_outlierfiltervector;
    std::for_each(this->outlierFilterOptionVector.begin(), this->outlierFilterOptionVector.end(),[&](const OutlierFilterOptions &opt){
      name = opt.filterName;
      if(name == "GenericDescriptorOutlierFilter")
      {
        params["source"] = opt.source;
        params["descName"] = opt.descriptor;
        params["useSoftThreshold"] = boost::lexical_cast<std::string>(opt.useSoftThreshold);
        params["useLargerThan"] = boost::lexical_cast<std::string>(opt.useLargerThan);
        params["threshold"] = boost::lexical_cast<std::string>(opt.threshold);
      }
      if(name == "MaxDistOutlierFilter")
      {
        params["maxDist"] = boost::lexical_cast<std::string>(opt.maxDist);
      }
      if(name == "MedianDistOutlierFilter")
      {
        params["factor"] = boost::lexical_cast<std::string>(opt.factor);
      }
      if(name == "MinDistOutlierFilter")
      {
        params["minDist"] = boost::lexical_cast<std::string>(opt.minDist);
      }
      // if(name == "NullOutlierFilter")
      // {
      // }
      if(name == "SurfaceNormalOutlierFilter")
      {
        params["maxAngle"] = boost::lexical_cast<std::string>(opt.maxAngle);
      }
      if(name == "TrimmedDistOutlierFilter")
      {
        params["ratio"] = boost::lexical_cast<std::string>(opt.ratio);
      }
      if(name == "VarTrimmedDistOutlierFilter")
      {
        params["minRatio"] = boost::lexical_cast<std::string>(opt.minRatio);
        params["maxRatio"] = boost::lexical_cast<std::string>(opt.maxRatio);
        params["lambda"] = boost::lexical_cast<std::string>(opt.lambda);
      }

      PM::OutlierFilter* pm_outlierfilter = 
          PM::get().OutlierFilterRegistrar.create(name, params);
      pm_outlierfiltervector.push_back(pm_outlierfilter);
      params.clear();
    });
    
    // Prepare error minimization
    name = 
      (this->ErrorMinimizer == kIdentityErrorMinimizer ? "IdentityErrorMinimizer" :
      (this->ErrorMinimizer == kPointToPlaneErrorMinimizer ? "PointToPlaneErrorMinimizer" :
      (this->ErrorMinimizer == kPointToPlaneWithCovErrorMinimizer ? "PointToPlaneWithCovErrorMinimizer" :
      (this->ErrorMinimizer == kPointToPointErrorMinimizer ? "PointToPointErrorMinimizer" :
      (this->ErrorMinimizer == kPointToPointSimilarityErrorMinimizer ? "PointToPointSimilarityErrorMinimizer" :
      (this->ErrorMinimizer == kPointToPointWithCovErrorMinimizer ? "PointToPointWithCovErrorMinimizer" :
      ""))))));
    switch (this->ErrorMinimizer)
    {
    case kPointToPlaneErrorMinimizer:
      params["force2D"] = boost::lexical_cast<std::string>(this->Force2D);
      break;
    case kPointToPlaneWithCovErrorMinimizer:
      params["force2D"] = boost::lexical_cast<std::string>(this->Force2D);
      params["sensorStdDev"] = boost::lexical_cast<std::string>(this->SensorStdDev);
      break;
    case kPointToPointWithCovErrorMinimizer:
      params["sensorStdDev"] = boost::lexical_cast<std::string>(this->SensorStdDev);
      break;
    }
    PM::ErrorMinimizer* pm_errorminimizer =   
      PM::get().ErrorMinimizerRegistrar.create(name, params);
    params.clear();

    // Prepare outlier filters
    if(!this->UseBoundTransformationChecker && !this->UseCounterTransformationChecker && !this->UseDifferentialTransformationChecker)
    {
      vtkWarningMacro( "At least one transformation checker should be selected!" );
      // Error
      return 0;
    }
    std::vector<PM::TransformationChecker*> pm_transformationcheckers;
    if(this->UseBoundTransformationChecker)
    {
      name = "BoundTransformationChecker";
      params["maxRotationNorm"] = boost::lexical_cast<std::string>(this->MaxRotationNorm);
      params["maxTranslationNorm"] = boost::lexical_cast<std::string>(this->MaxTranslationNorm);
      PM::TransformationChecker* pm_transformationchecker = 
        PM::get().TransformationCheckerRegistrar.create(name, params);
      pm_transformationcheckers.push_back(pm_transformationchecker);
      params.clear();
    }
    if(this->UseCounterTransformationChecker)
    {
      name = "CounterTransformationChecker";
      params["maxIterationCount"] = boost::lexical_cast<std::string>(this->MaxIterationCount);
      PM::TransformationChecker* pm_transformationchecker = 
        PM::get().TransformationCheckerRegistrar.create(name, params);
      pm_transformationcheckers.push_back(pm_transformationchecker);
      params.clear();
    }
    if(this->UseDifferentialTransformationChecker)
    {
      name = "DifferentialTransformationChecker";
      params["minDiffRotErr"] = boost::lexical_cast<std::string>(this->MinDiffRotationError);
      params["minDiffTransErr"] = boost::lexical_cast<std::string>(this->MinDiffTranslationError);
      params["smoothLength"] = boost::lexical_cast<std::string>(this->SmoothLength);      
      PM::TransformationChecker* pm_transformationchecker = 
        PM::get().TransformationCheckerRegistrar.create(name, params);
      pm_transformationcheckers.push_back(pm_transformationchecker);
      params.clear();
    }

    // Prepare inspector
    // NOTE We use the VTKFileInspector to save the clouds on disk during ICP
    //      and then load them later below. This is ugly, but it was the
    //      fastest way to implement.
    // FIXME Recover internals from ICP in a better way
    name = "VTKFileInspector";
    // params["baseFileName"] = "/tmp/point-matcher-output";
    params["dumpDataLinks"] = "1";
    params["dumpReading"] = "1";
    params["dumpReference"] = "1";
    // params["dumpIterationInfo"] = "1";
    PM::Inspector* pm_inspector =
      PM::get().InspectorRegistrar.create(name,params);
    params.clear();

    // Prepare transformation
    PM::Transformation* pm_rigidtrans =
    PM::get().TransformationRegistrar.create("RigidTransformation");

    // Build ICP solution
    icp.matcher.reset(pm_matcher);
    std::for_each(pm_outlierfiltervector.begin(), pm_outlierfiltervector.end(),[&](PM::OutlierFilter* ofilter){
      icp.outlierFilters.push_back(ofilter);
    });
    icp.errorMinimizer.reset(pm_errorminimizer);
    std::for_each(pm_transformationcheckers.begin(), pm_transformationcheckers.end(),[&](PM::TransformationChecker* tfchecker){
      icp.transformationCheckers.push_back(tfchecker);
    });
    icp.inspector.reset(pm_inspector);
    icp.transformations.push_back(pm_rigidtrans);

    // Compute the transformation to express reading in reference
    PM::TransformationParameters T = icp(*readingCloud, *referenceCloud);

    // Transform data to express it in ref
    // FIXME We don't need this, do we?
    PM::DataPoints data_out(*readingCloud);
    icp.transformations.apply(data_out, T);

    // FIXME For now just pass the input to the output to allow using the filter on ParaView
    output->DeepCopy(referenceInput);

    // OK
    return 1;

  }
  catch( const PointMatcherSupport::InvalidModuleType& e)
  {
    vtkErrorMacro( << e.what() );
  }
  catch( const PointMatcherSupport::Parametrizable::InvalidParameter& e)
  {
    vtkErrorMacro( << e.what() );
  }
  catch( const PointMatcherSupport::InvalidElement& e)
  {
    vtkErrorMacro( << e.what() );
  }
  catch( const PM::DataPoints::InvalidField& e)
  {
    vtkErrorMacro( << e.what() );
  }
  catch( const std::runtime_error& e)
  {
    vtkErrorMacro( << e.what() );
  }

  // Error
  return 0;

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

  this->outlierFilterOptionVector.push_back(OutlierFilterOptions());
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

  this->Modified();
}

void vtkPMIcpFilter::CleanOutlierFilters()
{
  this->outlierFilterOptionVector.clear();
  this->Modified();
}

void vtkPMIcpFilter::SetMaxDistField(int, int, int, int, const char *name)
{
  vtkDebugMacro( << this->GetClassName() << " (" << this << "): setting " << "MaxDistField" << " to " << name );
  if ( this->MaxDistField && name && (!strcmp(this->MaxDistField, name))) { return;}
  if (this->MaxDistField) { delete [] this->MaxDistField; }
  if (name)
  {
    this->MaxDistField = new char[strlen(name) + 1];
    strcpy(this->MaxDistField, name);
  }
  else
  {
    this->MaxDistField = NULL;
  }
  this->Modified();
}