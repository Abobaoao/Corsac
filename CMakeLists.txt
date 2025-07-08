cmake_minimum_required(VERSION 3.10)
project(Corsac)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF) 

# Найти Qt6
find_package(Qt6 REQUIRED COMPONENTS Widgets)

# Указываем исходные файлы
set(SOURCES WIN32
    main.cpp
    mainwindow.cpp
    aboba/console/CommandSuggestion.cpp   # Путь к CommandSuggestion в папке aboba/console
    aboba/console/ConsoleLog.cpp  # Путь к ConsoleLog в папке aboba/console
    aboba/console/CommandInput.cpp  # Путь к CommandInput в папке aboba/console
    aboba/console/ConsoleDisplay.cpp # Путь к ConsoleDisplay в папке aboba/console
    aboba/console/Console.cpp
    aboba/sidebar/SidebarAnimator.cpp
    aboba/sidebar/SidebarButtons.cpp 
    aboba/tabs/BaseTab.cpp
    aboba/tabs/AboutTab.cpp
    aboba/tabs/SettingsTab.cpp
    aboba/tabs/SettingsHelper/SettingsIconButton.cpp
    aboba/tabs/SettingsHelper/SettingsSearchPanel.cpp
    aboba/tabs/SettingsHelper/SettingsSectionWidget.cpp
    aboba/tabs/SettingsHelper/ToggleSwitch.cpp
)

# Указываем заголовочные файлы
set(HEADERS
    mainwindow.h
    aboba/console/CommandSuggestion.h   # Путь к CommandSuggestion в папке aboba/console
    aboba/console/ConsoleLog.h  # Путь к ConsoleLog в папке aboba/console
    aboba/console/CommandInput.h  # Путь к CommandInput в папке aboba/console
    aboba/console/ConsoleDisplay.h  # Путь к ConsoleDisplay в папке aboba/console
    aboba/console/Console.h
    aboba/sidebar/SidebarAnimator.h
    aboba/sidebar/SidebarButtons.h
    aboba/tabs/BaseTab.h
    aboba/tabs/SettingsTab.h
    aboba/tabs/AboutTab.h
    aboba/tabs/SettingsHelper/SettingsIconButton.h
    aboba/tabs/SettingsHelper/SettingsSearchPanel.h
    aboba/tabs/SettingsHelper/SettingsSectionWidget.h
    aboba/tabs/SettingsHelper/ToggleSwitch.h
)

# Указываем, что проект использует Qt
qt6_wrap_cpp(MOC_SOURCES ${HEADERS})  # Генерация moc-файлов для всех заголовочных файлов

set(RC_FILE app.rc)

# Собираем исполняемый файл
add_executable(Corsac ${SOURCES} ${MOC_SOURCES} ${RC_FILE})

# Линковка с библиотеками Qt
target_link_libraries(Corsac PRIVATE Qt6::Widgets)
