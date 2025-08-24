#include "GameOfLife.h"

#include "rendering/opengl/core/programs/Shader.h"

#include <rendering/opengl/core/evaluators/ComputeEvaluator.h>
#include <rendering/opengl/core/programs/Program.h>

namespace
{
std::string golShader = "\
#version 440 core\n\
layout(local_size_x = 1);\n\
layout(binding = 0) buffer inp { bool inp_v[]; };\n\
layout(binding = 1) buffer outp { bool outp_v[]; };\n\
uniform uint width;\n\
\
uint idx(uint idx, uint offset_x, uint offset_y)\n\
{\n\
	return offset_x * width + y;\n\
}\n\
\
void main() {\n\
	uint baseIdx = gl_GlobalInvocationID.y + width * gl_GlobalInvocationID.x;\n\
	int alive_count =\n\
		inp_v[idx(baseIdx, -1, -1)] +\n\
		inp_v[idx(baseIdx, -1, 0)] +\n\
		inp_v[idx(baseIdx, -1, 1)] +\n\
		inp_v[idx(baseIdx, 0, -1)] +\n\
		inp_v[idx(baseIdx, 0, 1)] +\n\
		inp_v[idx(baseIdx, 1, -1)] +\n\
		inp_v[idx(baseIdx, 1, 0)] +\n\
		inp_v[idx(baseIdx, 1, 1)];\n\
	outp_v[baseIdx] =\n\
		inp_v[baseIdx] && (alive_count >=2 && alive_count <= 3)\n\
		|| (!inp_v[baseIdx] && alive_count == 3);\n\
}\n\
";

}
namespace solutions
{
void GameOfLife::Iterate(GameGrid& grid)
{
	core_gfx::open_gl::ComputeShader shader(golShader);
	core_gfx::open_gl::Program program;
	program.Use();
	core_gfx::open_gl::InputBuffer<uint8_t> in(grid.data(), grid.data() + grid.size());
	core_gfx::open_gl::OutputBuffer<uint8_t> out(grid.size());
	core_gfx::open_gl::ComputeEvaluator evaluator;
	evaluator.Evaluate({ static_cast<int>(grid.rows()), static_cast<int>(grid.cols()), 1 }, in,
					   out);
	grid = Eigen::Map<GameGrid>(out->data(), grid.rows(), grid.cols());
}
} // namespace solutions
