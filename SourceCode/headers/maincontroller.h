/**
 * @file   maincontroller.h
 * @author Álvaro Sánchez González <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 Álvaro Sánchez González
 *
 * @brief  Clase controladora principal de todo el programa.
 *
 */
#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H


#define MC_DFLTLABELZ "Phase"           /**< Etiqueta.*/

#include <QImage>
#include <QRect>
#include <QFileInfo>
#include <QFile>
#include <QDir>
#include <QTextStream>
#include <QDateTime>
#include <QDebug>
#include <QDesktopServices>
#include "cameracontroller.h"
#include "spectrogramplot.h"
#include "cudageneralpurpose.h"
#include "abstractimagefactory.h"
#include "imagefactoryforcuda.h"
#include "imagefactoryforcpu.h"
#include "algorithmsequencecontroller.h"
#include "imagesequencecontroller.h"
#include "programsettings.h"
#include "fullimage.h"
#include "phaseimage.h"
#include "data2d.h"
#include "zipaux.h"


/*! \class MainController maincontroller.h "maincontroller.h"
 * \brief Controlador principal del programa.
 *
 *  Es la clase central de toda la aplicación. Tiene acceso directo (Conoce los objetos entidad) o indirecto (Conoce los objetos controlador)
 *  que conocen a los objetos entidad) a toda la información y funcionalidad. Por lo tanto es a él al que se realizan tanto las peticiones de
 *  datos, como de cálculos.
 */


class MainController{

private:
    CameraController * cameraController;            /**< Puntero al controlador de la cámara.*/

    ProgramSettings programSettings;                /**< Objeto que contiene las preferencias del programa.*/
    QString currentProject;                         /**< Ruta del proyecto actual abierto. La cadena estará vacía si no hay ninguno abierto.*/
    bool currentProjectSaved;                       /**< Indica si el projecto abierto actual está guardado.*/

    AlgorithmSequenceController algController;      /**< Controlador con acceso a las clases entidad con la lista de algoritmos.*/
    ImageSequenceController imgController;          /**< Controlador con acceso a las clases entidad con la lista de imágenes.*/

    QImage * refSourceQImage;                       /**< Puntero a un objeto que contiene la imagen original de la referencia. Será NULL si no se ha tomado ninguna.*/
    bool sourceHMirror;                             /**< Atributo que indica si se debe aplicar una transformación de espejo horizontalmente a la imagen de la referencia antes de procesarla.*/
    bool sourceVMirror;                             /**< Atributo que indica si se debe aplicar una transformación de espejo verticalmente a la imagen de la referencia antes de procesarla.*/
    int sourceRotation;                             /**< Atributo que indica si se debe aplicar una rotación de 90*sourceRotation grados en sentido horario a la imagen de la referencia.*/
    int fullImageSizeX;                             /**< Ancho efectivo que tendrá el objeto FullImage. Será un número potencia de 2.*/
    int fullImageSizeY;                             /**< Alto efectivo que tendrá el objeto FullImage. Será un número potencia de 2.*/
    float pixelsMmX;                                /**< Número de píxeles por milímetro en el eje horizontal.*/
    float pixelsMmY;                                /**< Número de píxeles por milímetro en el eje vertical.*/

    AbstractImageFactory * imageFactory;            /**< Puntero a la factoría concreta que se esté usando para crear o duplicar los objetos del tipo FullImage, PhaseImage o PhaseLine.*/

    FullImage * refFullImage;                       /**< Puntero al objeto tipo FullImage de la referencia.*/
    bool refFullImageUpdated;                       /**< Flag que indica si el objeto refFullImage está actualizado (Verdadero) o si por el contrario está obsoleto (Falso).*/

    FullImage * refFullImageFourier;                /**< Puntero al objeto tipo FullImage de la transformada de fourier de la referencia.*/
    bool refFullImageFourierUpdated;                /**< Flag que indica si el objeto refFullImageFourier está actualizado (Verdadero) o si por el contrario está obsoleto (Falso).*/


    QImage * refFullImageFourierAbsolute;           /**< Puntero al objeto tipo QImage con la imagen del valor absoluto de la transformada de fourier.*/
    bool refFullImageFourierAbsoluteUpdated;        /**< Flag que indica si el objeto refFullImageFourierAbsolute está actualizado (Verdadero) o si por el contrario está obsoleto (Falso).*/

    FullImage * refFullImageInverse;                /**< Puntero al objeto tipo FullImage de la transformada inversa de fourier de la referencia después de haber aplicado las máscaras.*/
    bool refFullImageInverseUpdated;                /**< Flag que indica si el objeto refFullImageInverse está actualizado (Verdadero) o si por el contrario está obsoleto (Falso).*/

    PhaseImage * refPhaseImage;                     /**< Puntero al objeto tipo PhaseImage de la referecia ya recortada, con la fase sin escanear (0-2Pi).*/
    PhaseImage * refPhaseImageScanned;              /**< Puntero al objeto tipo PhaseImage de la referecia ya recortada, con la fase escaneada. Este objeto es el se usará para procesar las imágenes tomadas.*/
    bool refPhaseImageUpdated;                      /**< Flag que indica si los objetos refPhaseImage y refPhaseImageScanned estás actualizados (Verdadero) o si por el contrario está obsoletos (Falso).*/

    QPoint originPoint;                             /**< Punto, respecto de la esquina superior izquierda de la imagen de la referencia, donde se encuentra el origen de coordenadas.*/
    QRect fullImageMask;                            /**< Máscara, respecto de la esquina superior izquierda de la imagen de la referencia, para aplicar a la referencia antes de realizar la transformada de fourier.*/
    QRect fullImageFourierMask;                     /**< Máscara, respecto de la esquina superior izquierda de la imagen de la referencia, para aplicar a la transformada de fourier de la referencia.*/

    QString zLabel;                                 /**< Etiqueta para la el eje de la variable dependiente.*/
    bool autoScaleAxes;                             /**< Indica si se debe autoescalar los ejes de lso gráficos en la ventana principal.*/
    float minZValue;                                /**< Mínimo valor para el escalado no automático de la variable dependiente.*/
    float maxZValue;                                /**< Máximo valor para el escalado no automático de la variable dependiente.*/
    float sequenceFPS;                              /**< Imágenes por segundo que se deben mostrar en el modo secuencia.*/
    bool viewLevelCurves;                           /**< Indica si en el espectrograma se deben mostrar las curvas de nivel.*/
    float threshold;                                /**< Indica el valor de corte para el gráfico paramétrico.*/

    bool applyAlgorithmsLive;                       /**< Indica si se debe aplicar la lista de algoritmos en el modo "en vivo".*/
    bool autoScaleAxesLive;                         /**< Indica si la opción de autoescalado de ejes en el modo "en vivo" se está activada.*/
    float thresholdLive;                            /**< Indica el valor de corte para el gráfico paramétrico en el modo "en vivo".*/
    float liveFPS;                                  /**< Indica el número de imágenes que se desean procesar por segundo en el modo "en vivo".*/


public:
    MainController();
    /**< Constructor. Se encargara de iniciar todos los parámetros, crear el resto de controladores,... */
    ~MainController();
    /**< Destructor.*/

    ProgramSettings GetProgramSettings();
    /**< Devuelve el atributo ProgramSettings.
      @return Valor del atributo.*/
    void SetProgramSettings(ProgramSettings settings);
    /**< Establece el atributo ProgramSettings.
      @param settings Valor del atributo.*/
    bool GetFirstExecution();
    /**< Indica si es la primera vez que se ejecuta el programa.
      @return Verdadero si es la primera vez.*/
    void SetMode(CalcMode mode);
    /**< Establece el modo de funcionamiento creando la factoría concreta para las imágenes. Debe llamarse sólo una vez antes de realizar ningún cálculo.
      @param mode Modo de cálculo del programa que se desee configurar.*/
    CalcMode GetMode();
    /**< Devuelve el modo de funcionamiento del programa.
      @return Valor del atributo.*/
    void SetDefaultMode(bool value);
    /**< Establece si se debe usar el modo por defecto sin preguntar al usuario la próxima vez.
      @param value Verdadero si se debe usar el modo por defecto.*/
    bool GetDefaultMode();
    /**< Devuelve información acerca de si se debe usar el modo por defecto sin preguntar al usuario.
      @return Verdadero si se debe usar el modo por defecto.*/
    bool GetApplyUntilCurrentAlgorithm();
    /**< Devuelve información acerca de si se debe aplicar sólo hasta el algoritmo selccionado, o por el contrario se debe aplicar siempre toda la lista.
      @return Verdadero si se debe aplicar sólo hasta el algoritmo seleccionado.*/
    bool GetSaveGraphicsViewSize();
    /**< Devuelve información acerca de si las imágenes se deben guardar con el tamaño de la vista actual, o por el contrario con el tamaño personalizado.
      @return Verdadero si se debe guardar en el tamaño actual.*/
    QSize GetSaveGraphicsCustomSize();
    /**< Devuelve el tamaño personalizado para las imágenes.
      @return Tamaño personalizado para el usuario.*/
    QString GetSaveGraphicsDefaultFormat();
    /**< Devuelve el formato por defecto para las imágenes.
      @return Cadena con las letras de la extensión  del formato elegido, sin punto.*/
    QString GetOutputFolder();
    /**< Devuelve la ruta del directorio por defecto para la salida.
      @return Cadena con la ruta, sin "/" al final.*/
    QString GetCurrentProject();
    /**< Devuelve el valor del atributo currentProject.
      @return Valor del atributo.*/
    bool GetCurrentProjectSaved();
    /**< Devuelve el valor del atributo currentProjectSaved.
      @return Valor del atributo.*/
    QImage * GetRefSourceQImage();
    /**< Devuelve el valor del atributo refSourceQImage.
      @return Valor del atributo.*/
    QString GetZLabel();
    /**< Devuelve el valor del atributo zLabel.
      @return Valor del atributo.*/
    bool GetAutoScaleAxes();
    /**< Devuelve el valor del atributo autoScaleAxes.
      @return Valor del atributo.*/
    float GetMinZValue();
    /**< Devuelve el valor del atributo minZValue.
      @return Valor del atributo.*/
    float GetMaxZValue();
    /**< Devuelve el valor del atributo maxZValue.
      @return Valor del atributo.*/
    float GetSequenceFPS();
    /**< Devuelve el valor del atributo sequenceFPS.
      @return Valor del atributo.*/
    bool GetViewLevelCurves();
    /**< Devuelve el valor del atributo viewLevelCurves.
      @return Valor del atributo.*/
    float GetThreshold();
    /**< Devuelve el valor del atributo threshold.
      @return Valor del atributo.*/
    bool GetApplyAlgorithmsLive();
    /**< Devuelve el valor del atributo applyAlgorithmsLive.
      @return Valor del atributo.*/
    bool GetAutoScaleAxesLive();
    /**< Devuelve el valor del atributo autoScaleAxesLive.
      @return Valor del atributo.*/
    float GetThresholdLive();
    /**< Devuelve el valor del atributo thresholdLive.
      @return Valor del atributo.*/
    float GetLiveFPS();
    /**< Devuelve el valor del atributo liveFPS.
      @return Valor del atributo.*/
    bool GetSourceHMirror();
    /**< Devuelve el valor del atributo sourceHMirror.
      @return Valor del atributo.*/
    bool GetSourceVMirror();
    /**< Devuelve el valor del atributo sourceVMirror.
      @return Valor del atributo.*/
    int GetSourceRotation();
    /**< Devuelve el valor del atributo sourceRotation.
      @return Valor del atributo.*/
    int GetFullImageSizeX();
    /**< Devuelve el valor del atributo fullImageSizeX.
      @return Valor del atributo.*/
    int GetFullImageSizeY();
    /**< Devuelve el valor del atributo fullImageSizeY.
      @return Valor del atributo.*/
    float GetPixelsMmX();
    /**< Devuelve el valor del atributo pixelsMmX.
      @return Valor del atributo.*/
    float GetPixelsMmY();
    /**< Devuelve el valor del atributo pixelsMmY.
      @return Valor del atributo.*/
    QPoint GetOriginPoint();
    /**< Devuelve el valor del atributo originPoint.
      @return Valor del atributo.*/
    QRect GetFullImageMask();
    /**< Devuelve el valor del atributo fullImageMask.
      @return Valor del atributo.*/
    QRect GetFullImageFourierMask();
    /**< Devuelve el valor del atributo fullImageFourierMask.
      @return Valor del atributo.*/
    QImage * GetRefFullImageFourier(int gain);
    /**< Devuelve el valor del atributo refFullImageFourierAbsolute, recalculándolo si es necesario.
      @param gain Ganancia para el cálculo del módulo.
      @return Valor del atributo.*/
    Data2D * GetRefPhaseData2D();
    /**< Devuelve el valor del atributo refPhaseImage, recalculándolo si es necesario.
      @return Valor del atributo.*/
    AlgorithmSequenceController GetAlgorithmSequenceController();
    /**< Devuelve el valor del atributo algController, controlador de los algormitmos. Nótese que es sólo una copia, por lo que deberá enviarse otra vez al controlador si se desea conservar los cambios que haya realizado.
      @return Valor del atributo.*/
    ImageSequenceController GetImageSequenceController();
    /**< Devuelve el valor del atributo imgController, controlador de los algormitmos. Nótese que es sólo una copia, por lo que deberá enviarse otra vez al controlador si se desea conservar los cambios que haya realizado.
      @return Valor del atributo.*/
    Data2D * GetPhaseData2D(int index,int nAlg=-1);
    /**< Crea un objeto tipo Data2D procesando la imagen indicada en caso necesario, con los algoritmos indicados.
      @param index Índice de la imagen en la lista que se debe procesar.
      @param nAlg Número de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Objeto tipo data2d con todos los atributos configurados para su representación, responsabilidad del cliente.*/
    Data1D * GetPhaseData1D(int index,int nAlg=-1);
    /**< Crea un objeto tipo Data1D procesando la imagen indicada en caso necesario, con los algoritmos indicados.
      @param index Índice de la imagen en la lista que se debe procesar.
      @param nAlg Número de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Objeto tipo data1d con todos los atributos configurados para su representación, responsabilidad del cliente.*/
    QImage GetPhaseQImage(int index,int nAlg);
    /**< Devuelve una imagen de la fase generada a partir de un spectrograma.
      @param index Índice de la imagen en la lista que se debe procesar.
      @param nAlg Número de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Imagen del spectrograma.*/

    void SetOutputFolder(QString value);
    /**< Establece la ruta del directorio de salida por defecto para los archivos.
      @param value Cadena con la ruta, sin "/" al final.*/

    void SetZLabel(QString value);
    /**< Establece el valor del atributo zLabel.
      @param value Valor para el atributo.*/
    void SetAutoScaleAxes(bool value);
    /**< Establece el valor del atributo autoScaleAxes.
      @param value Valor para el atributo.*/
    void SetMinZValue(float value);
    /**< Establece el valor del atributo minZValue.
      @param value Valor para el atributo.*/
    void SetMaxZValue(float value);
    /**< Establece el valor del atributo maxZValue.
      @param value Valor para el atributo.*/
    void SetSequenceFPS(float value);
    /**< Establece el valor del atributo sequenceFPS.
      @param value Valor para el atributo.*/
    void SetViewLevelCurves(bool value);
    /**< Establece el valor del atributo viewLevelCurves.
      @param value Valor para el atributo.*/
    void SetThreshold(float value);
    /**< Establece el valor del atributo threshold.
      @param value Valor para el atributo.*/
    void SetApplyAlgorithmsLive(bool value);
    /**< Establece el valor del atributo applyAlgorithmsLive.
      @param value Valor para el atributo.*/
    void SetAutoScaleAxesLive(bool value);
    /**< Establece el valor del atributo autoScaleAxesLive.
      @param value Valor para el atributo.*/
    void SetThresholdLive(float value);
    /**< Establece el valor del atributo thresholdLive.
      @param value Valor para el atributo.*/
    void SetLiveFPS(float value);
    /**< Establece el valor del atributo liveFPS.
      @param value Valor para el atributo.*/
    void SetSourceHMirror(bool value);
    /**< Establece el valor del atributo sourceHMirror.
      @param value Valor para el atributo.*/
    void SetSourceVMirror(bool value);
    /**< Establece el valor del atributo sourceVMirror.
      @param value Valor para el atributo.*/
    int SetSourceRotation(int value);
    /**< Establece el valor del atributo sourceRotation.
      @param value Valor para el atributo.*/
    int SetFullImageSizeX(int value);
    /**< Establece el valor del atributo fullImageSizeX.
      @param value Valor para el atributo.*/
    int SetFullImageSizeY(int value);
    /**< Establece el valor del atributo fullImageSizeY.
      @param value Valor para el atributo.*/
    float SetPixelsMmX(float value);
    /**< Establece el valor del atributo pixelsMmX.
      @param value Valor para el atributo.*/
    float SetPixelsMmY(float value);
    /**< Establece el valor del atributo pixelsMmY.
      @param value Valor para el atributo.*/
    void SetOriginPoint(QPoint value);
    /**< Establece el valor del atributo originPoint.
      @param value Valor para el atributo.*/
    void SetFullImageMask(QRect value);
    /**< Establece el valor del atributo fullImageMask.
      @param value Valor para el atributo.*/
    void SetFullImageFourierMask(QRect value);
    /**< Establece el valor del atributo fullImageFourierMask.
      @param value Valor para el atributo.*/
    void SetAlgorithmSequenceController(AlgorithmSequenceController value, bool unSave=true);
    /**< Establece el valor del atributo algController.
      @param value Valor para el atributo.
      @param unSave Indica si se debe marcar el proyecto como no guardado tras la llamada.*/
    void SetImageSequenceController(ImageSequenceController value, bool unSave=true);
    /**< Establece el valor del atributo imgController.
      @param value Valor para el atributo.
      @param unSave Indica si se debe marcar el proyecto como no guardado tras la llamada.*/

    void DeleteImage(int index);
    /**< Elimina la imagen de la fase en la posición indicada.
      @param index Índice (0,total-1) de la imagen a eliminar.*/

    bool CameraIsReady();
    /**< Indica si la cámara está lista.
      @return Verdadero si la cámara está iniciada y funcionando.*/
    CameraController * GetCameraController();
    /**< Devuelve un puntero al controlador de la cámara.
      @return Puntero al controlador de la cámara, sigue siendo responsabilidad del controlador principal.*/

    bool LoadRefImageFromFile(QString filePath);
    /**< Carga una imagen de un archivo que será fijada como referencia.
      @param filePath ruta de la imagen a cargar para la referencia.
      @return Verdadero en caso de éxito.*/
    bool LoadRefImageFromCamera();
    /**< Toma una imagen con la cámara para la referencia.
      @return Verdadero en caso de éxito.*/
    bool AddImageFromFile(QString filePath);
    /**< Carga una imagen de un archivo para procesarla con la referencia hasta obtener una imagen de la fase que se guarda al final de la lista.
      @param filePath ruta de la imagen a cargar para su procesado.
      @return Verdadero en caso de éxito.*/
    bool AddImageFromCamera();
    /**< Toma una imagen con la cámara para procesarla con la referencia hasta obtener una imagen de la fase que se guarda al final de la lista.
      @return Verdadero en caso de éxito.*/
    Data2D * ProcessImageFromCamera(bool save,bool applyAlg);
    /**< Método para el modo "en vivo". Toma una imagen de la cámara y la procesa con la referencia, para obtener una imagen de la fase.
      @param save Indica si además la imagen de la fase se debe salvar para la lista de imágenes.
      @param applyAlg Indica si se debe aplicar la lista de algoritmos o no.
      @return Puntero a un objeto tipo Data2D responsabilidad del cliente con los datos calculados.*/
    Data1D * ProcessImageFromCamera1D(bool save);
    /**< Método para el modo "en vivo". Toma una imagen de la cámara y la procesa con la referencia, para obtener una línea de fase.
      @param save Indica si además la imagen de la fase se debe salvar para la lista de imágenes.
      @return Puntero a un objeto tipo Data1D responsabilidad del cliente con los datos calculados.*/

    bool LoadProject(QString filePath);
    /**< Carga un proyecto .jpp de la ruta indicada.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se cargó correctamente.*/
    bool SaveProject(QString filePath);
    /**< Guarda un proyecto .jpp en la ruta indicada.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guardó correctamente.*/

    void MarkProjectUnsaved();
    /**< Fuerza el marcado de un proyecto como no guardado.*/

    bool LoadFullImageMaskOperationsAndSize(QString filePath);
    /**< Carga un fichero .ops con las operaciones a realizar sobre la referencia.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se cargó correctamente.*/
    bool SaveFullImageMaskOperationsAndSize(QString filePath);
    /**< Guarda un fichero .ops con las operaciones a realizar sobre la referencia.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guardó correctamente.*/

    bool LoadFullImageMaskParameters(QString filePath);
    /**< Carga un fichero .msk1 con la máscara que se aplica a la referencia.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se cargó correctamente.*/
    bool SaveFullImageMaskParameters(QString filePath);
    /**< Guarda un fichero .msk1 con la máscara que se aplica a la referencia.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guardó correctamente.*/
    bool LoadFullImageMaskFourierParameters(QString filePath);
    /**< Carga un fichero .msk2 con la máscara que se aplica a la transformada de fourier de la referencia.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se cargó correctamente.*/
    bool SaveFullImageMaskFourierParameters(QString filePath);
    /**< Guarda un fichero .msk2 con la máscara que se aplica a la transformada de fourier de la referencia.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guardó correctamente.*/
    bool LoadViewOptions(QString filePath);
    /**< Carga un fichero .viw con las opciones de visualización.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se cargó correctamente.*/
    bool SaveViewOptions(QString filePath);
    /**< Guarda un fichero .viw con las opciones de visualización.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guardó correctamente.*/


    QString GetDefaultNameForExport2D(int imgNum);
    /**< Obtiene el nombre por defecto para datos 2D exportados.
      @param imgNum Número de imagen de la lista para la que se exportan los datos.
      @return Nombre por defecto para el archivo.*/
    QString GetDefaultNameForExport1D(int imgNum);
    /**< Obtiene el nombre por defecto para datos 1D exportados.
      @param imgNum Número de imagen de la lista para la que se exportan los datos.
      @return Nombre por defecto para el archivo.*/
    bool ExportSingleData2D(QString filePath,int imgNum,int algNum);
    /**< Guarda una tabla con los datos de una imagen de la fase en un fichero de texto .txt.
      @param filePath Ruta y nombre para el archivo de salida.
      @param imgNum Índice de la imagen en la lista cuyos datos se deben guardar.
      @param algNum Número de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Verdadero en caso de éxito.*/
    bool ExportSingleData1D(QString filePath,int imgNum,int algNum);
    /**< Guarda una tabla con los datos de una línea de fase en un fichero de texto .txt.
      @param filePath Ruta y nombre para el archivo de salida.
      @param imgNum Índice de la imagen en la lista cuyos datos se deben guardar.
      @param algNum Número de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Verdadero en caso de éxito.*/
    bool ExportSeriesData2D(QString label,int algNum);
    /**< Guarda tablas con los datos de todas las imagenes de la fase en ficheros de texto .txt en una carpeta creada en el directorio de salida.
      @param label Etiqueta para la serie. Será el nombre de la carpeta creada.
      @param algNum Número de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Verdadero en caso de éxito.*/
    bool ExportSeriesData1D(QString label,int algNum);
    /**< Guarda tablas con los datos de todas las líneas de fase en ficheros de texto .txt en una carpeta creada en el directorio de salida.
      @param label Etiqueta para la serie. Será el nombre de la carpeta creada.
      @param algNum Número de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Verdadero en caso de éxito.*/


private:
    void LoadPersistentSettings();
    /**< Carga las preferencias del programa de un archivo en la carpeta del usuario al iniciar la aplicación.*/
    void SavePersistentSettings();
    /**< Guarda las preferencias del programa de un archivo en la carpeta del usuario al finalizar la aplicación.*/
    void CalculateRefFullImage();
    /**< Hace lo necesario para actualizar el atributo refFullImage.*/
    void CalculateRefFullImageFourier();
    /**< Hace lo necesario para actualizar el atributo refFullImageFourier.*/
    void CalculateRefFullImageFourierAbsolute(int gainIn);
    /**< Hace lo necesario para actualizar el atributo refFullImageFourierAbsolute.
        @param gainIn Ganancia con la que lo debe actualizar.*/
    void CalculateRefFullImageInverse();
    /**< Hace lo necesario para actualizar el atributo refFullImageInverse.*/
    void CalculateRefPhaseImage();
    /**< Hace lo necesario para actualizar los atributos refFaseImage y refPhaseImageScanned.*/
    void CalculatePhaseImage(int index,int nAlg=-1);
    /**< Aplica los algoritmos indicados a la imagen de la fase indicada.
      @param index Índice de la imagen en la lista que se debe procesar.
      @param nAlg Número de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).*/
    PhaseImage * ProcessSourcePictureImage(QImage auxImage);
    /**< Procesa una imagen con la referencia hasta obtener la imagen de la fase, sin todavia aplicar la lista de algoritmos.
      @param auxImage Imagen que se debe procesar.
      @return Imagen de la fase procedente del procesado de la imagen pasada como argumento.*/
    void AddPhaseImage(PhaseImage * phaseImage, QString label);
    /**< Añade al final de la lista una imagen d ela fase con la etiqueta indicada.
      @param phaseImage Puntero a la imagen que se va a añadir.
      @param label Etiqueta que será el nombre de la imagen.*/
    void SavePictureToFileDated(QImage * auxImage, QString extraLabel="");
    /**< Guarda una imagen de pasada como argumento en la carpeta de imágenes tomadas con la cámara, en el caso de tener actvada la opción en las preferencias, indicando en el nombre del archivo la fecha y hora de guardado.
      @param auxImage Puntero a la imagen que se va a guardar.
      @param extraLabel Etiqueta extra para el nombre del archivo.*/
};

#endif // MAINCONTROLLER_H
