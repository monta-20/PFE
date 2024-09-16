cmake_minimum_required(VERSION 3.0)  
project(pcan_communication)          
##set(CMAKE_PREFIX_PATH "C:\\Users\\mobenbra\\Downloads\\poco-1.9.0-all2\\poco-1.9.0-all\\out\\build\\x86-Debug\\Poco")

set(CMAKE_PREFIX_PATH "C:\\Users\\monta\\OneDrive\\Desktop\\POCO\\poco\\out\\build\\x86-Debug\\Poco")


set(INC
    inc/pcan_communication.h
)

set(SRC
    src/pcan_communication.cpp
)

# Create pcan_communication library
add_library(pcan_comm ${INC} ${SRC} "inc/canMessage.h" "inc/Frame.h" "inc/TotalFrame.h" "inc/simulation.h" "src/simulation.cpp")
add_library(Link_pcan_comm ALIAS pcan_comm)
# Target include directories for pcan_comm
target_include_directories(pcan_comm PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/inc  # Include directory for pcan_communication headers
    ${CMAKE_SOURCE_DIR}/DDM-Server/inc
)

##find_path(PCAN_INCLUDE_DIR NAMES PCANBasic.h PATHS "C:\\Users\\mobenbra\\Downloads\\PCAN-ISO-TP\\Include")

find_path(PCAN_INCLUDE_DIR NAMES PCAN-ISO-TP_2016 "C:\\Users\\monta\\Downloads\\PCAN-ISO-TP\\Include")

if(PCAN_INCLUDE_DIR)
  include_directories(${PCAN_INCLUDE_DIR})
  message(STATUS "Found PCANBasic.h in: ${PCAN_INCLUDE_DIR}")

  # Assuming the library is named "PCAN" 

  #find_library(PCAN_LIBRARY NAMES PCANBasic HINTS "C:\\Users\\mobenbra\\Downloads\\PCAN-ISO-TP\\x86\\VC_LIB")

  find_library(PCAN_LIBRARY NAMES PCAN-ISO-TP_2016 HINTS "C:\\Users\\monta\\Downloads\\PCAN-ISO-TP\\x86\\VC_LIB")

  if(PCAN_LIBRARY)
    message(STATUS "Found PCAN library: ${PCAN_LIBRARY}")
    target_link_libraries(pcan_comm PRIVATE ${PCAN_LIBRARY})  # Replace with your target name
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
    target_include_directories(pcan_comm PUBLIC ${Poco_INCLUDE_DIR})

    # Link pcan_comm library with Poco libraries
    target_link_libraries(pcan_comm PUBLIC ${Poco_LIBRARIES})
else()
    message(FATAL_ERROR "libPoco not found")  # Informative error message
endif()
# Install the pcan_comm library (adjust destination if needed)
install(TARGETS pcan_comm LIBRARY DESTINATION lib)