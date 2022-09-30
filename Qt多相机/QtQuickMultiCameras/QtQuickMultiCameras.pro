QT += quick

SOURCES += \
        baslercamera.cpp \
        hub.cpp \
        imageprovider.cpp \
        main.cpp

resources.files = main.qml 
resources.prefix = /$${TARGET}
RESOURCES += resources

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    baslercamera.h \
    hub.h \
    imageprovider.h

#pylon
INCLUDEPATH += C:/Basler/pylon6/Development/include
LIBS += -LC:/Basler/pylon6/Development/lib/x64 \
        -lGCBase_MD_VC141_v3_1_Basler_pylon \
        -lGenApi_MD_VC141_v3_1_Basler_pylon \
        -lPylonBase_v6_3 \
        -lPylonC \
        -lPylonGUI_v6_3 \
        -lPylonUtility_v6_3
