// train.cpp

#include "train.h"
#include "main.h"


#define SPHERE_RADIUS 5.0
#define SPHERE_COLOUR 238/255.0, 106/255.0, 20/255.0

// Draw the train.
//
// 'flag' is toggled by pressing 'F' and can be used for debugging

 
void Train::draw( mat4 &WCStoVCS, mat4 &WCStoCCS, vec3 lightDir, bool flag )

{
#if 0

  // YOUR CODE HERE

#else
  
  float t = spline->paramAtArcLength( pos );

  // Draw sphere
  
  vec3 o, x, y, z;
  spline->findLocalSystem( t, o, x, y, z );

  mat4 M   = translate( o ) * scale( SPHERE_RADIUS, SPHERE_RADIUS, SPHERE_RADIUS );
  mat4 MV  = WCStoVCS * M;
  mat4 MVP = WCStoCCS * M;

  sphere->draw( MV, MVP, lightDir, vec3( SPHERE_COLOUR ) );

#endif
}


void Train::advance( float elapsedSeconds )

{
#if 0

  // YOUR CODE HERE

#else

  pos += 1;

#endif
}
