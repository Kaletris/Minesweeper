cmake_minimum_required(VERSION 3.20)
project(Minesweeper C)

set(CMAKE_C_STANDARD 11)

include_directories(.)

add_executable(Minesweeper
        debugmalloc.h
        econio.c
        econio.h
        jatek.c
        jatek.h
        main.c
        menuvezerles.c
        menuvezerles.h
        ranglista.c
        typedefs.h)
