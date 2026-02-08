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
add_executable(${PROJECT_NAME} ${SOURCES}   "include/simple_gnuplot.h"  "include/scientific_file.h" "src/scientific_file.cpp" "src/simple_gnuplot.cpp")

# Заголовочные файлы
target_include_directories(${PROJECT_NAME} PRIVATE include)

# Настройки компилятора
if(MSVC)
    target_compile_options(${PROJECT_NAME} PRIVATE /W4 /utf-8)  # Убран /WX
    target_compile_definitions(${PROJECT_NAME} PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Wextra -Wpedantic)  # Убран -Werror
    target_link_options(${PROJECT_NAME} PRIVATE -static-libgcc -static-libstdc++)
endif()

# Копирование файлов данных после сборки
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/data)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            ${CMAKE_CURRENT_SOURCE_DIR}/data/raw.csv
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/data/
        COMMENT "Копирование файла данных"
        VERBATIM
    )
endif()