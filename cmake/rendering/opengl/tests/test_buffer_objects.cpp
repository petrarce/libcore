//
// Created by ivan on 5/28/25.
//

//
// Created by ivan on 5/25/25.
// #define BOOST_TEST_DYN_LINK
#include "GLMesaTestFixture.h"

#include <boost/test/unit_test.hpp>
#include <glad/glad.h>
#include <thread>
#include <chrono>
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

BOOST_FIXTURE_TEST_CASE(TestMapBufferRAII, GLMesaTestFixture)
{
	const std::vector<int> data = { 10, 20, 30 };
	ShaderStorageBuffer buffer(data);

	// First mapping scope
	void* firstPtr = nullptr;
	{
		auto map1 = MapBuffer(buffer);
		firstPtr = map1.RawPointer();
		BOOST_REQUIRE(firstPtr != nullptr);
		BOOST_TEST(memcmp(map1.As<int>(), data.data(), buffer.GetSize()) == 0);
	}

	// Second mapping scope
	void* secondPtr = nullptr;
	{
		auto map2 = MapBuffer(buffer);
		secondPtr = map2.RawPointer();
		BOOST_REQUIRE(secondPtr != nullptr);
		BOOST_TEST(memcmp(map2.As<int>(), data.data(), buffer.GetSize()) == 0);
	}

	BOOST_REQUIRE(glGetError() == GL_NO_ERROR); // No GL errors after unmapping

	// Verify we can map again after previous mappings were destroyed
	{
		auto map3 = MapBuffer(buffer);
		BOOST_REQUIRE(map3.RawPointer() != nullptr);
	}
}

BOOST_FIXTURE_TEST_CASE(TestMultipleMappings, GLMesaTestFixture)
{
	const std::vector<int> data = { 1, 2, 3 };
	ShaderStorageBuffer buffer(data);

	// First mapping
	auto map1 = MapBuffer(buffer);
	BOOST_REQUIRE(map1.RawPointer() != nullptr);

	// Second mapping while first is still active
	auto map2 = MapBuffer(buffer);
	BOOST_REQUIRE(map2.RawPointer() != nullptr);
	BOOST_TEST(map2.RawPointer() == map1.RawPointer());

	// Third mapping
	{
		auto map3 = MapBuffer(buffer);
		BOOST_REQUIRE(map3.RawPointer() != nullptr);
		BOOST_TEST(map3.RawPointer() == map1.RawPointer());
	}

	// First two mappings still valid
	BOOST_TEST(memcmp(map1.As<int>(), data.data(), buffer.GetSize()) == 0);
	BOOST_TEST(memcmp(map2.As<int>(), data.data(), buffer.GetSize()) == 0);

	BOOST_REQUIRE(glGetError() == GL_NO_ERROR);
}

BOOST_AUTO_TEST_SUITE_END()
