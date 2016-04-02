if (NOT DEFINED Boost_USE_STATIC_LIBS)
    set(Boost_USE_STATIC_LIBS    ON)
endif()

if (NOT Boost_USE_STATIC_LIBS)
    ADD_DEFINITIONS(-DBOOST_TEST_DYN_LINK)
endif()

set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions(-DBOOST_NO_AUTO_PTR)
find_package(Boost REQUIRED COMPONENTS thread system unit_test_framework filesystem locale)

file(GLOB SRC_GLOB "*.h" "*.cpp" "*.c")
list(APPEND SRC ${SRC_GLOB})
include_directories("../src/")

include("../src/CMakeCompilerSettings.cmake")
add_executable(unit_test ${SRC})
target_link_libraries(unit_test jsoncpp_lib_static)
target_link_libraries(unit_test server commands events traits util editor com)
target_link_libraries(unit_test ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_LOCALE_LIBRARY} ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

IF (WIN32)
    target_link_libraries(unit_test Ws2_32 Mswsock)
ELSE()
    target_link_libraries(unit_test icuuc icui18n pthread rt)
ENDIF()

enable_testing()

add_test(unit_test unit_test)
