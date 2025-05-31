//
// Created by ivan on 5/29/25.
//
#ifndef COMPUTEEVALUATOR_H
#define COMPUTEEVALUATOR_H
#include "programs/Shader.h"
#include <core/utils/StrongTypedef.h>
#include <rendering/opengl/core/buffers/BufferObject.h>

namespace core_gfx::open_gl
{
namespace detail
{
struct InputBufferTag
{
};
struct OutputBufferTag
{
};
} // namespace detail

template<class T>
using InputBuffer = StrongTypedef<detail::InputBufferTag, std::vector<T> >;
template<class T>
using OutputBuffer = StrongTypedef<detail::OutputBufferTag, std::vector<T> >;

template<class T>
concept IsInputBuffer = std::is_same_v<T::TypeTag, detail::InputBufferTag>;
template<class T>
concept IsOutputBuffer = std::is_same_v<T::TypeTag, detail::OutputBufferTag>;

class ComputeEvaluator
{

	explicit ComputeEvaluator(ComputeShader&& computeShader);

	template<class... InputsOutputs>
	void Evaluate(InputsOutputs&&... buffers, const std::array<int, 3> dispatchConfig)
	{
		auto _tie = std::tie(buffers...);

		auto inputs = filterBuffer<IsInputBuffer>(_tie);
		auto outputs = filterBuffer<IsOutputBuffer>(_tie);

		auto inputBuffers = SetupInputBuffers(inputs);
		auto outputBuffers = SetupOutputBuffers(outputs);

		// bind locations for the input and ouput buffers
		for (int i = 0; i < inputBuffers.size(); ++i)
			inputBuffers[i].BindLocationIndexBase(i);
		for (int i = 0; i < outputBuffers.size(); ++i)
			outputBuffers[i].BindLocationIndexBase(i + inputBuffers.size());

		glDispatchCompute(dispatchConfig[0], dispatchConfig[1], dispatchConfig[2]);
		glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);

		// Load all opengl buffers back to the supplied output buffers
		[&]<size_t... I>(std::index_sequence<I...>)
		{
			((outputBuffers[I].LoadData(std::get<I>(outputs))), ...);
		}
		(std::make_index_sequence<std::tuple_size(outputs)>{});
	}

private:
	template<template<class> typename Constraint, template<class...> class Tuple, class... Args>
	auto filterBuffer(Tuple<Args...>&& tpl)
	{
		return [&]<size_t... I>(std::index_sequence<I...>)
		{
			return std::tuple_cat(
				[&]<size_t Index>()
				{
					using ElemT = std::remove_cvref_t<
						std::tuple_element_t<Index, std::remove_cvref_t<Tuple<Args...> > > >;
					if constexpr (Constraint<ElemT>())
						return std::forward_as_tuple(std::get<Index>(tpl));
					return std::tuple<>();
				}.template operator()<I>()...);
		}
		(std::index_sequence_for<Args...>{});
	}
	template<IsInputBuffer... Inputs>
	auto SetupInputBuffers(const Inputs&... inputs)
	{
		return std::vector<ShaderStorageBuffer>({ ShaderStorageBuffer(inputs)... });
	}

	template<IsOutputBuffer... Outputs>
	auto SetupOutputBuffers(const Outputs&... outputs)
	{
		return std::vector<ShaderStorageBuffer>({ ShaderStorageBuffer(outputs)... });
	}

	ComputeShader mCs;
};
} // namespace core_gfx::open_gl
#endif // COMPUTEEVALUATOR_H
