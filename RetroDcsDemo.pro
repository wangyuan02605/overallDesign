QT += core gui widgets

CONFIG += c++17
CONFIG -= app_bundle

TEMPLATE = app
TARGET = retro_dcs_demo

macx {
    # Homebrew Qt 5 still requests the legacy AGL framework, which was
    # removed from recent macOS SDKs. This demo only needs modern OpenGL.
    QMAKE_LIBS_OPENGL = -framework OpenGL

    # Qt 5's mkspec predates recent macOS SDK version numbers.
    CONFIG += sdk_no_version_check
}

SOURCES += \
    src/main.cpp
