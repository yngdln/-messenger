#pragma once

#include <iostream>
#include <concepts>
#include "Iterator.h"

template<typename It>
concept RandomAccessIterator = requires(It i, It j, int n)
{
	{ i++ }->std::same_as<It>;
	{ i-- }->std::same_as<It>;
	{ ++i }->std::same_as<It&>;
	{ --i }->std::same_as<It&>;

	{ i == j }->std::convertible_to<bool>;
	{ i != j }->std::convertible_to<bool>;

	{ *i }->std::same_as<std::iter_reference_t<It>>;

};

template<typename Iter, typename Func>
concept PredicateReq = requires(Func pred, Iter it)
{
	{pred(*it)}->std::convertible_to<bool>;
};

template<typename Iter, typename Func>
concept LambdaReq = requires(Func lambda, Iter it)
{
	lambda(*it);
};

template<typename Iter, typename Func>
requires RandomAccessIterator<Iter>&& PredicateReq<Iter, Func>
Iter findIf(Iter begin, Iter end, Func pred)
{
	while (begin != end)
	{
		if (pred(*begin))
		{
			break;
		}
		++begin;
	}
	Iter result(begin);
	return result;
}

template<typename Iter>
requires RandomAccessIterator<Iter>
Iter minElement(Iter begin, Iter end)
{
	Iter min = begin;
	while (begin != end)
	{
		if (*begin < *min)
		{
			min = begin;
		}
		++begin;
	}
	return min;
}

template<typename Iter, typename Func>
requires RandomAccessIterator<Iter>&& LambdaReq<Iter, Func>
Iter minElement(Iter begin, Iter end, Func func)
{
	Iter res = begin;
	while (begin != end)
	{
		if (func(*begin, *res))
		{
			res = begin;
		}
		begin++;
	}
	return res;
}
\
template<typename Iter>
requires RandomAccessIterator<Iter>
Iter maxElement(Iter begin, Iter end)
{
	Iter max = begin;
	while (begin != end)
	{
		if (*begin > *max)
		{
			max = begin;
		}
		++begin;
	}

	return max;
}

template<typename Iter, typename Func>
requires RandomAccessIterator<Iter>&& LambdaReq<Iter, Func>
Iter maxElement(Iter begin, Iter end, Func func)
{
	Iter res = begin;
	while (begin != end)
	{
		if (func(*begin, *res)) {
			res = begin;
		}
		begin++;
	}
	return res;

}

template<typename Iter, typename Func>
requires RandomAccessIterator<Iter>&& LambdaReq<Iter, Func>
void forEach(Iter begin, Iter end, Func op)
{
	while (begin != end)
	{
		op(*begin);
		begin++;

	}
}

template <typename Iter, typename Func>
requires RandomAccessIterator<Iter>&& LambdaReq<Iter, Func>
void Sort(Iter begin, Iter end, Func func)
{
	for (; begin != end; ++begin)
	{
		std::iter_swap(begin, minElement(begin, end, func));
	}
}

template <typename Iter>
requires RandomAccessIterator<Iter>
void Sort(Iter begin, Iter end)
{
	while (begin != end)
	{
		std::iter_swap(begin, minElement(begin, end));
		++begin;
	}
}


template<typename Iter, typename OtherIter, typename Func>
requires RandomAccessIterator<Iter>&& RandomAccessIterator<OtherIter>&& PredicateReq<Iter, Func>
OtherIter copyIf(Iter begin, Iter end, OtherIter beg, Func pred)
{
	while (begin != end)
	{
		if (pred(*begin))
		{
			*beg = *begin;

			++beg;
		}
		++begin;
	}
	return beg;
}

