Texture2D ColorTex : register(t0);

SamplerState gLinearSample : register(s0);

RWTexture2D<float4> ResultRT : register(u0);

cbuffer ConstantBuffer : register(b0) {
	float2 ScreenSize;
};

[numthreads(32, 32, 1)]
void
main(uint3 groupThreadID	: SV_GroupThreadID,
	 uint3 groupID			: SV_GroupID,
	 uint3 dispatchID		: SV_DispatchThreadID,
	 uint  groupIndex		: SV_GroupIndex) {
	const float2 uvScale = float2(dispatchID.xy);
	const float2 uv = float2(dispatchID.xy) * rcp(float2(640.0f, 640.0f));

	const float3 color = ColorTex.SampleLevel(gLinearSample, uv, 0).xyz;
	
	ResultRT[uvScale] = float4(color, 1.0f);
}