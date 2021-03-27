QT -= gui
QT += remoteobjects widgets
QT += core

CONFIG += c++17 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    src/client.cpp \
    src/clientnode.cpp \
        src/dynamiclibrary.cpp \
        src/crawlserver.cpp \
  src/getter.cpp \
        src/indexingservice.cpp \
        src/linkchecker.cpp \
    src/testcrawlserver.cpp \
        src/testlinkchecker.cpp \
        src/main.cpp \
        src/schemapluginmanager.cpp \


# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../actors/release/ -lactors
else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../actors/debug/ -lactors
else:unix: LIBS += -L$$OUT_PWD/../actors/ -lactors

INCLUDEPATH += $$PWD/../actors/include
DEPENDPATH += $$PWD/../actors/include

INCLUDEPATH += $$PWD/include

HEADERS += \
  src/client.h \
  src/clientnode.h \
  src/dynamiclibrary.h \
  include/schemaplugin.h \
  src/crawlserver.h \
  src/getter.h \
  src/indexingservice.h \
  src/linkchecker.h \
  src/schemapluginmanager.h \
  src/testcrawlserver.h \
  src/testlinkchecker.h

REPC_SOURCE += \
    src/indexingservice.rep \
    src/clientnode.rep

#win32:CONFIG(release, debug|release): LIBS += -L$$OUT_PWD/../libs/QGumboParser/QGumboParser/release/ -lQGumboParser
#else:win32:CONFIG(debug, debug|release): LIBS += -L$$OUT_PWD/../libs/QGumboParser/QGumboParser/debug/ -lQGumboParser
##else:unix: LIBS += -L$$OUT_PWD/../libs/QGumboParser/QGumboParser/ -lQGumboParser

#INCLUDEPATH += $$PWD/../libs/QGumboParser/QGumboParser
#DEPENDPATH += $$PWD/../libs/QGumboParser/QGumboParser

#win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/QGumboParser/QGumboParser/release/libQGumboParser.a
#else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/QGumboParser/QGumboParser/debug/libQGumboParser.a
#else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/QGumboParser/QGumboParser/release/QGumboParser.lib
#else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$OUT_PWD/../libs/QGumboParser/QGumboParser/debug/QGumboParser.lib
#else:unix: PRE_TARGETDEPS += $$OUT_PWD/../libs/QGumboParseçr/QGumboParser/libQGumboParser.a

unix:!macx: LIBS += -L$$PWD/../libs/gumbo-query/lib/ -lgq

INCLUDEPATH += $$PWD/../libs/gumbo-query/src
DEPENDPATH += $$PWD/../libs/gumbo-query/src

unix:!macx: PRE_TARGETDEPS += $$PWD/../libs/gumbo-query/lib/libgq.a

unix: LIBS += -ldl

DISTFILES += \
    src/clientnode.rep \
    src/indexingservice.rep
