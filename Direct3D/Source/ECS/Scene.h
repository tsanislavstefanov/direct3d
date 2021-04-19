#pragma once

#include "Actor.h"
#include "Entity.h"

class Scene
{
public:
    Scene() = default;

    Actor CreateActor();
    void Update();
private:
    EntityManager m_Entities;
};