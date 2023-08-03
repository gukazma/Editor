#version 460 core
layout (location = 0) out vec4 FragColor;

layout (location = 0) in vec3 in_Pos; 
layout (location = 1) in vec3 in_Normal; 

uniform vec3 u_LightPos;
uniform vec3 u_LightPos1;

void main()
{

	vec3 n = normalize(in_Normal);
	vec4 diffuse = vec4(0.0);
	vec4 diffuse1 = vec4(0.0);
	vec4 specular = vec4(0.0);
	vec4 specular1 = vec4(0.0);

	// the material properties are embedded in the shader (for now)
	vec4 mat_ambient = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 mat_diffuse = vec4(1.0, 1.0, 1.0, 1.0);
	vec4 mat_specular = vec4(1.0, 1.0, 1.0, 1.0);

	// ambient term
	vec4 ambient = mat_ambient * 0.4;

	// diffuse color
	vec4 kd = mat_diffuse * 0.6;

	// specular color
	vec4 ks = mat_specular * 0.1;

	// diffuse term
	vec3 lightDir = normalize(u_LightPos - in_Pos);
	vec3 lightDir1 = normalize(u_LightPos1 - in_Pos);
	float NdotL = dot(n, lightDir);
	float NdotL1 = dot(n, lightDir1);

	if (NdotL > 0.0)
		diffuse = kd * NdotL;
	if (NdotL1 > 0.0)
		diffuse1 = kd * NdotL1;

	// specular term
	vec3 rVector = normalize(2.0 * n * dot(n, lightDir) - lightDir);
	vec3 rVector1 = normalize(2.0 * n * dot(n, lightDir1) - lightDir1);
	vec3 viewVector = normalize(-in_Pos);
	float RdotV = dot(rVector, viewVector);
	float RdotV1 = dot(rVector1, viewVector);

	if (RdotV > 0.0)
		specular = ks * pow(RdotV, 0.6);
	if (RdotV1 > 0.0)
		specular1 = ks * pow(RdotV1, 0.6);

	FragColor = (ambient + (diffuse + specular + diffuse1 + specular1)/1.75);

    
}