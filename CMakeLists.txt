cmake_minimum_required(VERSION 3.10)

# Set your project name
project(XML_GENERATION VERSION 1.0)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find Boost
find_package(Boost REQUIRED COMPONENTS filesystem program_options)

# Include directories for header files
include_directories(include)

# Include Boost headers
include_directories(${Boost_INCLUDE_DIRS})

# Include directories for BACH-5.5
include_directories(${CMAKE_SOURCE_DIR}/lib/BACH-5.5/include)

# Add the TinyXML2 library
add_subdirectory(lib/tinyxml2-master)

# Automatically include all source files from the src directory
file(GLOB_RECURSE PROJECT_SOURCES "src/*.cpp")

# Add executable target with automatically collected source files
add_executable(XML_GENERATION ${PROJECT_SOURCES})

# Link the executable with the TinyXML2 library
target_link_libraries(XML_GENERATION tinyxml2)

# Link Boost libraries
target_link_libraries(XML_GENERATION ${Boost_LIBRARIES})

# Link against cryptominisat5
target_link_libraries(XML_GENERATION ${CMAKE_SOURCE_DIR}/lib/BACH-5.5/lib/libcryptominisat5.so.5.6)

# Ensure the executable can find shared libraries at runtime
set_target_properties(XML_GENERATION PROPERTIES
    INSTALL_RPATH "$ORIGIN/../lib/BACH-5.5/lib"
    BUILD_WITH_INSTALL_RPATH True
)
