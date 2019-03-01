cbuffer ConstantBuffer {
	float2 ScreenSize;
};

[numthreads(32, 32, 1)]
void
main(uint3 groupThreadID  : SV_GroupThreadID,
		 uint3 groupID				: SV_GroupID,
		 uint3 dispatchID			: SV_DispatchThreadID,
		 uint  groupIndex			: SV_GroupIndex) {
	const float2 uv = float2(dispatchID.x, dispatchID.y) * rcp(ScreenSize);
}