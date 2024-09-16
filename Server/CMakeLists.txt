project ("DDM_SecondServ")
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_subdirectory(Simulation)
add_subdirectory(WS)

add_subdirectory(Main)
