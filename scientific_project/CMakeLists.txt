cmake_minimum_required(VERSION 3.8)

project(scientific_project)

# Настройка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Включение горячей перезагрузки для компиляторов MSVC, если поддерживается.
if(POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

# Включаем настройки для отладки
if(MSVC)
    # Для Windows убираем /WX (предупреждения как ошибки), чтобы избежать проблем с кодировкой
    add_compile_options(/W4)  # Только предупреждения уровня 4, без трактовки как ошибок
else()
    add_compile_options(-Wall -Wextra -Wpedantic -Werror -g -O0)
endif()

# Структура проекта
set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# Основная программа
add_executable(scientific_project 
    ${SRC_DIR}/main.cpp 
    ${SRC_DIR}/math_utils.cpp 
    
)

# Подключаем заголовочные файлы
target_include_directories(scientific_project PUBLIC ${INCLUDE_DIR})

# Проверяем существование тестового файла
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_matrix.cpp)
    message(STATUS "Found test_matrix.cpp in tests/ directory")
    
    # Тесты для Matrix
    add_executable(matrix_tests
        
        
    )
    
    # Подключаем заголовочные файлы для тестов
    target_include_directories(matrix_tests PUBLIC ${INCLUDE_DIR})
    
    # На Windows отключаем /WX для тестов, чтобы избежать проблем с кодировкой
    if(MSVC)
        target_compile_options(matrix_tests PRIVATE /W4)  # Только предупреждения, не ошибки
    endif()
    
else()
    message(WARNING "test_matrix.cpp not found! Tests will not be built.")
endif()