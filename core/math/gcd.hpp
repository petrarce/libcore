#ifndef gcd_xpp
#define gcd_xpp
namespace libcore
{
namespace math
{

template <class T> T gcd (const T &a, const T &b)
{
	auto aa = a > b ? a : b;
	auto bb = a > b ? b : a;

	while (aa % bb != 0)
	{
		const auto bbb = bb;
		bb = aa % bb;
		aa = bbb;
	}
	return bb;
}

} // namespace math
} // namespace libcore
#endif
