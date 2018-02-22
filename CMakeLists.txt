cmake_minimum_required(VERSION 2.8)
project(cn64_show C CXX)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_language(ASM_NASM)
if(CMAKE_ASM_NASM_COMPILER_LOADED)
  file(GLOB ASM_FILE "ecops64/ecops64.asm")
else()
  message(FATAL_ERROR "Could not load NASM compiler." )
endif(CMAKE_ASM_NASM_COMPILER_LOADED)

set(CMAKE_CXX_FLAGS "-O3")
set(CMAKE_C_FLAGS "-O3")

file(GLOB SRCFILES *.cpp ecops32/*.c ecops64/*.c crypt-aux/*.c)

add_executable(cn64_show ${SRCFILES} ${ASM_FILE})

install(TARGETS cn64_show RUNTIME DESTINATION bin)
