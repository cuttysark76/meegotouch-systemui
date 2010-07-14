/*
 * This file was generated by qdbusxml2cpp version 0.7
 * Command line was: qdbusxml2cpp statusarearenderer.xml -a statusarearendereradaptor -c StatusAreaRendererAdaptor -l StatusAreaRenderer -i statusarearenderer.h
 *
 * qdbusxml2cpp is Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 *
 * This is an auto-generated file.
 * Do not edit! All changes made to it will be lost.
 */

#include "statusarearendereradaptor.h"
#include <QtCore/QMetaObject>
#include <QtCore/QByteArray>
#include <QtCore/QList>
#include <QtCore/QMap>
#include <QtCore/QString>
#include <QtCore/QStringList>
#include <QtCore/QVariant>

/*
 * Implementation of adaptor class StatusAreaRendererAdaptor
 */

StatusAreaRendererAdaptor::StatusAreaRendererAdaptor(StatusAreaRenderer *parent)
    : QDBusAbstractAdaptor(parent)
{
    // constructor
    setAutoRelaySignals(true);
}

StatusAreaRendererAdaptor::~StatusAreaRendererAdaptor()
{
    // destructor
}

uint StatusAreaRendererAdaptor::sharedPixmapHandle()
{
    // handle method call com.meego.core.MStatusBar.sharedPixmapHandle
    return parent()->sharedPixmapHandle();
}
