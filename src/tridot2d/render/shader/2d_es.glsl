R"(
#type vertex
#version 300 es
precision mediump float;

in vec3 vPosition;
in vec4 vColor;
in vec2 vTexCoords;
in float vTextureIndex;
in vec2 vTexCoords2;
in float vTextureIndex2;

uniform mat4 uProjection;

out vec4 fColor;
out vec2 fTexCoords;
out float fTextureIndex;
out vec2 fTexCoords2;
out float fTextureIndex2;

void main(){
    gl_Position = uProjection * vec4(vPosition, 1.0);
    fColor = vColor;
    fTexCoords = vTexCoords;
    fTextureIndex = vTextureIndex;
    fTexCoords2 = vTexCoords2;
    fTextureIndex2 = vTextureIndex2;
}

#type fragment
#version 300 es
precision mediump float;

in vec4 fColor;
in vec2 fTexCoords;
in float fTextureIndex;
in vec2 fTexCoords2;
in float fTextureIndex2;

uniform sampler2D uTextures[16];

out vec4 oColor;

vec4 tex(int index, vec2 coords){
    if(index == 0) return texture(uTextures[0], coords);
    if(index == 1) return texture(uTextures[1], coords);
    if(index == 2) return texture(uTextures[2], coords);
    if(index == 3) return texture(uTextures[3], coords);
    if(index == 4) return texture(uTextures[4], coords);
    if(index == 5) return texture(uTextures[5], coords);
    if(index == 6) return texture(uTextures[6], coords);
    if(index == 7) return texture(uTextures[7], coords);
    if(index == 8) return texture(uTextures[8], coords);
    if(index == 9) return texture(uTextures[9], coords);
    if(index == 10) return texture(uTextures[10], coords);
    if(index == 11) return texture(uTextures[11], coords);
    if(index == 12) return texture(uTextures[12], coords);
    if(index == 13) return texture(uTextures[13], coords);
    if(index == 14) return texture(uTextures[14], coords);
    if(index == 15) return texture(uTextures[15], coords);
    return vec4(1.0, 1.0, 1.0, 1.0);
}

void main(){
    oColor = tex(int(fTextureIndex), fTexCoords) * tex(int(fTextureIndex2), fTexCoords2) * fColor;
}
)"