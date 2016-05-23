/*=========================================================================

  Program:   Lib Point Matcher Plugin for ParaView
  Module:    pqShowIfBooleanIsWidgetDecorator.h

  Copyright (c) Ellon Paiva Mendes
  All rights reserved.
  See LICENSE file for details.

=========================================================================*/
// .NAME pqShowIfBooleanIsWidgetDecorator -
// .SECTION Description
//
#ifndef pqShowIfBooleanIsWidgetDecorator_h
#define pqShowIfBooleanIsWidgetDecorator_h

#include "pqShowWidgetDecorator.h"
#include "vtkWeakPointer.h"
class vtkObject;

class pqShowIfBooleanIsWidgetDecorator : public pqPropertyWidgetDecorator
{
  Q_OBJECT
  typedef pqPropertyWidgetDecorator Superclass;
public:
  pqShowIfBooleanIsWidgetDecorator(
    vtkPVXMLElement* config, pqPropertyWidget* parentObject);
  virtual ~pqShowIfBooleanIsWidgetDecorator();

  /// Overridden to hide the widget when ShrinkFactor < 0.1
  virtual bool canShowWidget(bool show_advanced) const;

private:
  Q_DISABLE_COPY(pqShowIfBooleanIsWidgetDecorator)
  std::string Name;
  bool Value;
  vtkWeakPointer<vtkObject> ObservedObject;
  unsigned long ObserverId;
};

#endif
