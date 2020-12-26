CONFIG += qt c++11

# Add plugins that are used
QT += quick
QT += qml

# Errors, delicious errors.
QMAKE_CXXFLAGS += -Wall -Wextra

LIBS += -lvlc -lX11
LIBS += -lmusicbrainz5cc -lm -lstdc++ -lcoverartcc

DEFINES += QT_DEPRECATED_WARNINGS

RESOURCES += qml.qrc

SOURCES += \
        src/main.cpp \
        src/cpp/AudioPlayer.cpp \
        src/cpp/Base64.cpp \
        src/cpp/CoverArt.cpp \
        src/cpp/RadioStation.cpp \
        src/cpp/RadioStations.cpp

HEADERS += \
        src/cpp/AudioPlayer.hpp \
        src/cpp/Base64.hpp \
        src/cpp/CoverArt.hpp \
        src/cpp/RadioStation.hpp \
        src/cpp/RadioStations.hpp

# All this stuff to just copy the res directory to the build directory...
copydata.commands = $(COPY_DIR) $$PWD/res $$OUT_PWD
first.depends = $(first) copydata
export(first.depends)
export(copydata.commands)
QMAKE_EXTRA_TARGETS += first copydata
