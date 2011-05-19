#pragma once

//! @file
//! \brief Special cyclic operator for vector used throughout the project

#include <vector>
#include <unordered_map>
#ifdef DEBUG
	#include <sstream>
#endif

//! \brief vector cyclic iterator
//!
//! Iterator used to iterate over all points of a vector, which comes
//! back to the beginning and never goes out of range. This allows
//! easy iteration over all edges of a path using two iterators.
template <typename T>
class cyclic_iterator {
protected:
	const std::vector<T>* p;
	int i;
	short cycles;
	cyclic_iterator(const std::vector<T>& _p, int start, short _cycles)
		: p(&_p)
		, i(start)
		, cycles(_cycles)
	{
	}
public:
	cyclic_iterator(const std::vector<T>& _p, int start)
		: p(&_p)
		, i(start)
		, cycles(0)
	{
	}
	bool operator==(const cyclic_iterator& o) const
	{
		return i == o.i && cycles >= o.cycles;
	}
	bool operator!=(const cyclic_iterator& o) const
	{
		return !operator==(o);
	}
	const T& operator*() const
	{
#ifdef DEBUG
		if (i >= (int)p->size() || i < 0) {
			std::stringstream ss;
			ss << "iterator out of range: " << i << " (size: " << p->size() << ")";
			throw std::out_of_range(ss.str());
		}
#endif
		return (*p)[i];
	}
	const T* operator->() const
	{
#ifdef DEBUG
		if (i >= (int)p->size() || i < 0) {
			std::stringstream ss;
			ss << "iterator out of range: " << i << " (size: " << p->size() << ")";
			throw std::out_of_range(ss.str());
		}
#endif
		return &(*p)[i];
	}
	int get_index() const
	{
		return i;
	}
};

template <typename T>
class reverse_cyclic_iterator;

template <typename T>
class forward_cyclic_iterator : public cyclic_iterator<T> {
public:
	typedef cyclic_iterator<T> base;
private:
	forward_cyclic_iterator(const std::vector<T>& _p, int start, short _cycles)
		: cyclic_iterator<T>(_p, start, _cycles)
	{
	}
public:
	forward_cyclic_iterator(const reverse_cyclic_iterator<T>& r_it)
		: cyclic_iterator<T>(*r_it.p, r_it.i, r_it.cycles)
	{
	}
	forward_cyclic_iterator next_cycle() const
	{
		return forward_cyclic_iterator(*base::p, base::i, base::cycles+1);
	}
	forward_cyclic_iterator& operator++()
	{
		if (++base::i == (int)base::p->size()) {
			base::i = 0;
			++base::cycles;
		}
		return *this;
	}
	forward_cyclic_iterator(const std::vector<T>& _p, int start)
		: cyclic_iterator<T>(_p, start)
	{
	}
	//! Iterator move.
	forward_cyclic_iterator<T> operator+(size_t o) const
	{
		const int it = base::i + o;
		if (it >= (int)base::p->size())
			return forward_cyclic_iterator<T>(*base::p, it - base::p->size(), base::cycles + 1);
		else
			return forward_cyclic_iterator<T>(*base::p, it, base::cycles);
	}
	friend class reverse_cyclic_iterator<T>;
};

template <typename T>
class reverse_cyclic_iterator : public cyclic_iterator<T> {
public:
	typedef cyclic_iterator<T> base;
private:
	reverse_cyclic_iterator(const std::vector<T>& _p, int start, short _cycles)
		: cyclic_iterator<T>(_p, start, _cycles)
	{
	}
public:
	reverse_cyclic_iterator(const forward_cyclic_iterator<T>& f_it)
		: cyclic_iterator<T>(*f_it.p, f_it.i, f_it.cycles)
	{
	}
	reverse_cyclic_iterator next_cycle() const
	{
		return reverse_cyclic_iterator(*base::p, base::i, base::cycles+1);
	}
	reverse_cyclic_iterator& operator++()
	{
		if (--base::i == -1) {
			base::i = base::p->size() - 1;
			++base::cycles;
		}
		return *this;
	}
	reverse_cyclic_iterator(const std::vector<T>& _p, int start)
		: cyclic_iterator<T>(_p, start)
	{
	}
	//! Iterator move.
	reverse_cyclic_iterator<T> operator+(size_t o) const
	{
		const int it = base::i - o;
		if (it < 0)
			return reverse_cyclic_iterator<T>(*base::p, it + base::p->size(), base::cycles + 1);
		else
			return reverse_cyclic_iterator<T>(*base::p, it, base::cycles);
	}
	friend class forward_cyclic_iterator<T>;
};

namespace std {
	template <typename T>
	class hash<forward_cyclic_iterator<T>> : private std::hash<unsigned long> {
	public:
		size_t operator()(const forward_cyclic_iterator<T>& it) const
		{
			return std::hash<unsigned long>::operator()(reinterpret_cast<unsigned long>(&*it));
		}
	};
	template <typename T>
	class hash<reverse_cyclic_iterator<T>> : private std::hash<unsigned long> {
	public:
		size_t operator()(const reverse_cyclic_iterator<T>& it) const
		{
			return std::hash<unsigned long>::operator()(reinterpret_cast<unsigned long>(&*it));
		}
	};
};

//! \brief const pointer-map iterator
//!
//! const iterator used to iterate over all elements of a map of pointers to
//! elements
template <typename T, typename K = int>
class const_pointer_map_iterator {
private:
	typename std::unordered_map<K, T*>::const_iterator i;
public:
	const_pointer_map_iterator(const typename std::unordered_map<K, T*>::const_iterator& _i)
		: i(_i)
	{
	}
	const_pointer_map_iterator& operator++()
	{
		++i;
		return *this;
	}
	const T& operator*()
	{
		return *i->second;
	}
	const T* operator->()
	{
		return i->second;
	}
	bool operator==(const const_pointer_map_iterator& o)
	{
		return i == o.i;
	}
	bool operator!=(const const_pointer_map_iterator& o)
	{
		return i != o.i;
	}
};

//! \brief non-const pointer-map iterator
//!
//! Iterator used to iterate over all elements of a map of pointers to
//! elements
template <typename T, typename K = int>
class pointer_map_iterator {
private:
	typename std::unordered_map<K, T*>::iterator i;
public:
	pointer_map_iterator(const typename std::unordered_map<K, T*>::iterator& _i)
		: i(_i)
	{
	}
	pointer_map_iterator& operator++()
	{
		++i;
		return *this;
	}
	T& operator*()
	{
		return *i->second;
	}
	T* operator->()
	{
		return i->second;
	}
	bool operator==(const pointer_map_iterator& o)
	{
		return i == o.i;
	}
	bool operator!=(const pointer_map_iterator& o)
	{
		return i != o.i;
	}
};
