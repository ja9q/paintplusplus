QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    BaseTool.cpp \
    CanvasWidget.cpp \
    CircleTool.cpp \
    ColorPicker.cpp \
    ColorWidget.cpp \
    DrawTool.cpp \
    FillTool.cpp \
    LineTool.cpp \
    PaintModel.cpp \
    PolygonTool.cpp \
    ShapeTool.cpp \
    SquareTool.cpp \
    ToolSelector.cpp \
    ToolSettingWidget.cpp \
    User.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    BaseTool.h \
    CanvasWidget.h \
    CircleTool.h \
    ColorPicker.h \
    ColorWidget.h \
    DrawTool.h \
    FillTool.h \
    LineTool.h \
    PaintModel.h \
    PolygonTool.h \
    ShapeTool.h \
    SquareTool.h \
    ToolSelector.h \
    ToolSettingWidget.h \
    User.h \
    mainwindow.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
