include(../mconfig.pri)

TEMPLATE = subdirs
SUBDIRS = \
    images \
    style \
    svg

INSTALLS += \
    conf

conf.files = \
    sysuid.conf
conf.path = $$SYSTEMUI_THEMES_DIR

QMAKE_EXTRA_TARGETS += check
check.commands = $$system(true)
QMAKE_EXTRA_TARGETS += check-xml
check-xml.commands = $$system(true)
