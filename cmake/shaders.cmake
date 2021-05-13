
function(add_shader TARGET SHADER)
    find_program(GLSLC glslc)
    find_program(RCC rcc)

    file(RELATIVE_PATH shader_relative ${CMAKE_CURRENT_SOURCE_DIR}/src/ ${SHADER})
    set(shader_output ${CMAKE_BINARY_DIR}/shaders/${shader_relative}.spv)

    get_filename_component(shader_directory ${shader_output} DIRECTORY)
    file(MAKE_DIRECTORY ${shader_directory})

    add_custom_command(
            OUTPUT ${shader_output}
            COMMAND ${GLSLC} -o ${shader_output} ${SHADER}
            DEPENDS ${SHADER}
            IMPLICIT_DEPENDS CXX ${shader_directory}
            VERBATIM)

    set(qrc_file "${CMAKE_BINARY_DIR}/shaders/${shader_relative}.qrc")

    add_custom_command(
            OUTPUT ${shader_output}
            COMMAND ${GLSLC} -o ${shader_output} ${SHADER}
            DEPENDS ${SHADER}
            IMPLICIT_DEPENDS CXX ${shader_directory}
            VERBATIM)

    file(WRITE ${qrc_file}
            "<!DOCTYPE RCC> <RCC version=\"1.0\"> <qresource>
                <file alias=\"${shader_relative}\">${shader_output}</file>
            </qresource> </RCC>")


    # Make sure our build depends on this output.
    set_source_files_properties(${shader_output} PROPERTIES GENERATED TRUE)
    target_sources(${TARGET} PRIVATE ${qrc_file})
endfunction(add_shader)


function(add_all_shaders TARGET)

    file(GLOB_RECURSE SHADER_FILES
            "${PROJECT_SOURCE_DIR}/src/*.vert"
            "${PROJECT_SOURCE_DIR}/src/*.frag"
            )

    foreach(shader IN LISTS SHADER_FILES)
        add_shader(${TARGET} "${shader}")
    endforeach()

endfunction(add_all_shaders)
