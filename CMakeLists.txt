cmake_minimum_required(VERSION 3.28)
project(final C)

set(CMAKE_C_STANDARD 90)

add_executable(final main.c
        preproc.c
        front.c
        front.c
        firstPass.c
        CodCovert.c
        dataTable.c
        secondPass.c
        assembler.c
)
