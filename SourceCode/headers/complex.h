/**
 * @file   complex.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Cabecera que define constantes matemáticas y estructuras usadas en la aplicación
 *
 */
#ifndef COMPLEX_H
#define COMPLEX_H


/** Número Pi*/
#define PI_NUM (float)3.141592

/** \struct complex complex.h "complex.h"
 * \brief Estructura para variables complejas.
 */
typedef struct {
    float x;    /**< Parte real.*/
    float y;    /**< Parte imaginaria.*/
} complex;


#endif // COMPLEX_H
