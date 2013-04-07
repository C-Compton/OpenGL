/**
 * @file ParticleSystem.hpp
 * @author Chris Compton <christopher_compton@student.uml.edu>
 * @date 2013-02-24
 * @brief TODO: Documentation from CC/NVV
 * @details TODO: Documentation from CC/NVV
 */

#ifndef __PARTICLESYSTEM_H
#define __PARTICLESYSTEM_H

#include <string>
#include <vector>
#include "mat.hpp"
#include "Object.hpp"
#include "Particle.hpp"
#include "Camera.hpp"
#include "Engine.hpp"
#include "vec.hpp"

using Angel::vec2;
using Angel::vec3;
using Angel::vec4;
using Angel::mat4;
using std::string;
using std::vector;

typedef Particle* ParticleP;

class ParticleSystem : public Object {
  
public:
  
  ParticleSystem( int particleAmt, const std::string &name, GLuint shader );
  ~ParticleSystem( void );

  void addParticles( void );

  // Getters and Setters
  vec4 getColor( void );
  float getLifespan( void );
  float getMaxLife( void );
  float getMinLife( void );
  int getNumParticles( void );

  void setColor( vec4 newColor );
  void setLifespan( float minLifespan, float maxLifespan );
  void setNumParticles( int newNumParticles );

  /* The Do-All function.  Will do everything needed in order
   to have the particles behave according to our specification
   on each call to Draw(). That's the idea, anyway. */
  void update();

  virtual void buffer( void );
  virtual void draw( void );

//protected:
  
private:  

  float rangeRandom( float min, float max );

  vector<ParticleP> particles;
  int    numParticles;   // Number of particles that each instance of ParticleSystem will manage
  float  minLife;
  float  maxLife;

  //vec4* positions;
  
  // Pass-through variable... maybe not be needed and function can pass through
  vec4   color;
};

#endif
