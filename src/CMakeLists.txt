cmake_minimum_required(VERSION 3.5)

project(CrossedWord VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_ENABLE_EXPORTS ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(JSON_BuildTests OFF CACHE INTERNAL "")

find_package(QT NAMES Qt6 Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets LinguistTools REQUIRED)

set(TS_FILES CrossedWord_fr_FR.ts)

set(PROJECT_SOURCES
        src/main.cpp
        src/model/utils.h
        src/view/MainWindow.cpp
        src/view/MainWindow.h
        src/view/mainwindow.ui
        src/view/ShowGrid.cpp
        src/view/ShowGrid.h
        src/view/ShowNumber.cpp
        src/view/ShowNumber.h
        src/view/DefTable.cpp
        src/view/DefTable.h
        src/model/generation/Lexicon.cpp
        src/model/generation/Lexicon.h
        src/model/Difficulty.cpp
        src/model/Difficulty.h
        src/model/generation/Solver.cpp
        src/model/generation/Solver.h
        src/model/CrosswordFacade.h
        src/model/Crossword.cpp
        src/model/Crossword.h
        src/controller/CrosswordController.cpp
        src/controller/CrosswordController.h
        src/model/generation/Pattern.cpp
        src/model/generation/Pattern.h
        src/model/generation/Variable.cpp
        src/model/generation/Variable.h
        src/model/generation/AttributedWords.h
        src/model/generation/AttributedWords.cpp
        src/model/generation/Domain.h
        src/model/generation/Domain.cpp
        ${TS_FILES}
)

set(PROJECT_SOURCES2
        src/model/utils.h
        src/ourTests.cpp
        src/view/MainWindow.cpp
        src/view/MainWindow.h
        src/view/mainwindow.ui
        src/view/ShowGrid.cpp
        src/view/ShowGrid.h
        src/view/ShowNumber.cpp
        src/view/ShowNumber.h
        src/view/DefTable.cpp
        src/view/DefTable.h
        src/model/generation/Lexicon.cpp
        src/model/generation/Lexicon.h
        src/model/Difficulty.cpp
        src/model/Difficulty.h
        src/model/generation/Solver.cpp
        src/model/generation/Solver.h
        src/model/CrosswordFacade.h
        src/model/Crossword.cpp
        src/model/Crossword.h
        src/controller/CrosswordController.cpp
        src/controller/CrosswordController.h
        src/model/generation/Pattern.cpp
        src/model/generation/Pattern.h
        src/model/generation/Variable.cpp
        src/model/generation/Variable.h
        src/model/generation/AttributedWords.h
        src/model/generation/AttributedWords.cpp
        src/model/generation/Domain.h
        src/model/generation/Domain.cpp
        ${TS_FILES}
)

add_subdirectory(build/nlohmann_json)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(CrossedWord
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
    qt_add_executable(CrossedWordTest
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES2}
    )

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    add_executable(CrossedWord
        ${PROJECT_SOURCES}
    )
    add_executable(CrossedWordTest ${PROJECT_SOURCES2})
endif()

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})

target_link_libraries(CrossedWord PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CrossedWord PRIVATE nlohmann_json::nlohmann_json)
#target_link_libraries(CrossedWord PRIVATE gtest_main)

set_target_properties(CrossedWord PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)

target_link_libraries(CrossedWordTest PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(CrossedWordTest PRIVATE nlohmann_json::nlohmann_json)
target_link_libraries(CrossedWordTest PRIVATE gtest_main)

set_target_properties(CrossedWordTest PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER my.example.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
)
enable_testing()
add_subdirectory(build/googletest)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(CrossedWord)
    qt_finalize_executable(CrossedWordTest)
endif()

add_test(
    NAME CrossedWordTest
    COMMAND CrossedWordTest
)