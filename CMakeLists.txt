cmake_minimum_required(VERSION 3.0.0)
project(ScheduledGitCommit)

set(CMAKE_CXX_STANDARD 17)

add_executable(ScheduledGitCommit main.cpp
        src/git_manager.cpp
        includes/options_manager.hpp)

# Add the include directory
target_include_directories(ScheduledGitCommit PRIVATE include)
# Set the executable name to "line-scanner"
set_target_properties(ScheduledGitCommit PROPERTIES OUTPUT_NAME "scheduled-git-commit")
# set the output directory to bin
set_target_properties(ScheduledGitCommit PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
# Set the output directory to bin/lib
set_target_properties(ScheduledGitCommit PROPERTIES LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
# Set the output directory to bin/lib
set_target_properties(ScheduledGitCommit PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/lib")
# Set the output directory to bin/obj
set_target_properties(ScheduledGitCommit PROPERTIES PDB_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/obj")
# Set the output directory to bin/obj
set_target_properties(ScheduledGitCommit PROPERTIES INTERMEDIATE_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/obj")
# Set the debug executable name to "line-scanner-debug"
set_target_properties(ScheduledGitCommit PROPERTIES DEBUG_POSTFIX "-debug")



# Set the cmake debug directory to bin/obj/cmake/debug
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin/obj/cmake/debug")
# Set the cmake release directory to bin/obj/cmake/release
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin/obj/cmake/release")