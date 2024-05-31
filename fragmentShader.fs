#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCords;


uniform vec3 viewPos;
uniform vec3 goalPosition;
uniform bool flashLightOn;
uniform sampler2D texture1;
uniform int typeClass;

float computeFalloff(float dist){
	float k1 = 1.0, k2 = 0.7, k3 = 1.8;
	return 1.0 / (k1 + k2 * dist + k3 * dist * dist);
}

void main()
{	
	//things with the north wall are birghter
	vec3 goalDir = normalize(goalPosition - FragPos);

	float GlowFalloff = computeFalloff(distance(goalPosition,FragPos)); //mmm math
	vec3 goalGlow = (max(dot(goalDir, Normal),0.0) * vec3(0.6,0.4,0.2)) * GlowFalloff;
	vec3 viewDir = normalize(viewPos- FragPos);
	vec3 magic = dot(viewDir, Normal) * vec3(0.5,0.5,0.5);
	FragColor = texture(texture1,TexCords) * vec4(magic,1.0);
	//FragColor = vec4(vec3(0.02) * texture(texture1, TexCords).rgb,1.0);
	FragColor += vec4(goalGlow,0.0);
};