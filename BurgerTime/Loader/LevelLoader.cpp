#include "pch.h"
#include "LevelLoader.h"

#include <array>
#include <string>
#include <fstream>
#include <set>
#include <nlohmann/json.hpp>

#include <tei.h>

#include "../Level/Ingredient.h"
#include "../Level/StaticEntity.h"
#include "../Level/Anima.h"
#include "../Level/Level.h"

using namespace std::literals;
using namespace tei;
using namespace tei::literals;

constexpr inline auto resourcedir = "resources/"sv;

bool resembles(nlohmann::json const& data, std::string_view word)
{
	// Null always true
	if (data.is_null())
		return true;

	// String compare
	if (data.is_string())
		return std::string_view{ data } == word;

	// Set includes
	if (data.is_array())
	{

		for (auto& entry : data)
		{
			if (entry.is_string() && std::string_view{ entry } == word)
				return true;
		}
	}
	return false;
}

std::vector<AnimaData> LoadAnima(LevelData const& level, nlohmann::json const& allentitydata)
{

	auto& animadata = allentitydata["anima"];

	std::vector<AnimaData> anima{};
	std::vector<AnimaData::State> states{};

	unit::Dimentions box{};
	box.x = animadata["box"][0];
	box.y = animadata["box"][1];

	std::string_view spritesep{ allentitydata["spriteseparator"] };
	std::string_view spriteext{ allentitydata["spriteextention"] };
	Clock::duration spritedur{ 1_s / long(allentitydata["spritefrequency"]) };
	
	std::set const types{
		AnimaData::PLAYER,
		AnimaData::ENEMY,
	};

	for (auto& entitydata : animadata["entities"])
	{
		if (resembles(entitydata["mode"], level.mode))
		{
			AnimaData entity{
				.type = *types.find(entitydata["type"]),
				.box = box,
				.name = entitydata["name"],
				.id = entitydata["id"]
			};
			if (resembles(entitydata["type"], AnimaData::ENEMY))
			{
				entity.score = entitydata["score"];
				entity.flips = entitydata["flips"];
			}
			
			std::string_view animasrc = entitydata["src"];
			entity.state.sprites.reserve(5);

			for (auto& statedata : animadata["states"])
			{
				if (resembles(statedata["type"], entity.type))
				{
					if (statedata.find("states") != statedata.end())
					{
						for (std::string_view substate : statedata["states"])
							entity.state.sprites.push_back(
								Resources->LoadShared<resource::Sprite>(
									std::string{ resourcedir }.append(animasrc).append(spritesep).append<std::string_view>(statedata["src"]).append(spritesep).append<std::string_view>(substate).append(spriteext),
									spritedur, statedata["frames"], 1, statedata["loop"]
									)
							);
					}
					else
					{
						entity.state.sprites.push_back(
							Resources->LoadShared<resource::Sprite>(
								std::string{ resourcedir }.append(animasrc).append(spritesep).append<std::string_view>(statedata["src"]).append(spriteext),
								spritedur, statedata["frames"], 1, statedata["loop"]
							)
						);
					}
				}
				else entity.state.sprites.push_back({});
			}

			anima.push_back(std::move(entity));
		}
	}

	return anima;
}

std::vector<IngredientData> LoadIngredients(nlohmann::json const& allentitydata)
{
	auto const& ingredientsdata = allentitydata["ingredients"];
	
	std::vector<IngredientData> ingredients{};
	ingredients.reserve(6);

	std::string_view spritesep{ allentitydata["spriteseparator"] };
	std::string_view spriteext{ allentitydata["spriteextention"] };
	
	unit::Dimentions box{};
	box.x = ingredientsdata["box"][0];
	box.y = ingredientsdata["box"][1];

	std::set const types{
		IngredientData::BUN_TOP,
		IngredientData::BUN_BOTTOM,
		IngredientData::PATTY,
		IngredientData::LETTUCE,
		IngredientData::CHEESE,
		IngredientData::TOMATO
	};

	for (auto& entitydata : ingredientsdata["entities"].items())
	{
		IngredientData ingredient{
			.type = *types.find(entitydata.key()),
			.name = entitydata.value()["name"],
			.texture = Resources->LoadShared<resource::Texture>(
				std::string{ resourcedir }.append<std::string_view>(entitydata.value()["src"]).append(spriteext)
			)
		};
		if (entitydata.value()["unique"].is_boolean() && bool(entitydata.value()["unique"]))
		{
			ingredient.unique = true;
			ingredient.position = entitydata.value()["position"].is_number() ? int(entitydata.value()["position"]) : int();
		}
		ingredients.push_back(std::move(ingredient));
	}

	return ingredients;
}

std::vector<StaticEntityData> LoadOther(nlohmann::json const& allentitydata)
{
	auto const& otherdata = allentitydata["other"];
	
	std::vector<StaticEntityData> other{};
	other.reserve(4);
	
	std::string_view spritesep{ allentitydata["spriteseparator"] };
	std::string_view spriteext{ allentitydata["spriteextention"] };

	std::set const types{
		StaticEntityData::PLATE,
		StaticEntityData::LADDER,
		StaticEntityData::PLATFORM,
		StaticEntityData::SHELF,
		StaticEntityData::HEARTH,
		StaticEntityData::LEVEL_1,
		StaticEntityData::LEVEL_5,
		StaticEntityData::LEVEL_10,
	};

	for (auto& entitydata : otherdata["entities"].items())
	{
		
		unit::Dimentions box{};
		box.x = entitydata.value()["box"][0];
		box.y = entitydata.value()["box"][1];

		other.push_back(StaticEntityData{
			.type = *types.find(entitydata.key()),
			.box = box,
			.name = entitydata.value()["name"],
			.texture = Resources->LoadShared<resource::Texture>(
				std::string{ resourcedir }.append<std::string_view>(entitydata.value()["src"]).append(spriteext)
			)
		});
	}

	return other;
}

std::vector<LevelLayoutData> LoadLevelLayout()
{
	std::vector<LevelLayoutData> levels{};

	auto const path = std::string{ resourcedir }.append("Level").append("_");
	auto const ext = ".dat"sv;

	for (int id{ 1 }; id < 4; ++id)
	{
		std::ifstream ifs{ path + std::to_string(id).append(ext) };

		LevelLayoutData layout{};
		layout.field.reserve(170);

		std::string line{};
		std::getline(ifs, line);
		layout.width = int(std::size(line));

		do
		{
			++layout.height;
			std::ranges::transform(
				line,
				std::back_inserter(layout.field),
				[] (char c) 
				{
					return static_cast<LevelLayoutData::TileType>(c);
				}
			);
		}
		while (std::getline(ifs, line));

		levels.push_back(std::move(layout));
	}

	return levels;
}

LevelData LoadLevel(std::string_view mode)
{
	try
	{
		LevelData level{ .mode = mode };

		nlohmann::json const entitydata =
		[] (std::filesystem::path const& path)
		{
			nlohmann::json data{};
			std::ifstream{ path } >> data;
			return data;
		} 
		("resources/EntityData.json");

		level.anima = LoadAnima(level, entitydata);
		level.ingrendients = LoadIngredients(entitydata);
		level.other = LoadOther(entitydata);
		level.levels = LoadLevelLayout();

		return level;
	}
	catch (const std::exception& e)
	{
		puts("[ERROR] An error occured while loading level data from datafile");
		puts(e.what());
		throw;
	}
}

LevelData LoadLevelData([[maybe_unused]] std::filesystem::path const& path, std::string_view mode)
{
	//nlohmann::json data{};
	//std::ifstream{ path } >> data;
	return LoadLevel(mode);
}

void Load(std::shared_ptr<LevelData>& ref, std::filesystem::path const& path, std::string_view mode)
{
	ref = std::make_shared<LevelData>(LoadLevelData(path, mode));
}