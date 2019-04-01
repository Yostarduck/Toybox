Texture2D WorldPosition     : register(t0);
Texture2D MetallicTexture   : register(t1);
Texture2D RoughnessTexture  : register(t2);
Texture2D EmissiveTexture   : register(t3);
Texture2D DepthTexture      : register(t4);

SamplerState gLinearSample : register(s0);

struct PS_INPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

float4
main(PS_INPUT input) : SV_TARGET0 {
  float2 uv = input.Texcoord;

  return float4(WorldPosition.Sample(gLinearSample, uv).xyz, 1.0f);
  //return float4(RoughnessTexture.Sample(gLinearSample, uv).xyz, 1.0f);
}
