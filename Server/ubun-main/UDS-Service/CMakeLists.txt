project(UDS-Service)

##set(CMAKE_PREFIX_PATH "C:\\Users\\mobenbra\\Downloads\\poco-1.9.0-all2\\poco-1.9.0-all\\out\\build\\x86-Debug\\Poco")
set(CMAKE_PREFIX_PATH "C:\\Users\\monta\\OneDrive\\Desktop\\POCO\\poco\\out\\build\\x64-Debug\\Poco")


find_package(Poco COMPONENTS Foundation XML REQUIRED)

set(INC
    inc/DID.h
    inc/SBF.h
    inc/Service.h
)

set(SRC
    src/Service.cpp
)

add_library(UDS_service ${INC} ${SRC})
add_library(Link_UDS_service ALIAS UDS_service)

target_include_directories(UDS_service PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/DDM-Server/inc
)


if (Poco_FOUND)
target_include_directories(UDS_service PUBLIC ${Poco_INCLUDE_DIR})
target_link_libraries(UDS_service PUBLIC ${Poco_LIBRARIES})
else ()
message(FATAL_ERROR "libPoco not found")
endif()

install(TARGETS UDS_service LIBRARY DESTINATION lib)