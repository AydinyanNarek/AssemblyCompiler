################################################################################
######
###### AssemblyCompiler Configuration File
######
################################################################################
######
###### Example Usage
######

cmake_minimum_required(VERSION 3.3.0)

project(AssemblerInterpretator)

set(PROJECT_NAME AssemblerInterpretator)
set(PROJECT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/${PROJECT_NAME})

if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
  set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "None" "Debug" "Release")
endif()

# verbose output - useful for debugging, but turned off by default
set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "Verbose")

set(UTILITIES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/Utilities/" CACHE PATH "Location of Utilities sources.")
mark_as_advanced(UTILITIES_SOURCE_DIR)

#guard to prevent this file from running twice
if(NOT TARGET AssemblerInterpretator)

    #=============================================
	#============== DEPENDENCIES =================
    #=============================================
    
    set(UTILITIES_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Utilities" CACHE PATH "Location of Utilities installation.")

	# If Utilities is not installed on the system, try building from sources
    if(NOT EXISTS "${UTILITIES_INSTALL_DIR}")
    
        if(NOT EXISTS "${UTILITIES_SOURCE_DIR}")
            # Here we could also checkout the appropriate version of Utilities from Stash
            message(FATAL_ERROR "A path to Utilities installation or Utilities sources must be provided.")
        endif()

        message(STATUS "Building Utilities from sources.")

        # Choose build directory for UTILITIES.
		set(UTILITIES_BUILD_DIR "${CMAKE_BINARY_DIR}/Utilities/build")
        file(MAKE_DIRECTORY "${UTILITIES_BUILD_DIR}")

        # Install UTILITIES_BUILD_DIR in the build tree
        set(UTILITIES_INSTALL_DIR "${CMAKE_BINARY_DIR}/Utilities/install")
        
		# Configure UTILITIES_BUILD_DIR_INSTALL_DIR
		set(UTILITIES_CMAKE_OPTIONS
			"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
			"-DCMAKE_INSTALL_PREFIX=${UTILITIES_INSTALL_DIR}"
            )
            
        list(APPEND UTILITIES_CMAKE_OPTIONS -G ${CMAKE_GENERATOR})
        if(CMAKE_GENERATOR_TOOLSET)
            list(APPEND UTILITIES_CMAKE_OPTIONS -T ${CMAKE_GENERATOR_TOOLSET})
        endif()
        if(CMAKE_GENERATOR_PLATFORM)
            list(APPEND UTILITIES_CMAKE_OPTIONS -A ${CMAKE_GENERATOR_PLATFORM})
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} ${UTILITIES_CMAKE_OPTIONS} "${UTILITIES_SOURCE_DIR}"
            WORKING_DIRECTORY "${UTILITIES_BUILD_DIR}"
        )
        # Build and install UTILITIES
		execute_process(
			COMMAND ${CMAKE_COMMAND} --build "${UTILITIES_BUILD_DIR}" --config ${CMAKE_BUILD_TYPE} --target install
		)
    endif ()
endif()

# Add cmake scripts distributed with UTILITIES to cmake prefix path
set(CMAKE_PREFIX_PATH "${UTILITIES_INSTALL_DIR}/cmake")

#find_package(UTILITIES)

# define source files
set(interpreter_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(assembly_interpreter_SOURCE_FILES
    ${interpreter_SOURCE_DIR}/src/main.cpp 
    ${interpreter_SOURCE_DIR}/src/VirtualCpu.cpp
    ${interpreter_SOURCE_DIR}/src/Interpretator.cpp
    ${interpreter_SOURCE_DIR}/src/Input.cpp
    ${interpreter_SOURCE_DIR}/src/Compiler.cpp
)

set(assembly_interpreter_HEADER_FILES 
    ${interpreter_SOURCE_DIR}/include/Input.hpp
    ${interpreter_SOURCE_DIR}/include/Compiler.hpp
    ${interpreter_SOURCE_DIR}/include/Comands.hpp
    ${interpreter_SOURCE_DIR}/include/BaseCommands.hpp
    ${interpreter_SOURCE_DIR}/include/Iterpretator.hpp
    ${interpreter_SOURCE_DIR}/include/IterpretatorUtilities.hpp
    ${interpreter_SOURCE_DIR}/include/VirtualCpu.hpp
)

set(UTIL
    ${UTILITIES_SOURCE_DIR}/install/lib/libUTILITIES.a
)

add_executable(interpret ${UTIL} ${assembly_interpreter_SOURCE_FILES})

set_source_files_properties(
  ${UTIL}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

#compilation options
set_target_properties(interpret
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    CXX_VISIBILITY_PRESET hidden
    ARCHIVE_OUTPUT_DIRECTORY "${interpreter_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${interpreter_SOURCE_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${interpreter_SOURCE_DIR}/../bin"
)

target_compile_options(interpret
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-Wfloat-equal"
    PRIVATE "-pedantic"
    PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-O3>"
)

