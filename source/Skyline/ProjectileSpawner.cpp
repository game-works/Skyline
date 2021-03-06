#include "ProjectileSpawner.h"
#include "Game.h"
#include <iostream>

ProjectileSpawner::ProjectileSpawner() {

}

ProjectileSpawner::ProjectileSpawner(float fireRate, glm::vec2 size, float projectileDamage) {
  _fireRate = fireRate;
  _fireRateCounter = 0.9f;
  _projectileWidth = size.x;
  _projectileHeight = size.y;
  _projectileDamage = projectileDamage;
}

ProjectileSpawner::~ProjectileSpawner() {

}

int ProjectileSpawner::update(float deltaTime, bool isFiring, const glm::vec2& position, const glm::vec2& velocity, float angle) {
  if(isFiring || _fireRateCounter < 1) {
    _fireRateCounter += _fireRate * deltaTime;
  }

  int projectileCount = (int)floor(_fireRateCounter);

  if (isFiring && projectileCount > 0) {
    spawn(projectileCount, position, velocity, angle);
    _fireRateCounter -= projectileCount;
  }

  if(!isFiring && projectileCount > 0) {
    _fireRateCounter = 1;
  }
  
  if (!isFiring) {
    projectileCount = 0;
  }

  return projectileCount;
}

void ProjectileSpawner::spawn(int projectileCount, const glm::vec2& position, const glm::vec2& velocity, float angle) {
  Game* gameInstance = Game::GetInstance();

  for(int i = 0; i < projectileCount; i++) {
    Projectile* projectile = new Projectile(0, glm::vec4(0.0f, 0.0f, 1.0f, 1.0f),
                                            _projectileWidth, _projectileHeight, glm::vec2(position.x, position.y), angle, _source, _projectileDamage);
    projectile->setSkin(_projectileSkin);
    projectile->createB2Data();
    projectile->setVelocity(velocity * _projectileVelocity);
    projectile->spawn();
    gameInstance->getGameplayScreen()->getProjectileManager()->add(projectile);

    if(_playSound) {
      gameInstance->getAudioManager()->playEvent("event:/effects/laser_shot", true);
    }
  }
}

void ProjectileSpawner::setSkin(const std::string& skinName) {
  _projectileSkin = skinName;
}

void ProjectileSpawner::setVelocity(float velocity) {
  _projectileVelocity = velocity;
}

void ProjectileSpawner::setPlaySound(bool playSound) {
  _playSound = playSound;
}
