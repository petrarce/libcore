//
// Created by ivan on 9/5/25.
//

#ifndef RING_BUFFER_H
#define RING_BUFFER_H
#include <stdexcept>
#include <vector>

namespace libcore
{
namespace utils
{

template<class T>
class ring_buffer
{
public:
	explicit ring_buffer(int capacity)
		// element to which end points will always be empty
		: buf(capacity + 1)
		, start(0)
		, end(0)
	{
	}
	void push_back(const T& elem)
	{
		buf[end] = elem;
		end = (end + 1) % buf.size();
	}
	T front() const { return buf[start]; }
	T back() const { return buf[!end ? buf.size() - 1 : end - 1]; }
	void pop_front() { start = (start + 1) % buf.size(); }
	int size() const
	{
		const int baseSize = end - start;
		return baseSize + (baseSize < 0) * buf.size();
	}

private:
	std::vector<T> buf;
	int start; // index of the first element
	int end;   // index of the last element
};

} // namespace utils
} // namespace libcore

#endif // RING_BUFFER_H
