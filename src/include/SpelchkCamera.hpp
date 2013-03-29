/*
 * Camera.h
 *
 *  Created on: Dec 1, 2012
 *      Author: Hoanh Nguyen
 */

#ifndef CAMERA_H_
#define CAMERA_H_

#include <cmath>
#include "mat.hpp"
#include "vec.hpp"
using namespace Angel;

class SpelchkCamera {
private:
  int _projectionType;

  // Projection transformation parameters
  GLfloat _fovy;  // Field-of-view in Y direction angle (in degrees)
  GLfloat _aspect;       // Viewport aspect ratio
  
  GLfloat _left, _right;
  GLfloat _bottom, _top;
  GLfloat _zNear, _zFar;

  GLuint _timeRef;

  int _screenWidth, _screenHeight;

  GLfloat _xDepth, _yDepth, _zDepth;
  GLfloat _xAngle, _yAngle, _zAngle;
  GLfloat _xHead, _yHead, _zHead;
  float _xHeadStart, _yHeadStart, _zHeadStart;
  GLfloat _xHeadAngle, _yHeadAngle, _zHeadAngle;

  vec4 _initialTranslationVector;
  vec4 _translationVector;
  vec4 _oldTranslationVector;

  mat4 _modelViewMatrix;

  int _inboundHeadData;
  vec4 _initialHeadPosition;

  void calculateTranslationVector();

public:
  SpelchkCamera( vec4 initialTranslationVector );
  virtual ~SpelchkCamera();

  mat4 getProjectionMatrix();
  mat4 getModelViewMatrix();
  vec4 getTranslationVector();

  // Displaces camera from its current location
  void moveCamera( float xDepth, float yDepth, float zDepth );

  // Rotate camera from its current orientation
  void rotateCamera( float xAngle, float yAngle, float zAngle );

  void setScreenSize( int width, int height );
  void setProjection( int projectionType );

  void reset();

  void setLightMovementRef( GLuint ref );
  void setLightMovementTime( float elapsed );

  void getReadyForZero( int usernum );
  void headMovement( int usernum, double x, double y, double z );
};

#endif /* CAMERA_H_ */
