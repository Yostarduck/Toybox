cbuffer ConstantBuffer : register(b0) {
	float4x4 World;
  float4x4 View;
  float4x4 Projection;
};

struct VS_INPUT {
  float4 Position : POSITION;
  float4 Normal   : NORMAL;
  float4 Binormal : BINORMAL;
  float4 Tangent  : TANGENT;
  float2 Texcoord : TEXCOORD;
};

struct VS_OUTPUT {
  float4 Position : SV_POSITION;
	float4 WorldPos : TEXCOORD0;
  float2 Texcoord : TEXCOORD1;
};

VS_OUTPUT
main(VS_INPUT input) {
  VS_OUTPUT Output;

  const float4x4 WVP = mul(mul(Projection, View), World);

  Output.Position = mul(WVP, input.Position);
  Output.WorldPos = float4(mul(World, input.Position).xyz, 1.0f);
  Output.Texcoord = input.Texcoord;
  
  return Output;
}