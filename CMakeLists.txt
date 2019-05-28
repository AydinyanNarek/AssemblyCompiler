
cmake_minimum_required(VERSION 3.3.2)

project(AssemblerInterpretator)


# define source files
set(interpreter_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
set(assembly_interpreter_SOURCE_FILES
    ${interpreter_SOURCE_DIR}/main.cpp 
    ${interpreter_SOURCE_DIR}/VirtualCpu.cpp
    ${interpreter_SOURCE_DIR}/Interpretator.cpp
    ${interpreter_SOURCE_DIR}/Input.cpp
    ${interpreter_SOURCE_DIR}/Compiler.cpp
)

add_executable(interpret ${assembly_interpreter_SOURCE_FILES})


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