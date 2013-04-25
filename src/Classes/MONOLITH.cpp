/**
 @file MONOLITH.cpp
 @author UMLComputerGraphics <https://github.com/UMLComputerGraphics>
 @date 2013-03-29
 @brief This is a monolith of every component.
 @details Final project rough draft.
 Original engine based on Ed Angel's book code.
 **/

#include "MONOLITH.hpp"

MONOLITH::~MONOLITH( void ) {
  
}

/* Default and only constructor */
MONOLITH::MONOLITH( int argc, char** argv ) {
  _argc = argc;
  _argv = argv;
}

/**
 * Cleans up our scene graph.
 */
void MONOLITH::cleanup( void ) {
  Engine::instance()->rootScene()->delObject();
}

/**
 * Apply animations and whatever else your heart desires.
 */
void MONOLITH::monolith_idle( void ) {
  static Scene *rootScene = Engine::instance()->rootScene();
  
  // Animation variables.
  double timer = glutGet( GLUT_ELAPSED_TIME ) / 500.0;
  float percent = (sin( timer ) + 1) / 2;

  //(*rootScene)["candle_top"]->morphPercentage(percent);
  
  /*
  Object &candle_base = *((*rootScene)["candle_base"]);
  Object &candle_top = *((*rootScene)["candle_top"]);
  candle_base.animation( candleMeltAnim );
  candle_top.animation(candleTopMeltDown);
  */
  
  // Update the morph percentage.
  // (*rootScene)["bottle"]->morphPercentage( percent );
}

#ifndef WITHOUT_QT
/**
 * Implementation of all slots and sighnals.
 * Qt-specific
 */
void MONOLITH::slotParticleAdd(int value) {
  ps->addSomeParticles(10);
}
#endif //WITHOUT_QT
/**
 * This will initialize and run MONOLITH
 */
void MONOLITH::run() {
  Engine::instance()->init( &_argc, _argv,
                            "WE ARE THE BORG. RESISTANCE IS FUTILE!" );
  Engine::instance()->registerIdle( monolith_idle );

  
  // Get handles to the Scene and the Screen.
  rootScene = Engine::instance()->rootScene();
  primScreen = Engine::instance()->mainScreen();
  
  shader[0] = Angel::InitShader( "shaders/vEngine.glsl",
                                 "shaders/fMONOLITH.glsl" );
  shader[1] = rootScene->shader(); // Default shader we've already compiled.
  shader[2] = Angel::InitShader( "shaders/vParticle.glsl",
                                 "shaders/fFlameParticle.glsl" );
  

  tick.setTimeUniform( glGetUniformLocation( shader[1], "ftime" ) );

  // --- Wine Bottle --- //
  
  // Create the Bottle Object handle...
  bottle = rootScene->addObject( "bottle" );

  // Load model from file.
  ObjLoader::loadModelFromFile( bottle, "../models/bottle_wine_high.obj" );
  ObjLoader::loadMaterialFromFile( bottle, "../models/bottle_wine_high.mtl" );

  // Scale the bottle down!
  //bottle->_trans._scale.set( 0.30 );
  vec4 min = bottle->getMin();
  bottle->_trans._offset.set( 0, (0 - min.y), 0 );
  bottle->buffer();
  bottle->propagateOLD();

  // this obscure allusion to "the thong song" brought to you by Eric McCann
  GLuint sisqo = glGetUniformLocation( bottle->shader(),
				       "letMeSeeThatPhong" );
  glUniform1f( sisqo, true );
  
  // Let the bodies hit the floor
  Object *table;
  table = rootScene->addObject( "table", shader[1] );
  ObjLoader::loadModelFromFile(table, "../models/table.obj");
  ObjLoader::loadMaterialFromFile(table, "../models/table.mtl");
  table->buffer();
  
  // Load up that goddamned candle
  Object *stick;
  Object *candle_top;
  Object *candle_top_melted;
  Object *candle_base;

  candle_top = rootScene->addObject( "candle_top", shader[1] );
  candle_base = rootScene->addObject("candle_base", shader[1]);
  stick = rootScene->addObject("stick", shader[1]);

  ObjLoader::loadModelFromFile( candle_top, "../models/candle_top_unmelted.obj" );
  ObjLoader::loadMaterialFromFile( candle_top, "../models/candle.mtl" );
  ObjLoader::loadModelFromFile(candle_base, "../models/candle_bottom.obj");
  ObjLoader::loadMaterialFromFile( candle_base, "../models/candle.mtl" );
  ObjLoader::loadModelFromFile(stick, "../models/candlestick.obj");
  ObjLoader::loadMaterialFromFile(stick, "../models/candlestick.mtl");

/*
  candle_top->genMorphTarget();
  candle_top_melted = candle_top->morphTarget();
  ObjLoader::loadModelFromFile(candle_top_melted, "../models/candle_top_melted.obj");
  ObjLoader::loadMaterialFromFile(candle_top_melted, "../models/candle.mtl");
  */

  min = candle_base->getMin();
  vec4 max = stick->getMax();
  fprintf( stderr, "Min: (%f,%f,%f)\nMax: (%f,%f,%f)\n", min.x, min.y, min.z,
           max.x, max.y, max.z );
  candle_top->_trans._offset.set( 2.5, max.y - 4, 2.5 );
  candle_base->_trans._offset.set( 2.5, max.y - 4, 2.5 );
  stick->_trans._offset.set( 2.5, 0, 2.5);

  candle_top->propagateOLD();
  candle_base->propagateOLD();
  stick->propagateOLD();

  candle_top->buffer();
  candle_base->buffer();
  stick->buffer();

  
  max = candle_top->getMax();
  ps = new ParticleSystem( 10, "ps1", shader[2] );
  ps->setLifespan( 5, 7.5 );
  ps->setEmitterRadius( 0.001 );
  candle_top->insertObject( ps );
  ps->_trans._offset.set( 0, max.y, 0 );
  ps->fillSystemWithParticles();
  //ps->propagateOLD();
  ps->buffer();
  candle_top->propagateOLD();

  Engine::instance()->cams()->active()->pos(2.0, 5.0, 9.0);
  
#ifndef WITHOUT_QT
#ifndef __APPLE__
  printf("LOOPING!\n");
  glutMainLoop(); // on Linux the glutMaintLoop is called here when using Qt
#endif
#else
  printf("LOOPING!\n");
  glutMainLoop(); // if we are not using Qt, the glutMainLoop is called here for both platforms.
#endif

}

/**
 * A simple animation callback.
 * Rotates the object about its Y axis,
 * as it orbits the object around a point about the Y axis.
 * @param obj The object to animate.
 */

void MONOLITH::simpleRotateAnim( TransCache &obj ) {
  obj._rotation.rotateY( tick.scale() * 1.5 );
  obj._orbit.rotateY( tick.scale() * -0.5 );
}

void MONOLITH::candleMeltAnim(TransCache &obj) {
  //obj._offset.set(0.0, 0.0, 0.0);
  obj._scale.adjust(1.0, 0.999, 1.0);
}

void MONOLITH::candleTopMeltDown(TransCache &obj) {
 // obj._offset.delta(0.0, -0.0025, 0.0);
}
