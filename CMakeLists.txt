cmake_minimum_required(VERSION 4.1)
project(ProyectoV1)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProyectoV1 main.cpp
        Equipo.cpp
        Equipo.h
        Incidencia.cpp
        Incidencia.h
        IncidenciaBaja.cpp
        IncidenciaBaja.h
        IncidenciaMedia.cpp
        IncidenciaMedia.h
        IncidenciaAlta.cpp
        IncidenciaAlta.h
        Mantenimiento.cpp
        Mantenimiento.h
        Sistema.cpp
        Sistema.h)

