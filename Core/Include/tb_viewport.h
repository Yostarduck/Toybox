#pragma once
#include "tb_core_prerequisites.h"

namespace toyboxSDK {

struct TB_CORE_EXPORT Viewport
{
	float topLeftX;
	float topLeftY;
	UInt32 width;
	UInt32 height;
	float minDepth;
	float maxDepth;
};

}
