project(all_test)
message(BUILD_TESTING)
message(STATUS "googletest> Creating google-test target...")


##set(CMAKE_PREFIX_PATH "C:\\Users\\mobenbra\\Downloads\\poco-1.9.0-all2\\poco-1.9.0-all\\out\\build\\x86-Debug\\Poco")
set(CMAKE_PREFIX_PATH "C:\\Users\\monta\\OneDrive\\Desktop\\POCO\\poco\\out\\build\\x86-Debug\\Poco")

find_package(Poco COMPONENTS Foundation Net JSON XML Util REQUIRED)
enable_testing()
add_executable(my_test 
   Testall.cpp)
target_include_directories(my_test PUBLIC
                        ${CMAKE_SOURCE_DIR}/DDM-Server/inc
                        ${CMAKE_SOURCE_DIR}/MainApp/inc
                        ${CMAKE_SOURCE_DIR}/UDS-Service/inc
                        ${CMAKE_SOURCE_DIR}/Communication/inc
                        ${Poco_INCLUDE_DIR})
target_link_libraries(my_test PUBLIC 
    gtest
    GTest::gtest_main
    ${Poco_LIBRARIES}
)
target_link_libraries(my_test PUBLIC Link_DDM_Server Link_pcan_comm Link_UDS_service)
include(googletest)
add_test(
    NAME my_test
    COMMAND my_test
)
message(STATUS "Poco INCLUDE Dirs : ${Poco_INCLUDE_DIR}")
message(STATUS "Poco Libraries : ${Poco_LIBRARIES}")


