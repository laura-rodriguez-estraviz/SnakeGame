#version 330 core

in vec3 ourNormal;
in vec3 FragPos;
in vec2 TexCoord;

out vec4 FragColor;

uniform vec3 viewPos;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 objectColor;
uniform sampler2D texture1;

void main(){

	vec3 luzDir = vec3(0,0,-1);
	float ambientI=0.2;
	vec3 ambient=ambientI*lightColor;
	vec3 ld=normalize(-lightPos);
	ld=luzDir;
	vec3 fd=normalize(vec3((FragPos-lightPos)));



	//difuse
	vec3 norm=normalize (ourNormal);
	vec3 lightDir=normalize(lightPos-FragPos);
	float diff=max(dot(norm,lightDir),0.0);
	vec3 diffuse=diff*lightColor;
	
	//specular
	float specularStrength=0.1;
	vec3 viewDir=normalize(viewPos-FragPos);
	vec3 reflectDir=reflect(-lightDir,norm);
	float spec=pow(max(dot(viewDir,reflectDir),0.0),2);
	vec3 specular=specularStrength*spec*lightColor;
	

		
	vec3 result=(ambient+diffuse);
		

	FragColor=vec4(result,1.0f)*texture(texture1,TexCoord);


}
