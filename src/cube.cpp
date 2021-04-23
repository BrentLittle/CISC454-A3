#include "cube.h"

void Cube::setupVAO()
{
  GLfloat cube_positions[ 3 * 36 ] = {
    // define all vertex positions in the following face order
    // front, back, right, left, top, bottom
    -1, 0, 1,
     1, 0, 1,
    -1, 2, 1,
    -1, 2, 1,
     1, 2, 1,
     1, 0, 1,
    
     1, 0, -1,
    -1, 0, -1,
     1, 2, -1,
    -1, 0, -1,
    -1, 2, -1,
     1, 2, -1,

    1, 0, -1,
    1, 0,  1,
    1, 2, -1,
    1, 0,  1,
    1, 2,  1,
    1, 2, -1,

    -1, 0, -1,
    -1, 0,  1,
    -1, 2, -1,
    -1, 0,  1,
    -1, 2, -1,
    -1, 2,  1,

     1, 2, -1,
     1, 2,  1,
    -1, 2,  1,
     1, 2, -1,
    -1, 2, -1,
    -1, 2,  1,

     1, 0, -1,
    -1, 0, -1,
    -1, 0,  1,
     1, 0, -1,
     1, 0,  1,
    -1, 0,  1
  };

  GLfloat cube_normals[ 3 * 36 ] = {
    // define all normals in the following face order
    // front, back, right, left, top, bottom
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    0, 0, 1,
    
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    0, 0, -1,
    
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    1, 0, 0,
    
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    -1, 0, 0,
    
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    0, 1, 0,
    
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0,
    0, -1, 0
  };

  glGenVertexArrays( 1, &VAO );
  glBindVertexArray( VAO );

  // store positions into VAO
  GLuint vertexbuffer;
  glGenBuffers(1, &vertexbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_positions), cube_positions, GL_STATIC_DRAW);

  glEnableVertexAttribArray(0);
  glBindBuffer(GL_ARRAY_BUFFER, vertexbuffer);
  glVertexAttribPointer( 0, 3, GL_FLOAT, GL_FALSE, 0, 0 );

  // store normals into VAO
  GLuint normalbuffer;
  glGenBuffers(1, &normalbuffer);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glBufferData(GL_ARRAY_BUFFER, sizeof(cube_normals), cube_normals, GL_STATIC_DRAW);

  glEnableVertexAttribArray(1);
  glBindBuffer(GL_ARRAY_BUFFER, normalbuffer);
  glVertexAttribPointer( 1, 3, GL_FLOAT, GL_FALSE, 0, 0 );
}


void Cube::draw( mat4 &MV, mat4 &MVP, vec3 lightDir, vec3 colour )
{
  gpu.activate();
  
  gpu.setMat4( "MV", MV );
  gpu.setMat4( "MVP", MVP );
  gpu.setVec3( "colour", colour );
  gpu.setVec3( "lightDir", lightDir );
  
  glUniformMatrix4fv( glGetUniformLocation( gpu.id(), "MVP"), 1, GL_TRUE, &MVP[0][0] );
  glBindVertexArray( VAO );
  glDrawArrays( GL_TRIANGLES, 0, 36 );
  glBindVertexArray( 0 );

  gpu.deactivate();
}


const char *Cube::vertShader = R"(

  #version 300 es

  uniform mat4 MVP;
  uniform mat4 MV;

  layout (location = 0) in mediump vec3 vertPosition;
  layout (location = 1) in mediump vec3 vertNormal;

  smooth out mediump vec3 normal;

  void main() {

    gl_Position = MVP * vec4( vertPosition, 1.0 );
    normal = vec3( MV * vec4( vertNormal, 0.0 ) );
  }
)";


const char *Cube::fragShader = R"(

  #version 300 es

  uniform mediump vec3 colour;
  uniform mediump vec3 lightDir;

  smooth in mediump vec3 normal;
  out mediump vec4 outputColour;

  void main() {

    mediump float NdotL = dot( normalize(normal), lightDir );

    if (NdotL < 0.0)
      NdotL = 0.1; // some ambient

    outputColour = vec4( NdotL * colour, 1.0 );
  }
)";