cmake_minimum_required(VERSION 3.19)
project(untitled LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets)
qt_standard_project_setup()

# 🔑 ОБЯЗАТЕЛЬНО: для std::thread, std::atomic из твоего Timer.h
find_package(Threads REQUIRED)
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets Sql)

qt_add_executable(untitled
    WIN32 MACOSX_BUNDLE
    main.cpp
    mainModule_1.cpp
    mainModule_1.h
    mainModule_1.ui
    databaseqt.cpp
    databaseqt.h
    ../../Timer/Timer.cpp  # 🔑 Исходник таймера (путь от CMakeLists.txt)
)

target_link_libraries(untitled
    PRIVATE
        Qt::Core
        Qt${QT_VERSION_MAJOR}::Widgets
        Qt${QT_VERSION_MAJOR}::Sql
        Threads::Threads   # 🔑 Линковка библиотеки потоков
)

# 🔑 Скажи компилятору, где искать заголовок Timer.h
target_include_directories(untitled PRIVATE ../../Timer)

install(TARGETS untitled
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET untitled
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})