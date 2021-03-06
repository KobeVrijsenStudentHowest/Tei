#pragma once

#include <tuple>
#include <typeinfo>
#include <tei/internal/ecs.h>

namespace tei::internal::components
{

	template <typename ... Components>
	class RefTuple
	{
	public:

		constexpr RefTuple() noexcept
			: m_Data{ *static_cast<Components*>(nullptr) ... }
		{}

		template <typename Component> requires (std::same_as<Component, Components> || ...)
		constexpr Component& get() const noexcept
		{
			return std::get<std::reference_wrapper<Component>>(as_tuple());
		}
		
		template <size_t INDEX> requires (INDEX < sizeof...(Components))
		constexpr auto& get() const noexcept
		{
			return std::get<INDEX>(as_tuple()).get();
		}

		constexpr std::tuple<std::reference_wrapper<Components>...>& as_tuple()
		{
			return m_Data;
		}
		constexpr std::tuple<std::reference_wrapper<Components>...> const& as_tuple() const
		{
			return m_Data;
		}

		template <typename Component> requires (std::same_as<Component, Components> || ...)
		constexpr operator Component& () const noexcept
		{
			return get<Component>();
		}

	private:

		std::tuple<std::reference_wrapper<Components>...> m_Data;

	};

	namespace detail
	{
		void ExceptRefNotValid(std::type_info const&);
	}

	//template <size_t N>
	//struct Name
	//{
	//	constexpr Name(char const (&name)[N + 1]) noexcept
	//	{
	//		for (size_t i{}; i < N; ++i)
	//			_name[i] = name[i];
	//	}
	//	char _name[N]{};
	//};

	//template <size_t N>
	//Name(char const (&)[N]) -> Name<N - 1>;

	template <typename ... Components>
	class RefComponent
	{
	public:

		constexpr RefComponent() noexcept = default;

		constexpr RefTuple<Components...>& Refs() noexcept
		{
			return m_Refs;
		}
		
		constexpr RefTuple<Components...> const& Refs() const noexcept
		{
			return m_Refs;
		}

	private:

		RefTuple<Components...> m_Refs{};

	};

}

namespace tei::external::components
{
	using tei::internal::components::RefComponent;
}

template <typename ... Components>
struct std::tuple_size<tei::internal::components::RefTuple<Components...>> : std::integral_constant<size_t, sizeof...(Components)>
{};

template <size_t INDEX, typename ... Components>
struct std::tuple_element<INDEX, tei::internal::components::RefTuple<Components...>> : std::tuple_element<INDEX, std::tuple<Components&...>>
{};


template <typename ... Components>
inline void OnEnable(std::nullptr_t, tei::internal::components::RefComponent<Components...>& refcomp, tei::internal::ecs::Object& object)
{
	auto const getter = [&object] <typename Component> (std::reference_wrapper<Component>& ref)
	{
		if (auto* pData = object.HasComponent<Component>())
		{
			ref = *pData;
		}
		else
		{
			if constexpr (std::default_initializable<Component>)
				ref = object.AddComponent<Component>();
			else
				tei::internal::components::detail::ExceptRefNotValid(typeid(Component));
		}
	};
	(getter(std::get<std::reference_wrapper<Components>>(refcomp.Refs().as_tuple())), ...);
}
