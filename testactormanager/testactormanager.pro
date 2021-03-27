QT += testlib widgets
QT -= gui

CONFIG += qt console warn_on depend_includepath testcase
CONFIG -= app_bundle

TEMPLATE = app

SOURCES +=  \
tst_actormanagertest.cpp

HEADERS += \
../testactors/testactor.h

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../actors/release/ -lactors
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../actors/debug/ -lactors
else:unix: LIBS += -L$$OUT_PWD/../actors/ -lactors

INCLUDEPATH += $$PWD/../actors/include
DEPENDPATH += $$PWD/../actors/include
