/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    pqShowIfBooleanIsWidgetDecorator.cxx

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
#include "pqShowIfBooleanIsWidgetDecorator.h"

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
pqShowIfBooleanIsWidgetDecorator::pqShowIfBooleanIsWidgetDecorator(
  vtkPVXMLElement* config, pqPropertyWidget* parentObject)
  : Superclass(config, parentObject)
{
  std::string name = config->GetAttributeOrEmpty("name");
  if(name.empty())
  {
    qDebug("Could not locate 'name' attribute in the xml file."
      "pqShowIfBooleanIsWidgetDecorator will have no effect.");
    return;
  }

  vtkSMProxy* proxy = parentObject->proxy();
  vtkSMProperty* prop = proxy? proxy->GetProperty(name.c_str()) : NULL;
  if (!prop)
  {
    qDebug() << "Could not locate property named " << name << 
      ". pqShowIfBooleanIsWidgetDecorator will have no effect.";
    return;
  }

  this->Name = name;

  std::string value_str = config->GetAttributeOrDefault("value", "true");
  if (value_str != "true" && value_str != "false")
  {
    qDebug() << "pqShowIfBooleanIsWidgetDecorator only supports " 
             << "'true' and 'false' for 'value'. Assuming 'value' as 'true'";
    value_str = "true";
  }
  if (value_str == "true")
    this->Value = true;
  else
    this->Value = false;

  this->ObservedObject = prop;
  this->ObserverId = pqCoreUtilities::connect(
  prop, vtkCommand::UncheckedPropertyModifiedEvent,
  this, SIGNAL(visibilityChanged()));
}

//-----------------------------------------------------------------------------
pqShowIfBooleanIsWidgetDecorator::~pqShowIfBooleanIsWidgetDecorator()
{
  if (this->ObservedObject && this->ObserverId)
  {
  this->ObservedObject->RemoveObserver(this->ObserverId);
  }
}

//-----------------------------------------------------------------------------
bool pqShowIfBooleanIsWidgetDecorator::canShowWidget(bool show_advanced) const
{
  pqPropertyWidget* parentObject = this->parentWidget();
  vtkSMProxy* proxy = parentObject->proxy();
  vtkSMProperty* prop = proxy? proxy->GetProperty(this->Name.c_str()) : NULL;
  if (prop)
  {
    int currentValue = vtkSMUncheckedPropertyHelper(prop).GetAsInt();
    if (currentValue != this->Value)
    {
      return false;
    }
  }

  return this->Superclass::canShowWidget(show_advanced);
}

