project ("DDM_PFE")
cmake_minimum_required (VERSION 3.8)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} /MT")#Change

#[[set(CompilerFlags
        CMAKE_CXX_FLAGS
        CMAKE_CXX_FLAGS_DEBUG
        CMAKE_CXX_FLAGS_RELEASE
        CMAKE_C_FLAGS
        CMAKE_C_FLAGS_DEBUG
        CMAKE_C_FLAGS_RELEASE
        )
foreach(CompilerFlag ${CompilerFlags})
  string(REPLACE "/MD" "/MT" ${CompilerFlag} "${${CompilerFlag}}")
endforeach()]]

#DEVELOPPEMENT
add_subdirectory(Communication)
add_subdirectory(UDS-Service)
add_subdirectory(DDM-Server)
add_subdirectory(MainApp)

#TEST
include(CTest)
#add_subdirectory(googletest)
#add_subdirectory(Test_Unitaire) #Change