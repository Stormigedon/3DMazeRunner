#version 460 core
out vec4 FragColor;

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCords;


uniform vec3 viewPos;
uniform vec3 viewDir;
uniform vec3 goalPosition;
uniform bool flashLightOn;
uniform sampler2D texture1;
uniform int typeClass;

float computeFalloff(float dist){
	float k1 = 1.0, k2 = 0.7, k3 = 1.8;
	return 1.0 / (k1 + k2 * dist + k3 * dist * dist);
}

vec3 flashLightCompute(vec3 fDir) {
	float flashLightInnerCone = 0.998, flashLightOuterCone = 0.97;
	float viewDot = dot(fDir, -viewDir);
	float fragDot = dot(fDir, Normal);
	if(viewDot >= flashLightOuterCone) {
		if (viewDot >= flashLightInnerCone) {
			return vec3(0.4,0.4,0.6) * fragDot;
		} else {
			//maping from 0 to 1
			float viewMap = (viewDot - flashLightOuterCone) / (flashLightInnerCone - flashLightOuterCone);
			//0.8 + 2.40833x - 8x^{2} + 4.79167x^{3}
			//0.8 + x(2.40833 - 8x + 4.79167x^2)
			//0.8 + x(2.40833 + x(-8 + 4.79167x))
			
			float scale = 0.8 + viewMap * (2.40833 + viewMap * (-8 + 4.79167 * viewMap));
			return vec3(0.4,0.4,0.7) * fragDot * viewMap;
		}
	} else {
		return vec3(0.0,0.0,0.01);
	}
}

void main()
{	
	//things with the north wall are birghter
	vec3 goalDir = normalize(goalPosition - FragPos);

	float GlowFalloff = computeFalloff(distance(goalPosition,FragPos)); //mmm math
	vec3 goalGlow = (max(dot(goalDir, Normal),0.0) * vec3(0.6,0.4,0.2)) * GlowFalloff;
	vec3 fragDir = normalize(viewPos- FragPos);
	//vec3 magic = dot(fragDir, Normal) * vec3(0.5,0.5,0.5);
	FragColor = texture(texture1,TexCords) * vec4(flashLightCompute(fragDir),1.0);
	FragColor += vec4(vec3(0.02) * texture(texture1, TexCords).rgb,1.0);
	FragColor += vec4(goalGlow,0.0);
};