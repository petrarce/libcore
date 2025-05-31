#ifndef STRONGTYPEDEF_H
#define STRONGTYPEDEF_H

#include <type_traits>

template<typename Tag, typename Base>
class StrongTypedef : public Base
{
public:
	using TypeTag = Tag;
	using Base::Base; // inherit all constructors

	template<typename... Args>
	explicit StrongTypedef(Args&&... args)
		: Base(std::forward<Args>(args)...)
	{
	}
};

#endif // STRONGTYPEDEF_H
