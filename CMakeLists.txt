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

set(COMPILER_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src/" CACHE PATH "Location of Utilities sources.")


#guard to prevent this file from running twice
if(NOT TARGET AssemblerInterpretator)

    #=============================================
	#============== DEPENDENCIES =================
    #=============================================
    
    set(UTILITIES_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/Utilities" CACHE PATH "Location of Utilities installation.")
    set(COMPILER_INSTALL_DIR "${CMAKE_CURRENT_SOURCE_DIR}/build/Compiler" CACHE PATH "Location of Compiler installation.")

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

    
    if(NOT EXISTS "${COMPILER_INSTALL_DIR}")
    
        if(NOT EXISTS "${COMPILER_SOURCE_DIR}")
            # Here we could also checkout the appropriate version of COMPILER from Stash
            message(FATAL_ERROR "A path to COMPILER installation or COMPILER sources must be provided.")
        endif()

        message(STATUS "Building COMPILER from sources.")

        # Choose build directory for COMPILER.
		set(COMPILER_BUILD_DIR "${CMAKE_BINARY_DIR}/Compiler/build")
        file(MAKE_DIRECTORY "${COMPILER_BUILD_DIR}")

        # Install COMPILER_BUILD_DIR in the build tree
        set(COMPILER_INSTALL_DIR "${CMAKE_BINARY_DIR}/Compiler/install")
        
		# Configure COMPILER_BUILD_DIR
		set(COMPILER_CMAKE_OPTIONS
			"-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
			"-DCMAKE_INSTALL_PREFIX=${COMPILER_INSTALL_DIR}"
            )
            
        list(APPEND COMPILER_CMAKE_OPTIONS -G ${CMAKE_GENERATOR})
        if(CMAKE_GENERATOR_TOOLSET)
            list(APPEND COMPILER_CMAKE_OPTIONS -T ${CMAKE_GENERATOR_TOOLSET})
        endif()
        if(CMAKE_GENERATOR_PLATFORM)
            list(APPEND COMPILER_CMAKE_OPTIONS -A ${CMAKE_GENERATOR_PLATFORM})
        endif()

        execute_process(
            COMMAND ${CMAKE_COMMAND} ${COMPILER_CMAKE_OPTIONS} "${COMPILER_SOURCE_DIR}"
            WORKING_DIRECTORY "${COMPILER_BUILD_DIR}"
        )
        # Build and install COMPILER
		execute_process(
			COMMAND ${CMAKE_COMMAND} --build "${COMPILER_BUILD_DIR}" --config ${CMAKE_BUILD_TYPE} --target install
		)
    endif ()
endif()

# Add cmake scripts distributed with COMPILER to cmake prefix path
set(CMAKE_PREFIX_PATH "${UTILITIES_INSTALL_DIR}/cmake")

# Add cmake scripts distributed with COMPILER to cmake prefix path
set(CMAKE_PREFIX_PATH "${COMPILER_INSTALL_DIR}/cmake")

# define source files
set(interpreter_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(assembly_interpreter_SOURCE_FILES
    ${interpreter_SOURCE_DIR}/src/interpretator/main.cpp 
    ${interpreter_SOURCE_DIR}/src/interpretator/Interpretator.cpp
)

set(assembly_interpreter_HEADER_FILES 
    ${interpreter_SOURCE_DIR}/include/interpretator/Iterpretator.hpp
)

set(UTIL
    ${CMAKE_CURRENT_SOURCE_DIR}/build/Utilities/install/lib/libUTILITIES.a
)

set(COMP
    ${CMAKE_CURRENT_SOURCE_DIR}/build/Compiler/install/lib/libCOMPILER.a
)

add_executable(g+++ ${COMP} ${UTIL} ${assembly_interpreter_SOURCE_FILES})

set_source_files_properties(
  ${COMP}
  ${UTIL}
  PROPERTIES
  EXTERNAL_OBJECT true
  GENERATED true
)

#compilation options
set_target_properties(g+++
    PROPERTIES
    CXX_STANDARD 17
    CXX_STANDARD_REQUIRED YES
    CXX_EXTENSIONS NO
    CXX_VISIBILITY_PRESET hidden
    ARCHIVE_OUTPUT_DIRECTORY "${interpreter_SOURCE_DIR}/lib"
    LIBRARY_OUTPUT_DIRECTORY "${interpreter_SOURCE_DIR}/lib"
    RUNTIME_OUTPUT_DIRECTORY "${interpreter_SOURCE_DIR}/../bin"
)

target_compile_options(g+++
    PRIVATE "-Wall"
    PRIVATE "-Wextra"
    PRIVATE "-Wfloat-equal"
    PRIVATE "-pedantic"
    PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-O3>"
)

