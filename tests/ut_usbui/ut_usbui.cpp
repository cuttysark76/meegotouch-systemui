/***************************************************************************
 **
 ** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
 ** All rights reserved.
 ** Contact: Nokia Corporation (directui@nokia.com)
 **
 ** This file is part of system-ui.
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
#include <MApplication>
#include <MSceneWindow>
#include <MNotification>
#include <MLabel>
#include <MBasicListItem>
#include <MGConfItem>
#include <QGraphicsLinearLayout>
#include <QtTest/QtTest>
#include <usbui.h>

#include "ut_usbui.h"
#include "usbmode_stub.h"
#include "qmlocks_stub.h"

static bool dialog_visible = false;

void MDialog::appear(MSceneWindow::DeletionPolicy)
{
    dialog_visible = true;
}

void MSceneWindow::disappear()
{
    dialog_visible = false;
}

QStringList mNotificationBodies;
bool MNotification::publish()
{
    mNotificationBodies.append(body());
    return true;
}

bool MNotification::isPublished() const
{
    return true;
}

bool MNotification::remove()
{
    return true;
}

QVariant mGConfItemValue(false);
QVariant MGConfItem::value() const
{
    return mGConfItemValue;
}

int argc = 1;
char *argv[] = { (char *) "./ut_usbui", NULL };

void Ut_UsbUi::initTestCase()
{
    m_App = new MApplication(argc, argv);
}

void Ut_UsbUi::cleanupTestCase()
{
    m_App->deleteLater();
}

void Ut_UsbUi::init()
{
    m_subject = new UsbUi;
#ifdef HAVE_QMSYSTEM
    m_subject->usbMode->setMode(MeeGo::QmUSBMode::Undefined);
#endif
}

void Ut_UsbUi::cleanup()
{
    delete m_subject;
    dialog_visible = false;
    mGConfItemValue = QVariant(false);
    mNotificationBodies.clear();
}

#ifdef HAVE_QMSYSTEM
void Ut_UsbUi::testConnections()
{
    QVERIFY(disconnect(m_subject->usbMode, SIGNAL(modeChanged(MeeGo::QmUSBMode::Mode)), m_subject, SLOT(applyUSBMode(MeeGo::QmUSBMode::Mode))));
}

Q_DECLARE_METATYPE(MeeGo::QmUSBMode::Mode)

void Ut_UsbUi::testShowDialog_data()
{
    QTest::addColumn<MeeGo::QmUSBMode::Mode>("mode");

    QTest::newRow("Ask") << MeeGo::QmUSBMode::Ask;
    QTest::newRow("Mode request") << MeeGo::QmUSBMode::ModeRequest;
}

void Ut_UsbUi::testShowDialog()
{
    QFETCH(MeeGo::QmUSBMode::Mode, mode);

    QSignalSpy spy(m_subject, SIGNAL(dialogShown()));
    m_subject->usbMode->setDefaultMode(mode);
    m_subject->applyUSBMode(mode);
    QCOMPARE(dialog_visible, true);
    QVERIFY(!m_subject->isModal());
    QVERIFY(m_subject->isSystem());
    QCOMPARE(spy.count(), 1);
}

void Ut_UsbUi::testHideDialog_data()
{
    QTest::addColumn<MeeGo::QmUSBMode::Mode>("mode");

    QTest::newRow("Disconnected") << MeeGo::QmUSBMode::Disconnected;
    QTest::newRow("Ovi Suite") << MeeGo::QmUSBMode::OviSuite;
    QTest::newRow("Mass Storage") << MeeGo::QmUSBMode::MassStorage;
    QTest::newRow("SDK") << MeeGo::QmUSBMode::SDK;
}

void Ut_UsbUi::testHideDialog()
{
    QFETCH(MeeGo::QmUSBMode::Mode, mode);

    m_subject->usbMode->setDefaultMode(MeeGo::QmUSBMode::Ask);
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Ask);
    m_subject->applyUSBMode(mode);
    QCOMPARE(dialog_visible, false);
}

void Ut_UsbUi::testUSBNotifications_data()
{
    QTest::addColumn<MeeGo::QmUSBMode::Mode>("mode");
    QTest::addColumn<QString>("body");

    QTest::newRow("Disconnected") << MeeGo::QmUSBMode::Disconnected << qtTrId("qtn_usb_disconnected");
    QTest::newRow("Ovi Suite") << MeeGo::QmUSBMode::OviSuite << qtTrId("qtn_usb_sync_active");
    QTest::newRow("Mass Storage") << MeeGo::QmUSBMode::MassStorage << qtTrId("qtn_usb_storage_active");
    QTest::newRow("SDK") << MeeGo::QmUSBMode::SDK << qtTrId("qtn_usb_sdk_active");
}

void Ut_UsbUi::testUSBNotifications()
{
    QFETCH(MeeGo::QmUSBMode::Mode, mode);
    QFETCH(QString, body);

    m_subject->applyUSBMode(mode);
    QCOMPARE(mNotificationBodies.last(), body);
}

void Ut_UsbUi::testDialogButtons()
{
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Connected);

    m_subject->setOviSuiteMode();
    m_subject->setRequestedUSBMode();
    QCOMPARE(m_subject->usbMode->getMode(), MeeGo::QmUSBMode::OviSuite);

    m_subject->setMassStorageMode();
    m_subject->setRequestedUSBMode();
    QCOMPARE(m_subject->usbMode->getMode(), MeeGo::QmUSBMode::MassStorage);

    m_subject->setSDKMode();
    m_subject->setRequestedUSBMode();
    QCOMPARE(m_subject->usbMode->getMode(), MeeGo::QmUSBMode::SDK);
}

Q_DECLARE_METATYPE(MeeGo::QmLocks::State)

void Ut_UsbUi::testConnectingUSBWhenDeviceIsLockedEmitsDialogShown_data()
{
    QTest::addColumn<MeeGo::QmLocks::State>("deviceLocked");
    QTest::addColumn<int>("dialogShownCount");
    QTest::newRow("Device locked") << MeeGo::QmLocks::Locked << 1;
    QTest::newRow("Device not locked") << MeeGo::QmLocks::Unlocked << 0;
}

void Ut_UsbUi::testConnectingUSBWhenDeviceIsLockedEmitsDialogShown()
{
    QFETCH(MeeGo::QmLocks::State, deviceLocked);
    QFETCH(int, dialogShownCount);

    QSignalSpy spy(m_subject, SIGNAL(dialogShown()));
    gQmLocksStub->stubSetReturnValue("getState", deviceLocked);
    m_subject->applyUSBMode(MeeGo::QmUSBMode::Connected);
    QCOMPARE(spy.count(), dialogShownCount);
}
#endif

void Ut_UsbUi::testShowError()
{
    m_subject->showError("test");
    QCOMPARE(mNotificationBodies.isEmpty(), true);

    m_subject->showError("qtn_usb_filessystem_inuse");
    QCOMPARE(mNotificationBodies.last(), qtTrId("qtn_usb_filessystem_inuse"));

    m_subject->showError("mount_failed");
    QCOMPARE(mNotificationBodies.last(), qtTrId("qtn_usb_mount_failed"));
}

void Ut_UsbUi::testRetranslateUi()
{
    m_subject->setTitle(QString());
    m_subject->chargingLabel->setText(QString());
    m_subject->massStorageItem->setTitle(QString());
    m_subject->oviSuiteItem->setTitle(QString());
    m_subject->sdkItem->setTitle(QString());
    m_subject->retranslateUi();
    QCOMPARE(m_subject->title(), qtTrId("qtn_usb_connected"));
    QCOMPARE(m_subject->chargingLabel->text(), qtTrId("qtn_usb_charging"));
    QCOMPARE(m_subject->massStorageItem->title(), qtTrId("qtn_usb_mass_storage"));
    QCOMPARE(m_subject->oviSuiteItem->title(), qtTrId("qtn_usb_ovi_suite"));
    QCOMPARE(m_subject->sdkItem->title(), qtTrId("qtn_usb_sdk_mode"));
}

void Ut_UsbUi::testSDKItemVisibleOnlyWhenDeveloperModeEnabled()
{
    QVERIFY(disconnect(m_subject->developerMode, SIGNAL(valueChanged()), m_subject, SLOT(updateSDKItemVisibility())));
    delete m_subject;

    mGConfItemValue = QVariant(true);
    m_subject = new UsbUi;
    QCOMPARE(m_subject->layout->count(), 4);
    QCOMPARE(m_subject->sdkItem->parentLayoutItem(), m_subject->layout);

    mGConfItemValue = QVariant(false);
    m_subject->updateSDKItemVisibility();
    QCOMPARE(m_subject->layout->count(), 3);
    QCOMPARE(m_subject->sdkItem->parentLayoutItem(), (QGraphicsLayoutItem *)NULL);
}

QTEST_APPLESS_MAIN (Ut_UsbUi)
