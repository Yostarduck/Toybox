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
  
  
  Output.Position  = mul(WVP, vertexTransformed);
  output.WorldPosition  = input.WorldPos;
  output.Metallic       = float4(1.0f, 0.0f, 0.0f, 1.0f);
  output.Roughness      = float4(0.0f, 1.0f, 0.0f, 1.0f);
  output.Emissive       = float4(0.0f, 0.0f, 1.0f, 1.0f);
  
  return output;
}