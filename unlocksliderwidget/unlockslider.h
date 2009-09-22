#ifndef UNLOCKSLIDER_H
#define UNLOCKSLIDER_H

#include <DuiWidgetController>
#include "unlockslidermodel.h"

class UnlockSlider : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(UnlockSlider)

    Q_PROPERTY(bool handlePressed READ isHandlePressed)
    Q_PROPERTY(qreal position READ position WRITE setPosition)
    Q_PROPERTY(bool iconVisible READ isIconVisible WRITE setIconVisible)
    Q_PROPERTY(bool magnetic READ isMagnetic WRITE setMagnetic)

public:
    UnlockSlider();
    virtual ~UnlockSlider();

    /*!
      \brief Returns true if the slider handle is pressed down
      */
    bool isHandlePressed() const;

    /*!
      \brief Returns the slider position in 0..1 range
      */
    const qreal& position() const;

    /*!
      \brief Returns true if the icon at the end of the slider is visible
      */
    bool isIconVisible() const;

    /*!
      \brief Enables or disables the slider icon
    */
    void setIconVisible(bool);

    /*!
      \brief Returns true if the magnetic effect is enabled
      */
    bool isMagnetic() const;

    /*!
      \brief Enables or disables the magnetic effect on the slider
      The magnetic effect pulls the slider handle back to starting position if released.
      */
    void setMagnetic(bool);

signals:
    /*!
      \brief This signal is emitted when the slider is dragged to the end
      */
    void unlocked();

public Q_SLOTS:
    /*!
      \brief Sets the slider position (range 0..1)
      */
    void setPosition(const qreal& position);

    /*!
      \brief Starts the blinking effect to get user's attention to the handle
      */
    void blink();

protected slots:
    //! \reimp
    virtual void modelModified(const QList<const char*>& modifications);
    //! \reimp_end

};

#endif // UNLOCKSLIDER_H
