// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-
// Module:			Gameplay Programming
// Assignment1:		Cheats:Enabled
// Student Name:	Eugene Long - [Engine Designer]
// Student No.:		S10193060J
// -=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-

// ===========================================================================
// Main Menu Scene Implementation
// ===========================================================================

// include specification
#include "levelConverter.h"
#include "../entities/wall/wall.h"
#include "../entities/player/player.h"
#include "../entities/coin/coin.h"
#include "../entities/turret/turret.h"
#include "../entities/enemy/enemy.h"

// ===========================================================================
// default constructor
// ===========================================================================
LevelConverter::LevelConverter() :
    wallTexture()
{

}


// shared methods

void LevelConverter::convertStringToLevel(EntityManager& entityManager, std::string s) {

    std::vector<std::string> rows = LevelConverter::split_string_by(s, '\n');
    for (int i = 0; i < rows.size(); i++) {
        std::vector<std::string> cols = LevelConverter::split_string_by(rows[i], ',');
        if (cols[0] == "WALL") {
            Wall* wall = new Wall();
            wall->initialize(std::stof(cols[1]), std::stof(cols[2]), std::stof(cols[3]), std::stof(cols[4]));
            entityManager.getGraphics()->drawPrimitiveRect(
                std::stof(cols[1]) - std::stof(cols[3]),
                std::stof(cols[2]) - std::stof(cols[4]),
                std::stof(cols[3]) * 2,
                std::stof(cols[4]) * 2,
                CommonColors::WHITE,
                CommonColors::WHITE,
                0);
            wall->setCenter(VECTOR2(std::stof(cols[1]), std::stof(cols[2])));
            entityManager.addEntity(wall, 0);
        }
        else if (cols[0] == "SPAWN_PLAYER") {
            Player* player = new Player();
            TextureManager playerTexture;
            if (!playerTexture.initialize(
                entityManager.getGraphics(),
                L"./assets/player.jpg"
            ))
                throw(
                    InkdotError(
                        InkdotErrorType::FATAL_ERROR,
                        "Error initializing player texture!"
                    ));
            player->initialize(entityManager.getGraphics(), &playerTexture);
            player->setCenter(VECTOR2(
                std::stof(cols[1]),
                std::stof(cols[2])
            ));
            entityManager.addEntity(player, 0);
        }
        else if (cols[0] == "COIN") {
            Coin* coin = new Coin();
            TextureManager coinTexture;
            if (!coinTexture.initialize(
                entityManager.getGraphics(),
                L"./assets/coin.png"
            ))
                throw(
                    InkdotError(
                        InkdotErrorType::FATAL_ERROR,
                        "Error initializing coin texture!"
                    ));
            coin->initialize(entityManager.getGraphics(), &coinTexture);
            coin->setCenter(VECTOR2(
                std::stof(cols[1]),
                std::stof(cols[2])
            ));
            entityManager.addEntity(coin, 0);
        }
        else if (cols[0] == "TURRET_SPAWNL") {
            Turret* turret = new Turret();
            TextureManager turretTexture;
            if (!turretTexture.initialize(
                entityManager.getGraphics(),
                L"./assets/turret.png"
            ))
                throw(
                    InkdotError(
                        InkdotErrorType::FATAL_ERROR,
                        "Error initializing turret texture!"
                    ));
            turret->initialize(entityManager.getGraphics(), &turretTexture);
            turret->setCenter(VECTOR2(
                std::stof(cols[1]),
                std::stof(cols[2])
            ));
            entityManager.addEntity(turret, 0);
        }
        else if (cols[0] == "TURRET_SPAWNR") {
            Turret* turret = new Turret();
            TextureManager turretTexture;
            if (!turretTexture.initialize(
                entityManager.getGraphics(),
                L"./assets/turret.png"
            ))
                throw(
                    InkdotError(
                        InkdotErrorType::FATAL_ERROR,
                        "Error initializing turret texture!"
                    ));
            turret->initialize(entityManager.getGraphics(), &turretTexture);
            turret->setCenter(VECTOR2(
                std::stof(cols[1]),
                std::stof(cols[2])
            ));
            turret->getSprite()->flipHorizontal(TRUE);
            entityManager.addEntity(turret, 0);
        }

        else if (cols[0] == "SPAWN_HUNTER") {
            Enemy* enemy = new Enemy();
            TextureManager enemyTexture;
            if (!enemyTexture.initialize(
                entityManager.getGraphics(),
                L"./assets/enemy.jpg"
            ))
                throw(
                    InkdotError(
                        InkdotErrorType::FATAL_ERROR,
                        "Error initializing enemy texture!"
                    ));
            enemy->initialize(entityManager.getGraphics(), &enemyTexture, mainplayer);
            enemy->setCenter(VECTOR2(
                std::stof(cols[1]),
                std::stof(cols[2])
            ));
            entityManager.addEntity(enemy, 0);
        }
        else if (cols[0] == "SPIKES") {
        }

    }
}

std::vector<std::string> LevelConverter::split_string_by(const std::string& str, char delim)
{
    auto result = std::vector<std::string>{};
    auto ss = std::stringstream{ str };

    for (std::string line; std::getline(ss, line, delim);)
        result.push_back(line);

    return result;
}
