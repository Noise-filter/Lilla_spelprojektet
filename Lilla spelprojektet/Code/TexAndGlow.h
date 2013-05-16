#pragma once
#include "WindowsUtility.h"

class TexAndGlow
{
	public:
		static std::vector<std::string> loadMainBuildingTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Main Building test 1.png");
			return t;
		}

		static std::vector<std::string> loadPlaneTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Tile test 1.png");
			return t;
		}

		static std::vector<std::string> loadSupplyLowerTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Power Building part 1.png");
			return t;
		}

		static std::vector<std::string> loadSupplyUpperTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Power Building part 2.png");
			return t;
		}

		static std::vector<std::string> loadTowerLowerTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/tower part 1.png");
			return t;
		}

		static std::vector<std::string> loadTowerUpperTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/tower part 2 test.png");
			return t;
		}

		static std::vector<std::string> loadTowerShotsTex() //Not correct texture
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Green node 1.png");
			return t;
		}

		static std::vector<std::string> loadFriendlyNodeTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Green node 1.png");
			return t;
		}

		static std::vector<std::string> loadNeutralNodeTex() //Not correct texture
		{
			std::vector<std::string> t;
			t.push_back("Texturer/tower part 2 test.png");
			return t;
		}

		static std::vector<std::string> loadHostileNodeTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Enemy test 1.png");
			return t;
		}

		static std::vector<std::string> loadEnemyTex()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Enemy test 1.png");
			return t;
		}


		static std::vector<std::string> loadMainBuildingGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/Main Building test 1 GLOW.png");
			return t;
		}

		static std::vector<std::string> loadPlaneGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/Tile test 1 GLOW.png");
			return t;
		}

		static std::vector<std::string> loadSupplyLowerGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/Power Building part 1 GLOW.png");
			return t;
		}

		static std::vector<std::string> loadSupplyUpperGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/Power Building part 2 GLOW.png");
			return t;
		}

		static std::vector<std::string> loadTowerLowerGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/tower part 1 GLOW.png");
			return t;
		}

		static std::vector<std::string> loadTowerUpperGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/tower part 2 test GLOW.png");
			return t;
		}

		static std::vector<std::string> loadTowerShotsGlow()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Main Building test 1.png");
			return t;
		}

		static std::vector<std::string> loadFriendlyNodeGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/Green Node GLOW TEST.png");
			return t;
		}

		static std::vector<std::string> loadNeutralNodeGlow()
		{
			std::vector<std::string> t;
			t.push_back("Texturer/Main Building test 1.png");
			return t;
		}

		static std::vector<std::string> loadHostileNodeGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/Enemy test 1 GLOW.png");
			return t;
		}

		static std::vector<std::string> loadEnemyGlow()
		{
			std::vector<std::string> t;
			t.push_back("Glow/Enemy test 1 GLOW.png");
			return t;
		}
};