cmake_minimum_required(VERSION 3.16)
project(NewPass)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Найти LLVM
find_package(LLVM 15 REQUIRED CONFIG)

message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

include_directories(${LLVM_INCLUDE_DIRS})
add_definitions(${LLVM_DEFINITIONS})

# Создание .so плагина
add_library(NewPass SHARED NewPass.cpp)

llvm_map_components_to_libnames(llvm_libs core passes support)

target_link_libraries(NewPass PRIVATE ${llvm_libs})

