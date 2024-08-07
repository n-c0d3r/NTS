
# nts.samples.hello_nts target source files
file(GLOB source_files
    "source/*.cpp"
    "source/*.hpp"
)
source_group("source" FILES ${source_files})



# Create nts.samples.hello_nts target
add_executable(nts.samples.hello_nts
    ${source_files}
)



# nts.samples.hello_nts target settings
set_target_properties(nts.samples.hello_nts
    PROPERTIES 
        FOLDER "NCoder/nts/samples"
        PROJECT_LABEL "hello_nts"
)
target_link_libraries(nts.samples.hello_nts
    PUBLIC 
        nts
)
target_include_directories(nts.samples.hello_nts
    PUBLIC 
        nts
        ${CMAKE_CURRENT_SOURCE_DIR}/source
)



# nts.samples.hello_nts target dependencies
add_dependencies(nts.samples.hello_nts 
    nts
)