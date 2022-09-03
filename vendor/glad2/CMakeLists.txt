cmake_minimum_required(VERSION 3.20)
project(glad2)

add_library(glad2 STATIC 
  "src/gl.c" 
  "include/glad/gl.h"
  "include/KHR/khrplatform.h"
)
target_include_directories(glad2 PUBLIC "include")
