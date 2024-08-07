
message(STATUS "<NTS::ConfigureNCPP> Start configuring NCPP")



#####################################################################################
#   Add NCPP subdirectory
#####################################################################################
if(NOT TARGET ncpp)
    add_subdirectory("${NTS_SUBMODULES_DIR}/NCPP" "${NTS_BINARY_DIR}/submodules/NCPP")
endif()



#####################################################################################
#   NCPP cmake module path
#####################################################################################
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${NCPP_CMAKE_MODULE_DIR}")



message(STATUS "<NTS::ConfigureNCPP> Configure NCPP done")