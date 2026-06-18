cmake_minimum_required(VERSION 3.16)
project(PhoneBillingApp)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_PREFIX_PATH "/opt/Qt/6.5.0/gcc_64")

find_package(Qt6 REQUIRED COMPONENTS Core Widgets Sql)

# Core
set(CORE_SOURCES
    core/Config.cpp
    core/Logger.cpp
    core/DatabaseManager.cpp
)

set(CORE_HEADERS
    core/Config.h
    core/Logger.h
    core/DatabaseManager.h
)

# Models
set(MODEL_SOURCES
    models/user.cpp
    models/call.cpp
    models/calldetail.cpp
)

set(MODEL_HEADERS
    models/model.h
    models/user.h
    models/call.h
    models/calldetail.h
)

# Controllers (БЕЗ AbonentController)
set(CONTROLLER_SOURCES
    controllers/basecontroller.cpp
    controllers/authcontroller.cpp
    controllers/callcontroller.cpp
)

set(CONTROLLER_HEADERS
    controllers/basecontroller.h
    controllers/authcontroller.h
    controllers/callcontroller.h
)

# Views
set(VIEW_SOURCES
    views/loginview.cpp
    views/registerview.cpp
    views/mainwindow.cpp
    views/callview.cpp
)

set(VIEW_HEADERS
    views/loginview.h
    views/registerview.h
    views/mainwindow.h
    views/callview.h
)

add_executable(PhoneBillingApp
    main.cpp
    ${CORE_SOURCES}
    ${CORE_HEADERS}
    ${MODEL_SOURCES}
    ${MODEL_HEADERS}
    ${CONTROLLER_SOURCES}
    ${CONTROLLER_HEADERS}
    ${VIEW_SOURCES}
    ${VIEW_HEADERS}
)

target_link_libraries(PhoneBillingApp
    Qt6::Core
    Qt6::Widgets
    Qt6::Sql
)

install(TARGETS PhoneBillingApp DESTINATION bin)
install(FILES config.ini DESTINATION bin)