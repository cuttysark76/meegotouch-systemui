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

#include "dbusinterfacenotificationsource.h"
#include "dbusinterfacenotificationsourceadaptor.h"

DBusInterfaceNotificationSource::DBusInterfaceNotificationSource(NotificationManagerInterface &interface)
    : NotificationSource(interface)
{
    new DBusInterfaceNotificationSourceAdaptor(this);
}

uint DBusInterfaceNotificationSource::notificationUserId()
{
    return manager.notificationUserId();
}

uint DBusInterfaceNotificationSource::addNotification(uint notificationUserId, uint groupId, const QString &eventType, NotificationManagerInterface::NotificationType type)
{
    return manager.addNotification(notificationUserId, notificationParameters(eventType), groupId, type);
}

uint DBusInterfaceNotificationSource::addNotification(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count, NotificationManagerInterface::NotificationType type)
{
    return manager.addNotification(notificationUserId, notificationParameters(eventType, summary, body, action, imageURI, count), groupId, type);
}

bool DBusInterfaceNotificationSource::updateNotification(uint notificationUserId, uint notificationId, const QString &eventType)
{
    return manager.updateNotification(notificationUserId, notificationId, notificationParameters(eventType));
}

bool DBusInterfaceNotificationSource::updateNotification(uint notificationUserId, uint notificationId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return manager.updateNotification(notificationUserId, notificationId, notificationParameters(eventType, summary, body, action, imageURI, count));
}

bool DBusInterfaceNotificationSource::removeNotification(uint notificationUserId, uint notificationId)
{
    return manager.removeNotification(notificationUserId, notificationId);
}

uint DBusInterfaceNotificationSource::addGroup(uint notificationUserId, const QString &eventType)
{
    return manager.addGroup(notificationUserId, notificationParameters(eventType));
}

uint DBusInterfaceNotificationSource::addGroup(uint notificationUserId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return manager.addGroup(notificationUserId, notificationParameters(eventType, summary, body, action, imageURI, count));
}

bool DBusInterfaceNotificationSource::updateGroup(uint notificationUserId, uint groupId, const QString &eventType)
{
    return manager.updateGroup(notificationUserId, groupId, notificationParameters(eventType));
}

bool DBusInterfaceNotificationSource::updateGroup(uint notificationUserId, uint groupId, const QString &eventType, const QString &summary, const QString &body, const QString &action, const QString &imageURI, uint count)
{
    return manager.updateGroup(notificationUserId, groupId, notificationParameters(eventType, summary, body, action, imageURI, count));
}

bool DBusInterfaceNotificationSource::removeGroup(uint notificationUserId, uint groupId)
{
    return manager.removeGroup(notificationUserId, groupId);
}

QList<uint> DBusInterfaceNotificationSource::notificationIdList(uint notificationUserId)
{
    return manager.notificationIdList(notificationUserId);
}
