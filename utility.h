#pragma once

//! @file
//! \brief Useful standard declarations

#include <stdexcept>
#include <iterator>

extern const double pinf, ninf;

template <typename It>
struct dereference {
	auto operator()(const It& it) -> decltype(*it)
	{
		return *it;
	}
};

template <typename T1, typename T2>
struct take_second {
	const T2& operator()(const std::pair<T1, T2>& p)
	{
		return p.second;
	}
};
template <typename T1, typename T2>
struct take_first {
	const T1& operator()(const std::pair<T1, T2>& p)
	{
		return p.first;
	}
};
