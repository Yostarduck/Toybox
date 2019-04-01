Texture2D AlbedoTex : register(t0);

SamplerState gLinearSample : register(s0);

cbuffer ConstantBuffer : register(b0) {
	float4x4 World;
  float4x4 View;
  float4x4 Projection;
};

struct PS_INPUT {
  float4 Position : SV_POSITION;
	float4 WorldPos : TEXCOORD0;
  float2 Texcoord : TEXCOORD1;
};

struct PS_OUTPUT {
  float4 WorldPosition  : SV_TARGET0;
  float4 Metallic       : SV_TARGET1;
	float4 Roughness      : SV_TARGET2;
	float4 Emissive       : SV_TARGET3;
};

PS_OUTPUT
main(PS_INPUT input) {
	PS_OUTPUT output;
  
  float2 uv = input.Texcoord;
  
  float4 albedo = float4(AlbedoTex.Sample(gLinearSample, uv).zyx, 1.0f);
  
  output.WorldPosition  = albedo;
  //output.WorldPosition  = input.WorldPos;
  //output.WorldPosition  = float4(uv, 0.0f, 1.0f);
  output.Metallic       = float4(1.0f, 0.0f, 0.0f, 1.0f);
  output.Roughness      = float4(0.0f, 1.0f, 0.0f, 1.0f);
  output.Emissive       = float4(0.0f, 0.0f, 1.0f, 1.0f);
  
  return output;
}