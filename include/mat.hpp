/*
  mat.hpp
  Based on Ed Angel's mat.h, from
  'Interactive Computer Graphics: 
  A Top-Down Approach with Shader-Based OpenGL (6th Edition)'
*/

#ifndef __ANGEL_MAT_H__
#define __ANGEL_MAT_H__

#include <cstdio>
#include "vec.hpp"

namespace Angel {

  //  mat2 - 2D square matrix
  class mat2 {
    
    // It's possible that this needs to be first for GL compatibility...?
    vec2  _m[2];
    
  public:
    //  --- Constructors and Destructors ---
    mat2( const GLfloat d = GLfloat(1.0));  // Create a diagional matrix
    mat2( const vec2& a, const vec2& b );
    mat2( GLfloat m00, GLfloat m10, GLfloat m01, GLfloat m11 );
    mat2( const mat2& m );
    
    //  --- Indexing Operator ---
    vec2& operator [] ( int i );
    const vec2& operator [] ( int i ) const;
    
    //  --- (non-modifying) Arithmatic Operators ---
    mat2 operator + ( const mat2& m ) const;
    mat2 operator - ( const mat2& m ) const;
    mat2 operator * ( const GLfloat s ) const;
    mat2 operator / ( const GLfloat s ) const;
    friend mat2 operator * ( const GLfloat s, const mat2& m );	
    mat2 operator * ( const mat2& m ) const;
    
    //  --- (modifying) Arithmetic Operators ---
    mat2& operator += ( const mat2& m );
    mat2& operator -= ( const mat2& m );
    mat2& operator *= ( const GLfloat s );
    mat2& operator *= ( const mat2& m );
    mat2& operator /= ( const GLfloat s );

    //  --- Matrix / Vector operators ---
    vec2 operator * ( const vec2& v ) const;

    //  --- Insertion and Extraction Operators ---
    friend std::ostream& operator<<( std::ostream& os, const mat2& m );
    friend std::istream& operator>>( std::istream& is, mat2& m );

    //  --- Conversion Operators ---
    operator const GLfloat* () const;
    operator GLfloat* ();

  };
  
  //  --- Non-class mat2 Methods ---
  inline mat2 matrixCompMult( const mat2& A, const mat2& B );
  inline mat2 transpose( const mat2& A );



  /**
     mat3 - 3D square matrix.
  **/
  class mat3 {
    vec3  _m[3];
  public:

    //  --- Constructors and Destructors ---
    mat3( const GLfloat d = GLfloat(1.0) );  // Create a diagional matrix
    mat3( const vec3& a, const vec3& b, const vec3& c );
    mat3( GLfloat m00, GLfloat m10, GLfloat m20,
	  GLfloat m01, GLfloat m11, GLfloat m21,
	  GLfloat m02, GLfloat m12, GLfloat m22 );
    mat3( const mat3& m );

    //  --- Indexing Operator ---
    vec3& operator [] ( int i );
    const vec3& operator [] ( int i ) const;

    //  --- (non-modifying) Arithmatic Operators ---
    mat3 operator + ( const mat3& m ) const;
    mat3 operator - ( const mat3& m ) const;
    mat3 operator * ( const GLfloat s ) const;
    mat3 operator / ( const GLfloat s ) const;
    friend mat3 operator * ( const GLfloat s, const mat3& m );
    mat3 operator * ( const mat3& m ) const;

    //  --- (modifying) Arithmetic Operators ---
    mat3& operator += ( const mat3& m );
    mat3& operator -= ( const mat3& m );
    mat3& operator *= ( const GLfloat s );
    mat3& operator *= ( const mat3& m );
    mat3& operator /= ( const GLfloat s );

    //  --- Matrix / Vector operators ---
    vec3 operator * ( const vec3& v ) const;

    //  --- Insertion and Extraction Operators ---
    friend std::ostream& operator << ( std::ostream& os, const mat3& m );
    friend std::istream& operator >> ( std::istream& is, mat3& m );

    //  --- Conversion Operators ---
    operator const GLfloat* () const;
    operator GLfloat* ();
  };

  //  --- Non-class mat3 Methods ---
  inline mat3 matrixCompMult( const mat3& A, const mat3& B );
  inline mat3 transpose( const mat3& A );



  //------------------------------------------------------------------------
  //
  //  mat4.h - 4D square matrix
  //
  class mat4 {

    vec4  _m[4];

  public:

    //  --- Constructors and Destructors ---
    mat4( const GLfloat d = GLfloat(1.0) );  // Create a diagional matrix
    mat4( const vec4& a, const vec4& b, const vec4& c, const vec4& d );
    mat4( GLfloat m00, GLfloat m01, GLfloat m02, GLfloat m03,
	  GLfloat m10, GLfloat m11, GLfloat m12, GLfloat m13,
	  GLfloat m20, GLfloat m21, GLfloat m22, GLfloat m23,
	  GLfloat m30, GLfloat m31, GLfloat m32, GLfloat m33 );
    mat4( const mat4& m );

    //  --- Indexing Operator ---
    vec4& operator [] ( int i );
    const vec4& operator [] ( int i ) const;

    //  --- (non-modifying) Arithematic Operators ---
    mat4 operator + ( const mat4& m ) const;
    mat4 operator - ( const mat4& m ) const;
    mat4 operator * ( const GLfloat s ) const;
    mat4 operator / ( const GLfloat s ) const;
    friend mat4 operator * ( const GLfloat s, const mat4& m );
    friend vec4 operator* ( const vec4 &v, const mat4& _m );
    mat4 operator * ( const mat4& m ) const;

    //  --- (modifying) Arithematic Operators ---
    mat4& operator += ( const mat4& m );
    mat4& operator -= ( const mat4& m );
    mat4& operator *= ( const GLfloat s );
    mat4& operator *= ( const mat4& m );
    mat4& operator /= ( const GLfloat s );

    //  --- Matrix / Vector operators ---
    vec4 operator * ( const vec4& v ) const;

    //  --- Insertion and Extraction Operators ---
    friend std::ostream& operator << ( std::ostream& os, const mat4& m );
    friend std::istream& operator >> ( std::istream& is, mat4& m );

    //  --- Conversion Operators ---
    operator const GLfloat* () const;
    operator GLfloat* ();

  };

  //  --- Non-class mat4 Methods ---
  inline mat4 matrixCompMult( const mat4& A, const mat4& B );
  inline mat4 transpose( const mat4& A );

  /************************************************************************/
  //  Helpful Matrix Methods
  /************************************************************************/

#define Error( str ) do {				\
    std::cerr << "[" __FILE__ ":" << __LINE__ << "] "	\
	      << str << std::endl; } while(0)
    
  inline vec4 mvmult( const mat4& a, const vec4& b );
  inline mat4 RotateX( const GLfloat theta );
  inline mat4 RotateY( const GLfloat theta );
  inline mat4 RotateZ( const GLfloat theta );
  //  Translation matrix generators
  inline mat4 Translate( const GLfloat x, const GLfloat y, 
			 const GLfloat z );
  inline mat4 Translate( const vec3& v );
  inline mat4 Translate( const vec4& v );
  //  Scale matrix generators
  inline mat4 Scale( const GLfloat x, const GLfloat y, const GLfloat z );
  inline mat4 Scale( const vec3& v );
  //  Projection transformation matrix geneartors
  inline mat4 Ortho( const GLfloat left, const GLfloat right,
		     const GLfloat bottom, const GLfloat top,
		     const GLfloat zNear, const GLfloat zFar );
  inline mat4 Ortho2D( const GLfloat left, const GLfloat right,
		       const GLfloat bottom, const GLfloat top );
  inline mat4 Frustum( const GLfloat left, const GLfloat right,
		       const GLfloat bottom, const GLfloat top,
		       const GLfloat zNear, const GLfloat zFar );
  inline mat4 Perspective( const GLfloat fovy, const GLfloat aspect,
			   const GLfloat zNear, const GLfloat zFar);
  //  Viewing transformation matrix generation
  inline mat4 LookAt( const vec4& eye, const vec4& at, const vec4& up );

  //--Deprecated?-----------------------------------------------------------
  inline vec4 minus(const vec4& a, const vec4&  b );
  inline void printv(const vec4& a );
  inline void printm(const mat4 a);
  inline mat4 identity();
  
}  // namespace Angel

#endif // __ANGEL_MAT_H__
