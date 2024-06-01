R"(
#type vertex
#version 400 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec4 vColor;
layout (location=2) in vec2 vTexCoords;
layout (location=3) in float vTextureIndex;
layout (location=4) in vec2 vTexCoords2;
layout (location=5) in float vTextureIndex2;

uniform mat4 uProjection = mat4(1);

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
#version 400 core

in vec4 fColor;
in vec2 fTexCoords;
in float fTextureIndex;
in vec2 fTexCoords2;
in float fTextureIndex2;

uniform sampler2D uTextures[32];

out vec4 oColor;

vec4 tex(int index, vec2 coords){
    for(int i = 0; i < 32; i++){
        if(i == index){
            return texture(uTextures[i], coords);
        }
    }
    return vec4(1, 1, 1, 1);
}

void main(){
    oColor = tex(int(fTextureIndex), fTexCoords) * tex(int(fTextureIndex2), fTexCoords2) * fColor;
}
)"