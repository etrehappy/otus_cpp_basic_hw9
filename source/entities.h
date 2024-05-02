#pragma once
#include <SFML/Graphics.hpp>
#include "box2d/box2d.h"

#include "settings.h"
#include "body_type.h"

class Entities
{
public:
    virtual void Update() = 0;
    virtual void Draw(sf::RenderWindow&)  = 0;
    virtual ~Entities() {};
};

/*!
* @brief Objects is non-living items on maps (e.g. food or platforms)
*/
class Objects : public Entities
{
public:
    Objects::Objects(std::shared_ptr<b2World>);

    virtual void SetTexture() = 0;
    virtual void SetPhysics() = 0;

    b2Body* GetBody();
    bool IsDestroyable();    

protected:
    /*!
    * @brief Used to load textures only once
    */
    struct TextureСatalog
    {
    private:
        TextureСatalog();
        TextureСatalog(const TextureСatalog&) = delete;
        TextureСatalog& operator=(const TextureСatalog&) = delete;

    public:
        ~TextureСatalog() = default;
        static TextureСatalog& GetInstance();
        sf::Texture& GetWildStrawberry();
        sf::Texture& GetStick();

    private:
        sf::Texture wild_strawberry_t_;
        sf::Texture stick_t_;
    };

    sf::Sprite sprite_;
    std::shared_ptr<b2World> physical_world_;
    b2Body* physic_body_;
};