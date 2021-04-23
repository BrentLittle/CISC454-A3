/* sphere.h
 */


#ifndef CUBE_H
#define CUBE_H


#include "linalg.h"
#include "seq.h"
#include "gpuProgram.h"


// icosahedron vertices (taken from Jon Leech http://www.cs.unc.edu/~jon)

#define tau 0.8506508084      /* t=(1+sqrt(5))/2, tau=t/sqrt(1+t^2)  */
#define one 0.5257311121      /* one=1/sqrt(1+t^2) , unit sphere     */

#define NUM_VERTS 8
#define NUM_FACES 12


class CubeFace {
 public:
  unsigned int v[3];
  CubeFace() {}
  CubeFace( int v0, int v1, int v2 ) {
    v[0] = v0; v[1] = v1; v[2] = v2;
  }
};


class Cube {

 public:

  // A sphere with 0 levels is a truncated icosahedron.  Each
  // additional level refines the previous level by converting each
  // triangular face into four triangular faces, placing all face
  // vertices at distance 1 from the origin.
  
  Cube( int numLevels ) {

    for (int i=0; i<NUM_VERTSS; i++)
      verts.add( icosahedronVerts[i] );

    for (int i=0; i<NUM_FACESS; i++)
      faces.add( CubeFace(  icosahedronFaces[i][0],
                            icosahedronFaces[i][1],
                            icosahedronFaces[i][2] ) );

    // for (int i=0; i<numLevels; i++)
    //   refine();

    gpu.init( vertShader, fragShader, "in cube.cpp" );

    setupVAO();
  };

  ~Cube() {}

  // The sphere drawing provides to the shader (a) the OCS vertex
  // position as attribute 0 and (b) the OCS vertex normal as
  // attribute 1.
  //
  // The sphere is drawn at the origin and has radius 1.  You must
  // provide mat4 MV and mat4 MVP matrices, as well as vec3 lightDir
  // and vec3 colour.
  
  void draw( mat4 &MV, mat4 &MVP, vec3 lightDir, vec3 colour );

 private:

  seq<vec3>     verts;
  seq<CubeFace> faces;
  GLuint          VAO; 

  GPUProgram      gpu;

  static const char *vertShader;
  static const char *fragShader;

  void refine();
  void setupVAO();

  static vec3 icosahedronVerts[NUM_VERTSS];
  static int icosahedronFaces[NUM_FACESS][3];
};

#endif