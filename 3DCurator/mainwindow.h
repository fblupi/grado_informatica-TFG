#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QFile>
#include <QProgressDialog>

#include <sstream>

#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkInteractorStyleTrackballCamera.h>
#include <vtkImageViewer2.h>
#include <vtkSmartPointer.h>
#include <vtkImageMapToWindowLevelColors.h>
#include <vtkLookupTable.h>
#include <vtkWindowToImageFilter.h>
#include <vtkImageWriter.h>
#include <vtkPNGWriter.h>
#include <vtkJPEGWriter.h>
#include <vtkDistanceWidget.h>
#include <vtkDistanceRepresentation.h>

#include "utils.h"
#include "figura.h"
#include "plano.h"
#include "interactorStyleImage.h"
#include "interactorStyleDeleter.h"
#include "colorTFChart.h"
#include "opacityTFChart.h"

#define MIN_INTENSITY -1024.0 /**< Valor m�nimo de intensidad que podr�a tener una imagen DICOM */
#define MAX_INTENSITY 3071.0 /**< Valor m�ximo de intensidad que podr�a tener una imagen DICOM */

namespace Ui {
    class MainWindow;
}

/**
* @class MainWIndow
* Clase con la ventana principal
*/
class MainWindow : public QMainWindow {
    Q_OBJECT

public:
	/**
	 * Constructor
	 */
	explicit MainWindow(QWidget *parent = 0);

	/*
	 * Destructor
	 */
    ~MainWindow();

private slots:
	// eventos GUI
	void on_actionExit_triggered();
	void on_actionExportSliceImage_triggered();
	void on_actionExportVolumeImage_triggered();
	void on_actionExportPreset_triggered();
	void on_actionImportPreset_triggered();
	void on_actionOpenDICOM_triggered();
	void on_actionDelete_triggered();

	void on_axialPlane_pressed();
	void on_coronalPlane_pressed();
	void on_sagitalPlane_pressed();
	void on_exportSliceImage_pressed();
	void on_exportSliceImageAs_pressed();
	void on_importPreset_pressed();
	void on_exportPreset_pressed();
	void on_exportPresetAs_pressed();
	void on_restoreMaterial_pressed();
	void on_updateProperties_pressed();
	void on_completePreset_pressed();
	void on_woodPreset_pressed();
	void on_stuccoPreset_pressed();
	void on_metalPreset_pressed();
	void on_updateMesh_pressed();

	void on_enablePlane_stateChanged();
	void on_enableRule_stateChanged();

	void on_colorTFMaxSlider_valueChanged();
	void on_colorTFMinSlider_valueChanged();
	void on_gradientTFMaxSlider_valueChanged();
	void on_gradientTFMinSlider_valueChanged();
	void on_scalarTFMaxSlider_valueChanged();
	void on_scalarTFMinSlider_valueChanged();

	/**
	 * Establece un color a un Renderer
	 * @param	ren Renderer al que se le quiere cambiar el color
	 * @param	r	Componente roja
	 * @param	g	Componente verde
	 * @param	b	Componente azul
	 */
	void setBackgroundColor(vtkSmartPointer<vtkRenderer> ren, float r, float g, float b);

	/**
	 * Conecta los componentes de VTK con las interfaces
	 */
	void connectComponents();

	/**
	 * A�ade el volumen a su respectivo visor
	 */
	void drawVolume();

	/**
	 * A�ade la malla a su respectivo visor
	 */
	void drawMesh();

	/**
	 * Elimina el volumen de su respectivo visor
	 */
	void removeVolume();

	/**
	 * Elimina la malla de su respectivo visor
	 */
	void removeMesh();

	/**
	 * Renderiza la ventana del volumen y el plano
	 */
	void renderVolume();

	/**
	* Renderiza la ventana de la malla
	*/
	void renderMesh();

	/**
	 * Renderiza la ventana del corte producidor por el plano en el volumen
	 */
	void renderSlice();

	/**
	 * Asigna una funci�n de trasnferencia por defecto
	 */
	void defaultTF();

	/**
	 * Asigna un material por defecto
	 */
	void defaultMaterial();

	/**
	 * Coloca el plano en una posici�n por defecto
	 */
	void defaultPlanePosition();

	/**
	 * Actualiza si el volumen aparece o no sombreado
	 */
	void updateShadow();

	/**
	 * Actualiza el material
	 */
	void updateMaterial();

	/**
	 * Actualiza el modo de renderizado
	 */
	void updateRenderMode();

	/**
	 * Actualiza los sliders d�ndole los valores seg�n la funci�n de transferencia
	 */
	void updateSliders();

	/**
	 * Importa archivos DICOM para representarlos
	 */
	void importDICOM();

	/**
	 * Importa un preset de una funci�n de transferencia
	 */
	void importPreset();

	/**
	 * Carga un preset de los que por defecto incluye el programa
	 * @param	file	Archivo con el preset que se va a cargar
	 */
	void loadDefaultPreset(QFile *file);

	/**
	 * Exporta una imagen de un Render Window
	 * @param	renWin		RenderWindow del que se quiere extraer una imagen
	 * @param	filename	Nombre del archivo de salida
	 */
	void exportImageFromRenderWindow(vtkSmartPointer<vtkRenderWindow> renWin, const QString filename);

	/**
	 * Exporta el preset
	 * @param	filename	Nombre del archivo de salida
	 */
	void exportPreset(const QString filename);

	/**
	 * Obtiene el nombre del archivo de salida para exportar una imagen
	 * @param	defaultFilename		Nombre que se dar� por defecto (fecha y hora)
	 * @return	Archivo de salida
	 */
	QString getExportImageFilename(const QString defaultFilename);

	/**
	 * Obtiene el nombre del archivo de salida para exportar un preset
	 * @param	defaultFilename		Nombre que se dar� por defecto (nombre de la funci�n de transferencia)
	 * @return	Archivo de salida
	 */
	QString getExportPresetFilename(const QString defaultFilename);

private:
	Ui::MainWindow *ui; /**< Puntero a la interfaz gr�fica */
	Figura *figura; /**< Puntero a la figura */
	Plano *plano; /**< Puntero al plano */
	ColorTFChart *colorTFChart; /**< Puntero a la gr�fica de la parte de color de la funci�n de transferencia */
	OpacityTFChart *scalarTFChart; /**< Puntero a la gr�fica de la parte de opacida escalar de la funci�n de transferencia */
	OpacityTFChart *gradientTFChart; /**< Puntero a la gr�fica de la parte de opacida gradiente de la funci�n de transferencia */
	vtkSmartPointer<vtkRenderer> volumeRen; /**< Puntero al Renderer donde estar� el volumen y el plano de corte */
	vtkSmartPointer<vtkRenderer> meshRen; /**< Puntero al Renderer donde se visulizar� la malla a extraer */
	vtkSmartPointer<vtkImageViewer2> sliceViewer; /**< Puntero al ImageViewer2 donde se ver� el corte producido por el plano en el volumen */
	vtkSmartPointer<vtkInteractorStyleTrackballCamera> volumeStyle; /**< Estilo para la ventana donde se visualizar� el volumen */
	vtkSmartPointer<InteractorStyleImage> sliceStyle; /**< Estilo para la ventana donde se visualizar�n los cortes de la figura con el plano */
	vtkSmartPointer<InteractorStyleDeleter> deleterStyle; /**< Estilo para poder borrar en la ventana donde se visualizar� el volumen */
	vtkSmartPointer<vtkWindowToImageFilter> filter;  /**< Filtro para pasar de un RenderWindow a una imagen */
	vtkSmartPointer<vtkImageWriter> writer; /**< Exportador de imagen */
	vtkSmartPointer<vtkDistanceWidget> distanceWidget; /**< Widget para realizar medidas */
	bool deleting; /**< Variable para ver si est� en modo borrado o no */
};

#endif // MAINWINDOW_H
