
QT       += core gui
QT += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    ajoutcontact.cpp \
    ajoutinteraction.cpp \
    contact.cpp \
    detailinteraction.cpp \
    fichecontact.cpp \
    gestionbdd.cpp \
    gestionlistecontacts.cpp \
    gestionlisteinteractions.cpp \
    gestionlistetaches.cpp \
    interraction.cpp \
    listetaches.cpp \
    listetotalitetaches.cpp \
    main.cpp \
    premierefenetre.cpp \
    todo.cpp

HEADERS += \
    ajoutcontact.h \
    ajoutinteraction.h \
    contact.h \
    date.h \
    detailinteraction.h \
    fichecontact.h \
    gestionbdd.h \
    gestionlistecontacts.h \
    gestionlisteinteractions.h \
    gestionlistetaches.h \
    interraction.h \
    listetaches.h \
    listetotalitetaches.h \
    premierefenetre.h \
    todo.h

FORMS += \
    ajoutcontact.ui \
    ajoutinteraction.ui \
    detailinteraction.ui \
    fichecontact.ui \
    listetaches.ui \
    listetotalitetaches.ui \
    premierefenetre.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
