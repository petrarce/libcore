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

BOOST_FIXTURE_TEST_CASE(TestBufferObjectCreation, GLMesaTestFixture)
{
	const std::vector<int> data = { 1, 2, 3, 4, 5 };
	ShaderStorageBuffer buffer(data, GL_STATIC_DRAW);

	BOOST_TEST(buffer.GetSize() == data.size() * sizeof(int));
	BOOST_REQUIRE(glGetError() == GL_NO_ERROR);
}

BOOST_FIXTURE_TEST_CASE(TestBufferObjectMoveSemantics, GLMesaTestFixture)
{
	const std::vector<float> data = { 1.1f, 2.2f, 3.3f };
	ShaderStorageBuffer buffer1(data);

	// Test move constructor
	ShaderStorageBuffer buffer2(std::move(buffer1));
	BOOST_TEST(buffer2.GetSize() == data.size() * sizeof(float));

	// Test move assignment
	ShaderStorageBuffer buffer3 = std::move(buffer2);
	BOOST_TEST(buffer3.GetSize() == data.size() * sizeof(float));
}

BOOST_FIXTURE_TEST_CASE(TestBufferObjectUpdate, GLMesaTestFixture)
{
	std::vector<int> data = { 10, 20, 30 };
	ShaderStorageBuffer buffer(data);
	{
		auto map = MapBuffer(buffer);
		BOOST_TEST(memcmp(map.As<int>(), data.data(), buffer.GetSize()) == 0);
	}
	const std::vector<int> newData = { 40, 50, 60 };
	buffer.UpdateData(newData);

	{
		auto mapBuffer = MapBuffer(buffer);
		BOOST_TEST(memcmp(mapBuffer.As<int>(), newData.data(), buffer.GetSize()) == 0);
	}
}

BOOST_FIXTURE_TEST_CASE(TestMapBufferFunctionality, GLMesaTestFixture)
{
	const std::vector<double> data = { 1.5, 2.5, 3.5 };
	ShaderStorageBuffer buffer(data);

	{
		auto mapBuffer = MapBuffer(buffer, GL_READ_ONLY);
		const double* mappedData = mapBuffer.As<double>();

		BOOST_REQUIRE(mapBuffer.RawPointer() != nullptr);
		BOOST_TEST(memcmp(mappedData, data.data(), data.size() * sizeof(double)) == 0);
	}

	// Verify buffer is properly unmapped
	BOOST_REQUIRE(glGetError() == GL_NO_ERROR);
}

BOOST_FIXTURE_TEST_CASE(TestMapBufferMoveSemantics, GLMesaTestFixture)
{
	const std::vector<int> data = { 100, 200, 300 };
	ShaderStorageBuffer buffer(data);

	auto mapBuffer1 = MapBuffer(buffer);
	void* originalPtr = mapBuffer1.RawPointer();

	// Test move constructor
	auto mapBuffer2 = std::move(mapBuffer1);
	BOOST_TEST(mapBuffer2.RawPointer() == originalPtr);

	// Test move assignment
	auto mapBuffer3 = std::move(mapBuffer2);
	BOOST_TEST(mapBuffer3.RawPointer() == originalPtr);
}

BOOST_AUTO_TEST_SUITE_END()
