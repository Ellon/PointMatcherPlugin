/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    pqShowIfStringAttributeIsWidgetDecorator.cxx

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
#include "pqShowIfStringAttributeIsWidgetDecorator.h"

#include "pqCoreUtilities.h"
#include "pqPropertyWidget.h"
#include "vtkCommand.h"
#include "vtkSMProperty.h"
#include "vtkSMProxy.h"
#include "vtkSMUncheckedPropertyHelper.h"
#include "vtkPVXMLElement.h"

#include <boost/tokenizer.hpp>

#include <algorithm>    // std::all_of

//-----------------------------------------------------------------------------
pqShowIfStringAttributeIsWidgetDecorator::pqShowIfStringAttributeIsWidgetDecorator(
    vtkPVXMLElement* config, pqPropertyWidget* parentObject)
  : Superclass(config, parentObject)
{
  std::string property = config->GetAttributeOrEmpty("property");
  if(property.empty())
  {
    qDebug("Could not locate 'property' attribute in the xml file."
      "pqShowIfStringAttributeIsWidgetDecorator will have no effect.");
    return;
  }

  vtkSMProxy* proxy = parentObject->proxy();
  vtkSMProperty* prop = proxy? proxy->GetProperty(property.c_str()) : NULL;
  if (!prop)
    {
    qDebug() << "Could not locate property named " << property << 
      ". pqShowIfStringAttributeIsWidgetDecorator will have no effect.";
    return;
    }

  std::string value = config->GetAttributeOrEmpty("value");
  std::string values = config->GetAttributeOrEmpty("values");
  if(value.empty() && values.empty())
  {
    qDebug("Could not locate 'value' or 'values' attribute in the xml file."
      "pqShowIfStringAttributeIsWidgetDecorator will have no effect.");
    return;
  }

  // If we get here we're good to initialize attributes
  this->Property = property;

  if(not value.empty())
    this->ValueNames.push_back(value);

  if(not values.empty())
  {
    boost::tokenizer<> tok(values);
    for(boost::tokenizer<>::iterator beg=tok.begin(); beg!=tok.end();++beg){
      this->ValueNames.push_back(*beg);
    }
  }

  this->ObservedObject = prop;
  this->ObserverId = pqCoreUtilities::connect(
    prop, vtkCommand::UncheckedPropertyModifiedEvent,
    this, SIGNAL(visibilityChanged()));
}

//-----------------------------------------------------------------------------
pqShowIfStringAttributeIsWidgetDecorator::~pqShowIfStringAttributeIsWidgetDecorator()
{
  if (this->ObservedObject && this->ObserverId)
    {
    this->ObservedObject->RemoveObserver(this->ObserverId);
    }
}

//-----------------------------------------------------------------------------
bool pqShowIfStringAttributeIsWidgetDecorator::canShowWidget(bool show_advanced) const
{
  pqPropertyWidget* parentObject = this->parentWidget();
  vtkSMProxy* proxy = parentObject->proxy();
  vtkSMProperty* prop = proxy? proxy->GetProperty(this->Property.c_str()) : NULL;
  if (prop)
    {
    std::string currentValue = vtkSMUncheckedPropertyHelper(prop).GetAsString();
    if( std::all_of(ValueNames.begin(), ValueNames.end(), [&](std::string valueName){ return currentValue != valueName;}) )
    {
      return false;
    }
  }

  return this->Superclass::canShowWidget(show_advanced);
}
