project(Main)
cmake_minimum_required(VERSION 3.0)  
set(CMAKE_PREFIX_PATH "C:\\Users\\mobenbra\\Downloads\\poco-1.9.0-all2\\poco-1.9.0-all\\out\\build\\x86-Debug\\Poco")
find_package(Poco COMPONENTS Foundation Net Util JSON REQUIRED)
set(EXEC 
    src/appMain.cpp
    )
set(EXECH
    inc/appMain.h
    )

add_executable(exec ${EXEC} ${EXECH})

target_include_directories(exec PUBLIC
                                ${CMAKE_SOURCE_DIR}/WS/inc/
                                ${CMAKE_SOURCE_DIR}/Main/inc/
                                ${CMAKE_SOURCE_DIR}/Simulation/inc/
)

target_link_libraries(exec PUBLIC ${Poco_LIBRARIES})

target_link_libraries(exec PUBLIC Link_WS_Server Link_Peak_simul)

install(TARGETS exec DESTINATION bin)