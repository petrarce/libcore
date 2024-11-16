
#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include <buffers/VertexArrayBuffer.h>

#include "GLMesaTestFixture.h"

using namespace core_gfx::open_gl;

BOOST_AUTO_TEST_SUITE(TestArrayBuffers)

BOOST_FIXTURE_TEST_CASE(ArrayBuffer, GLMesaTestFixture)
{

	VertexArrayBuffer buf;
	buf.Init(std::vector<float>{ 1, 2, 3, 4, 5, 6 }, 2,
			 std::vector<int32_t>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }, 3);
	buf.Use();
	buf.Unuse();
	buf.Deinit();
	BOOST_TEST(true /* test assertion */);
}

BOOST_AUTO_TEST_SUITE_END()
