cmake_minimum_required(VERSION 3.0)  
project(Simulation)          
set(CMAKE_PREFIX_PATH "C:\\Users\\mobenbra\\Downloads\\poco-1.9.0-all2\\poco-1.9.0-all\\out\\build\\x86-Debug\\Poco")

set(INC
    inc/simulation.h
)

set(SRC
    src/simulation.cpp
)

# Create Peak_simul library
add_library(Peak_simul ${INC} ${SRC}  "inc/Frame.h" "inc/TotalFrame.h")
add_library(Link_Peak_simul ALIAS Peak_simul)
target_include_directories(Peak_simul PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc  
    ${CMAKE_SOURCE_DIR}/WS/inc
    ${CMAKE_SOURCE_DIR}/Main/inc/
)

find_path(PCAN_INCLUDE_DIR NAMES PCANBasic.h PATHS "C:\\Users\\mobenbra\\Downloads\\PCAN-ISO-TP\\Include")


if(PCAN_INCLUDE_DIR)
  include_directories(${PCAN_INCLUDE_DIR})
  message(STATUS "Found PCANBasic.h in: ${PCAN_INCLUDE_DIR}")

  # Assuming the library is named "PCAN" 
  find_library(PCAN_LIBRARY NAMES PCANBasic HINTS "C:\\Users\\mobenbra\\Downloads\\PCAN-ISO-TP\\x86\\VC_LIB")

  if(PCAN_LIBRARY)
    message(STATUS "Found PCAN library: ${PCAN_LIBRARY}")
    target_link_libraries(Peak_simul PRIVATE ${PCAN_LIBRARY})  # Replace with your target name
  else()
    message(WARNING "PCAN library not found")
  endif()
else()
  message(WARNING "PCANBasic.h not found")
endif()

# Find Poco library 
find_package(Poco COMPONENTS Foundation Net JSON REQUIRED)
if(Poco_FOUND)
    # Include directories for Poco library
    target_include_directories(Peak_simul PUBLIC ${Poco_INCLUDE_DIR})

    # Link pcan_comm library with Poco libraries
    target_link_libraries(Peak_simul PUBLIC ${Poco_LIBRARIES})
else()
    message(FATAL_ERROR "libPoco not found")  # Informative error message
endif()

# Install the pcan_comm library (adjust destination if needed)
install(TARGETS Peak_simul LIBRARY DESTINATION lib)