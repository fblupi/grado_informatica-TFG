# 3DCurator

> Francisco Javier Bolívar Lupiáñez

### Descripción

Software para visualizar e interactuar con los datos DICOM de esculturas.

### Software utilizado

* CMake 3.4.1
* Visual Studio Community 2013
* Qt5.5.1
* VTK 6.3.0

### Instalación y configuración

#### Windows

##### Entorno de desarrollo

###### Visual Studio Community 2013

* Descargar Visual Studio Community 2013 desde su [web oficial](https://www.visualstudio.com/es-es/downloads/download-visual-studio-vs.aspx) e instalar.

###### Qt5.5.1

* Descargar Qt5.5.1 desde [este enlace](http://download.qt.io/official_releases/qt/5.5/5.5.1/qt-opensource-windows-x86-msvc2013-5.5.1.exe) de su web e instalar.
* Crear una nueva variable de entorno con nombre: `QTDIR` y valor: `C:\Qt\Qt5.5.1` (directorio raiz de la versión instalada).
* Agregar al Path la siguiente dirección: `C:\Qt\Qt5.5.1\5.5\msvc2013\bin`

###### CMake 3.4.1

* Descargar CMake 3.4.1 desde [este enlace](https://cmake.org/files/v3.4/cmake-3.4.1-win32-x86.exe) de su web e instalar (al instalar recomiendo marcar la opción de agregar al PATH de todos los usuarios para no tener que hacerlo manualmente).

##### Compilar librerías

###### Estructura de directorios. 

Recomiendo usar la siguiente estructura de directorios:

```
C:\
  |-- librería\
  |   |-- versión1\
  |   |   |-- src\
  |   |   |-- build\
  |   |       |-- plataforma1\
  |   |       |-- plataforma2\
  |   |       |-- plataforman\
  |   |
  |   |-- versión2\
  |       |-- src\
  |       |-- build\
  |           |-- plataforma1\
  |           |-- plataforma2\
  |           |-- plataforman\
  |
  |-- librería\
  |   |-- versión\
  |       |-- src\
  |       |-- build\
  |           |-- plataforma1\
  |           |-- plataforma2\
  |           |-- plataforman\
  |-- ...
```

De esta forma es más fácil organizar versiones para una u otra plataforma de distintas versiones de la librería.

###### VTK 6.3.0

* Descargar VTK 6.3.0 desde [este enlace](http://www.vtk.org/files/release/6.3/VTK-6.3.0.zip) de su web oficial.
* Abrir CMake y completar:
  + src: `C:\VTK\6.3.0\src`
  + build: `C:\VTK\6.3.0\src\build\vs12`
* Elegir como generador `Visual Studio 12 2013`.
* Presionar en configurar.
* Una vez haya generado seleccionar los siguientes campos:
  + `BUILD_SHARED_LIBS`
  + `Module_vtkGUISupportQt`
  + `Module_vtkGUISupportQtOpenGL`
  + `Module_vtkGUISupportQtSQL`
  + `Module_vtkGUISupportQtWebkit`
  + `Module_vtkRenderingQt`
  + `Module_vtkViewsQt`
  + `VTK_Group_Qt`
* Agregar dos entradas:
  + `QT_QMAKE_EXECUTABLE:PATHFILE=C:/Qt/Qt5.5.1/5.5/msvc2013/bin/qmake.exe`
  + `CMAKE_PREFIX_PATH:PATH=C:/Qt/Qt5.5.1/5.5/msvc2013/`
* Presionar en configurar y aparecerá un error, habrá que elegir como versión de Qt la 5. Elegirla y volver a configurar.
* Configurar hasta que no aparezca ningún campo en rojo.
* Una vez configurado todo, pulsar en generar. Esto creará una serie de archivos en `C:\VTK\6.3.0\build\vs12`.
* Abrir `VTK.sln`.
* Construir en modo *Release* y esperar unos minutos a que termine (en mi caso tardó unos 20 minutos).
* Copiar los archivos `QVTKWidgetPlugin.lib` y `QVTKWidgetPlugin.dll` que se encuentran en `C:\VTK\6.3.0\build\vs12\lib\Release` y  `C:\VTK\6.3.0\build\vs12\bin\Release` respectivamente en `C:\Qt\Qt5.5.1\5.5\msvc2013\plugins\designer` (Si no se encuentran los archivos, comprobar que en CMake se marcó la opción `BUILD_SHARED_LIBS`) . Esto hará que desde Qt Designer se pueda crea un `QVTKWidget`.´
* Construir en modo *Debug*.
* Crear una nueva variable de entorno con nombre: `VTK_DIR` y valor: `C:\VTK\6.3.0\build\vs12`.
* Agregar al Path la siguiente dirección: `C:\VTK\6.3.0\build\vs12\bin\Release`

#### Linux

##### Paquetes

Instalar los siguientes paquetes sin no están instalados:

* Instalar compilador g++:
```
sudo apt-get install build-essential
```
* Instalar CMake y CCMake:
```
sudo apt-get install cmake cmake-curses-gui
```
* Instalar OpenGL:
```
sudo apt-get install freeglut3-dev
```

##### Instalar Qt5.5.1

* Descargar Qt desde [este enlace](http://download.qt.io/official_releases/qt/5.5/5.5.1/qt-opensource-linux-x64-5.5.1.run).
* Dar permisos de ejecución al ejecutable descargado:
```
chmod +x qt-opensource-linux-x64-5.5.1.run
```
y lanzarlo como administrador:
```
sudo ./qt-opensource-linux-x64-5.5.1.run
```
* Instalar en `/opt/Qt5.5.1`.
* Una vez instalado añadir la siguiente línea al archivo `.bashrc` que se encuentra en el home:
```
PATH=.:/opt/Qt5.4.1/5.4/gcc_64/bin:$PATH
```

##### Instalar VTK6.3.0

* Descargar VTK desde [este enlace](http://www.vtk.org/files/release/6.3/VTK-6.3.0.tar.gz).
* Extraer, por ejemplo, en el directorio de descargas del usuario.
* Con permisos de superusuario, crear un directorio para compilar la librería y compilarla con `ccmake` habilitando las siguientes opciones `BUILD_SHARED_LIBS`, `VTK_Group_Rendering`, y `VTK_Group_StandAlone`.
```
sudo su
mkdir /opt/VTK-build
cd /opt/VTK-build
ccmake /home/me/Descargas/VTK-6.3.0
```
  + Para compilar pulsar "c" para configurar y "g" para generar una vez se hayan configurado todas las opciones.
* Crear un directorio *release* y generar el Makefile:
```
mkdir /opt/VTK6.3
cd /opt/VTK6.3
cmake -DCMAKE_BUILD_TYPE:STRING=Release /home/me/Descargas/VTK-6.3.0
```
* Integrar Qt:
```
cd /opt/VTK6.3
cmake -DVTK_QT_VERSION:STRING=5 \
      -DQT_QMAKE_EXECUTABLE:PATH=/opt/Qt5.5.1/5.5/gcc_64/bin/qmake \
      -DVTK_Group_Qt:BOOL=ON \
      -DCMAKE_PREFIX_PATH:PATH=/opt/Qt5.5.1/5.5/gcc_64/lib/cmake \
      -DBUILD_SHARED_LIBS:BOOL=ON /opt/VTK6.3
```
* Una vez generado terminar ejecutando:
```
make -j4
make install
```
  + Donde el número que acompaña a "j" es el número de procesadores que actuen en paralelo.
