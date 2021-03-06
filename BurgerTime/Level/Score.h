#pragma once

#include <tei.h>

class ScoreManager;
struct LevelData;

class Score
{
public:

	ScoreManager* operator -> () const
	{ return m_pManager; }

	void OnEnable(tei::ecs::Object const&);
	void OnDisable();

	struct Event
	{
		tei::ecs::Object const& object;
		long const score;
	};

	static const long ENEMY_KILLED;
	static const long INGREDIENT_DROPPED;
	static const long ENEMY_DROPPED;
	static const long ENEMY_DROPPED_MULTIPLIER;

private:

	ScoreManager* m_pManager{};

};

class ScoreManager : public tei::components::RefComponent<tei::components::ObjectTransform>, public tei::components::Subject
{
public:

	ScoreManager(LevelData const& leveldata);

	void OnEnable();

	void OnUpdate(tei::ecs::Object&);

	void OnDisable();

	using ScoreType = long;

	ScoreType GetScore() const
	{ return m_Score; }

private:

	long m_Score;

	LevelData const* m_pData;
	std::vector<Score::Event> m_Events{};
	std::any m_Handle{};

};

class ScoreDisplay : public tei::components::RefComponent<Score, tei::components::Observed<std::string>>
{
public:

	static tei::ecs::Object& Create(tei::ecs::Object&);

	void OnEnable();

};