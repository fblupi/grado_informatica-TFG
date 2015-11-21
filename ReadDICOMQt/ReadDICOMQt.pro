QT += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ReadDICOMQt
TEMPLATE = app


SOURCES +=\
    ReadDICOMQt.cxx \
    main.cxx

HEADERS += ReadDICOMQt.h

FORMS += ReadDICOMQt.ui

QMAKE_CXXFLAGS += -Wno-deprecated

INCLUDEPATH += /usr/local/include/vtk-6.3

LIBS +=\
    -L/usr/local/lib \
    -ldl \
    -lGL \
    -lvtkalglib \
    -lvtkjsoncpp \
    -lvtksys \
    -lvtkzlib \
    -lvtkalglib \
    -lvtkChartsCore \
    -lvtkCommonColor \
    -lvtkCommonComputationalGeometry \
    -lvtkCommonCore \
    -lvtkCommonDataModel \
    -lvtkCommonExecutionModel \
    -lvtkCommonMath \
    -lvtkCommonMisc \
    -lvtkCommonSystem \
    -lvtkCommonTransforms \
    -lvtkDICOMParser \
    -lvtkDomainsChemistry \
    -lvtkexoIIc \
    -lvtkexpat \
    -lvtkFiltersAMR \
    -lvtkFiltersCore \
    -lvtkFiltersExtraction \
    -lvtkFiltersFlowPaths \
    -lvtkFiltersGeneral \
    -lvtkFiltersGeneric \
    -lvtkFiltersGeometry \
    -lvtkFiltersHybrid \
    -lvtkFiltersHyperTree \
    -lvtkFiltersImaging \
    -lvtkFiltersModeling \
    -lvtkFiltersParallelImaging \
    -lvtkFiltersParallel \
    -lvtkFiltersProgrammable \
    -lvtkFiltersSelection \
    -lvtkFiltersSMP \
    -lvtkFiltersSources \
    -lvtkFiltersStatistics \
    -lvtkFiltersTexture \
    -lvtkFiltersVerdict \
    -lvtkfreetype \
    -lvtkftgl \
    -lvtkGeovisCore \
    -lvtkgl2ps \
    -lvtkGUISupportQtOpenGL \
    -lvtkGUISupportQt \
    -lvtkGUISupportQtSQL \
    -lvtkGUISupportQtWebkit \
    -lvtkhdf5_hl \
    -lvtkhdf5 \
    -lvtkImagingColor \
    -lvtkImagingCore \
    -lvtkImagingFourier \
    -lvtkImagingGeneral \
    -lvtkImagingHybrid \
    -lvtkImagingMath \
    -lvtkImagingMorphological \
    -lvtkImagingSources \
    -lvtkImagingStatistics \
    -lvtkImagingStencil \
    -lvtkInfovisCore \
    -lvtkInfovisLayout \
    -lvtkInteractionImage \
    -lvtkInteractionStyle \
    -lvtkInteractionWidgets \
    -lvtkIOAMR \
    -lvtkIOCore \
    -lvtkIOEnSight \
    -lvtkIOExodus \
    -lvtkIOExport \
    -lvtkIOGeometry \
    -lvtkIOImage \
    -lvtkIOImport \
    -lvtkIOInfovis \
    -lvtkIOLegacy \
    -lvtkIOLSDyna \
    -lvtkIOMINC \
    -lvtkIOMovie \
    -lvtkIONetCDF \
    -lvtkIOParallel \
    -lvtkIOParallelXML \
    -lvtkIOPLY \
    -lvtkIOSQL \
    -lvtkIOVideo \
    -lvtkIOXMLParser \
    -lvtkIOXML \
    -lvtkjpeg \
    -lvtkjsoncpp \
    -lvtklibxml2 \
    -lvtkmetaio \
    -lvtkNetCDF_cxx \
    -lvtkNetCDF \
    -lvtkoggtheora \
    -lvtkParallelCore \
    -lvtkpng \
    -lvtkproj4 \
    -lvtkRenderingAnnotation \
    -lvtkRenderingContext2D \
    -lvtkRenderingContextOpenGL \
    -lvtkRenderingCore \
    -lvtkRenderingFreeType \
    -lvtkRenderingGL2PS \
    -lvtkRenderingImage \
    -lvtkRenderingLabel \
    -lvtkRenderingLIC \
    -lvtkRenderingLOD \
    -lvtkRenderingOpenGL \
    -lvtkRenderingQt \
    -lvtkRenderingVolumeOpenGL \
    -lvtkRenderingVolume \
    -lvtksqlite \
    -lvtksys \
    -lvtktiff \
    -lvtkverdict \
    -lvtkViewsContext2D \
    -lvtkViewsCore \
    -lvtkViewsInfovis \
    -lvtkViewsQt \
    -lvtkzlib

