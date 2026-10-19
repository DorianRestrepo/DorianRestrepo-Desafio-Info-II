QT = core

CONFIG += c++17 cmdline

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        Album.cpp \
        Consola.cpp \
        Fecha.cpp \
        Foto.cpp \
        Mensaje.cpp \
        Recursos.cpp \
        UdeABookGestor.cpp \
        Usuario.cpp \
        main.cpp \
        menuFun.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    Album.h \
    Configuracion.h \
    Consola.h \
    Fecha.h \
    Foto.h \
    ListaDinamica.h \
    Mensaje.h \
    Recursos.h \
    UdeABookGestor.h \
    Usuario.h \
    menuFun.h
