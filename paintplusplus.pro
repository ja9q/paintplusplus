QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    miscClasses/PaintModel.cpp \
    miscClasses/ResizeDialog.cpp \
    miscClasses/User.cpp \
    miscClasses/mainwindow.cpp \
    toolClasses/BaseTool.cpp \
    toolClasses/CircleTool.cpp \
    toolClasses/DrawTool.cpp \
    toolClasses/Editable.cpp \
    toolClasses/FillTool.cpp \
    toolClasses/LineTool.cpp \
    toolClasses/PolygonTool.cpp \
    toolClasses/SelectTool.cpp \
    toolClasses/ShapeTool.cpp \
    toolClasses/SquareSelectTool.cpp \
    toolClasses/SquareTool.cpp \
    toolClasses/TextTool.cpp \
    widgetClasses/CanvasWidget.cpp \
    widgetClasses/ColorPicker.cpp \
    widgetClasses/ColorWidget.cpp \
    widgetClasses/ToolSelector.cpp \
    widgetClasses/ToolSettingWidget.cpp

HEADERS += \
    miscClasses/PaintModel.h \
    miscClasses/ResizeDialog.h \
    miscClasses/User.h \
    miscClasses/mainwindow.h \
    toolClasses/BaseTool.h \
    toolClasses/CircleTool.h \
    toolClasses/DrawTool.h \
    toolClasses/Editable.h \
    toolClasses/FillTool.h \
    toolClasses/LineTool.h \
    toolClasses/PolygonTool.h \
    toolClasses/SelectTool.h \
    toolClasses/ShapeTool.h \
    toolClasses/SquareSelectTool.h \
    toolClasses/SquareTool.h \
    toolClasses/TextTool.h \
    widgetClasses/CanvasWidget.h \
    widgetClasses/ColorPicker.h \
    widgetClasses/ColorWidget.h \
    widgetClasses/ToolSelector.h \
    widgetClasses/ToolSettingWidget.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES +=
