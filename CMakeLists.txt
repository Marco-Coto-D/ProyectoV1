cmake_minimum_required(VERSION 3.20)
project(ProyectoV1)

set(CMAKE_CXX_STANDARD 17)

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
        Excepciones.h
        Mantenimiento.h
        MantenimientoCorrectivo.cpp
        MantenimientoCorrectivo.h
        MantenimientoPreventivo.cpp
        MantenimientoPreventivo.h
        MantenimientoPredictivo.cpp
        MantenimientoPredictivo.h
        Sistema.cpp
        Sistema.h)
