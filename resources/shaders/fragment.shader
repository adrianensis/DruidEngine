#version 420

precision mediump float;

out vec4 FragColor;

// Passed in from the vertex shader.
varying vec2 vTexcoord;

uniform float time;

uniform bool invertXAxis;
uniform bool alphaEnabled;

//uniform float regionX;
//uniform float regionY;
//uniform float regionWidth;
//uniform float regionHeight;

uniform uint animationSize;

uniform sampler2D uSampler;

uniform vec4 color;

uniform bool hasTexture;
uniform bool hasBorder;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec2 t = vTexcoord;

    if(hasTexture) {
      //t.y = 1.0 - t.y; // flip the texture along the y-Axis!
  
      //t.x = t.x * regionWidth + regionX;
      //t.y = t.y * regionHeight + regionY;
  
      /*if(invertXAxis){
        t.x = 1.0 - t.x - (1.0-(animationSize*regionWidth));
      }*/
  
      FragColor = texture2D(uSampler, t);
      
      if(alphaEnabled && (FragColor.r + FragColor.g + FragColor.b) == 0){
        FragColor.a = 0;
      } else {
        FragColor.r += color.r;
        FragColor.g += color.g;
        FragColor.b += color.b;
        FragColor.a = color.a;
      }
    } else {
      if((hasBorder && (t.x >= 0.01 && t.x <= 0.99 && t.y >= 0.01 && t.y <= 0.99)) || !hasBorder) {
        FragColor.r = color.r;
        FragColor.g = color.g;
        FragColor.b = color.b;
        FragColor.a = color.a;
      } else {
        FragColor = vec4(0,0,0,1);
      }
    }
}
