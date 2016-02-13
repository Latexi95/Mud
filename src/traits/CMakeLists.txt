file(GLOB SRC "*.h" "*.cpp" "*.c")
include("../CMakeCompilerSettings.cmake")
add_library(traits ${SRC})