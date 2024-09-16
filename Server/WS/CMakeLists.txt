project(WS)
set(CMAKE_PREFIX_PATH "C:\\Users\\mobenbra\\Downloads\\poco-1.9.0-all2\\poco-1.9.0-all\\out\\build\\x86-Debug\\Poco")


set(INC
    inc/ws.h
    inc/ws_req.h
)

set(SRC
    src/ws.cpp
    src/ws_req.cpp
)

add_library(WS_Server ${INC} ${SRC} )
add_library(Link_WS_Server ALIAS WS_Server)

target_include_directories(WS_Server PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc
    ${CMAKE_SOURCE_DIR}/Simulation/inc
)

find_path(PCAN_INCLUDE_DIR NAMES PCANBasic.h PATHS "C:\\Users\\mobenbra\\Downloads\\PCAN-Basic\\Include")

if(PCAN_INCLUDE_DIR)
  include_directories(${PCAN_INCLUDE_DIR})
  message(STATUS "Found PCANBasic.h in: ${PCAN_INCLUDE_DIR}")

  # Assuming the library is named "PCAN" 
  find_library(PCAN_LIBRARY NAMES PCANBasic HINTS "C:\\Users\\mobenbra\\Downloads\\PCAN-Basic\\x64\\VC_LIB" )

  if(PCAN_LIBRARY)
    message(STATUS "Found PCAN library: ${PCAN_LIBRARY}")
    target_link_libraries(WS_Server PRIVATE ${PCAN_LIBRARY}) 
  else()
    message(WARNING "PCAN library not found")
  endif()
else()
  message(WARNING "PCANBasic.h not found")
endif()

find_package(Poco COMPONENTS Foundation Net Util JSON REQUIRED)
if (Poco_FOUND)
target_include_directories(WS_Server PUBLIC ${Poco_INCLUDE_DIR})
target_link_libraries(WS_Server PUBLIC ${Poco_LIBRARIES})
target_link_libraries(WS_Server PUBLIC Poco::Net)
else ()
message(FATAL_ERROR "libPoco not found")
endif()

install(TARGETS WS_Server LIBRARY DESTINATION lib)