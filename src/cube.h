#ifndef CUBE_H
#define CUBE_H

#include "linalg.h"
#include "seq.h"
#include "gpuProgram.h"

class Cube {

 public:

  Cube() 
  {
    gpu.init( vertShader, fragShader, "in cube.cpp" );
    setupVAO();
  };

  ~Cube() {}
  
  void draw( mat4 &MV, mat4 &MVP, vec3 lightDir, vec3 colour );

 private:

  seq<vec3>         verts;
  seq<vec3>         normals;
  GLuint            VAO; 
  GPUProgram        gpu;

  static const char *vertShader;
  static const char *fragShader;

  void setupVAO();
};

#endif