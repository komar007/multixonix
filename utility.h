#pragma once

#include <stdexcept>
#include <iterator>

//! \brief Iterator wrapper that comes back to the beginning of a collection
//!
//! it_beg is the beginning of the collection, it_end - one element past the
//! end, when i == it_end, i is automatically reset to it_beg.
//! To make end-of-collection checking possible, there is a special end
//! iterator: one where i == it_end.
template <typename It>
class cyclic_iterator {
private:
	It it_beg;
	It it_end;
	It i;
	short cycles;
public:
	cyclic_iterator(It beg, It end, It start)
		: it_beg(beg)
		, it_end(end)
		, i(start)
		, cycles(0)
	{
	}
	cyclic_iterator& operator++()
	{
		if (++i == it_end) {
			i = it_beg;
			++cycles;
		}
		return *this;
	}
	auto operator*() const -> decltype(*i)&
	{
		return *i;
	}
	bool operator==(const cyclic_iterator<It>& o) const
	{
		return  i == o.i ||
			(o.i == it_end && i == it_beg && cycles > 0) ||    	// o is end
			(i == o.it_end && o.i == o.it_beg && o.cycles > 0);	// *this is end
	}
	bool operator!=(const cyclic_iterator<It>& o) const
	{
		return !operator==(o);
	}

	//! Iterator move. Does not support cycling!
	cyclic_iterator operator+(size_t o) const
	{
		return cyclic_iterator(it_beg, it_end, i+o);
	}
};

template <typename It>
struct dereference {
	auto operator()(const It& it) -> decltype(*it)
	{
		return *it;
	}
};
