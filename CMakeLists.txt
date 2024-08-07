cmake_minimum_required(VERSION 3.12)



#####################################################################################
#   Project NTS
#####################################################################################
project(nts)



#####################################################################################
#   CMake module
#####################################################################################
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")



#####################################################################################
#   Local variables
#####################################################################################
set(NTS_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}")
set(NTS_BINARY_DIR "${CMAKE_CURRENT_BINARY_DIR}")
set(NTS_SCRIPTS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/scripts")
set(NTS_SUBMODULES_DIR "${CMAKE_CURRENT_SOURCE_DIR}/submodules")
set(NTS_CMAKE_MODULE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
set(NTS_DEPENDENCIES_DIR "${CMAKE_CURRENT_BINARY_DIR}/dependencies")
set(NTS_GENERATED_FILES_DIR "${CMAKE_CURRENT_BINARY_DIR}/nts/generated_files/nts")
set(NTS_THIRD_PARTY_DIR "${CMAKE_CURRENT_SOURCE_DIR}/third_party")



#####################################################################################
#   Setup NCPP
#####################################################################################
include(NTS/CheckNCPP)
include(NTS/ConfigureNCPP)



#####################################################################################
#   Dependencies
#####################################################################################
include(NTS/CheckDependencies)
include(NTS/ConfigureDependencies)



#####################################################################################
#   Version
#####################################################################################
include(NCPP/Utilities/VersionHelper)
NCPP_VersionHelper_CreateVersionMacros(
    PROJECT_NAME NTS
    OUTPUT_FILE "${NTS_GENERATED_FILES_DIR}/version.hpp"
    MAJOR 0
    MINOR 0
    PATCH 0
)



#####################################################################################
#   Global variables
#####################################################################################
include(NCPP/Utilities/ApplyGlobal)
NCPP_ApplyGlobal(
    NTS_ROOT_DIR
    NTS_BINARY_DIR
    NTS_SCRIPTS_DIR
    NTS_SUBMODULES_DIR
    NTS_CMAKE_MODULE_DIR 
    NTS_DEPENDENCIES_DIR 
    NTS_GENERATED_FILES_DIR
)



#####################################################################################
#   Documentations
#####################################################################################
option(NTS_ENABLE_DOCUMENTATIONS "If enabled, creating a target being capable of generating documentations" OFF)
if(NTS_ENABLE_DOCUMENTATIONS)
    option(NTS_INTERNAL_DOCS "Generate docs for anything" OFF)

    include(NCPP/Utilities/DocsHelper)

    NCPP_DocsHelper_CreateTarget(
        NAME NTS
        BRIEF "Ncoder's Learning Aimed C++ Library."

        TARGET_NAME nts.documentations
        TARGET_FOLDER NCoder/nts
        TARGET_LABEL documentations

        INPUT_DIRS 
            "${CMAKE_CURRENT_SOURCE_DIR}/nts/source"
            "${CMAKE_CURRENT_SOURCE_DIR}/documentations"
            "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
        STYLE_DIRS
            "${CMAKE_CURRENT_SOURCE_DIR}/documentations/styles"
        OUTPUT_DIR "${CMAKE_CURRENT_BINARY_DIR}/documentations"
        CWD "${CMAKE_CURRENT_SOURCE_DIR}"
        INTERNAL_DOCS ${NTS_INTERNAL_DOCS}

        PREDEFINED 
            ${NCPP_DOCUMENTATIONS_PREDEFINED}

        MAIN_PAGE "${CMAKE_CURRENT_SOURCE_DIR}/README.md"
    )
endif()



#####################################################################################
#   Add main directory
#####################################################################################
add_subdirectory(nts)



include(CPack)