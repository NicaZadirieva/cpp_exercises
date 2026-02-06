cmake_minimum_required(VERSION 3.15)
project(scientific_project VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Устанавливаем выходные каталоги
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Проверяем существование исходных файлов
set(SOURCES
    src/scientific_file.cpp
    scientific_project.cpp
)

foreach(source ${SOURCES})
    if(NOT EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/${source})
        message(WARNING "Файл ${source} не найден!")
    endif()
endforeach()

# Создаем исполняемый файл
add_executable(${PROJECT_NAME} ${SOURCES})

# Заголовочные файлы
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Настройки компилятора
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /WX /utf-8)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic -Werror)
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif()

# Копирование файлов данных после сборки
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/data)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/data/test.csv
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/
        COMMENT "Копирование файла данных"
        VERBATIM
    )
endif()

# Устанавливаем рабочую директорию для отладки
set_target_properties(${PROJECT_NAME} PROPERTIES
    VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}"
)

# Проверка аргументов командной строки (опционально)
# Если программа требует аргументы, можно добавить тестовую цель
option(ADD_TEST_TARGET "Добавить цель для тестирования с аргументами" OFF)
if(ADD_TEST_TARGET)
    add_custom_target(run_${PROJECT_NAME}
        COMMAND ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${PROJECT_NAME} аргумент1 аргумент2
        DEPENDS ${PROJECT_NAME}
        WORKING_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}
        COMMENT "Запуск ${PROJECT_NAME} с тестовыми аргументами"
    )
endif()

# Вывод информации о сборке
message(STATUS "Проект: ${PROJECT_NAME}")
message(STATUS "Версия: ${PROJECT_VERSION}")
message(STATUS "Компилятор C++: ${CMAKE_CXX_COMPILER_ID}")
message(STATUS "Исполняемый файл будет создан в: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")