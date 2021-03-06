#include "pch.h"
#include "Ingredient.h"

#include "Level.h"
#include "Hitbox.h"
#include "Player.h"
#include "StaticEntity.h"
#include "Score.h"
#include "Enemy.h"

#include <numbers>

using namespace tei;
using namespace tei::components;

constexpr inline float visualOffset = -.2f;

tei::ecs::Object& IngredientEnity::Create(tei::ecs::Object& parent, IngredientData const& data, LevelData const& level)
{
	auto& object = parent.AddChild();

	auto& self = object.AddComponent<IngredientEnity>();

	self.m_pData = &data;
	self.m_pLevelData = &level;

	{
		auto& visualObject = object.AddChild();
		self.m_pVisualTransform = &visualObject.AddComponent<ObjectTransform>({ { unit::Position{ 0, visualOffset }, unit::Scale{ 2.75f }} });
		visualObject.AddComponent<TextureRenderComponent>();
		visualObject.AddComponent(data.texture);
	}

	for(int i{}; i < 3; ++i)
	{
		auto& triggerObject = object.AddChild();
		triggerObject.AddComponent<ObjectTransform>(unit::Position{ i - 1, 0 });
		triggerObject.AddComponent(Box{ .5f, .5f });
		auto& hitbox = triggerObject.AddComponent<Hitbox>();
		triggerObject.AddComponent(
			hitbox.AddObserver(
				[i, &self] (Hitbox::Hit const& hit)
				{
					if (hit.state == hit.ENTER)
					{
						if (hit.object.HasComponent<PlayerEffects>())
						{
							Audio->Play(self.m_pLevelData->sounds.at(Sound::PRESS).sound);
							self.Pressed(i);
						}
					}
				}
			)
		);
	}

	{
		auto& box = object.AddComponent(Box{ 3, .6f });
		auto& hitbox = object.AddComponent<Hitbox>();
		object.AddComponent(
			hitbox.AddObserver(
				[&self, &box, &object] (Hitbox::Hit const& hit)
				{
					if (hit.state != hit.ENTER || self.m_IsOnPlate || !self.m_Falling)
						return;

					auto& transform = self.Refs().get<ObjectTransform>();

					if (auto pEntity{ hit.object.HasComponent<StaticEntity>() })
					{
						if (!self.m_Timer)
						{
							if (pEntity->Type() == StaticEntityData::PLATE)
							{
								self.m_Timer = 0_s;
								self.m_IsOnPlate = true;
								transform.get().position = hit.object.GetComponent<ObjectTransform>()->position;
								transform.get().position.y += 0.4f;
								self.Notify(IsOnPlate{});
							}
							if (pEntity->Type() == StaticEntityData::SHELF)
							{
								self.m_Timer = 0_s;
								transform.get().position = hit.object.GetComponent<ObjectTransform>()->position;
							}
						}
					}

					if (auto pIngredient{ hit.object.HasComponent<IngredientEnity>() })
					{		
						if (!pIngredient->m_Timer)
						{
							self.m_Timer = 0_s;
							if (pIngredient->m_IsOnPlate)
							{
								self.m_IsOnPlate = true;
								transform.get().position = hit.object.GetComponent<ObjectTransform>()->position;
								transform.get().position.y += 0.65f;
								self.Notify(IsOnPlate{});
							}
							else
							{
								auto& otherTransform = pIngredient->Refs().get<ObjectTransform>();
								transform.get().position = otherTransform->position;
								otherTransform.get().position.y -= box.y * 1.1f;
								pIngredient->Pressed(-1);
								(*pIngredient->m_pVisualTransform).get().position.y = (*self.m_pVisualTransform).get().position.y += box.y * 1.f;
							}
						}
					}

					if (auto pEnemy{ hit.object.HasComponent<EnemyEffects>() })
					{
						if (auto& anima = pEnemy->Refs().get<Anima>(); anima.IsAlive())
						{
							anima.DoDeath();
							if (self.m_Timer)
								++self.m_Enemies;
							else
								pEnemy->Refs().get<Score>()->Notify(Score::Event{ hit.object, Score::ENEMY_KILLED });
						}
					}
				}
			)
		);
	}

	return object;
}

void IngredientEnity::OnUpdate(tei::ecs::Object& object)
{
	if (m_Timer)
	{
		m_Falling ^= true;
		if (m_Falling)
		{
			m_Enemies = {};
			Refs().get<Hitbox>().ReEnter();
			auto score = m_Enemies == 0 ? Score::INGREDIENT_DROPPED : Score::ENEMY_DROPPED * ScoreManager::ScoreType(std::pow(Score::ENEMY_DROPPED_MULTIPLIER, m_Enemies));
			Refs().get<Score>()->Notify(Score::Event{ object, score });
		}
	}

	if (m_Falling)
	{
		auto& transform = Refs().get<ObjectTransform>();
		transform.get().position.y -= 2.5f * Time->frame.delta.count();
	}

	if (auto& t{ *m_pVisualTransform }; t->position.y > visualOffset)
	{
		t.get().position.y = std::max(visualOffset, t->position.y - 2.f * Time->frame.delta.count());
	}
}

void IngredientEnity::Pressed(int index)
{
	if (m_Falling || m_IsOnPlate)
		return;

	if (index >= 0)
	{
		m_Pressed.set(index);
	}

	float y{ visualOffset };
	float r{};

	if (m_Pressed.all() || index < 0)
	{
		m_Timer = 0_s;
		m_Pressed = {};
		
		m_pVisualTransform->get().rotation.r = r;
		m_pVisualTransform->get().position.y = y;

		Audio->Play(m_pLevelData->sounds.at(Sound::DROP).sound);
	}
	else
	{
		float const rinc{ std::numbers::pi_v<float> / 24.f };
		float const yinc{ -.25f };

		if (m_Pressed.test(0))
			r -= rinc, y += yinc;
		if (m_Pressed.test(2))
			r += rinc, y += yinc;
		if (m_Pressed.test(1))
			r += r, y += yinc;

		m_pVisualTransform->get().rotation.r = r;
		m_pVisualTransform->get().position.y = y;
	}
}
