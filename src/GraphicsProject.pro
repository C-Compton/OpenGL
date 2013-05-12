#-------------------------------------------------
#
# Project created by QtCreator 2013-03-24T02:34:07
#
#-------------------------------------------------

QT       += core gui opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets


TARGET = FinalProject
TEMPLATE = app

release: DESTDIR = Binaries/
debug:   DESTDIR = Binaries/

OBJECTS_DIR =  Qt/obj/
MOC_DIR =  Qt/moc/
RCC_DIR =  Qt/rcc/
UI_DIR =  Qt/ui/

SOURCES += \
        # Classes
        Classes/bipartiteGraph.cpp \
        Classes/Camera.cpp \
        Classes/Cameras.cpp \
        Classes/ColorFunctions.cpp \
        Classes/Engine.cpp \
        Classes/Light.cpp \
        # KinectInator.cpp \
        Classes/mat.cpp \
        Classes/Material.cpp \
        # not needed anymore Classes/meshMapping.cpp \
        Classes/Object.cpp \
        Classes/Particle.cpp \
        Classes/ParticleFieldFunctions.cpp \
        Classes/ParticleSystem.cpp \
        #Classes/scaleModel.cpp \
        Classes/Scene.cpp \
        Classes/Screen.cpp \
        Classes/SpelchkCamera.cpp \
        Classes/Texture.cpp \
        Classes/TextureMan.cpp \
        Classes/Timer.cpp \
        Classes/TransCache.cpp \
        Classes/Transformation.cpp \
        Classes/vec.cpp \
        Classes/MONOLITH.cpp \
        # Common
        Common/glut_callbacks.cpp \
        Common/InitShader.cpp \
        Common/model.cpp \
        Common/modelFunctions.cpp \
        Common/ObjLoader.cpp \
        Common/Animation.cpp \
        #Common/WiiUtil.cpp\
        Common/Util.cpp \
        #Qt
        Qt/mainwindow.cpp \
        Binaries/FinalProject.cpp \
        Classes/nearestNeighbor.cpp \
        Classes/scanMatching.cpp \
        Classes/UserVectorField.cpp \
        Qt/dialog.cpp \
        Classes/scaleModel.cpp \
        # FMOD STUFF {<>,<>}
        Common/soundHelper.cpp

HEADERS  += include/Animation.hpp \
            include/bipartiteGraph.hpp \
            include/Camera.hpp \
            include/Cameras.hpp \
            include/ColorFunctions.hpp \
            include/Engine.hpp \
            include/globals.h \
            include/glut_callbacks.h \
            include/InitShader.hpp \
            include/Light.hpp \
            #include/KinectInator.hpp\
            include/mat.hpp \
            include/Material.hpp \
            # not needed anymore include/meshMapping.hpp \
            include/modelFunctions.hpp \
            include/model.hpp \
            include/Object.hpp \
            include/ObjLoader.hpp \
            include/OpenGL.h \ 
            include/Particle.hpp \
	    include/nearestNeighbor.hpp \
	    include/scaleModel.hpp \
	    include/scanMatching.hpp \
            include/ParticleFieldFunctions.hpp \
            include/ParticleSystem.hpp \
            include/partTest.hpp \
            include/platform.h \ 
            include/Scene.hpp \ 
            include/Screen.hpp \ 
            include/SpelchkCamera.hpp \
            include/Texture.hpp \ 
            include/TextureMan.hpp \ 
            include/Timer.hpp \ 
            include/TransCache.hpp \ 
            include/Transformation.hpp \ 
            include/UserVectorField.hpp \
            include/Util.hpp \ 
            include/vec.hpp \ 
            include/WiiUtil.h \
            # QT Stuff >(^_^)>
            include/mainwindow.h \
            include/MONOLITH.hpp \
            include/dialog.h \
            # fMod Suff
            fMod/include/*.* \
            include/soundHelper.hpp

# Common Settings
INCLUDEPATH += . ./include/ ./fMod/include
LIBS += -lMagick++

# Linux Settings
unix:!macx {
LIBS += -lGL -lGLU -lGLEW -lglut -lX11 -lm -lMagick++ -lboost_thread-mt -lfmodex
INCLUDEPATH +=. /usr/include/ImageMagick/ include/ /usr/local/include/opencv
}

# Mac OSX Settings
macx {
LIBS += -framework Carbon -framework OpenGL -framework GLUT \
INCLUDEPATH += . include /opt/local/include/ImageMagick/ /opt/local/include/
LIBS += -L/opt/local/lib -lMagick++ -lboost_thread-mt fMod/lib/libfmodex.dylib
}

WITH_EXPRTK {
DEFINES += EXPRTK=true

} else {

}
FORMS    += \
    Qt/mainwindow.ui \
    Qt/dialog.ui
