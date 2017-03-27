set(cpp_SRCS
    AbstractModel.cpp
    Debug.cpp
    Plugin.cpp
    Transaction.cpp
    TransactionModel.cpp
)

set(qml_SRCS
    qmldir
)

add_library(plasma-packagekit-declarative SHARED ${dbus_SRCS} ${cpp_SRCS} ${qml_SRCS})
target_link_libraries(plasma-packagekit-declarative
    Qt5::DBus
    Qt5::Quick
    PK::packagekitqt5
)

set(PRIVATE_QML_INSTALL_DIR ${QML_INSTALL_DIR}/org/kde/plasma/private/packagekit)
install(TARGETS plasma-packagekit-declarative DESTINATION ${PRIVATE_QML_INSTALL_DIR})
install(FILES ${qml_SRCS} DESTINATION ${PRIVATE_QML_INSTALL_DIR})
