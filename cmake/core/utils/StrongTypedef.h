#ifndef STRONGTYPEDEF_H
#define STRONGTYPEDEF_H

#include <utility>

template<typename Tag, typename Base>
class StrongTypedef
{
public:
	using TypeTag = Tag;
	using value_type = Base;

	// Constructors
	StrongTypedef() = default;

	explicit StrongTypedef(const Base& v)
		: m_value(v)
	{
	}
	explicit StrongTypedef(Base&& v)
		: m_value(std::move(v))
	{
	}

	template<typename... Args>
	explicit StrongTypedef(Args&&... args)
		: m_value(std::forward<Args>(args)...)
	{
	}

	// Access operators
	Base& operator*() & { return m_value; }
	const Base& operator*() const& { return m_value; }
	Base&& operator*() && { return std::move(m_value); }

	Base* operator->() { return &m_value; }
	const Base* operator->() const { return &m_value; }

private:
	Base m_value;
};

#endif // STRONGTYPEDEF_H
