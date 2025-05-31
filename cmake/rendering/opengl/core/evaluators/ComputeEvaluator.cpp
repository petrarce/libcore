//
// Created by ivan on 5/29/25.
//

#include "ComputeEvaluator.h"
namespace core_gfx::open_gl
{

ComputeEvaluator::ComputeEvaluator(ComputeShader&& computeShader)
	: mCs(std::move(computeShader))
{
}
} // namespace core_gfx::open_gl
