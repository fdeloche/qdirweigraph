QT += widgets \
    svg \
    webkit
SOURCES += \
    main.cpp \
    drawwidget.cpp \
    noeud.cpp \
    graphe.cpp \
    mainwindow.cpp \
    coord.cpp \
    graphexml.cpp \
    graphscale.cpp \
    graphoptions.cpp

HEADERS += \
    drawwidget.h \
    noeud.h \
    graphe.h \
    mainwindow.h \
    coord.h \
    coord.h \
    graphexml.h \
    graphscale.h \
    graphoptions.h

LIBS+= -ltinyxml

RESOURCES += \
    ressources.qrc
