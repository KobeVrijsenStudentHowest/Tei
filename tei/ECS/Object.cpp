#include "teipch.h"
#include "Object.h"

#include <algorithm>
#include <cassert>
#include <ranges>

using namespace tei::internal::ecs;
using namespace tei::internal;
namespace r = std::ranges;
namespace v = std::views;

Object::Object(Object* pParent, bool active)
	: m_pParent{ pParent }
	, m_Components{}
	, m_Children{}
	, m_Active{ false }
	, m_State{ active }
	, m_Initialised{}
{}

Object::~Object()
{
	Do(Message::CLEANUP);
}

void Object::SetState(bool state) 
{
	m_State = state;
}

Object& Object::AddChild(bool active)
{
	METRICS_TIMEBLOCK;
	return *m_Children.emplace_back(new Object{ this, active });
}

void Object::RemoveChild(Object const& child)
{
	auto it{ std::ranges::find(m_Children, &child, utility::projectors::to_address{})};
	m_Children.erase(it);
}

void Object::AddComponent(std::type_info const& type, ComponentBase* pComp)
{	
	METRICS_TIMEBLOCK;
	m_Components.push_back({ type, std::unique_ptr<ComponentBase>{ pComp } });
}

Object::ComponentBase* Object::GetComponent(std::type_info const& type) const
{
	METRICS_TIMEBLOCK;
	auto it{ r::find(m_Components | v::keys, type).base() };
	if (it == r::end(m_Components))
		return {};
	else
		return std::to_address(it->second);
}

std::unique_ptr<Object::ComponentBase> tei::internal::ecs::Object::ExtractComponent(std::type_info const& type)
{
	auto it{ r::find(m_Components | v::keys, type).base() };
	if (it == r::end(m_Components))
		ExceptComponentNotFound(type);

	auto uptr{ std::move(it->second) };
	m_Components.erase(it);
	return uptr;
}

template <typename Message>
void DoCall(Object& self, auto& components, auto& children, bool force = false)
{
	for (auto& [type, pComp] : utility::RangePerIndex(components))
		pComp->Do(Message{}, self);

	for (auto& child : utility::RangePerIndex(children))
		if (force || child->IsActive())
			child->Do(Message{});
}

// message == ENABLE || message == CLEANUP || message == UPDATE

void Object::Do(Message::Init)
{
	if (m_Initialised)
		return;

	DoCall<Message::Init>(*this, m_Components, m_Children);

	m_Initialised = true;
}

void Object::Do(Message::Cleanup)
{
	if (!m_Initialised)
		return;

	if (m_Active)
		Do(Message::DISABLE);

	DoCall<Message::Cleanup>(*this, m_Components, m_Children, true);
	
	m_Active = false;
	m_State = false;
	m_Initialised = false;
	m_Children.clear();
	m_Components.clear();
}

void Object::Do(Message::Enable)
{
	if (!m_State)
		return;

	if (!m_Initialised)
		Do(Message::INIT);

	DoCall<Message::Enable>(*this, m_Components, m_Children, true);
	
	m_Active = true;
}


void Object::Do(Message::Disable)
{
	DoCall<Message::Disable>(*this, m_Components, m_Children);
	
	m_Active = false;
}


void Object::Do(Message::Update)
{
	if (m_Active != m_State)
		if (m_State)
			Do(Message::ENABLE);
		else
			Do(Message::DISABLE);

	if (!m_Active)
		return;

	DoCall<Message::Update>(*this, m_Components, m_Children, true);
}

void Object::Do(Message::FixedUpdate)
{
	DoCall<Message::FixedUpdate>(*this, m_Components, m_Children);
}

void Object::Do(Message::Render)
{
	DoCall<Message::Render>(*this, m_Components, m_Children);
}

//{
//	METRICS_TIMEBLOCK;
//
//	using enum Message;
//	switch (message)
//	{
//	case CLEANUP:
//	{
//		if (!m_Initialised)
//			return;
//		if (m_Active)
//			Do(DISABLE);
//	}
//	break;
//	case ENABLE:
//	{
//		if (!m_State)
//			return;
//		if (!m_Initialised)
//			Do(INIT);
//	}
//	break;
//	case UPDATE:
//	{
//		if (m_Active != m_State)
//			Do(m_State ? ENABLE : DISABLE);
//		if (!m_Active)
//			return;
//	}
//	break;
//	case RENDER:
//	{
//		if (!m_Active)
//			return;
//	}
//	case FIXEDUPDATE:
//	{
//		if (!m_Active)
//			return;
//	}
//	break;
//	}
//
//	for (auto& [type, pComp, handle] : utility::RangePerIndex(m_Components)) // False intelisense error
//		handle(*pComp, message, *this);
//
//	for (auto& child : utility::RangePerIndex(m_Children))
//		if (child->m_Active || message == ENABLE || message == CLEANUP || message == UPDATE)
//			child->Do(message);
//
//	switch (message)
//	{
//	case ENABLE:
//	{
//		m_Active = true;
//	}
//	break;
//	case INIT:
//	{
//		m_Initialised = true;
//	}
//	break;
//	case CLEANUP:
//	{
//		m_Active = false;
//		m_State = false;
//		m_Initialised = false;
//		m_Children.clear();
//		m_Components.clear();
//	}
//	break;
//	case DISABLE:
//	{
//		m_Active = false;
//	}
//	break;
//	}
//}

Object::Object(Object const& other)
	: Object{ nullptr, other.m_State }
{	
	m_Initialised = other.m_Initialised;

	m_Components.reserve(std::ranges::size(other.m_Components));
	using cref_type = std::ranges::range_reference_t<decltype(std::as_const(m_Components))>;
	using value_type = std::ranges::range_value_t<decltype(m_Components)>;
	std::ranges::transform(
		other.m_Components,
		std::back_inserter(m_Components),
		[] (cref_type val) -> value_type
		{
			auto const& [type, pValue] = val;
			return { type, pValue->Clone() };
		}
	);

	std::ranges::transform(
		other.m_Children,
		std::back_inserter(m_Children),
		[] (Object const& obj) -> std::unique_ptr<Object>
		{
			return std::unique_ptr<Object>{ new Object{ obj } };
		},
		utility::projectors::to_address<true>{}
	);
}

void Object::ExceptComponentNotFound(std::type_info const& type)
{
	throw utility::TeiRuntimeError{ "No such component found in object", type.name() };
}

void Object::ComponentBase::ExceptCannotCopy(std::type_info const& type)
{
	throw utility::TeiRuntimeError{ "Cannot copy object component", type.name() };
}
