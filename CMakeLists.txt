cmake_minimum_required(VERSION 3.22)

# Read version from VERSION file
file(READ "${CMAKE_SOURCE_DIR}/VERSION" PROJECT_VERSION_STRING)
string(STRIP "${PROJECT_VERSION_STRING}" PROJECT_VERSION_STRING)

# Parse version components
string(REGEX MATCH "^([0-9]+)\\.([0-9]+)\\.([0-9]+)" VERSION_MATCH "${PROJECT_VERSION_STRING}")
if(VERSION_MATCH)
    string(REGEX REPLACE "^([0-9]+)\\.[0-9]+\\.[0-9]+" "\\1" VERSION_MAJOR "${PROJECT_VERSION_STRING}")
    string(REGEX REPLACE "^[0-9]+\\.([0-9]+)\\.[0-9]+" "\\1" VERSION_MINOR "${PROJECT_VERSION_STRING}")
    string(REGEX REPLACE "^[0-9]+\\.[0-9]+\\.([0-9]+)" "\\1" VERSION_PATCH "${PROJECT_VERSION_STRING}")
else()
    set(VERSION_MAJOR "1")
    set(VERSION_MINOR "0")
    set(VERSION_PATCH "0")
endif()

project(finirig VERSION ${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH} LANGUAGES C CXX)

# Store full version string
set(PROJECT_VERSION_FULL "${PROJECT_VERSION_STRING}")
message(STATUS "Finirig version: ${PROJECT_VERSION_FULL}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Generate compile_commands.json for IntelliSense
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Build options
option(BUILD_TESTS "Build tests" ON)
option(BUILD_STANDALONE "Build standalone application" ON)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)

# Include directories
include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/src
)

# Find Qt6
find_package(Qt6 REQUIRED COMPONENTS Core Widgets)

# Enable Qt MOC, UIC, RCC
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# JUCE setup
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/third_party/JUCE")
    message(FATAL_ERROR "JUCE not found. Please clone JUCE into third_party/JUCE")
endif()

add_subdirectory(third_party/JUCE)

# Source files
set(SOURCES
    src/main.cpp
    src/audio/AudioEngine.cpp
    src/audio/AudioProcessor.cpp
    src/pedals/PedalBase.cpp
    src/pedals/OverdrivePedal.cpp
    src/amps/AmpModel.cpp
    src/ui/MainWindow.cpp
    src/ui/AudioControlsWidget.cpp
    src/ui/LevelMeterWidget.cpp
    src/ui/DeviceInfoWidget.cpp
)

set(HEADERS
    include/finirig/audio/AudioEngine.h
    include/finirig/audio/AudioProcessor.h
    include/finirig/pedals/PedalBase.h
    include/finirig/pedals/OverdrivePedal.h
    include/finirig/amps/AmpModel.h
    include/finirig/ui/MainWindow.h
    include/finirig/ui/AudioControlsWidget.h
    include/finirig/ui/LevelMeterWidget.h
    include/finirig/ui/DeviceInfoWidget.h
)

# Standalone application
if(BUILD_STANDALONE)
    add_executable(finirig_standalone
        ${SOURCES}
        ${HEADERS}
    )

    target_link_libraries(finirig_standalone PRIVATE
        Qt6::Core
        Qt6::Widgets
    )

    # JUCE modules (audio only, no GUI modules since we use Qt)
    target_link_libraries(finirig_standalone PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_audio_formats
        juce::juce_audio_processors
        juce::juce_audio_utils
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
    )

    # Platform-specific settings
    if(APPLE)
        # macOS bundle configuration
        set_target_properties(finirig_standalone PROPERTIES
            MACOSX_BUNDLE TRUE
            MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/resources/Info.plist
            MACOSX_BUNDLE_GUI_IDENTIFIER "com.finirig.app"
            MACOSX_BUNDLE_ICON_FILE "Finirig.icns"
        )
        set_target_properties(finirig_standalone PROPERTIES
            OUTPUT_NAME "Finirig"
        )
        
        # Copy icons to bundle
        set_source_files_properties(
            ${CMAKE_SOURCE_DIR}/resources/Finirig.icns
            PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
        )
        set_source_files_properties(
            ${CMAKE_SOURCE_DIR}/resources/icon.png
            PROPERTIES MACOSX_PACKAGE_LOCATION "Resources"
        )
        target_sources(finirig_standalone PRIVATE
            ${CMAKE_SOURCE_DIR}/resources/Finirig.icns
            ${CMAKE_SOURCE_DIR}/resources/icon.png
        )
    elseif(WIN32)
        # Windows icon
        if(EXISTS ${CMAKE_SOURCE_DIR}/resources/Finirig.ico)
            set_target_properties(finirig_standalone PROPERTIES
                RC_ICONS ${CMAKE_SOURCE_DIR}/resources/Finirig.ico
            )
        endif()
    elseif(UNIX AND NOT APPLE)
        # Linux: Install desktop file and icon
        install(FILES ${CMAKE_SOURCE_DIR}/resources/Finirig.png
            DESTINATION share/pixmaps
            RENAME finirig.png
        )
        install(FILES ${CMAKE_SOURCE_DIR}/resources/finirig.desktop
            DESTINATION share/applications
        )
    endif()
    
    # Install rules (platform-agnostic)
    install(TARGETS finirig_standalone
        BUNDLE DESTINATION .
        RUNTIME DESTINATION bin
    )
endif()

# Testing
if(BUILD_TESTS)
    enable_testing()
    
    # Find Catch2 or Google Test
    find_package(Catch2 3 QUIET)
    if(NOT Catch2_FOUND)
        include(FetchContent)
        FetchContent_Declare(
            Catch2
            GIT_REPOSITORY https://github.com/catchorg/Catch2.git
            GIT_TAG v3.5.0
        )
        FetchContent_MakeAvailable(Catch2)
    endif()

    # Test executable
    add_executable(finirig_tests
        tests/test_main.cpp
        tests/audio/test_audio_processor.cpp
        tests/pedals/test_pedal_base.cpp
        tests/pedals/test_overdrive_pedal.cpp
        tests/amps/test_amp_model.cpp
    )

    # Disable AUTOMOC for tests (tests don't use Qt)
    set_target_properties(finirig_tests PROPERTIES
        AUTOMOC OFF
        AUTOUIC OFF
        AUTORCC OFF
    )

    target_link_libraries(finirig_tests PRIVATE
        Catch2::Catch2WithMain  # Includes main() function
    )

    # Link against source files for testing (excluding UI files that need Qt)
    target_sources(finirig_tests PRIVATE
        src/audio/AudioEngine.cpp
        src/audio/AudioProcessor.cpp
        src/pedals/PedalBase.cpp
        src/pedals/OverdrivePedal.cpp
        src/amps/AmpModel.cpp
        include/finirig/audio/AudioEngine.h
        include/finirig/audio/AudioProcessor.h
        include/finirig/pedals/PedalBase.h
        include/finirig/pedals/OverdrivePedal.h
        include/finirig/amps/AmpModel.h
    )

    # JUCE modules for tests (AudioEngine needs audio_devices and graphics for Colour)
    target_link_libraries(finirig_tests PRIVATE
        juce::juce_audio_basics
        juce::juce_audio_devices
        juce::juce_core
        juce::juce_data_structures
        juce::juce_events
        juce::juce_graphics
    )

    include(CTest)
    include(Catch)
    catch_discover_tests(finirig_tests)
endif()

