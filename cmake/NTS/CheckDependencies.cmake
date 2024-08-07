
message(STATUS "<NTS::CheckDependencies> Start checking dependencies")



#####################################################################################
#   Includes
#####################################################################################
include(NCPP/Utilities/GitHelper)



#####################################################################################
#   Setup directory for dependencies to be downloaded into
#####################################################################################
if(NOT EXISTS ${NTS_DEPENDENCIES_DIR})
    file(MAKE_DIRECTORY ${NTS_DEPENDENCIES_DIR})
endif()



message(STATUS "<NTS::CheckDependencies> Check dependencies done")