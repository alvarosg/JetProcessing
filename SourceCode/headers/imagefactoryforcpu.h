/**
 * @file   imagefactoryforcpu.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Implementación de AbstractImageFactory para trabajo con la CPU.
 *
 */
#ifndef IMAGEFACTORYFORCPU_H
#define IMAGEFACTORYFORCPU_H

#include"abstractimagefactory.h"
#include"fullimagecpu.h"
#include"phaseimagecpu.h"
#include"phaselinecpu.h"

/*! \class ImageFactoryForCPU imagefactoryforcpu.h "imagefactoryforcpu.h"
 * \brief Implementación de AbstractImageFactory para trabajo con la CPU
 *
 */

class ImageFactoryForCPU : public AbstractImageFactory
{
public:
    ImageFactoryForCPU();
    /**< Constructor.*/
    FullImage * CreateFullImage(QImage *source,int power2SizeX, int power2SizeY);
    FullImage * CopyFullImage(FullImage * input);
    PhaseImage * CreatePhaseImage(float * buffer, int sizeX_in, int sizeY_in);
    PhaseImage * CopyPhaseImage(PhaseImage * input);
    PhaseLine * CreatePhaseLine(float * buffer, int size_in);
    PhaseLine * CopyPhaseLine(PhaseLine * input);
};

#endif // IMAGEFACTORYFORCPU_H
