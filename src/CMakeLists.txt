################################################################################
######
###### COMPILER CMake Configuration File
######
###### Author: Narek Aydinyan - aydinyan.narek@gmail.com
#################################################################################


cmake_minimum_required(VERSION 3.3.0)

#set minimum MacOS target version if not defined as environment variable
if(APPLE AND NOT DEFINED ENV{MACOSX_DEPLOYMENT_TARGET})
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.8" CACHE STRING "Minimum OS X deployment version")
endif()

project(COMPILER VERSION 1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "None" "Debug" "Release")
endif()

# verbose output - useful for debugging, but turned off by default
set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "Verbose")

set(UTILITIES_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/Utilities/" CACHE PATH "Location of Utilities sources.")
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

	#=============================================
	#=========== MAIN PACLET LIBRARY =============
	#=============================================

	set(COMPILER_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include/compiliator)
	set(COMPILER_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src/compiliator)

	# define source files
	set(COMPILER_SOURCE_FILES
        ${COMPILER_SOURCE_DIR}/Compiler.cpp
        ${COMPILER_SOURCE_DIR}/Input.cpp
		${COMPILER_SOURCE_DIR}/VirtualCpu.cpp
    )

    # define header files
    set(COMPILER_HEADER_FILES
        ${COMPILER_INCLUDE_DIR}/BaseCommands.hpp
        ${COMPILER_INCLUDE_DIR}/Commands.hpp
        ${COMPILER_INCLUDE_DIR}/Compiler.hpp
        ${COMPILER_INCLUDE_DIR}/CompilerUtilities.hpp
        ${COMPILER_INCLUDE_DIR}/Input.hpp
        ${COMPILER_INCLUDE_DIR}/VirtualCpu.hpp
    )

	#add the main library
	add_library(COMPILER ${COMPILER_SOURCE_FILES} ${COMPILER_HEADER_FILES})

	#compilation options
	set_target_properties(COMPILER PROPERTIES
	    CXX_STANDARD 17
	    CXX_STANDARD_REQUIRED YES
	    CXX_EXTENSIONS NO
		CXX_VISIBILITY_PRESET hidden
	    POSITION_INDEPENDENT_CODE YES
	)

	# Sets rpath for a target.
	# - $ORIGIN on Linux
	# - @loader_path on Mac
	# On Windows rpath does not make sense.
	function(set_rpath TARGET_NAME NEW_RPATH)
		if(NOT NEW_RPATH)
			if(APPLE)
				#set the linker options to set rpath as @loader_path
				set(NEW_RPATH "@loader_path/")
			elseif(UNIX)
				#set the install_rpath to be $ORIGIN so that it automatically finds the dependencies in the current folder
				set(NEW_RPATH $ORIGIN)
			endif()
		endif ()
		set_target_properties(${TARGET_NAME} PROPERTIES INSTALL_RPATH ${NEW_RPATH})
	endfunction()

	if(MSVC)
		# Remove default warning level. It will be set later.
		string(REGEX REPLACE "/W[0-4]" "" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")

		# Force static runtime on Windows. See https://gitlab.kitware.com/cmake/community/wikis/FAQ#dynamic-replace
		foreach(flag_var CMAKE_CXX_FLAGS CMAKE_CXX_FLAGS_DEBUG CMAKE_CXX_FLAGS_RELEASE CMAKE_CXX_FLAGS_MINSIZEREL CMAKE_CXX_FLAGS_RELWITHDEBINFO)
			if(${flag_var} MATCHES "/MD")
				string(REGEX REPLACE "/MD" "/MT" ${flag_var} "${${flag_var}}")
			endif()
		endforeach()

		target_compile_options(COMPILER
			PRIVATE "/W4"
			PRIVATE "/EHsc"
			PRIVATE "$<$<CONFIG:Debug>:/Zi>"
			PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/O2>"
		)
	else()
		target_compile_options(COMPILER
			PRIVATE "-Wall"
			PRIVATE "-Wextra"
			PRIVATE "-pedantic"
			PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-O3>"
		)
    endif()
    
    target_link_libraries(COMPILER PRIVATE
        UTILITIES
    )

	# tell all targets importing COMPILER that public part of COMPILER uses C++17 features
	target_compile_features(COMPILER
		PUBLIC
			cxx_std_17
	)

	#add the include directory for the project
	target_include_directories(COMPILER
		PUBLIC
			$<INSTALL_INTERFACE:include>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/src
	)

	if (NOT WIN32)
		set_rpath(COMPILER "")
	endif ()

	##############################################
	# Installation instructions

	# By default install to "install" directory
	if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "Library install prefix" FORCE)
	endif ()

	install(TARGETS COMPILER
		EXPORT COMPILER-targets
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)



