//
// Created by ivan on 5/25/25.
// #define BOOST_TEST_DYN_LINK
#include "GLMesaTestFixture.h"

#include <boost/test/unit_test.hpp>
#include <programs/Shader.h>
#include <rendering/opengl/core/evaluators/ComputeEvaluator.h>
#include <glad/glad.h>

#include "programs/Program.h"

using namespace core_gfx::open_gl;

BOOST_AUTO_TEST_SUITE(TestComputeShader)

BOOST_FIXTURE_TEST_CASE(TryComputeShaderInitialization, GLMesaTestFixture)
{
	GLuint buf{ 0 };
	glGenBuffers(1, &buf);
	BOOST_REQUIRE(buf != 0);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	GLuint source[] = { 1, 2, 3, 4, 5 };
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(source), &source, GL_STATIC_READ);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	GLuint dest[] = { 0, 0, 0, 0, 0 };
	BOOST_CHECK(sizeof(dest) == sizeof(source));
	glGetBufferSubData(GL_SHADER_STORAGE_BUFFER, 0, sizeof(dest), dest);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	BOOST_TEST(memcmp(source, dest, sizeof(dest)) == 0);
	GLuint* dstMap = static_cast<GLuint*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
	BOOST_TEST(memcmp(dest, dstMap, sizeof(dest)) == 0);
	BOOST_TEST(memcmp(source, dstMap, sizeof(source)) == 0);
	glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
}

BOOST_FIXTURE_TEST_CASE(TryComputeShaderSumBuffers, GLMesaTestFixture)
{
	GLuint buf[3];
	glGenBuffers(3, buf);
	GLint in1[] = { 1, 2, 3, 4, 5 };
	GLint in2[] = { 5, 4, 3, 2, 1 };
	GLint out[] = { 6, 6, 6, 6, 6 };
	GLint empty[] = { 2321, 4213, 4251, 22123, 32124 };

	Shader<GL_COMPUTE_SHADER> computeShader(std::string(
		""
		"#version 430 core\n"
		"layout(local_size_x = 1) in;"
		"layout(binding = 0) buffer in1 { int v1[]; };"
		"layout(binding = 1) buffer in2 { int v2[]; };"
		"layout(binding = 2) buffer rbuf { int r[]; };"
		"void main() {"
		"   r[gl_GlobalInvocationID.x] = v1[gl_GlobalInvocationID.x] + v2[gl_GlobalInvocationID.x];"
		"}\n"
		""));
	Program program;
	program.Setup(computeShader);
	program.Use();

	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf[0]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(in1), in1, GL_STATIC_READ);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf[1]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(in2), in2, GL_STATIC_READ);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf[2]);
	glBufferData(GL_SHADER_STORAGE_BUFFER, sizeof(empty), empty, GL_STATIC_DRAW);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	{
		auto testBuf = static_cast<GLint*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
		BOOST_REQUIRE(memcmp(testBuf, out, sizeof(out)) != 0);
		BOOST_REQUIRE(memcmp(testBuf, empty, sizeof(empty)) == 0);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}

	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, buf[0]);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 1, buf[1]);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 2, buf[2]);
	BOOST_TEST(glGetError() == GL_NO_ERROR);

	glDispatchCompute(std::size(in1), 1, 1);
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
	BOOST_TEST(glGetError() == GL_NO_ERROR);
	{
		glBindBuffer(GL_SHADER_STORAGE_BUFFER, buf[2]);
		const auto res = static_cast<GLint*>(glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_READ_ONLY));
		BOOST_REQUIRE(glGetError() == GL_NO_ERROR);
		BOOST_TEST(memcmp(res, out, sizeof(out)) == 0);
		glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
	}
}

BOOST_FIXTURE_TEST_CASE(TestComputeEvaluatorSumBuffers, GLMesaTestFixture)
{
	// Create input and output buffers using StrongTypedef wrappers
	InputBuffer<int> in1(std::vector<int>{ 1, 2, 3, 4, 5 });
	InputBuffer<int> in2(std::vector<int>{ 5, 4, 3, 2, 1 });
	OutputBuffer<int> out(std::vector<int>{ 0, 0, 0, 0, 0 });
	std::vector<int> expected = { 6, 6, 6, 6, 6 };

	// Create compute shader that sums two buffers
	ComputeShader computeShader(std::string(
		"#version 430 core\n"
		"layout(local_size_x = 1) in;"
		"layout(binding = 0) buffer in1 { int v1[]; };"
		"layout(binding = 1) buffer in2 { int v2[]; };"
		"layout(binding = 2) buffer rbuf { int r[]; };"
		"void main() {"
		"   r[gl_GlobalInvocationID.x] = v1[gl_GlobalInvocationID.x] + v2[gl_GlobalInvocationID.x];"
		"}\n"));
	Program prog;
	prog.Setup(computeShader);
	prog.Use();
	// Create evaluator and run computation
	ComputeEvaluator evaluator;
	evaluator.Evaluate(std::array{ static_cast<int>(in1->size()), 1, 1 }, in1, in2, out);
	BOOST_REQUIRE(glGetError() == GL_NO_ERROR);
	// Verify results
	BOOST_TEST(*out == expected, boost::test_tools::per_element());
}

BOOST_AUTO_TEST_SUITE_END()
