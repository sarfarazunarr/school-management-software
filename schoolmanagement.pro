QT += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    exam.cpp \
    main.cpp \
    mainwindow.cpp \
    newstudent.cpp \
    newteacher.cpp \
    newtimetable.cpp \
    staff.cpp \
    student.cpp \
    teacher.cpp \
    timetable.cpp \
    updatestudent.cpp \
    updateteacher.cpp

HEADERS += \
    exam.h \
    mainwindow.h \
    newstudent.h \
    newteacher.h \
    newtimetable.h \
    staff.h \
    student.h \
    teacher.h \
    timetable.h \
    updatestudent.h \
    updateteacher.h

FORMS += \
    exam.ui \
    mainwindow.ui \
    newstudent.ui \
    newteacher.ui \
    newtimetable.ui \
    staff.ui \
    student.ui \
    teacher.ui \
    timetable.ui \
    updatestudent.ui \
    updateteacher.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    assets.qrc
