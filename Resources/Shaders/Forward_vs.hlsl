struct VS_INPUT {
  float4 Position : POSITION;
  float2 Texcoord : TEXCOORD;
};

struct VS_OUTPUT {
  float4 Position : SV_POSITION;
  float2 Texcoord : TEXCOORD0;
};

VS_OUTPUT
main(VS_INPUT input) {
  VS_OUTPUT Output;

  Output.Position = input.Position;
  Output.Texcoord = input.Texcoord;
  
  return Output;
}