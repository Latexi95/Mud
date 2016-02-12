file(GLOB SRC_A "*.h" "*.cpp" "*.c" "events/*.h" "events/*.cpp" "textgen/*.h" "textgen/*.cpp")
file(GLOB SRC_B "commands/*.h" "commands/*.cpp" "traits/*.h" "traits/*.cpp")
set(SRC ${SRC_A} ${SRC_B})

set(Boost_USE_STATIC_LIBS    ON)
set(Boost_USE_MULTITHREADED  ON)
set(Boost_USE_STATIC_RUNTIME OFF)
add_definitions(-DBOOST_NO_AUTO_PTR)
find_package(Boost REQUIRED COMPONENTS thread system filesystem locale)
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${JSONCPP_INCLUDE_DIR})
include_directories(".")

add_executable(mud ${SRC})
target_link_libraries(mud jsoncpp_lib_static)
target_link_libraries(mud ${Boost_FILESYSTEM_LIBRARY} ${Boost_THREAD_LIBRARY} ${Boost_SYSTEM_LIBRARY} ${Boost_LOCALE_LIBRARY})
IF (WIN32)
    target_link_libraries(mud Ws2_32 Mswsock)
ENDIF()


target_compile_features(mud PRIVATE cxx_range_for)
