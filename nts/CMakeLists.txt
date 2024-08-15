
#####################################################################################
#   Includes
#####################################################################################
include(NCPP/ConfigureCompiler)



#####################################################################################
#   Options
#####################################################################################
set(NTS_FRAME_HEAP_PARAM_COUNT 64 CACHE STRING "the param count of frame heap")
set(NTS_DEFAULT_TASK_BATCH_SIZE 32 CACHE STRING "the default number of task instances can be batched together into a work execution unit")
set(NTS_DEFAULT_TASK_CONTEXT_RING_BUFFER_CAPACITY 8192 CACHE STRING "the default capacity of task context ring buffer")



#####################################################################################
#   Generated files
#####################################################################################
# nts target configure file
configure_file(source/nts/.config "${NTS_GENERATED_FILES_DIR}/config.hpp")



#####################################################################################
#   nts target's files
#####################################################################################
set(NTS_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/source" PARENT_SCOPE)

# nts target source files
file(GLOB_RECURSE source_files
    "source/*.cpp"
    "source/*.hpp"
)

# Exclude the msvc platform files from the list if not using msvc
if(NOT MSVC)
  list(FILTER source_files EXCLUDE REGEX "source/nts/msvc/*.*")
endif()

# create source group
source_group(TREE ${CMAKE_CURRENT_SOURCE_DIR}/source PREFIX source FILES ${source_files})

file(GLOB_RECURSE generated_files
    "${CMAKE_CURRENT_BINARY_DIR}/generated_files/*.cpp"
    "${CMAKE_CURRENT_BINARY_DIR}/generated_files/*.hpp"
)
source_group(TREE "${CMAKE_CURRENT_BINARY_DIR}/generated_files" PREFIX generated_files FILES ${generated_files})



#####################################################################################
#   Create nts target
#####################################################################################
add_library(nts STATIC
    ${source_files}
    ${generated_files}
)



#####################################################################################
#   nts target settings
#####################################################################################
set_target_properties(nts
    PROPERTIES 
        FOLDER "NCoder/nts"
)

target_precompile_headers(nts
    PRIVATE
        source/nts/prerequisites.hpp
)

target_include_directories(nts
    PUBLIC 
        ${CMAKE_CURRENT_SOURCE_DIR}/source
        ${CMAKE_CURRENT_BINARY_DIR}/generated_files
)

target_link_libraries(nts
    PUBLIC
        ncpp
)

# MSVC compile options
if(MSVC)
  target_compile_options(nts PUBLIC /GT)
endif()



#####################################################################################
#   Dependencies
#####################################################################################
add_dependencies(nts ncpp)



#####################################################################################
#   Add dependencies
#####################################################################################
add_subdirectory(samples)



