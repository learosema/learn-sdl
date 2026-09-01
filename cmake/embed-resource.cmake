# A pure, dependency-free binary-to-header converter function
function(embed_resource INPUT_FILE OUTPUT_HEADER ARRAY_NAME)
    set(INPUT_FILE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/${INPUT_FILE}")
    if(NOT EXISTS "${INPUT_FILE_PATH}")
        message(FATAL_ERROR "Asset not found: ${INPUT_FILE}")
    endif()

    # Read raw file contents as a long hex string string
    file(READ "${INPUT_FILE_PATH}" HEX_CONTENT HEX)
    
    # Split the hex string into two-character bytes
    string(REGEX MATCHALL ".." HEX_BYTES "${HEX_CONTENT}")
    
    # Format them into standard C++ comma-separated tokens
    string(REPLACE ";" ", 0x" FORMATTED_BYTES "${HEX_BYTES}")
    
    # Write the compiled byte array file to the build folder
    file(WRITE "${OUTPUT_HEADER}"
        "#pragma once\n\n"
        "inline const unsigned char ${ARRAY_NAME}[] = { 0x${FORMATTED_BYTES} };\n"
        "inline const unsigned int ${ARRAY_NAME}_len = sizeof(${ARRAY_NAME});\n"
    )
endfunction()
