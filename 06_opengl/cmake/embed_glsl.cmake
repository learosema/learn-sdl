# Embeds a GLSL text file into a C++ header as
#   inline constexpr const char* <VARIABLE_NAME> = R"(...)";
#
# Anders als embed_resource() (fuer Binaerdaten, hex-encoded) ist das hier
# reiner Text, der 1:1 in ein raw string literal passt - kein Hex-Dump noetig.
#
# Beispiel:
#   embed_shader(shaders/quad.vert.glsl "${CMAKE_CURRENT_BINARY_DIR}/quad.vert.glsl.h" QUAD_VERT_SRC)
function(embed_glsl INPUT_FILE OUTPUT_HEADER VARIABLE_NAME)
    set(INPUT_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}")
    if(NOT EXISTS "${INPUT_FILE_PATH}")
        message(FATAL_ERROR "Shader not found: ${INPUT_FILE_PATH}")
    endif()

    file(READ "${INPUT_FILE_PATH}" SHADER_SOURCE)

    # Delimiter fuers raw string literal aus dem Variablennamen ableiten,
    # damit er nicht zufaellig als Text im Shader selbst vorkommt
    # (z.B. als Kommentar oder in einem Uniform-Namen).
    set(DELIM "GLSL_${VARIABLE_NAME}")
    string(FIND "${SHADER_SOURCE}" "${DELIM}" DELIM_FOUND)
    if(NOT DELIM_FOUND EQUAL -1)
        message(FATAL_ERROR
            "Shader ${INPUT_FILE_PATH} enthaelt den Raw-String-Delimiter "
            "'${DELIM}' - VARIABLE_NAME fuer embed_shader() aendern.")
    endif()

    file(WRITE "${OUTPUT_HEADER}"
        "#pragma once\n\n"
        "// Automatisch generiert aus ${INPUT_FILE} - nicht von Hand editieren\n"
        "inline constexpr const char* ${VARIABLE_NAME} = R\"${DELIM}(\n"
        "${SHADER_SOURCE})${DELIM}\";\n"
    )

    # Sorgt dafuer, dass CMake bei Aenderung der .glsl-Datei automatisch neu
    # konfiguriert (und damit den Header neu generiert), statt dass man von
    # Hand "touch CMakeLists.txt" machen muesste.
    set_property(DIRECTORY APPEND PROPERTY CMAKE_CONFIGURE_DEPENDS "${INPUT_FILE_PATH}")
endfunction()