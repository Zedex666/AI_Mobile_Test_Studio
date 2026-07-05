cmake_minimum_required(VERSION 3.19)
project(AI_Mobile_Test_Studio LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)

qt_standard_project_setup()

set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    resources.qrc
    app/widgets/navigationbar.h
    app/widgets/navigationbar.cpp
    app/widgets/devicepanel.h
    app/widgets/devicepanel.cpp
    app/widgets/chatpanel.h
    app/widgets/chatpanel.cpp
    app/widgets/bottompanel.h
    app/widgets/bottompanel.cpp
    app/widgets/titlebar.h
    app/widgets/titlebar.cpp
    core/adb/adbcontroller.h
    core/adb/adbcontroller.cpp
    core/appium/appiumcontroller.h
    core/appium/appiumcontroller.cpp
    core/logger/logger.h
    core/logger/logger.cpp
    core/llm/llm_provider.h
    core/llm/llm_provider.cpp
    core/config/configmanager.h
    core/config/configmanager.cpp
    core/scrcpy/scrcpycontroller.h
    core/scrcpy/scrcpycontroller.cpp
    services/chat_service.h
    services/chat_service.cpp
    services/device_service.h
    services/device_service.cpp
    services/ai_service.h
    services/ai_service.cpp
    services/script_service.h
    services/script_service.cpp
    services/plugin_service.h
    services/plugin_service.cpp
)

qt_add_executable(AI_Mobile_Test_Studio
    WIN32 MACOSX_BUNDLE
    ${PROJECT_SOURCES}
)

target_include_directories(AI_Mobile_Test_Studio PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/app
    ${CMAKE_CURRENT_SOURCE_DIR}/app/widgets
    ${CMAKE_CURRENT_SOURCE_DIR}/core
    ${CMAKE_CURRENT_SOURCE_DIR}/services
    ${CMAKE_CURRENT_SOURCE_DIR}/modules
)

target_link_libraries(AI_Mobile_Test_Studio
    PRIVATE
        Qt::Core
        Qt::Widgets
)

if(WIN32)
    target_link_libraries(AI_Mobile_Test_Studio PRIVATE user32)
endif()

# 复制资源文件到构建目录
add_custom_command(TARGET AI_Mobile_Test_Studio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/resources $<TARGET_FILE_DIR:AI_Mobile_Test_Studio>/resources
)

# 复制配置文件到构建目录
add_custom_command(TARGET AI_Mobile_Test_Studio POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/config $<TARGET_FILE_DIR:AI_Mobile_Test_Studio>/config
)

include(GNUInstallDirs)

install(TARGETS AI_Mobile_Test_Studio
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET AI_Mobile_Test_Studio
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
