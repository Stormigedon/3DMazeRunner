#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCords;


uniform vec3 viewPos;
uniform bool flashLightOn;
uniform sampler2D texture1;
uniform int typeClass;

void main()
{	
	//things with the north wall are birghter
	vec3 viewDir = normalize(viewPos- FragPos);
	vec3 magic = dot(viewDir, Normal) * vec3(0.5,0.5,0.5);
	FragColor = texture(texture1,TexCords) * vec4(magic,1.0);
	FragColor += vec4(vec3(0.02),1.0);
};