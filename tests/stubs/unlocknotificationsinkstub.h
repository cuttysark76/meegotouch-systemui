#ifndef UNLOCKNOTIFICATIONSINKSTUB_H
#define UNLOCKNOTIFICATIONSINKSTUB_H

#include <unlocknotificationsink.h>

UnlockNotificationSink::UnlockNotificationSink ()
{
    m_enabled = true;
}

void
UnlockNotificationSink::addNotification(const Notification &notification)
{
    Q_UNUSED (notification);
}

void
UnlockNotificationSink::removeNotification(uint notificationId)
{
    Q_UNUSED (notificationId);
}

void
UnlockNotificationSink::setLockedState(bool islocked)
{
    Q_UNUSED (islocked);
}

#endif
