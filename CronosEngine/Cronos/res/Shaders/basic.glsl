#type vertex
#version 430 core

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec3 a_Normal;
layout(location = 2) in vec2 a_TexCoords;

layout(std140, binding = 0) uniform u_CameraData
{
	mat4 u_View;
	mat4 u_Proj;
	vec3 u_CameraPosition;
};

uniform mat4 u_Model;

out vec2 v_TexCoords;
out vec3 v_Normal;
out vec3 v_FragPos;
out vec3 v_CamPos;

void main()
{
	v_TexCoords = a_TexCoords;
	v_CamPos = u_CameraPosition;
	v_FragPos = vec3(u_Model * vec4(a_Position, 1.0));
	v_Normal = mat3(transpose(inverse(u_Model))) * a_Normal;

	gl_Position = u_Proj * u_View * u_Model * vec4(a_Position, 1.0);
}


#type fragment
#version 430 core

#define CNSH_ASSERT(condition) while(!condition) { ++m_AssertTimes; break; }
#define COLOROUTPUT_EXIT() while(m_AssertTimes != 0) { color = AssertColorOutput(); break; }

uniform float u_ShaderPlaybackTime;
int m_AssertTimes = 0;

float randomNum(float num) { return fract(sin(dot(vec2(num), vec2(12.9898,78.233))) * 43758.5453); }
vec4 AssertColorOutput()
{
	//This is basically to set a blinking output color to denote an assertion on shader run time.
	vec4 blinkColor = vec4(1.0);
	blinkColor = vec4(randomNum(u_ShaderPlaybackTime), randomNum(u_ShaderPlaybackTime/2), randomNum(u_ShaderPlaybackTime/3), 1.0);
	return (mod(u_ShaderPlaybackTime, 1.0) > 0.5) ? blinkColor : vec4(1.0);
}


#define MAX_POINTLIGHTS 20
#define MAX_SPOTLIGHTS 20
#define MAX_DIRLIGHTS 8

//Input variables
in vec2 v_TexCoords;
in vec3 v_Normal;
in vec3 v_FragPos;
in vec3 v_CamPos;

//Output variables
out vec4 color;

//Lights Structures
struct DirLight 
{
	vec4 LightDir;
	vec4 LightColor;

	float LightIntensity;
};

struct PointLight
{
	vec4 LightPos;
	vec4 LightColor;

	float LightIntensity;

	float LightAtt_K;
	float LightAtt_L;
	float LightAtt_Q;
};

struct SpotLight
{
	vec4 LightPos;
	vec4 LightColor;
	vec4 LightDir;

	float LightIntensity;

	float LightAtt_K;
	float LightAtt_L;
	float LightAtt_Q;

	float cutoffAngleCos;
	float outerCutoffAngleCos;
};

//Actual Lights
layout(std430, binding = 0) buffer SSBOData
{
	int u_CurrentLightsNum[3];

	PointLight u_PointLightsArray[MAX_POINTLIGHTS];
};

layout(std430, binding = 1) buffer SSBOData2
{
	DirLight u_DirLightsArray[MAX_DIRLIGHTS];
};

layout(std430, binding = 2) buffer SSBOData3
{	
	SpotLight u_SPLightsArray[MAX_SPOTLIGHTS];
};

//uniform DirLight u_DirLightsArray[MAX_DIRLIGHTS] = DirLight[MAX_DIRLIGHTS](DirLight(vec3(0), vec3(1), 0.0));
//uniform PointLight u_PointLightsArray[MAX_POINTLIGHTS] = PointLight[MAX_POINTLIGHTS](PointLight(vec3(0), vec3(1), 0.0, 1.0, 0.09, 0.032));
//uniform SpotLight u_SPLightsArray[MAX_SPOTLIGHTS];

//Material Stuff
uniform vec4 u_AmbientColor;
uniform float u_Shininess = 32.0;

uniform sampler2D u_DiffuseTexture;
uniform sampler2D u_SpecularTexture;
//uniform sampler2D u_NormalMap;

uniform bool u_TextureEmpty = true;
uniform bool u_UseBlinnPhong = true;

//ZBuffer rendering -------------------------------------------------------------------------------------------------
uniform vec2 u_CamPlanes; //x for near plane, y for far plane
uniform bool u_drawZBuffer = false;

float LinearizeZ(float depth)
{
	float z = depth*2.0 - 1.0;
	return (2.0*u_CamPlanes.x*u_CamPlanes.y)/(u_CamPlanes.y + u_CamPlanes.x - z*(u_CamPlanes.y - u_CamPlanes.x));
}

//Light Calculations -----------------------------------------------------------------------------------------------
vec3 CalculateDiffSpecLightResult(bool hasTextures, vec3 LColor, float diff, float spec)
{
	//Result
	if(hasTextures)
	{
		vec3 diffuse = LColor * diff * texture(u_DiffuseTexture, v_TexCoords).rgb;
		vec3 specular = LColor * spec * texture(u_SpecularTexture, v_TexCoords).rgb;

		return (diffuse + specular);
	}
	else
	{
		vec3 diffuse = LColor * diff;
		vec3 specular = LColor * spec;

		return (diffuse + specular);
	}
}

//Dir Light Calculation
vec3 CalculateDirectionalLight(DirLight dLight, vec3 normal, vec3 viewDirection, bool hasTextures)
{
	vec3 lightDir = normalize(dLight.LightDir.xyz);
	
	//Diffuse Component
	float diffImpact = max(dot(normal, lightDir), 0.0);
	
	//Specular component
	vec3 halfwayDir = normalize(lightDir + viewDirection);
	float specImpact = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);

	//Result
	return CalculateDiffSpecLightResult(hasTextures, dLight.LightColor.xyz, diffImpact, specImpact) * dLight.LightIntensity;
}

//Point Light Calculation
vec3 CalculatePointLight(PointLight pLight, vec3 normal, vec3 FragPos, vec3 viewDirection, bool hasTextures)
{
	vec3 lightDir = normalize(pLight.LightPos.xyz - FragPos);

	//Diffuse Component
	float diffImpact = max(dot(normal, lightDir), 0.0);

	//Specular Component
	float specImpact = 1.0;
	if(u_UseBlinnPhong)
	{
		vec3 halfwayDir = normalize(lightDir + viewDirection);
		specImpact = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);
	}
	else
	{
		vec3 reflectDirection = reflect(-lightDir, normal);
		specImpact = pow(max(dot(viewDirection, reflectDirection), 0.0), u_Shininess);
	}

	//Attenuation Calculation
	float d = length(pLight.LightPos.xyz - FragPos);
	float lightAttenuation = 1.0/ (pLight.LightAtt_K + pLight.LightAtt_L * d + pLight.LightAtt_Q *(d * d));

	//Result
	return CalculateDiffSpecLightResult(hasTextures, pLight.LightColor.xyz, diffImpact, specImpact) * lightAttenuation * pLight.LightIntensity;
}

//Spot Light Calculation
vec3 CalculateSpotLight(SpotLight spLight, vec3 normal, vec3 FragPos, vec3 viewDirection, bool hasTextures)
{
	vec3 lightDir = normalize(spLight.LightPos.xyz - FragPos);	
	
	//Diffuse Component
	float diffImpact = max(dot(normal, lightDir), 0.0);

	//Specular Component
	vec3 halfwayDir = normalize(lightDir + viewDirection);
	float specImpact = pow(max(dot(normal, halfwayDir), 0.0), u_Shininess);

	//Spotlight Calcs for Soft Edges
	float theta = dot(lightDir, normalize(-spLight.LightDir.xyz));
	float epsilon = spLight.cutoffAngleCos - spLight.outerCutoffAngleCos;
	float lightIntensity = clamp((theta - spLight.outerCutoffAngleCos) / epsilon, 0.0, 1.0) * spLight.LightIntensity;

	//Attenuation Calculation
	float d = length(spLight.LightPos.xyz - FragPos);
	float lightAttenuation = 1.0/ (spLight.LightAtt_K + spLight.LightAtt_L * d + spLight.LightAtt_Q *(d * d));

	//Result
	return CalculateDiffSpecLightResult(hasTextures, spLight.LightColor.xyz, diffImpact, specImpact) * lightIntensity * lightAttenuation;
}

//------------------------------------------------------------------------------------------------------------------
//------------------------------------------------------------------------------------------------------------------
void main()
{
	if (u_drawZBuffer)
	{
		float depth = (LinearizeZ(gl_FragCoord.z) / u_CamPlanes.y);
		color = vec4(vec3(depth), 1.0);
	}
	else
	{
		CNSH_ASSERT((u_CurrentLightsNum[0] <= MAX_DIRLIGHTS && u_CurrentLightsNum[0] >= 0));
		CNSH_ASSERT((u_CurrentLightsNum[1] <= MAX_POINTLIGHTS && u_CurrentLightsNum[1] >= 0));
		CNSH_ASSERT((u_CurrentLightsNum[2] <= MAX_SPOTLIGHTS && u_CurrentLightsNum[2] >= 0));

		//Generic Light Calculations
		vec3 normalVec = normalize(v_Normal);
		vec3 viewDirection = normalize(v_CamPos - v_FragPos);

		//Color Output
		vec3 colorOutput = vec3(0.0);

		for(int i = 0; i < u_CurrentLightsNum[0]; ++i)
			colorOutput += CalculateDirectionalLight(u_DirLightsArray[i], normalVec, viewDirection, !u_TextureEmpty);

		for(int i = 0; i < u_CurrentLightsNum[1]; ++i)
			colorOutput += CalculatePointLight(u_PointLightsArray[i], normalVec, v_FragPos, viewDirection, !u_TextureEmpty);

		for(int i = 0; i < u_CurrentLightsNum[2]; ++i)
			colorOutput += CalculateSpotLight(u_SPLightsArray[i], normalVec, v_FragPos, viewDirection, !u_TextureEmpty);

		if(!u_TextureEmpty)
			color = vec4(colorOutput + u_AmbientColor.rgb * texture(u_DiffuseTexture, v_TexCoords).rgb, 1.0);
		else
			color = vec4(colorOutput + u_AmbientColor.rgb, 1.0);

		COLOROUTPUT_EXIT();
	}
}		