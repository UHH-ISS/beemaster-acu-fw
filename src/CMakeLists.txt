# CMakeLists.txt for libacu

# Source files
set(SOURCE_FILES
        acu.cpp)

# Library definition + linking
add_library(acu ${SOURCE_FILES})
target_link_libraries(acu ${LINK_LIBS})