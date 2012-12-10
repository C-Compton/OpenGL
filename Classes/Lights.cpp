/**
   @file Light.hpp
   @author Nicholas StPierre
   @authors John Huston, Nicholas StPierre, Chris Compton
   @since 2012-11-20
   @date 2012-12-08
   @brief Implementation for the Light container class.
**/


#include <stdexcept>
#include <vector>
#include "vec.hpp"
#include "mat.hpp"
#include "LightSource.hpp"
#include "Lights.hpp"

using Angel::vec2 ;
using Angel::vec3 ;
using Angel::vec4 ; 
using Angel::mat4 ;

using std::vector ;
using std::exception ;
using std::out_of_range ;



/**
   Default constructor.
 **/
Lights::Lights() {

}

/**
   Initialization constructor.
   This constructor allows the implementor to forego a call to init_lights();
   If you can use this one, please do!
   @param g_program A program object that we can link our arrays with.
 **/
Lights::Lights( GLuint &g_program, bool complexSwitch ) {

  if ( complexSwitch )
    {
    g_ambient  = glGetUniformLocation( g_program, "LightAmbientArray" ) ;
    g_diffuse  = glGetUniformLocation( g_program, "LightDiffuseArray" ) ;
    g_specular = glGetUniformLocation( g_program, "LightSpecularArray" ) ;
  
    } 
  else 
    {
       g_color = glGetUniformLocation( g_program, "LightColor" )  ;
    }


   g_position  = glGetUniformLocation( g_program, "LightPositionArray" ) ;
   g_direction = glGetUniformLocation( g_program, "LightDirectionArray" ) ;
   g_numLights = glGetUniformLocation( g_program, "numLights" ) ;

}

/**
   Default destructor.
 **/
Lights::~Lights(){

}

/**
   Removes the u'th element from the list of lights.
   This function is 1-indexed.
   @param u the index of the light to remove.
 **/
void Lights::removeLightSource( const unsigned int u ){

  if ( u < theLights.size() )
    theLights.erase(theLights.begin() + (u-1) ) ;
  else throw out_of_range("Index out of bounds.");

}

/**
   Add a light source to the list.
   @param l light source to add to the list.

 **/
void Lights::addLightSource( const LightSource &l ){

  theLights.push_back( l ) ;

}

/**
   Change an existing light source.
   @param ONE INDEXED index to select a light source.
   @param l light source to replace the existing light with.
 **/
void Lights::modifyLightSource( const unsigned int u, const LightSource &l ){

  if ( u <= theLights.size() && u != 0 )
    theLights[u-1] = l ;
  else throw out_of_range("Light index out of bounds.");


}

/**
   Initializes members in the object to link with shader uniform variables.
   IF YOU DON'T USE THE INITILIZATION CONSTRUCTOR,
   THIS MUST BE CALLED BEFORE TRYING TO CALL sendAll();
   @param g_program A gl program to use for linking with the shader.
 **/
void Lights::init_lights( GLuint &g_program, bool complexSwitch ) {


  // To get the arrays to the shader, we are going to need handles to them.
  // These variables will hold the handles.

  // Note, we must use these names in the shader.
  if ( complexSwitch )
    {
    g_ambient  = glGetUniformLocation( g_program, "LightAmbientArray" ) ;
    g_diffuse  = glGetUniformLocation( g_program, "LightDiffuseArray" ) ;
    g_specular = glGetUniformLocation( g_program, "LightSpecularArray" ) ;
  
    } 
  else 
    {
       g_color = glGetUniformLocation( g_program, "LightColor" )  ;
    }


   g_position  = glGetUniformLocation( g_program, "LightPositionArray" ) ;
   g_direction = glGetUniformLocation( g_program, "LightDirectionArray" ) ;
   g_numLights = glGetUniformLocation( g_program, "numLights" ) ;

  return ;

}

/**
   Get the number of lights we currently have active
 **/
unsigned int Lights::getNumLights() const {

  return (unsigned int) theLights.size();

}

/**
   This function call updates all the lighting arrays in the shader.
   If you make any changes to the lighting, you must call this function again.
   In fact, it probably belongs in your glutRedisplay() callback.
 **/
void Lights::sendAll( bool complexSwitch ) {

  unsigned int i ;
  vec4 temp4;

  for ( i = 0 ; i < theLights.size() && i < MAX_LIGHTS ; i++ ) {

    /*   color4 GetLight_specular() const;
	 color4 GetLight_diffuse() const;
	 color4 GetLight_ambient() const;
	 vec3 GetDirection() const;
	 vec4 GetPosition() const; */

    if (    complexSwitch    ) {
      temp4 = theLights[i].GetLight_ambient()  ;

      ambient[i*3]      =  temp4.x ; //r
      ambient[i*3 + 1]  =  temp4.y ; //g
      ambient[i*3 + 2]  =  temp4.z ; //b


      temp4 = theLights[i].GetLight_diffuse()  ;

      diffuse[i*3]    = temp4.x;
      diffuse[i*3 +1] = temp4.y;
      diffuse[i*3 +2] = temp4.z;


      temp4 = theLights[i].GetLight_specular()  ;

      specular[i*3]    = temp4.x;
      specular[i*3 +1] = temp4.y;
      specular[i*3 +2] = temp4.z;

    } else {

      temp4 = theLights[i].GetLight_color()  ;

      color[i*3]    = temp4.x;
      color[i*3 +1] = temp4.y;
      color[i*3 +2] = temp4.z;

    }

    temp4 =  theLights[i].GetPosition()    ;

    position[i*3]     = temp4.x ;
    position[i*3 +1]  = temp4.y ;
    position[i*3 +2]  = temp4.z ;


    temp4 =  theLights[i].GetDirection()    ;

    direction[i*3]    = temp4.x ;
    direction[i*3 +1] = temp4.y ;
    direction[i*3 +2] = temp4.z ;


    // theLights[i].SetShaderHandle( );

  }

  // send the light arrays

  if (  complexSwitch  ) {
    glUniform3fv( g_ambient,  i,  ambient  ) ;
    glUniform3fv( g_diffuse,  i,  diffuse  ) ;
    glUniform3fv( g_ambient,  i,  specular ) ;

  } else {

    glUniform3fv( g_color,  i,  color ) ;

  }

  // more light arrays
  glUniform3fv( g_position ,  i, position  ) ;
  glUniform3fv( g_direction,  i, direction ) ;

  // send an integer to index all these lovely arrays
  glUniform1i( g_numLights, (GLint) this->getNumLights() ) ;

}



/*
color4 LightSource::GetLight_specular() const {
  return light_specular ;
}
color4 LightSource::GetLight_diffuse() const {
  return light_diffuse ;
}
color4 LightSource::GetLight_ambient() const {
  return light_ambient ;
}
vec3 LightSource::GetDirection() const {
  return direction ;
}
point4 LightSource::GetPoint() const {
  return position ;
}
*/
