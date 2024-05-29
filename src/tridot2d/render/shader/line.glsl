R"(
#type vertex
#version 400 core

layout (location=0) in vec3 vPosition;
layout (location=1) in vec4 vColor;

uniform mat4 uProjection = mat4(1);

out vec4 fColor;

void main(){
	gl_Position = uProjection * vec4(vPosition, 1.0);
	fColor = vColor;
}

#type fragment
#version 400 core

in vec4 fColor;

out vec4 oColor;

void main(){
	oColor =  fColor;
}
)"