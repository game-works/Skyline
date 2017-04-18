#include "Projectile.h"
#include "Game.h"

Projectile::Projectile() {
}

Projectile::Projectile(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, float angle) : Entity(textureId, uv, width, height, position, angle, false) {}
Projectile::Projectile(int textureId, glm::vec4 uv, float width, float height, glm::vec2 position, float angle, int source, float damage) : Projectile(textureId, uv, width, height, position, angle) {
  _source = source; 
  _damage = damage;

  Ess2D::TextureAtlas * _playerAtlas = _game->getGameplayScreen()->getTextureCache()->getAtlas("Textures/bullet.png", "Textures/bullet.json");

  _animationManager = new Ess2D::AnimationManager();

  Ess2D::Animation* idleAnimation = _animationManager->add("IDLE");
  idleAnimation->setPlaybackRate(1.0f / 60.0f);
  idleAnimation->setTextureAtlas(_playerAtlas);
  idleAnimation->setRepeat(true);
  std::vector<std::string> animationFrames;
  for(int i = 0; i <= 12; i++) {
    animationFrames.push_back("missle_1_" + std::to_string(i));
  }
  idleAnimation->setFrames(
    std::vector<std::string> {"bullet_red_0", "bullet_red_1", "bullet_red_2", "bullet_red_3", "bullet_red_4", "bullet_red_5", "bullet_red_6", "bullet_red_7"}
  );
  _animationManager->play("IDLE");
}

Projectile::~Projectile() {
  delete _animationManager;
}

bool Projectile::update(float deltaTime) {
  Entity::update(deltaTime);

  _animationManager->update(deltaTime);

  if(!this->inViewport()) {
    return false;
  }
  return true;
}

void Projectile::draw() {
  if (_isSpawned) {
    b2Vec2 bodyPosition = this->_body->GetPosition();
    glm::vec2 screenPosition = _position;

    Ess2D::SpriteBatch* spriteBatch = _game->getGameplayScreen()->getSpriteBatch();
    Ess2D::TextureAtlas* textureAtlas = _animationManager->getCurrent()->getTextureAtlas();
    std::string currentAnimationFrame = _animationManager->getCurrent()->getCurrentFrame(); 
    spriteBatch->draw(glm::vec4(screenPosition.x - _width / 2, screenPosition.y - _height / 2, _width, _height), textureAtlas->getUV(currentAnimationFrame), textureAtlas->getTextureId(), _color, (float) _depth, _angle);
  }
}

void Projectile::contact(Entity* e) {
  if(
    (e->getType() == ET_ENTITY || e->getType() == ET_SPACESHIP_A || e->getType() == ET_SPACESHIP_B ||
      e->getType() == ET_SPACESHIP_C || e->getType() == ET_SPACESHIP_D)
    && _source == ET_PLAYER
  ) { //todo change to ET_DAMAGE
    e->applyDamage(this->getDamage());
    _game->getGameplayScreen()->getProjectileManager()->deleteProjectile(this->getId(), true);
  }
}

void Projectile::createFixtures() {
  b2PolygonShape boxShape;
  boxShape.SetAsBox(_width / 2, _height / 2);

  b2FixtureDef boxFixtureDef;
  boxFixtureDef.shape = &boxShape;
  boxFixtureDef.density = 1;
  boxFixtureDef.isSensor = 1;

  _body->CreateFixture(&boxFixtureDef);
  _body->SetBullet(true);
}
