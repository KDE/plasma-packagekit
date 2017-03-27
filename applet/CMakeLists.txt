plasma_install_package(. org.kde.plasma.packagekit)

file(GLOB QML_SRCS contents/*/*.qml)
file(GLOB JS_SRCS contents/*/*.js)
file(GLOB XML_SRCS contents/*/*.xml)
file(GLOB EXTRA_SRCS metadata.desktop *.sh)
add_custom_target(QmlFiles ALL echo SOURCES ${QML_SRCS} ${JS_SRCS} ${XML_SRCS} ${EXTRA_SRCS})
