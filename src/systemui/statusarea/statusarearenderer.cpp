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

#include <QDebug>
#include "statusarearenderer.h"
#include <MStyle>
#include "statusareastyle.h"
#include <QMeeGoLivePixmap>
#include <QMeeGoGraphicsSystemHelper>
#include <QX11Info>
#include "x11wrapper.h"

// Update the pixmap 5 times per second at most
static const int ACCUMULATION_INTERVAL = 200;

StatusAreaRenderer::StatusAreaRenderer(QObject *parent) :
    QObject(parent),
    scene(new QGraphicsScene),
    statusArea(new StatusArea),
    statusAreaLivePixmap(NULL),
    statusAreaPropertyWindow(0),
    windowManagerWindow(0),
    statusBarVisible(false),
#ifdef HAVE_QMSYSTEM
    displayState(new MeeGo::QmDisplayState()),
#endif
    renderScene(true)
{
    scene->setParent(this);
    scene->setObjectName("statusareascene");
    scene->addItem(statusArea);
    statusArea->setObjectName("statusarea");
    connect(this, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)), statusArea, SIGNAL(statusIndicatorMenuVisibilityChanged(bool)));

    // Get signaled when the scene changes
    connect(scene, SIGNAL(changed(QList<QRectF>)), this, SLOT(accumulateSceneChanges(QList<QRectF>)));
#ifdef HAVE_QMSYSTEM
    connect(displayState, SIGNAL(displayStateChanged(MeeGo::QmDisplayState::DisplayState)), this, SLOT(setSceneRender(MeeGo::QmDisplayState::DisplayState)));
    setSceneRender(displayState->get());
#endif

    statusBarVisibleAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_VISIBLE", False);
    windowManagerWindowAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_NET_SUPPORTING_WM_CHECK", False);

    setSizeFromStyle();
    if(!createSharedPixmapHandle() || !createBackPixmap()) {
        qWarning() << "Shared Pixmap was not created. Status area will not render";
    } else {
        createStatusAreaPropertyWindow();
        setSharedPixmapHandleToWindowProperty();
        setStatusAreaPropertyWindowIdToRootWindowProperty();

        setupStatusBarVisibleListener();
    }

    connect(&accumulationTimer, SIGNAL(timeout()), this, SLOT(renderAccumulatedRegion()));
}

void StatusAreaRenderer::setSizeFromStyle()
{
    const StatusAreaStyle *style = static_cast<const StatusAreaStyle *> (MTheme::style("StatusAreaStyle", "", "", "", M::Landscape, NULL));
    if(style) {
        statusAreaHeight = style->preferredSize().height();
        statusAreaWidth = style->preferredSize().width();
    }
}

bool StatusAreaRenderer::createSharedPixmapHandle()
{
    // Create a pixmap in which top portion TopLeft(0,0) BottomRight(status area width,status area height) is landscpae.
    // Bottom portion TopLeft(0,status area height) BottomRight(status area portrait width,2*status area height) is portrait. unused portion is portrait is not drawn when in portrait

    Pixmap pixmap = X11Wrapper::XCreatePixmap(QX11Info::display(), QX11Info::appRootWindow(),
                                              statusAreaWidth, 2*statusAreaHeight, QX11Info::appDepth());

    statusAreaPixmap = QPixmap::fromX11Pixmap(pixmap, QPixmap::ExplicitlyShared);

    if (!statusAreaPixmap.isNull()) {
        return true;
    } else {
        return false;
    }
}

bool StatusAreaRenderer::createBackPixmap()
{
    if (statusAreaLivePixmap) {
        delete statusAreaLivePixmap;
        statusAreaLivePixmap = NULL;
    }

    if (QMeeGoGraphicsSystemHelper::isRunningMeeGo()) {
        // FIXME: Round up to the nearest multiple of eight until NB#231246 is fixed
        int livePixmapWidth = (statusAreaWidth / 8 + 1) * 8;

        statusAreaLivePixmap = QMeeGoLivePixmap::livePixmapWithSize(livePixmapWidth, statusAreaHeight*2, QMeeGoLivePixmap::Format_ARGB32_Premultiplied);
        backPixmap = QPixmap::fromX11Pixmap(statusAreaLivePixmap->handle(), QPixmap::ExplicitlyShared);
    } else {
        backPixmap = QPixmap(statusAreaWidth, statusAreaHeight*2);
    }

    if (!backPixmap.isNull()) {
        return true;
    } else {
        return false;
    }
}

uint StatusAreaRenderer::sharedPixmapHandle()
{
    return static_cast<quint32> (statusAreaPixmap.handle());
}

void StatusAreaRenderer::createStatusAreaPropertyWindow()
{
    Display *dpy = QX11Info::display();
    XSetWindowAttributes attr;
    statusAreaPropertyWindow = X11Wrapper::XCreateWindow(dpy, DefaultRootWindow(dpy),
                                       0, 0,
                                       2, 2,
                                       0,
                                       None,
                                       InputOnly,
                                       None,
                                       None,
                                       &attr);
}

void StatusAreaRenderer::setStatusAreaPropertyWindowIdToRootWindowProperty()
{
    if (statusAreaPropertyWindow == 0) {
        return;
    }

    /* Set the property window id to root window property */
    Atom propertyWindowAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW", False);
    Window rootWindow = QX11Info::appRootWindow(QX11Info::appScreen());

    X11Wrapper::XChangeProperty(QX11Info::display(),    //display
                    rootWindow,                         //window
                    propertyWindowAtom,                 //property
                    XA_WINDOW,                          //type
                    32,                                 //format
                    PropModeReplace,
                    (unsigned char*)&statusAreaPropertyWindow,  //data
                    1);
}

void StatusAreaRenderer::setSharedPixmapHandleToWindowProperty()
{
    if (statusAreaPropertyWindow == 0) {
        return;
    }
    // Create atom for pixmap handle property
    Atom pixmapHandleAtom = X11Wrapper::XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PIXMAP", False);
    unsigned int handle = sharedPixmapHandle();

    X11Wrapper::XChangeProperty(QX11Info::display(),    //display
                    statusAreaPropertyWindow,           //window
                    pixmapHandleAtom,                   //property
                    XA_PIXMAP,                          //type
                    32,                                 //format
                    PropModeReplace,                    //mode
                    (unsigned char*)&handle,            //data
                    1);                                 //nelements
}

StatusAreaRenderer::~StatusAreaRenderer()
{
    XEventListener::unregisterEventFilter(this);

    scene->removeItem(statusArea);
    delete statusArea;

    if (statusAreaLivePixmap != NULL) {
        delete statusAreaLivePixmap;
    }

    if (statusAreaPropertyWindow != 0) {
        X11Wrapper::XDeleteProperty(QX11Info::display(),
                        QX11Info::appRootWindow(QX11Info::appScreen()),
                        X11Wrapper::XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PROPERTY_WINDOW", False));

        X11Wrapper::XDeleteProperty(QX11Info::display(),
                        statusAreaPropertyWindow,
                        X11Wrapper::XInternAtom(QX11Info::display(), "_MEEGOTOUCH_STATUSBAR_PIXMAP", False));
    }

    if (!statusAreaPixmap.isNull()) {
        Pixmap pixmap = statusAreaPixmap.handle();
        if (pixmap != 0) {
            X11Wrapper::XFreePixmap(QX11Info::display(), pixmap);
        }
    }

#ifdef HAVE_QMSYSTEM
    delete displayState;
#endif
}

void StatusAreaRenderer::accumulateSceneChanges(const QList<QRectF> &region)
{
    foreach(const QRectF & r, region) {
        accumulatedRegion = accumulatedRegion.united(r);
    }

    if (statusBarVisible && renderScene && !accumulationTimer.isActive()) {
        accumulationTimer.setSingleShot(true);
        accumulationTimer.start(ACCUMULATION_INTERVAL);
    }
}

void StatusAreaRenderer::renderAccumulatedRegion()
{
    if (!accumulatedRegion.isEmpty() && !statusAreaPixmap.isNull() && !backPixmap.isNull()) {
        if (statusAreaLivePixmap && !QMeeGoGraphicsSystemHelper::isRunningMeeGo()) {
            QMeeGoGraphicsSystemHelper::switchToMeeGo();
        }

        // Don't draw areas that are outside the pixmap
        if(accumulatedRegion.intersects(statusAreaPixmap.rect())) {
            QPainter painter;
            QRectF sourceRect = accumulatedRegion.intersected(statusAreaPixmap.rect());
            QImage *image = NULL;

            if (statusAreaLivePixmap) {
                image = statusAreaLivePixmap->lock();

                // FIXME: This shouldn't be needed after NB#231260 is fixed
                if (image->isNull()) {
                    createBackPixmap();
                    image = statusAreaLivePixmap->lock();
                }

                painter.begin(image);
            } else {
                painter.begin(&backPixmap);
            }

            if(painter.isActive()) {
                painter.fillRect(sourceRect, QColor(Qt::black));
                scene->render(&painter, sourceRect, sourceRect);
            }
            painter.end();

            if (statusAreaLivePixmap) {
                statusAreaLivePixmap->release(image);
            }

            painter.begin(&statusAreaPixmap);
            painter.drawPixmap(sourceRect, backPixmap, sourceRect);
            painter.end();
        }
        accumulatedRegion = QRectF(0.0f, 0.0f, 0.0f, 0.0f);
    }
}


void StatusAreaRenderer::setupStatusBarVisibleListener()
{
    Atom type;
    int format;
    unsigned long length, after;
    uchar *data = 0;

    bool success = false;
    if (X11Wrapper::XGetWindowProperty(QX11Info::display(), QX11Info::appRootWindow(), windowManagerWindowAtom,
                           0, 1024, False, XA_WINDOW, &type, &format, &length, &after, &data) == Success) {
        if (type == XA_WINDOW && format == 32) {
            windowManagerWindow = *((Window*) data);
            X11Wrapper::XFree(data);

            success = getStatusBarVisibleProperty();
        }
    }

    if (success) {
        XEventListener::registerEventFilter(this, PropertyChangeMask);
    } else {
        // Assume status bar is visible when WM window or _MEEGOTOUCH_STATUSBAR_VISIBLE property are not available
        statusBarVisible = true;
        XEventListener::unregisterEventFilter(this);
    }
}

bool StatusAreaRenderer::getStatusBarVisibleProperty()
{
    Atom type;
    int format;
    unsigned long length, after;
    uchar *data = 0;

    bool success = false;
    if (X11Wrapper::XGetWindowProperty(QX11Info::display(), windowManagerWindow, statusBarVisibleAtom,
                           0, 1024, False, XA_CARDINAL, &type, &format, &length, &after, &data) == Success) {
        statusBarVisible = *data;
        X11Wrapper::XFree(data);
        X11Wrapper::XSelectInput(QX11Info::display(), windowManagerWindow, PropertyChangeMask);
        success = true;
    }
    return success;
}

bool StatusAreaRenderer::xEventFilter(const XEvent &event)
{
    if (event.xproperty.window == windowManagerWindow && event.xproperty.atom == statusBarVisibleAtom) {
        if (!getStatusBarVisibleProperty()) {
            // Fetching property failed so try setuping the window and property again
            setupStatusBarVisibleListener();
        }
    }
    return false;
}

#ifdef HAVE_QMSYSTEM
void StatusAreaRenderer::setSceneRender(MeeGo::QmDisplayState::DisplayState state)
{
    bool oldRenderScene = renderScene;

    switch(state) {
    case MeeGo::QmDisplayState::Dimmed:
    case MeeGo::QmDisplayState::Off:
        renderScene = false;
        break;
    case MeeGo::QmDisplayState::On:
        renderScene = true;
        break;
    default:
        renderScene = true;
    }

    if (renderScene != oldRenderScene) {
        MOnDisplayChangeEvent::State state = renderScene ?
           MOnDisplayChangeEvent::FullyOnDisplay : MOnDisplayChangeEvent::FullyOffDisplay;

        MOnDisplayChangeEvent event(state, QRectF());
        foreach(QGraphicsItem *item, scene->items()) {
            scene->sendEvent(item, &event);
        }
    }
}
#endif
