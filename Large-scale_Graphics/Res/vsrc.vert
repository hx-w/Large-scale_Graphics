
#version 330
uniform mat4 uPMatrix,uVMatrix,uMMatrix;
layout (location = 0) in vec4 aPosition;
 
void main(void)
{

    gl_Position = uPMatrix * uVMatrix * uMMatrix * aPosition;

}
