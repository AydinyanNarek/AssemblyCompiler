################################################################################
######
###### Utilities CMake Configuration File
######
###### Author: Narek Aydinyan - aydinyan.narek@gmail.com
#################################################################################


cmake_minimum_required(VERSION 3.3.0)

#set minimum MacOS target version if not defined as environment variable
if(APPLE AND NOT DEFINED ENV{MACOSX_DEPLOYMENT_TARGET})
	set(CMAKE_OSX_DEPLOYMENT_TARGET "10.8" CACHE STRING "Minimum OS X deployment version")
endif()

project(UTILITIES VERSION 1.0.0 LANGUAGES CXX)

list(APPEND CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

# set default build type to Release
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release CACHE STRING "Choose the type of build." FORCE)
	set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "None" "Debug" "Release")
endif()

# verbose output - useful for debugging, but turned off by default
set(CMAKE_VERBOSE_MAKEFILE OFF CACHE BOOL "Verbose")

#guard to prevent this file from running twice
if(NOT TARGET UTILITIES)

	#=============================================
	#=========== MAIN PACLET LIBRARY =============
	#=============================================

	set(UTILITIES_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
	set(UTILITIES_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

	# define source files
	set(UTILITIES_SOURCE_FILES
		${UTILITIES_SOURCE_DIR}/ErrorRegister.cpp
	)

	#add the main library
	add_library(UTILITIES ${UTILITIES_SOURCE_FILES})

	#compilation options
	set_target_properties(UTILITIES PROPERTIES
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

		target_compile_options(UTILITIES
			PRIVATE "/W4"
			PRIVATE "/EHsc"
			PRIVATE "$<$<CONFIG:Debug>:/Zi>"
			PRIVATE "$<$<NOT:$<CONFIG:Debug>>:/O2>"
		)
	else()
		target_compile_options(UTILITIES
			PRIVATE "-Wall"
			PRIVATE "-Wextra"
			PRIVATE "-pedantic"
			PRIVATE "$<$<NOT:$<CONFIG:Debug>>:-O3>"
		)
	endif()

	# tell all targets importing UTILITIES that public part of UTILITIES uses C++17 features
	target_compile_features(UTILITIES
		PUBLIC
			cxx_std_17
	)

	#add the include directory for the project
	target_include_directories(UTILITIES
		PUBLIC
			$<INSTALL_INTERFACE:include>
			$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
		PRIVATE
			${CMAKE_CURRENT_SOURCE_DIR}/src
	)

	if (NOT WIN32)
		set_rpath(UTILITIES "")
	endif ()

	##############################################
	# Installation instructions

	# By default install to "install" directory
	if (CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
		set(CMAKE_INSTALL_PREFIX "${CMAKE_CURRENT_SOURCE_DIR}/install" CACHE PATH "Library install prefix" FORCE)
	endif ()

	include(GNUInstallDirs)
	set(INSTALL_CONFIGDIR cmake/Utilities)

	install(TARGETS UTILITIES
		EXPORT UTILITIES-targets
		LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
		ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
	)

endif() #end IF NOT TARGET UTILITIES

