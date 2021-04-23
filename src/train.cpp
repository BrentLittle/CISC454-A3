// train.cpp

#include "train.h"
#include "main.h"

#define SPHERE_RADIUS 5.0
#define SPHERE_COLOUR 238.0/255.0, 106.0/255.0, 20.0/255.0

#define CUBE_RADIUS 1.0
#define CAR1_RADIUS 1.0
#define CAR2_RADIUS 1.0
#define CAR3_RADIUS 1.0
#define CAR4_RADIUS 1.0
#define CAR5_RADIUS 1.0

#define CUBE_COLOUR 19/255.0, 52/255.0, 92/255.0
#define CAR1_COLOUR 19/255.0, 52/255.0, 92/255.0
#define CAR2_COLOUR 237/255.0, 188/255.0, 65/255.0
#define CAR3_COLOUR 237/255.0, 188/255.0, 65/255.0
#define CAR4_COLOUR 155/255.0, 28/255.0, 59/255.0
#define CAR5_COLOUR 155/255.0, 28/255.0, 59/255.0

// Draw the train.
//
// 'flag' is toggled by pressing 'F' and can be used for debugging

void Train::draw( mat4 &WCStoVCS, mat4 &WCStoCCS, vec3 lightDir, bool flag )
{
#if 1

  // YOUR CODE HERE

  float t = spline->paramAtArcLength( pos );
  
  vec3 o, x, y, z;
  spline->findLocalSystem( t, o, x, y, z );
  
  mat4 M   = spline->findLocalTransform(t) * scale( CUBE_RADIUS * 1.25, CUBE_RADIUS * 1.25, CUBE_RADIUS * 2 );
  mat4 MV  = WCStoVCS * M;
  mat4 MVP = WCStoCCS * M;
  cube->draw( MV, MVP, lightDir, vec3( CUBE_COLOUR ) );


  t = spline->paramAtArcLength( pos - 4.5 );
  spline->findLocalSystem( t, o, x, y, z );
  M   = spline->findLocalTransform(t) * scale( CAR1_RADIUS * 1.25, CAR1_RADIUS * 1.25, CAR1_RADIUS * 2 );
  MV  = WCStoVCS * M;
  MVP = WCStoCCS * M;
  car1->draw( MV, MVP, lightDir, vec3( CAR1_COLOUR ) );


  t = spline->paramAtArcLength( pos - 9 );
  spline->findLocalSystem( t, o, x, y, z );
  M   = spline->findLocalTransform(t) * scale( CAR2_RADIUS * 1.25, CAR2_RADIUS * 1.25, CAR2_RADIUS * 2 );
  MV  = WCStoVCS * M;
  MVP = WCStoCCS * M;
  car2->draw( MV, MVP, lightDir, vec3( CAR2_COLOUR ) );


  t = spline->paramAtArcLength( pos - 13.5 );
  spline->findLocalSystem( t, o, x, y, z );
  M   = spline->findLocalTransform(t) * scale( CAR3_RADIUS * 1.25, CAR3_RADIUS * 1.25, CAR3_RADIUS * 2 );
  MV  = WCStoVCS * M;
  MVP = WCStoCCS * M;
  car3->draw( MV, MVP, lightDir, vec3( CAR3_COLOUR ) );


  t = spline->paramAtArcLength( pos - 18 );
  spline->findLocalSystem( t, o, x, y, z );
  M   = spline->findLocalTransform(t) * scale( CAR4_RADIUS * 1.2, CAR4_RADIUS * 1.25, CAR4_RADIUS * 2 );
  MV  = WCStoVCS * M;
  MVP = WCStoCCS * M;
  car4->draw( MV, MVP, lightDir, vec3( CAR4_COLOUR ) );


  t = spline->paramAtArcLength( pos - 22.5 );
  spline->findLocalSystem( t, o, x, y, z );
  M   = spline->findLocalTransform(t) * scale( CAR5_RADIUS * 1.2, CAR5_RADIUS * 1.25, CAR5_RADIUS * 2 );
  MV  = WCStoVCS * M;
  MVP = WCStoCCS * M;
  car5->draw( MV, MVP, lightDir, vec3( CAR5_COLOUR ) );


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
#if 1
  // YOUR CODE HERE

  // Ensure the train moves continuously around the track and once it reaches the end of the spline 
  // it will wrap back around
  float arcLen = spline->totalArcLength();
  pos = fmod( pos, arcLen );

  // Move Train given a F = M*a physics model
  float t = spline->paramAtArcLength( pos );
  
  // obtain the local coordinate system at the parameter
  vec3 o, x, y, splineTangent;
  spline->findLocalSystem( t, o, x, y, splineTangent );
  
  // find the slope of the tangent to the spline (i.e the z compnent of our z vec3) 
  float slopeOfSplineInDegrees = ((splineTangent.normalize()).z) * 90;

  if (slopeOfSplineInDegrees > 0) { 
    // with an inclining slope we want to slow the train down
    // There is a decrease in speed due to a decrease in acceleration
    
    // For an increasing slope accel will be negative
    float accel = -9.8 * sin(slopeOfSplineInDegrees * (3.14159/180));
    
    //cout << "slope > 0 Accel: " << accel << "\n";
    
    // Calculate the change in speed needed over the elapsedSeconds
    speed += accel * elapsedSeconds;
    
    // Enforce a minimum speed the train will travel at to 
    // ensure it keeps moving along the track and will not stop
    if (speed < 40 ){
      speed = 40;
    }

    
  }
  else if (slopeOfSplineInDegrees < 0) {
    // with a decreasing slope we want to speed the train up
    // There is an increase in speed due to an increase in acceleration
    
    // as we are looking at negative slopes the acceleration will be positive
    float accel = -9.8 * sin(slopeOfSplineInDegrees * (3.14159/180));
    
    //cout << "slope < 0 Accel: " << accel << "\n";
    
    // Calculate the change in speed needed over the elapsedSeconds
    speed += accel * elapsedSeconds;
    
  }

  //cout << "speed: " << speed << "\n";
  
  // update the position of the train by 1% of the speed of the train 
  pos += speed/100;

#else

  pos += 0.5;

#endif
}
