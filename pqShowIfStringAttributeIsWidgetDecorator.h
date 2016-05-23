/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    pqShowIfStringAttributeIsWidgetDecorator.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
// .NAME pqShowIfStringAttributeIsWidgetDecorator -
// .SECTION Description
//
#ifndef pqShowIfStringAttributeIsWidgetDecorator_h
#define pqShowIfStringAttributeIsWidgetDecorator_h

#include "pqPropertyWidgetDecorator.h"
#include "vtkWeakPointer.h"
class vtkObject;

class pqShowIfStringAttributeIsWidgetDecorator : public pqPropertyWidgetDecorator
{
  Q_OBJECT
  typedef pqPropertyWidgetDecorator Superclass;
public:
  pqShowIfStringAttributeIsWidgetDecorator(
    vtkPVXMLElement* config, pqPropertyWidget* parentObject);
  virtual ~pqShowIfStringAttributeIsWidgetDecorator();

  /// Overridden to hide the widget when ShrinkFactor < 0.1
  virtual bool canShowWidget(bool show_advanced) const;

private:
  Q_DISABLE_COPY(pqShowIfStringAttributeIsWidgetDecorator)

  std::string Property;
  std::vector<std::string> ValueNames;
  vtkWeakPointer<vtkObject> ObservedObject;
  unsigned long ObserverId;
};

#endif
