#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location=2) in vec2 aTexCoord;


out vec3 ourNormal;
out vec3 FragPos;
out vec2 TexCoord;


uniform mat4 transform;
uniform mat4 view;
uniform mat4 projection;

void main(){
	gl_Position = projection * view * transform * vec4(aPos, 1.0f);
	ourNormal=aNormal;
	FragPos=vec3(transform*vec4(aPos,1.0));
	TexCoord=vec2(aTexCoord.x,aTexCoord.y);
}



