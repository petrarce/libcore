#ifndef GLMESATESTFIXTURE_H
#define GLMESATESTFIXTURE_H

#include <boost/test/unit_test.hpp>
#include <glad/glad.h>
#include <GL/osmesa.h>

class GLMesaTestFixture
{
public:
	GLMesaTestFixture()
	{
		ctx = OSMesaCreateContextExt(GL_RGBA, 16, 0, 0, NULL);
		BOOST_REQUIRE(ctx);

		// Create a buffer for rendering
		const int width = 800, height = 600;
		buffer.resize(width * height * 4, 0); // RGBA

		bool ok = OSMesaMakeCurrent(ctx, buffer.data(), GL_UNSIGNED_BYTE, width, height);
		BOOST_REQUIRE(ok);
		BOOST_REQUIRE(gladLoadGLLoader(reinterpret_cast<GLADloadproc>(OSMesaGetProcAddress)));
	}
	~GLMesaTestFixture() { OSMesaDestroyContext(ctx); }

private:
	OSMesaContext ctx;
	std::vector<unsigned char> buffer;
};

#endif // GLMESATESTFIXTURE_H
