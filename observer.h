#pragma once

//! @file
//! \brief Observer pattern implementation

#include <vector>
#include <algorithm>
#include <stdexcept>

template <typename M>
class Observer;

template <typename M>
class Observable {
private:
	std::vector<Observer<M>*> observers;
protected:
	void notify(const M& msg);
public:
	void attach(Observer<M>& obs);
	void detach(Observer<M>& obs) throw (std::out_of_range);
	virtual ~Observable()
	{
	}
	friend class Observer<M>;
};

template <typename M>
void Observable<M>::notify(const M& msg)
{
	for (auto i = observers.begin(); i != observers.end(); ++i)
		(*i)->update(*this, msg);
}

template <typename M>
void Observable<M>::attach(Observer<M>& obs)
{
	observers.push_back(&obs);
}
template <typename M>
void Observable<M>::detach(Observer<M>& obs) throw (std::out_of_range)
{
	auto obs_it = find(observers.begin(), observers.end(), &obs);
	if (obs_it == observers.end())
		throw std::out_of_range("observer not attached");
	observers.erase(obs_it);
}

template <typename M>
class Observer {
private:
	virtual void update(Observable<M>& obj, const M& msg) = 0;
public:
	friend void Observable<M>::notify(const M& msg);
};
