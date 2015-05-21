QT += widgets
SOURCES += \
    main.cpp \
    drawwidget.cpp \
    noeud.cpp \
    graphe.cpp \
    mainwindow.cpp \
    coord.cpp \
    graphexml.cpp

HEADERS += \
    drawwidget.h \
    noeud.h \
    graphe.h \
    mainwindow.h \
    coord.h \
    coord.h \
    graphexml.h

LIBS+= -ltinyxml
