/* oh god please no */
/* no               */
/* nooooooooooo     */

#include "Angel.h"
typedef Angel::vec4  color4;
typedef Angel::vec4  point4;
extern vec4  points[];
extern vec4  colors[];
extern vec3 normals[];


void createPoint( vec4 const &the_point, vec4 const &the_color, vec3 const &the_normal ) {
  static int Index = 0;
  points[Index]  = the_point ;
  colors[Index]  = the_color ;
  normals[Index] = the_normal;
  Index++;
}

//--------------------------------------------------------------------

void triangle( const vec4& a, const vec4& b, 
	       const vec4& c, const int color ) {

  static vec4  base_colors[] = {
    vec4( 1.0, 0.0, 0.0, 1.0 ), /* R */
    vec4( 0.0, 1.0, 0.0, 1.0 ), /* G */
    vec4( 0.0, 0.0, 1.0, 1.0 ), /* B */
    vec4( 1.0, 1.0, 0.0, 1.0 )  /* Y */
  };

  // Initialize temporary vectors along the quad's edge to
  //   compute its face normal

  vec4 u = b - a;
  vec4 v = c - a;

  vec3 normal = normalize( cross(u, v) );

  createPoint( a, base_colors[ 0 ], normal );
  createPoint( b, base_colors[ 1 ], normal );
  createPoint( c, base_colors[ 2 ], normal );
  
}

//--------------------------------------------------------------------

void tetra( const vec4& a, const vec4& b, 
	    const vec4& c, const vec4& d ) {

  triangle( a, b, c, 0 );
  triangle( a, c, d, 1 );
  triangle( a, d, b, 2 );
  triangle( b, d, c, 3 );

}

//--------------------------------------------------------------------

void divide_tetra( const vec4& a, const vec4& b,
		   const vec4& c, const vec4& d, int count ) {

  if ( count > 0 ) {
    vec4 v0 = ( a + b ) / 2.0;
    vec4 v1 = ( a + c ) / 2.0;
    vec4 v2 = ( a + d ) / 2.0;
    vec4 v3 = ( b + c ) / 2.0;
    vec4 v4 = ( c + d ) / 2.0;
    vec4 v5 = ( b + d ) / 2.0;
    divide_tetra( a, v0, v1, v2, count - 1 );
    divide_tetra( v0, b, v3, v5, count - 1 );
    divide_tetra( v1, v3, c, v4, count - 1 );
    divide_tetra( v2, v4, v5, d, count - 1 );
  } else {
    tetra( a, b, c, d );   // draw tetrahedron at end of recursion
  }

}

//--------------------------------------------------------------------

void quad( int a, int b, int c, int d ) {

  static const point4 vertices[8] = {
    point4( -0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5,  0.5,  0.5, 1.0 ),
    point4(  0.5, -0.5,  0.5, 1.0 ),
    point4( -0.5, -0.5, -0.5, 1.0 ),
    point4( -0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5,  0.5, -0.5, 1.0 ),
    point4(  0.5, -0.5, -0.5, 1.0 )
  };

  static const color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
  };


  // Initialize temporary vectors along the quad's edge to                                                                                 
  //   compute its face normal                                                                                                             
  vec4 u = vertices[b] - vertices[a];
  vec4 v = vertices[c] - vertices[b];

  vec3 normal = normalize( cross(u, v) );

  createPoint( vertices[a], vertex_colors[a], normal );
  createPoint( vertices[b], vertex_colors[b], normal );
  createPoint( vertices[c], vertex_colors[c], normal );

  createPoint( vertices[a], vertex_colors[a], normal ); // if we did this right, we don't
  createPoint( vertices[c], vertex_colors[c], normal ); // need to recompute the normal 
  createPoint( vertices[d], vertex_colors[d], normal ); // because it is on the same plane, 
                                                // and therefore equivalent.

}

void quad( const point4 &a, const point4 &b,
	   const point4 &c, const point4 &d,
	   const color4 &A, const color4 &B,
	   const color4 &C, const color4 &D ) {


  // Initialize temporary vectors along the quad's edge to                                                                                 
  //   compute its face normal                                                                                                             
  vec4 u = b - a;
  vec4 v = c - b;

  vec3 normal = normalize( cross(u, v) );

  createPoint( a, A, normal );
  createPoint( b, B, normal );
  createPoint( c, C, normal );

  createPoint( a, A, normal );
  createPoint( c, C, normal );
  createPoint( d, D, normal );

}

//--------------------------------------------------------------------

#define QUAD(A,B,C,D)						\
  quad( vertices[A], vertices[B], vertices[C], vertices[D],	\
	colors[A], colors[B], colors[C], colors[D] );

void cube( const GLfloat &size, const color4 colors[8] ) {

  const GLfloat lower = -(size/2);
  const GLfloat upper = (size/2);

  const point4 vertices[8] = {
    point4( lower, lower, upper, 1.0 ),
    point4( lower, upper, upper, 1.0 ),
    point4( upper, upper, upper, 1.0 ),
    point4( upper, lower, upper, 1.0 ),
    point4( lower, lower, lower, 1.0 ),
    point4( lower, upper, lower, 1.0 ),
    point4( upper, upper, lower, 1.0 ),
    point4( upper, lower, lower, 1.0 )
  };
  
  QUAD( 1, 0, 3, 2 );
  QUAD( 2, 3, 7, 6 );
  QUAD( 3, 0, 4, 7 );
  QUAD( 6, 5, 1, 2 );
  QUAD( 4, 5, 6, 7 );
  QUAD( 5, 4, 0, 1 );
  
}

// generate 12 triangles: 36 vertices and 36 colors
void colorcube() { 
  quad( 1, 0, 3, 2 );
  quad( 2, 3, 7, 6 );
  quad( 3, 0, 4, 7 );
  quad( 6, 5, 1, 2 );
  quad( 4, 5, 6, 7 );
  quad( 5, 4, 0, 1 );
}

void colorcube( GLfloat size ) {

  static const color4 vertex_colors[8] = {
    color4( 0.0, 0.0, 0.0, 1.0 ),  // black
    color4( 1.0, 0.0, 0.0, 1.0 ),  // red
    color4( 1.0, 1.0, 0.0, 1.0 ),  // yellow
    color4( 0.0, 1.0, 0.0, 1.0 ),  // green
    color4( 0.0, 0.0, 1.0, 1.0 ),  // blue
    color4( 1.0, 0.0, 1.0, 1.0 ),  // magenta
    color4( 1.0, 1.0, 1.0, 1.0 ),  // white
    color4( 0.0, 1.0, 1.0, 1.0 )   // cyan
  };

  cube( size, vertex_colors );

}

//--------------------------------------------------------------------
