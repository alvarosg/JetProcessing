/**
 * @file   complex.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
 *
 * @brief  Cabecera que define constantes matem�ticas y estructuras usadas en la aplicaci�n
 *
 */
#ifndef COMPLEX_H
#define COMPLEX_H


/** N�mero Pi*/
#define PI_NUM (float)3.141592

/** \struct complex complex.h "complex.h"
 * \brief Estructura para variables complejas.
 */
typedef struct {
    float x;    /**< Parte real.*/
    float y;    /**< Parte imaginaria.*/
} complex;


#endif // COMPLEX_H
