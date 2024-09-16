project(MainApp)
cmake_minimum_required(VERSION 3.0)  
##set(CMAKE_PREFIX_PATH "C:\\Users\\mobenbra\\Desktop\\Poco1.9.0\\out\\build\\x86-Debug\\Poco")
set(CMAKE_PREFIX_PATH "C:\\Users\\monta\\OneDrive\\Desktop\\POCO\\poco\\out\\build\\x86-Debug\\Poco")

find_package(Poco COMPONENTS Foundation Net Util JSON REQUIRED)
set(EXEC 
    src/main_app.cpp
    )
set(EXECH
    inc/main_app.h
    )

add_executable(exec ${EXEC} ${EXECH})

target_include_directories(exec PUBLIC
                                ${CMAKE_SOURCE_DIR}/DDM-Server/inc/
                                ${CMAKE_SOURCE_DIR}/UDS-Service/inc/
                                ${CMAKE_SOURCE_DIR}/Communication/inc/
                                ${CMAKE_SOURCE_DIR}/MainApp/inc/
                                ${CMAKE_SOURCE_DIR}/MainApp/src/
)

target_link_libraries(exec PUBLIC ${Poco_LIBRARIES})

target_link_libraries(exec PUBLIC Link_DDM_Server Link_pcan_comm Link_UDS_service)

install(TARGETS exec DESTINATION bin)