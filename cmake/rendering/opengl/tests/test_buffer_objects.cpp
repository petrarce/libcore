//
// Created by ivan on 5/28/25.
//

//
// Created by ivan on 5/25/25.
// #define BOOST_TEST_DYN_LINK
#include "GLMesaTestFixture.h"

#include <boost/test/unit_test.hpp>
#include <glad/glad.h>
#include <buffers/BufferObject.h>

using namespace core_gfx::open_gl;

BOOST_AUTO_TEST_SUITE(TestBufferObjects)

BOOST_FIXTURE_TEST_CASE(TestBufferObjectCreationAndMap, GLMesaTestFixture)
{
	const auto buf = std::vector<int>({ 24, 42, 13, 31, 26 });
	ShaderStorageBuffer buffer(buf, GL_STATIC_READ);
	// Test Map
	{
		auto mapBuffer = MapBuffer(buffer);
		const auto bufPtr = static_cast<const int*>(mapBuffer.RawPointer());
		BOOST_REQUIRE(glGetError() == GL_NO_ERROR);
		BOOST_TEST(memcmp(bufPtr, buf.data(), sizeof(int) * buf.size()) == 0);
	}
	// Test Remap
	{
		auto mapBuffer = MapBuffer(buffer);
		const auto bufPtr = static_cast<const int*>(mapBuffer.RawPointer());
		BOOST_REQUIRE(glGetError() == GL_NO_ERROR);
		BOOST_TEST(memcmp(bufPtr, buf.data(), sizeof(int) * buf.size()) == 0);
	}
}
BOOST_AUTO_TEST_SUITE_END()
