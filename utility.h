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
	unsigned looped;
	cyclic_iterator(It beg, It end, It start)
		: it_beg(beg)
		, it_end(end)
		, i(start)
		, looped(0)
	{
	}
	cyclic_iterator& operator++()
	{
		if (++i == it_end) {
			i = it_beg;
			++looped;
		}
		return *this;
	}
	auto operator*() const -> decltype(*i)&
	{
		return *i;
	}
	bool operator==(const cyclic_iterator<It>& o) const
	{
		return it_beg == o.it_beg && it_end == o.it_end && i == o.i;
	}
	bool operator!=(const cyclic_iterator<It>& o) const
	{
		return !operator==(o);
	}
};
