file(GLOB SRC "*.h" "*.cpp" "*.c")
include("../CMakeCompilerSettings.cmake")
add_library(server ${SRC})