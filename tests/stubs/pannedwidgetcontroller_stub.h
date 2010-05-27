/****************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of systemui.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#ifndef PANNEDWIDGETCONTROLLER_STUB
#define PANNEDWIDGETCONTROLLER_STUB

#include "statusindicatormenuwindow.h"
#include <QGraphicsItem>
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class PannedWidgetControllerStub : public StubBase {
public:
    virtual void PannedWidgetControllerConstructor(QGraphicsItem *parent);
    virtual void setGeometry(const QRectF &rect);
};

// 2. IMPLEMENT STUB
void PannedWidgetControllerStub::PannedWidgetControllerConstructor(QGraphicsItem *parent) {
    Q_UNUSED(parent);

}
void PannedWidgetControllerStub::setGeometry(const QRectF &rect) {
    QList<ParameterBase*> params;
    params.append( new Parameter<QRectF >(rect));
    stubMethodEntered("setGeometry",params);
}

// 3. CREATE A STUB INSTANCE
PannedWidgetControllerStub gDefaultPannedWidgetControllerStub;
PannedWidgetControllerStub* gPannedWidgetControllerStub = &gDefaultPannedWidgetControllerStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
PannedWidgetController::PannedWidgetController(QGraphicsItem *parent) {
    gPannedWidgetControllerStub->PannedWidgetControllerConstructor(parent);
}

void PannedWidgetController::setGeometry(const QRectF &rect) {
    gPannedWidgetControllerStub->setGeometry(rect);
}


#endif