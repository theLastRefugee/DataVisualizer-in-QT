QT       += core gui
QT       += charts
QT += webenginewidgets
QT       += webchannel

greaterThan(QT_MAJOR_VERSION, 5): QT += widgets

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    filevisualizer.cpp \
    global_variable.cpp \
    main.cpp \
    mainwindow.cpp \
    thread_function.cpp \
    timer.cpp \
    utils.cpp

HEADERS += \
    filevisualizer.h \
    global_variable.h \
    mainwindow.h \
    thread_function.h \
    timer.h \
    utils.h

FORMS += \
    filevisualizer.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    html_file/poi_heatmap.html \
    html_file/trajectory_map.html
