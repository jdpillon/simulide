TEMPLATE = app

SOURCES += src/*.cpp \
    src/gui/*.cpp \
    src/gui/circuitwidget/*.cpp \
    src/gui/oscopewidget/*.cpp \
    src/gui/QPropertyEditor/*.cpp \
    src/simulator/*.cpp \
    src/simulator/elements/*.cpp \
    src/simulator/elements/processors/*.cpp \
    src/simulator/elements/processors/avr/*.cpp \
    src/simulator/graphic/*.cpp \
    src/simulator/graphic/components/*.cpp \
    src/simulator/graphic/components/mcu/*.cpp \
    src/simulator/graphic/components/mcu/avr/*.cpp \


HEADERS += src/*.h \
    src/gui/*.h \
    src/gui/circuitwidget/*.h \
    src/gui/oscopewidget/*.h \
    src/gui/QPropertyEditor/*.h \
    src/simulator/*.h \
    src/simulator/elements/*.h \
    src/simulator/elements/processors/*.h \
    src/simulator/elements/processors/avr/*.h \
    src/simulator/graphic/*.h \
    src/simulator/graphic/components/*.h \
    src/simulator/graphic/components/mcu/*.h \
    src/simulator/graphic/components/mcu/avr/*.h \

INCLUDEPATH += src \
    src/gui \
    src/gui/circuitwidget \
    src/gui/oscopewidget \
    src/gui/QPropertyEditor \
    src/simulator \
    src/simulator/elements \
    src/simulator/elements/processors \
    src/simulator/elements/processors/avr \
    src/simulator/graphic \
    src/simulator/graphic/components \
    src/simulator/graphic/components/mcu \
    src/simulator/graphic/components/mcu/avr \
    include \
    include/simavr/sim \

RESOURCES = application.qrc

# TRANSLATIONS += SimulIDE.ts
QT += xml

CONFIG += qt \
    warn_on \
#    thread

LIBS += $$_PRO_FILE_PWD_/include/simavr/libsimavr.a


TARGET = SimulIDE/SimulIDE

INCLUDEPATH += build

OBJECTS_DIR = build

MOC_DIR = build


