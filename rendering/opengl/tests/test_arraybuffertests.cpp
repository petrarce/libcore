
#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include <buffers/VertexArrayBuffer.h>
#include "GLMesaTestFixture.h"

namespace std
{
template<class T>
std::ostream& operator<<(std::ostream& s, const std::vector<T>& v)
{
	s << "[";
	for (const auto& val : v)
		s << val << ", ";
	return s << "]";
}
} // namespace std
using namespace core_gfx::open_gl;

BOOST_AUTO_TEST_SUITE(TestArrayBuffers)

BOOST_FIXTURE_TEST_CASE(ArrayBuffer, GLMesaTestFixture)
{

	VertexArrayBuffer buf;
	buf.Init(std::vector<float>{ 1, 2, 3, 4, 5, 6 }, 2,
			 std::vector<int32_t>{ 1, 2, 3, 4, 5, 6, 7, 8, 9 }, 3);
	buf.Use();
	auto expAttribs = std::vector<GLint>{ 0, 1 };
	BOOST_CHECK_EQUAL(buf.ListEnabledAttributes(), expAttribs);
	buf.Unuse();
	buf.Deinit();
	BOOST_CHECK_NE(buf.ListEnabledAttributes(), expAttribs);
	expAttribs = std::vector<GLint>{};
	BOOST_CHECK_EQUAL(buf.ListEnabledAttributes(), expAttribs);
}

BOOST_AUTO_TEST_SUITE_END()
