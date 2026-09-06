# Helper function to embed glsl files.
#
# embed_glsl(shaders/quad.vert.glsl "${CMAKE_CURRENT_BINARY_DIR}/quad.vert.glsl.h" QUAD_VERT_SRC)
function(embed_glsl INPUT_FILE OUTPUT_HEADER VARIABLE_NAME)
    set(INPUT_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}")
    if(NOT EXISTS "${INPUT_FILE_PATH}")
        message(FATAL_ERROR "Shader not found: ${INPUT_FILE_PATH}")
    endif()

    file(READ "${INPUT_FILE_PATH}" SHADER_SOURCE)

    # check for name clashes
    set(DELIM "GLSL_${VARIABLE_NAME}")
    string(FIND "${SHADER_SOURCE}" "${DELIM}" DELIM_FOUND)
    if(NOT DELIM_FOUND EQUAL -1)
        message(FATAL_ERROR
            "Shader ${INPUT_FILE_PATH} contains Raw-String-Delimiter "
            "'${DELIM}' - VARIABLE_NAME should be changed in embed_glsl() call.")
    endif()

    file(WRITE "${OUTPUT_HEADER}"
        "#pragma once\n\n"
        "// Generated from ${INPUT_FILE} - do not edit\n"
        "inline constexpr const char* ${VARIABLE_NAME} = R\"${DELIM}(\n"
        "${SHADER_SOURCE})${DELIM}\";\n"
    )

    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${INPUT_FILE_PATH}")
endfunction()