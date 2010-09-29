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

#ifndef NOTIFICATIONAREA_H_
#define NOTIFICATIONAREA_H_

#include <MWidgetController>
#include "notificationsink.h"
#include "notificationareamodel.h"

class NotificationManager;
class NotificationAreaSink;
class MBanner;

/*!
 * The NotificationArea is a widget that shows notifications.
 */
class NotificationArea : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(NotificationArea)

public:
    /*!
     * Constructs a NotificationArea widget.
     *
     * \param parent the parent widget of the NotificationArea, defaults to NULL
     */
    NotificationArea(MWidget *parent = NULL);

    /*!
     * Destroys the NotificationArea.
     */
    virtual ~NotificationArea();

signals:
    /*!
     * Requests removal of a notification from the notification system.
     * \param notificationId Id of the notification to be removed from the system.
     */
    void notificationRemovalRequested(uint notificationId);

    /*!
     * Requests removal of notifications in a group.
     * \param groupId Id of the group to be cleared.
     */
    void notificationGroupClearingRequested(uint groupId);

private slots:
    /*!
     * Adds a notification to the notification area.
     *
     * \param notification the MBanner to be added
     */
    void addNotification(MBanner &notification);

    /*!
     * Moves the banner to top. Called when relayouting is asked by notification area sink
     * when a notification/group is updated.
     *
     * \param notification the MBanner to be made top
     */
    void moveNotificationToTop(MBanner &notification);

    /*!
     * Removes a notification from the notification area.
     *
     * \param notification the MBanner to be removed
     */
    void removeNotification(MBanner &notification);

    //! Requests the sink to remove all notifications that have removable banners in the model
    void removeAllRemovableBanners();

signals:
    /*!
     * \brief A signal that is emitted whenever an event banner on the notification area is clicked
     */
    void bannerClicked();

private:
    //! Notification sink for visualizing the notification on the notification area
    NotificationAreaSink *notificationAreaSink;

#ifdef UNIT_TEST
    friend class Ut_NotificationArea;
#endif
};

#endif /* NOTIFICATIONAREA_H_ */