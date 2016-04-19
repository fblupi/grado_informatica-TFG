#ifndef FIGURA_H
#define FIGURA_H

#include <vtkAlgorithm.h>
#include <vtkColorTransferFunction.h>
#include <vtkDICOMImageReader.h>
#include <vtkSmartVolumeMapper.h>
#include <vtkImageData.h>
#include <vtkPiecewiseFunction.h>
#include <vtkSmartPointer.h>
#include <vtkVolume.h>
#include <vtkVolumeProperty.h>
#include <vtkPointData.h>
#include <vtkDataArray.h>
#include <vtkImageAccumulate.h>

#include "transferFunction.h"

/**
 * @class Figura
 * Clase que tiene el volumen que se visualizará
 */
class Figura {
public:
	/**
	 * Constructor
	 */
    Figura();

	/*
	 * Destructor
	 */
    ~Figura();

	/**
	 * Obtiene la función de transferencia
	 * @return	Puntero a la función de transferencia
	 */
	TransferFunction *getTransferFunction() const;

	/**
	 * Obtiene el volumen
	 * @return	Puntero al volumen
	 */
	vtkSmartPointer<vtkVolume> getVolume() const;

	/**
	 * Obtiene el lector
	 * @return	Puntero al lector de datos
	 */
	vtkSmartPointer<vtkAlgorithm> getReader() const;

	/** 
	 * Obtiene el histograma de frecuencias de valores de intensidad
	 * @return	Puntero al histograma
	 */
	vtkSmartPointer<vtkImageAccumulate> getHistogram() const;

	/**
	 * Obtiene el límite inferior de la figura en el eje X
	 * @return	Límite
	 */
	double getMinXBound() const;

	/**
	 * Obtiene el límite inferior de la figura en el eje Y
	 * @return	Límite
	 */
	double getMinYBound() const;

	/**
	 * Obtiene el límite inferior de la figura en el eje Z
	 * @return	Límite
	 */
	double getMinZBound() const;

	/**
	 * Obtiene el límite superior de la figura en el eje X
	 * @return	Límite
	 */
	double getMaxXBound() const;

	/**
	 * Obtiene el límite superior de la figura en el eje Y
	 * @return	Límite
	 */
	double getMaxYBound() const;

	/**
	 * Obtiene el límite superior de la figura en el eje Z
	 * @return	Límite
	 */
	double getMaxZBound() const;

	/**
	 * Establece la carpeta de donde se leerán los archivos DICOM
	 * @param	s	Carpeta con los datos DICOM
	 */
	void setDICOMFolder(const std::string s);

	/**
	 * Borra todos los puntos de todas las partes de la función de transferencia
	 */
	void removeTFPoints();

	/**
	 * Añade un punto en la parte de opacidad escalar de la función de transfencia
	 * @param	value	Valor de densidad
	 * @param	alpha	Opacidad
	 */
	void addScalarPoint(const double value, const double alpha);

	/**
	 * Añade un punto en la parte de opacidad gradiente de la función de transfencia
	 * @param	value	Valor de gradiente
	 * @param	alpha	Opacidad
	 */
	void addGradientPoint(const double value, const double alpha);

	/**
	 * Añade un punto en la parte de color de la función de transfencia
	 * @param	value	Valor de densidad
	 * @param	r		Componente roja
	 * @param	g		Componente verde
	 * @param	b		Componente azul
	 */
	void addRGBPoint(const double value, const double r, const double g, const double b);

	/**
	 * Habilita o deshabilita la sombra en el volumen
	 * @param	onOff	Sombra activada si true o desactivada si false
	 */
	void enableShadow(const bool onOff);

	/**
	 * Asigna el material al volumen
	 * @param	ambient		Componente ambiental
	 * @param	diffuse		Componente difusa
	 * @param	specular	Componente especular
	 * @param	power		Potencia especular
	 */
	void setMaterial(const double ambient, const double diffuse, const double specular, const double power);

	/**
	 * Establece el modo de renderizado
	 * @param	mode	Si es 0 usa GPU, si es 1 usa CPU
	 */
	void setRenderMode(const int mode);

private:
	vtkSmartPointer<vtkDICOMImageReader> imageReader; /**< Lector de datos DICOM */
	vtkSmartPointer<vtkAlgorithm> reader; /**< Lector necesario para obtener el puerto de salida */
    vtkSmartPointer<vtkVolume> volume;  /**< Volumen con la figura */
	vtkSmartPointer<vtkSmartVolumeMapper> mapper;  /**< Mapeador que usa DVR GPU-Raycasting */
    vtkSmartPointer<vtkVolumeProperty> volumeProperty;  /**< Propiedades del volumen */
	vtkSmartPointer<vtkImageAccumulate> histogram; /**< Histograma de frecuencias de valores de intensidad */
	TransferFunction *tf;  /**< Función de transferencia */

    void setProperties(); // establace las propiedades del volumen
};

#endif // FIGURA_H
