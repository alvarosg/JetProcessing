/**
 * @file   maincontroller.h
 * @author �lvaro S�nchez Gonz�lez <alvarosg@usal.es>
 * @date   Mon Jul  23 2012
 *
 * Copyright (c) 2012 �lvaro S�nchez Gonz�lez
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
 *  Es la clase central de toda la aplicaci�n. Tiene acceso directo (Conoce los objetos entidad) o indirecto (Conoce los objetos controlador)
 *  que conocen a los objetos entidad) a toda la informaci�n y funcionalidad. Por lo tanto es a �l al que se realizan tanto las peticiones de
 *  datos, como de c�lculos.
 */


class MainController{

private:
    CameraController * cameraController;            /**< Puntero al controlador de la c�mara.*/

    ProgramSettings programSettings;                /**< Objeto que contiene las preferencias del programa.*/
    QString currentProject;                         /**< Ruta del proyecto actual abierto. La cadena estar� vac�a si no hay ninguno abierto.*/
    bool currentProjectSaved;                       /**< Indica si el projecto abierto actual est� guardado.*/

    AlgorithmSequenceController algController;      /**< Controlador con acceso a las clases entidad con la lista de algoritmos.*/
    ImageSequenceController imgController;          /**< Controlador con acceso a las clases entidad con la lista de im�genes.*/

    QImage * refSourceQImage;                       /**< Puntero a un objeto que contiene la imagen original de la referencia. Ser� NULL si no se ha tomado ninguna.*/
    bool sourceHMirror;                             /**< Atributo que indica si se debe aplicar una transformaci�n de espejo horizontalmente a la imagen de la referencia antes de procesarla.*/
    bool sourceVMirror;                             /**< Atributo que indica si se debe aplicar una transformaci�n de espejo verticalmente a la imagen de la referencia antes de procesarla.*/
    int sourceRotation;                             /**< Atributo que indica si se debe aplicar una rotaci�n de 90*sourceRotation grados en sentido horario a la imagen de la referencia.*/
    int fullImageSizeX;                             /**< Ancho efectivo que tendr� el objeto FullImage. Ser� un n�mero potencia de 2.*/
    int fullImageSizeY;                             /**< Alto efectivo que tendr� el objeto FullImage. Ser� un n�mero potencia de 2.*/
    float pixelsMmX;                                /**< N�mero de p�xeles por mil�metro en el eje horizontal.*/
    float pixelsMmY;                                /**< N�mero de p�xeles por mil�metro en el eje vertical.*/

    AbstractImageFactory * imageFactory;            /**< Puntero a la factor�a concreta que se est� usando para crear o duplicar los objetos del tipo FullImage, PhaseImage o PhaseLine.*/

    FullImage * refFullImage;                       /**< Puntero al objeto tipo FullImage de la referencia.*/
    bool refFullImageUpdated;                       /**< Flag que indica si el objeto refFullImage est� actualizado (Verdadero) o si por el contrario est� obsoleto (Falso).*/

    FullImage * refFullImageFourier;                /**< Puntero al objeto tipo FullImage de la transformada de fourier de la referencia.*/
    bool refFullImageFourierUpdated;                /**< Flag que indica si el objeto refFullImageFourier est� actualizado (Verdadero) o si por el contrario est� obsoleto (Falso).*/


    QImage * refFullImageFourierAbsolute;           /**< Puntero al objeto tipo QImage con la imagen del valor absoluto de la transformada de fourier.*/
    bool refFullImageFourierAbsoluteUpdated;        /**< Flag que indica si el objeto refFullImageFourierAbsolute est� actualizado (Verdadero) o si por el contrario est� obsoleto (Falso).*/

    FullImage * refFullImageInverse;                /**< Puntero al objeto tipo FullImage de la transformada inversa de fourier de la referencia despu�s de haber aplicado las m�scaras.*/
    bool refFullImageInverseUpdated;                /**< Flag que indica si el objeto refFullImageInverse est� actualizado (Verdadero) o si por el contrario est� obsoleto (Falso).*/

    PhaseImage * refPhaseImage;                     /**< Puntero al objeto tipo PhaseImage de la referecia ya recortada, con la fase sin escanear (0-2Pi).*/
    PhaseImage * refPhaseImageScanned;              /**< Puntero al objeto tipo PhaseImage de la referecia ya recortada, con la fase escaneada. Este objeto es el se usar� para procesar las im�genes tomadas.*/
    bool refPhaseImageUpdated;                      /**< Flag que indica si los objetos refPhaseImage y refPhaseImageScanned est�s actualizados (Verdadero) o si por el contrario est� obsoletos (Falso).*/

    QPoint originPoint;                             /**< Punto, respecto de la esquina superior izquierda de la imagen de la referencia, donde se encuentra el origen de coordenadas.*/
    QRect fullImageMask;                            /**< M�scara, respecto de la esquina superior izquierda de la imagen de la referencia, para aplicar a la referencia antes de realizar la transformada de fourier.*/
    QRect fullImageFourierMask;                     /**< M�scara, respecto de la esquina superior izquierda de la imagen de la referencia, para aplicar a la transformada de fourier de la referencia.*/

    QString zLabel;                                 /**< Etiqueta para la el eje de la variable dependiente.*/
    bool autoScaleAxes;                             /**< Indica si se debe autoescalar los ejes de lso gr�ficos en la ventana principal.*/
    float minZValue;                                /**< M�nimo valor para el escalado no autom�tico de la variable dependiente.*/
    float maxZValue;                                /**< M�ximo valor para el escalado no autom�tico de la variable dependiente.*/
    float sequenceFPS;                              /**< Im�genes por segundo que se deben mostrar en el modo secuencia.*/
    bool viewLevelCurves;                           /**< Indica si en el espectrograma se deben mostrar las curvas de nivel.*/
    float threshold;                                /**< Indica el valor de corte para el gr�fico param�trico.*/

    bool applyAlgorithmsLive;                       /**< Indica si se debe aplicar la lista de algoritmos en el modo "en vivo".*/
    bool autoScaleAxesLive;                         /**< Indica si la opci�n de autoescalado de ejes en el modo "en vivo" se est� activada.*/
    float thresholdLive;                            /**< Indica el valor de corte para el gr�fico param�trico en el modo "en vivo".*/
    float liveFPS;                                  /**< Indica el n�mero de im�genes que se desean procesar por segundo en el modo "en vivo".*/


public:
    MainController();
    /**< Constructor. Se encargara de iniciar todos los par�metros, crear el resto de controladores,... */
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
    /**< Establece el modo de funcionamiento creando la factor�a concreta para las im�genes. Debe llamarse s�lo una vez antes de realizar ning�n c�lculo.
      @param mode Modo de c�lculo del programa que se desee configurar.*/
    CalcMode GetMode();
    /**< Devuelve el modo de funcionamiento del programa.
      @return Valor del atributo.*/
    void SetDefaultMode(bool value);
    /**< Establece si se debe usar el modo por defecto sin preguntar al usuario la pr�xima vez.
      @param value Verdadero si se debe usar el modo por defecto.*/
    bool GetDefaultMode();
    /**< Devuelve informaci�n acerca de si se debe usar el modo por defecto sin preguntar al usuario.
      @return Verdadero si se debe usar el modo por defecto.*/
    bool GetApplyUntilCurrentAlgorithm();
    /**< Devuelve informaci�n acerca de si se debe aplicar s�lo hasta el algoritmo selccionado, o por el contrario se debe aplicar siempre toda la lista.
      @return Verdadero si se debe aplicar s�lo hasta el algoritmo seleccionado.*/
    bool GetSaveGraphicsViewSize();
    /**< Devuelve informaci�n acerca de si las im�genes se deben guardar con el tama�o de la vista actual, o por el contrario con el tama�o personalizado.
      @return Verdadero si se debe guardar en el tama�o actual.*/
    QSize GetSaveGraphicsCustomSize();
    /**< Devuelve el tama�o personalizado para las im�genes.
      @return Tama�o personalizado para el usuario.*/
    QString GetSaveGraphicsDefaultFormat();
    /**< Devuelve el formato por defecto para las im�genes.
      @return Cadena con las letras de la extensi�n  del formato elegido, sin punto.*/
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
    /**< Devuelve el valor del atributo refFullImageFourierAbsolute, recalcul�ndolo si es necesario.
      @param gain Ganancia para el c�lculo del m�dulo.
      @return Valor del atributo.*/
    Data2D * GetRefPhaseData2D();
    /**< Devuelve el valor del atributo refPhaseImage, recalcul�ndolo si es necesario.
      @return Valor del atributo.*/
    AlgorithmSequenceController GetAlgorithmSequenceController();
    /**< Devuelve el valor del atributo algController, controlador de los algormitmos. N�tese que es s�lo una copia, por lo que deber� enviarse otra vez al controlador si se desea conservar los cambios que haya realizado.
      @return Valor del atributo.*/
    ImageSequenceController GetImageSequenceController();
    /**< Devuelve el valor del atributo imgController, controlador de los algormitmos. N�tese que es s�lo una copia, por lo que deber� enviarse otra vez al controlador si se desea conservar los cambios que haya realizado.
      @return Valor del atributo.*/
    Data2D * GetPhaseData2D(int index,int nAlg=-1);
    /**< Crea un objeto tipo Data2D procesando la imagen indicada en caso necesario, con los algoritmos indicados.
      @param index �ndice de la imagen en la lista que se debe procesar.
      @param nAlg N�mero de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Objeto tipo data2d con todos los atributos configurados para su representaci�n, responsabilidad del cliente.*/
    Data1D * GetPhaseData1D(int index,int nAlg=-1);
    /**< Crea un objeto tipo Data1D procesando la imagen indicada en caso necesario, con los algoritmos indicados.
      @param index �ndice de la imagen en la lista que se debe procesar.
      @param nAlg N�mero de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Objeto tipo data1d con todos los atributos configurados para su representaci�n, responsabilidad del cliente.*/
    QImage GetPhaseQImage(int index,int nAlg);
    /**< Devuelve una imagen de la fase generada a partir de un spectrograma.
      @param index �ndice de la imagen en la lista que se debe procesar.
      @param nAlg N�mero de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
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
    /**< Elimina la imagen de la fase en la posici�n indicada.
      @param index �ndice (0,total-1) de la imagen a eliminar.*/

    bool CameraIsReady();
    /**< Indica si la c�mara est� lista.
      @return Verdadero si la c�mara est� iniciada y funcionando.*/
    CameraController * GetCameraController();
    /**< Devuelve un puntero al controlador de la c�mara.
      @return Puntero al controlador de la c�mara, sigue siendo responsabilidad del controlador principal.*/

    bool LoadRefImageFromFile(QString filePath);
    /**< Carga una imagen de un archivo que ser� fijada como referencia.
      @param filePath ruta de la imagen a cargar para la referencia.
      @return Verdadero en caso de �xito.*/
    bool LoadRefImageFromCamera();
    /**< Toma una imagen con la c�mara para la referencia.
      @return Verdadero en caso de �xito.*/
    bool AddImageFromFile(QString filePath);
    /**< Carga una imagen de un archivo para procesarla con la referencia hasta obtener una imagen de la fase que se guarda al final de la lista.
      @param filePath ruta de la imagen a cargar para su procesado.
      @return Verdadero en caso de �xito.*/
    bool AddImageFromCamera();
    /**< Toma una imagen con la c�mara para procesarla con la referencia hasta obtener una imagen de la fase que se guarda al final de la lista.
      @return Verdadero en caso de �xito.*/
    Data2D * ProcessImageFromCamera(bool save,bool applyAlg);
    /**< M�todo para el modo "en vivo". Toma una imagen de la c�mara y la procesa con la referencia, para obtener una imagen de la fase.
      @param save Indica si adem�s la imagen de la fase se debe salvar para la lista de im�genes.
      @param applyAlg Indica si se debe aplicar la lista de algoritmos o no.
      @return Puntero a un objeto tipo Data2D responsabilidad del cliente con los datos calculados.*/
    Data1D * ProcessImageFromCamera1D(bool save);
    /**< M�todo para el modo "en vivo". Toma una imagen de la c�mara y la procesa con la referencia, para obtener una l�nea de fase.
      @param save Indica si adem�s la imagen de la fase se debe salvar para la lista de im�genes.
      @return Puntero a un objeto tipo Data1D responsabilidad del cliente con los datos calculados.*/

    bool LoadProject(QString filePath);
    /**< Carga un proyecto .jpp de la ruta indicada.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se carg� correctamente.*/
    bool SaveProject(QString filePath);
    /**< Guarda un proyecto .jpp en la ruta indicada.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guard� correctamente.*/

    void MarkProjectUnsaved();
    /**< Fuerza el marcado de un proyecto como no guardado.*/

    bool LoadFullImageMaskOperationsAndSize(QString filePath);
    /**< Carga un fichero .ops con las operaciones a realizar sobre la referencia.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se carg� correctamente.*/
    bool SaveFullImageMaskOperationsAndSize(QString filePath);
    /**< Guarda un fichero .ops con las operaciones a realizar sobre la referencia.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guard� correctamente.*/

    bool LoadFullImageMaskParameters(QString filePath);
    /**< Carga un fichero .msk1 con la m�scara que se aplica a la referencia.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se carg� correctamente.*/
    bool SaveFullImageMaskParameters(QString filePath);
    /**< Guarda un fichero .msk1 con la m�scara que se aplica a la referencia.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guard� correctamente.*/
    bool LoadFullImageMaskFourierParameters(QString filePath);
    /**< Carga un fichero .msk2 con la m�scara que se aplica a la transformada de fourier de la referencia.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se carg� correctamente.*/
    bool SaveFullImageMaskFourierParameters(QString filePath);
    /**< Guarda un fichero .msk2 con la m�scara que se aplica a la transformada de fourier de la referencia.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guard� correctamente.*/
    bool LoadViewOptions(QString filePath);
    /**< Carga un fichero .viw con las opciones de visualizaci�n.
      @param filePath Ruta y nombre del fichero para cargar.
      @return Verdadero si se carg� correctamente.*/
    bool SaveViewOptions(QString filePath);
    /**< Guarda un fichero .viw con las opciones de visualizaci�n.
      @param filePath Ruta y nombre del fichero para guardar.
      @return Verdadero si se guard� correctamente.*/


    QString GetDefaultNameForExport2D(int imgNum);
    /**< Obtiene el nombre por defecto para datos 2D exportados.
      @param imgNum N�mero de imagen de la lista para la que se exportan los datos.
      @return Nombre por defecto para el archivo.*/
    QString GetDefaultNameForExport1D(int imgNum);
    /**< Obtiene el nombre por defecto para datos 1D exportados.
      @param imgNum N�mero de imagen de la lista para la que se exportan los datos.
      @return Nombre por defecto para el archivo.*/
    bool ExportSingleData2D(QString filePath,int imgNum,int algNum);
    /**< Guarda una tabla con los datos de una imagen de la fase en un fichero de texto .txt.
      @param filePath Ruta y nombre para el archivo de salida.
      @param imgNum �ndice de la imagen en la lista cuyos datos se deben guardar.
      @param algNum N�mero de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Verdadero en caso de �xito.*/
    bool ExportSingleData1D(QString filePath,int imgNum,int algNum);
    /**< Guarda una tabla con los datos de una l�nea de fase en un fichero de texto .txt.
      @param filePath Ruta y nombre para el archivo de salida.
      @param imgNum �ndice de la imagen en la lista cuyos datos se deben guardar.
      @param algNum N�mero de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Verdadero en caso de �xito.*/
    bool ExportSeriesData2D(QString label,int algNum);
    /**< Guarda tablas con los datos de todas las imagenes de la fase en ficheros de texto .txt en una carpeta creada en el directorio de salida.
      @param label Etiqueta para la serie. Ser� el nombre de la carpeta creada.
      @param algNum N�mero de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Verdadero en caso de �xito.*/
    bool ExportSeriesData1D(QString label,int algNum);
    /**< Guarda tablas con los datos de todas las l�neas de fase en ficheros de texto .txt en una carpeta creada en el directorio de salida.
      @param label Etiqueta para la serie. Ser� el nombre de la carpeta creada.
      @param algNum N�mero de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).
      @return Verdadero en caso de �xito.*/


private:
    void LoadPersistentSettings();
    /**< Carga las preferencias del programa de un archivo en la carpeta del usuario al iniciar la aplicaci�n.*/
    void SavePersistentSettings();
    /**< Guarda las preferencias del programa de un archivo en la carpeta del usuario al finalizar la aplicaci�n.*/
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
      @param index �ndice de la imagen en la lista que se debe procesar.
      @param nAlg N�mero de algoritmos que se deben aplicar secuencialmente (-1 para aplicar toda la lista).*/
    PhaseImage * ProcessSourcePictureImage(QImage auxImage);
    /**< Procesa una imagen con la referencia hasta obtener la imagen de la fase, sin todavia aplicar la lista de algoritmos.
      @param auxImage Imagen que se debe procesar.
      @return Imagen de la fase procedente del procesado de la imagen pasada como argumento.*/
    void AddPhaseImage(PhaseImage * phaseImage, QString label);
    /**< A�ade al final de la lista una imagen d ela fase con la etiqueta indicada.
      @param phaseImage Puntero a la imagen que se va a a�adir.
      @param label Etiqueta que ser� el nombre de la imagen.*/
    void SavePictureToFileDated(QImage * auxImage, QString extraLabel="");
    /**< Guarda una imagen de pasada como argumento en la carpeta de im�genes tomadas con la c�mara, en el caso de tener actvada la opci�n en las preferencias, indicando en el nombre del archivo la fecha y hora de guardado.
      @param auxImage Puntero a la imagen que se va a guardar.
      @param extraLabel Etiqueta extra para el nombre del archivo.*/
};

#endif // MAINCONTROLLER_H
