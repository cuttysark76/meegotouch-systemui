/*
 * This file is part of system ui
 *
 * Copyright (C) 2009 Nokia Corporation. All rights reserved.
 *
 * This software, including documentation, is protected by copyright
 * controlled by Nokia Corporation. All rights are reserved.
 * Copying, including reproducing, storing, adapting or translating,
 * any or all of this material requires the prior written consent of
 * Nokia Corporation. This material also contains confidential
 * information which may not be disclosed to others without the prior
 * written consent of Nokia.
 */

#ifndef STATUSINDICATORMENUWINDOW_H
#define STATUSINDICATORMENUWINDOW_H

#include <DuiWindow>

class DuiApplicationPage;
class DuiEscapeButtonPanel;

/*!
 * Main window for the DUI Status Indicator Menu.
 * The window contains an application page which is populated
 * with a list of plugins.
 */
class StatusIndicatorMenuWindow : public DuiWindow
{
    Q_OBJECT

public:
    /*!
     * Constructs a main window for the DUI Status Indicator Menu.
     *
     * \param parent Parent for the widget, defaults to NULL
     */
    StatusIndicatorMenuWindow(QWidget *parent = NULL);

    /*!
     * Destructor
     */
    ~StatusIndicatorMenuWindow();

private:
    /*!
     * Sets the X window properties for the window so that the window is not
     * included in the task bar.
     */
    void excludeFromTaskBar();

    //! The main application page
    QSharedPointer<DuiApplicationPage> applicationPage;

    //! The escape button
    QSharedPointer<DuiEscapeButtonPanel> escapeButtonPanel;
};

#endif // STATUSINDICATORMENUWINDOW_H
