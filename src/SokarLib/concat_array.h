//
// Created by adam on 07.04.2021.
//
#pragma once
#include <array>
#include <tuple>

// https://stackoverflow.com/questions/42749032/concatenating-a-sequence-of-stdarrays

template<std::size_t Last = 0, typename TF, typename TArray, typename... TRest>
constexpr auto with_acc_sizes(TF &&f, const TArray &array, const TRest &... rest) {
	f(array, std::integral_constant<std::size_t, Last>{});

	if constexpr(sizeof...(TRest) != 0) {
		with_acc_sizes<Last + std::tuple_size_v<TArray>>(f, rest...);
	}
}


template<typename T, std::size_t... Sizes>
constexpr auto concat_array(const std::array<T, Sizes> &... arrays) {
	std::array<T, (Sizes + ...)> result{};

	with_acc_sizes([&](const auto &arr, auto offset) {
		std::copy(arr.begin(), arr.end(), result.begin() + offset);
	}, arrays...);

	return result;
}