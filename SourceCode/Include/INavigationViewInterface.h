#pragma once
#include <QWidget>
#include <QString>
#include <QIcon>
#include "FluDef.h"

class INavigationViewInterface
{
  public:
    virtual void AddStackedWidget(QWidget* widget, const QString& name, const QIcon& icon, Qt::AlignmentFlag pos = Qt::AlignCenter) = 0;
    virtual void AddStackedWidget(QWidget* widget, const QString& name, FluAwesomeType type, Qt::AlignmentFlag pos = Qt::AlignCenter) = 0;
};