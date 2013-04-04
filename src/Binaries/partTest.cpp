/**
 @file partTest.cpp
 @author Nicholas St.Pierre
 @authors John Huston, Nicholas VerVoort, Chris Compton
 @date 2013-02-23
 @brief This is a derivative of our main project file, fly.cpp.
 @details This is a tech demo for a particle system
 */

#include "Engine.hpp"
#include "ParticleSystem.hpp"
/* Utilities and Common */
#include "glut_callbacks.h"
#include "InitShader.hpp"
#include "model.hpp"
/* System Headers */
#include <cmath>
#include <cstdio>
#include <sstream>
#include <cstdlib>
#include <time.h>

// Type Aliases
using Angel::vec3;
using Angel::vec4;
typedef Angel::vec4 Color4;
typedef Angel::vec4 Point4;

// Global objects for magical camera success
Screen myScreen( 800, 600 );
bool fixed_yaw = true;

// Initialization: load and compile shaders, initialize camera(s), load models.
void init() {
  
  GLuint shader;
  //GLuint testShader;
  Screen *primScreen = Engine::instance()->mainScreen();
  Scene *rootScene = Engine::instance()->rootScene();
  
  // Load shaders and use the resulting shader program. 
  shader = Angel::InitShader( "shaders/vParticle.glsl",
                              "shaders/fParticle.glsl",
                              "shaders/gParticle.glsl" );
  
  rootScene->shader(shader);
  primScreen->_camList.shader(shader);

  primScreen->_camList.addCamera( "Camera1" );
  primScreen->_camList.next();
  //camList->active()->changePerspective( Camera::IDENTITY );

  //  Object *testObj = rootScene->AddObject("testObj");
  //  loadModelFromFile(testObj, "../models/bottle-a.obj");

  //  testObj->Buffer();
  //  testObj->trans.scale.Set(1);
  //  testObj->trans.offset.SetY(5);
  
  Object *particleSystem = new ParticleSystem( 5, "ParticleSystem", shader );
  rootScene->insertObject( particleSystem );
  particleSystem->buffer();

  // Generic OpenGL setup: Enable the depth buffer and set a nice background color.
  glEnable( GL_DEPTH_TEST );
  glClearColor( 0.3, 0.5, 0.9, 1.0 );
  
}

void cleanup( void ) {
  //Engine::instance()->rootScene()->DestroyObject();
}

//--------------------------------------------------------------------

void draw( void ) {
  static Scene *theScene = Engine::instance()->rootScene();
  static Cameras *camList = Engine::instance()->cams();
  
  theScene->draw();
  camList->draw();
}

// GLUT display callback. Effectively calls displayViewport per-each Camera.
void display( void ) {
  static Cameras *camList = Engine::instance()->cams();
  
  // Clear the buffer.
  glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
  
  // Tell camList to draw using our 'draw' rendering function.
  camList->view( draw );
  
  // Swap to the next buffer.
  glutSwapBuffers();
  
}

void idle( void ) {
  static Cameras *camList = Engine::instance()->cams();
  
  // Compute the time since last idle().
  tick.tock();
  
  // Move all camera(s).
  camList->idleMotion();
  glutPostRedisplay();
}

int main( int argc, char **argv ) {
  
  // OS X suppresses events after mouse warp.  This resets the suppression 
  // interval to 0 so that events will not be suppressed. This also found
  // at http://stackoverflow.com/questions/728049/
  // glutpassivemotionfunc-and-glutwarpmousepointer
#ifdef __APPLE__
  CGSetLocalEventsSuppressionInterval( 0.0 );
#endif
  Util::InitRelativePaths(argc, argv);
  

  glutInit( &argc, argv );
  glutInitDisplayMode( GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH );
  glutInitWindowSize( myScreen.width(), myScreen.height() );
  glutCreateWindow( "Particle Test" );
  //  glutFullScreen();
  glutSetCursor( GLUT_CURSOR_NONE );
  
  GLEW_INIT();
  init();
  
  /* Register our Callbacks */
  glutDisplayFunc( display );
  glutKeyboardFunc( engineKeyboard );
  glutKeyboardUpFunc( engineKeylift );
  glutSpecialFunc( engineSpecialKeyboard );
  glutMouseFunc( engineMouse );
  glutMotionFunc( engineMouseMotion );
  glutPassiveMotionFunc( EngineMousePassive );
  glutIdleFunc( idle );
  glutReshapeFunc( engineResize );
  
  /* PULL THE TRIGGER */
  glutMainLoop();
  return EXIT_SUCCESS;
  
}
