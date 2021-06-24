#version 420

precision mediump float;

out vec4 FragColor;

// Passed in from the vertex shader.
varying vec2 vTexcoord;
varying vec4 vColor;

uniform float time;

uniform bool alphaEnabled;

uniform sampler2D uSampler;

uniform bool hasTexture;
uniform bool hasBorder;

float rand(vec2 co){
    return fract(sin(dot(co.xy ,vec2(12.9898,78.233))) * 43758.5453);
}

void main()
{
    vec2 t = vTexcoord;

    if(hasTexture) {

      FragColor = texture2D(uSampler, t);
      
      if(alphaEnabled && (FragColor.r + FragColor.g + FragColor.b) == 0){
        FragColor.a = 0;
      } else {
        FragColor.r += vColor.r;
        FragColor.g += vColor.g;
        FragColor.b += vColor.b;
        FragColor.a = vColor.a;
      }
    } else {
        FragColor.r = vColor.r;
        FragColor.g = vColor.g;
        FragColor.b = vColor.b;
        FragColor.a = vColor.a;

      /*if((hasBorder && (t.x >= 0.01 && t.x <= 0.99 && t.y >= 0.01 && t.y <= 0.99)) || !hasBorder) {
        FragColor.r = vColor.r;
        FragColor.g = vColor.g;
        FragColor.b = vColor.b;
        FragColor.a = vColor.a;
      } else {
        FragColor = vec4(0,0,0,1);
      }*/
    }

    /*if(gl_FragCoord.x < 300) {
      FragColor = vec4(0,0,0,0);
    }*/
}
