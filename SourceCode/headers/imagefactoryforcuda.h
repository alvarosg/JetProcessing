/**
 * @file   imagefactoryforcuda.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de AbstractImageFactory para trabajo con la CUDA.
 *
 */
#ifndef IMAGEFACTORYFORCUDA_H
#define IMAGEFACTORYFORCUDA_H

#include"abstractimagefactory.h"
#include"fullimagecuda.h"
#include"phaseimagecuda.h"
#include"phaselinecuda.h"

/*! \class ImageFactoryForCUDA imagefactoryforcuda.h "imagefactoryforcuda.h"
 * \brief Implementación de AbstractImageFactory para trabajo con la CUDA
 *
 */

class ImageFactoryForCUDA : public AbstractImageFactory
{
public:
    ImageFactoryForCUDA();
    /**< Constructor.*/
    FullImage * CreateFullImage(QImage *source,int power2SizeX, int power2SizeY);
    FullImage * CopyFullImage(FullImage * input);
    PhaseImage * CreatePhaseImage(float * buffer, int sizeX_in, int sizeY_in);
    PhaseImage * CopyPhaseImage(PhaseImage * input);
    PhaseLine * CreatePhaseLine(float * buffer, int size_in);
    PhaseLine * CopyPhaseLine(PhaseLine * input);
};

#endif // IMAGEFACTORYFORCUDA_H
