project(DDM_Server)
##set(CMAKE_PREFIX_PATH "C:\\Users\\mobenbra\\Downloads\\poco-1.9.0-all2\\poco-1.9.0-all\\out\\build\\x86-Debug\\Poco")
set(CMAKE_PREFIX_PATH "C:\\Users\\monta\\OneDrive\\Desktop\\POCO\\poco\\out\\build\\x86-Debug\\Poco")


set(INC
    inc/websocket_server.h
)

set(SRC
    src/websocket_server.cpp
)

add_library(DDM_Server ${INC} ${SRC} "src/websocket_server_req.cpp" "inc/websocket_server_req.h")
add_library(Link_DDM_Server ALIAS DDM_Server)

target_include_directories(DDM_Server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/UDS-Service/inc
    ${CMAKE_SOURCE_DIR}/Communication/inc  
)

##find_path(PCAN_INCLUDE_DIR NAMES PCANBasic.h PATHS "C:\\Users\\mobenbra\\Downloads\\PCAN-ISO-TP\\Include")
find_path(PCAN_INCLUDE_DIR NAMES PCAN-ISO-TP_2016 "C:\\Users\\monta\\Downloads\\PCAN-ISO-TP\\Include")
if(PCAN_INCLUDE_DIR)
  include_directories(${PCAN_INCLUDE_DIR})
  message(STATUS "Found PCANBasic.h in: ${PCAN_INCLUDE_DIR}")

  # Assuming the library is named "PCAN" 

  ##find_library(PCAN_LIBRARY NAMES PCANBasic HINTS "C:\\Users\\mobenbra\\Downloads\\PCAN-ISO-TP\\x86\\VC_LIB")
  find_library(PCAN_LIBRARY NAMES PCAN-ISO-TP_2016 HINTS "C:\\Users\\monta\\Downloads\\PCAN-ISO-TP\\x86\\VC_LIB")

  if(PCAN_LIBRARY)
    message(STATUS "Found PCAN library: ${PCAN_LIBRARY}")
    target_link_libraries(DDM_Server PRIVATE ${PCAN_LIBRARY}) 
  else()
    message(WARNING "PCAN library not found")
  endif()
else()
  message(WARNING "PCANBasic.h not found")
endif()

find_package(Poco COMPONENTS Foundation Net Util JSON XML REQUIRED)
if (Poco_FOUND)
target_include_directories(DDM_Server PUBLIC ${Poco_INCLUDE_DIR})
target_link_libraries(DDM_Server PUBLIC ${Poco_LIBRARIES})
else ()
message(FATAL_ERROR "libPoco not found")
endif()

install(TARGETS DDM_Server LIBRARY DESTINATION lib)