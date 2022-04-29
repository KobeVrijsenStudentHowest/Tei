#include "Hitbox.h"

burger::Hitbox::Hitbox(std::shared_ptr<std::vector<Hitbox*>> others)
	: m_pParent{ nullptr }
	, m_Objects{ std::move(others) }
	, m_Overlaps{ }
{}

burger::Hitbox::~Hitbox()
{
	DelistSelf();
}

void burger::Hitbox::OnEnbale(tei::ecs::Object& parent)
{
	m_pParent = &parent;
	EnlistSelf();
}


void burger::Hitbox::OnDisable()
{
	DelistSelf();
}

void burger::Hitbox::OnUpdate()
{
	if (std::ranges::subrange matches{ std::ranges::find(*m_Objects, this), std::ranges::end(*m_Objects) }; matches.size() > 1)
	{
		for (auto& pMatch : m_Overlaps)
		{
			if (!CollidesWith(*pMatch))
			{
				Hit hit{ *m_pParent, *std::exchange(pMatch, {})->m_pParent, Hit::LEAVE };
				this->Notify(hit);
				pMatch->Notify(hit);
			}
		}
		auto [from, to] = std::ranges::remove(m_Overlaps, nullptr);
		m_Overlaps.erase(from, to);
		auto overlaps{ tei::utility::SubrangeViewInvalidationSafe(m_Overlaps) };

		for (auto pMatch : matches)
		{
			if (CollidesWith(*pMatch) && std::ranges::find(overlaps, pMatch) == overlaps.end())
			{
				m_Overlaps.push_back(pMatch);
				Hit hit{ *m_pParent, *pMatch->m_pParent, Hit::ENTER };
				this->Notify(hit);
				pMatch->Notify(hit);
			}
		}
	}
}

bool IsOverlapping(tei::unit::Transform const& a, tei::unit::Transform const& b)
{
#if false /*circle*/
	auto min = (a.scale.x + a.scale.y + b.scale.x + b.scale.y) / 2;
	auto diff = b.position - a.position;
	return min * min > glm::dot(diff, diff);
#else /*rectangle*/
	auto min = a.scale + b.scale / 2.f;
	auto diff{ glm::abs(a.position - b.position) };
	return min.x > diff.x && min.t > diff.y;
#endif
}

bool burger::Hitbox::CollidesWith(Hitbox const& other) const
{
	auto& [selfTransform] = refs;
	auto& [otherTransform] = other.refs;
	return IsOverlapping(selfTransform, otherTransform);
}

void burger::Hitbox::DelistSelf()
{
	if (auto it{ std::ranges::find(*m_Objects, this) }; it != std::ranges::end(*m_Objects))
		m_Objects->erase(it);
}

void burger::Hitbox::EnlistSelf()
{
	if (std::ranges::find(*m_Objects, this) == std::ranges::end(*m_Objects))
		m_Objects->push_back(this);
}