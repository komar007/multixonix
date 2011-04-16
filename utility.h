#pragma once

#include <stdexcept>
#include <iterator>

template <typename It>
class cyclic_iterator {
private:
	It it_beg;
	It it_end;
	It i;
public:
	cyclic_iterator(It beg, It end, It start)
		: it_beg(beg)
		, it_end(end)
		, i(start)
	{
	}
	cyclic_iterator& operator++()
	{
		if (++i == it_end)
			i = it_beg;
		return *this;
	}
	auto operator*() const -> decltype(*i)&
	{
		return *i;
	}
	bool operator==(const cyclic_iterator<It>& o) const
	{
		return  i == o.i;
	}
	bool operator!=(const cyclic_iterator<It>& o) const
	{
		return !operator==(o);
	}
};
