R"(
#type vertex
#version 400 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec3 vNormal;
layout (location=2) in vec2 vTexCoords;

uniform mat4 uProjection = mat4(1);
uniform mat4 uTransform = mat4(1);

out vec2 fTexCoords;
out vec3 fNormal;

void main(){
	gl_Position = uProjection * uTransform * vec4(vPosition, 1.0);
	fTexCoords = vTexCoords;
	fNormal = normalize(vec3(transpose(inverse(uTransform)) * vec4(vNormal, 0.0)));
}

#type fragment
#version 400 core

in vec2 fTexCoords;
in vec3 fNormal;

uniform sampler2D uTexture;
uniform vec4 uColor = vec4(1);
uniform vec3 uLightDirection = vec3(0.5, -0.6, 0.7);

out vec4 oColor;

void main(){
	float ndotl = max(0.0, dot(fNormal, normalize(-uLightDirection)));
	float attenuation = ndotl * 0.4 + 0.6;
	oColor = texture(uTexture, fTexCoords) * uColor * attenuation;
}
)"