# 3DCurator

> Francisco Javier Bolívar Lupiáñez

### Descripción

Software para visualizar e interactuar con los datos DICOM de esculturas.

### Instalación y configuración

#### Paquetes

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

#### Instalar Qt5.5.1

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

#### Instalar VTK6.3.0

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
