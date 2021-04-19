#pragma once

#include <utility>

#include "Entity.h"

class Actor
{
public:
    Actor(const Entity& entity, EntityManager* entities)
        :
        m_Entity(entity),
        m_Entities(entities)
    {}

    template<typename T>
    bool HasComponent()
    {
        return m_Entities->HasComponent<T>(m_Entity);
    }

    template<typename T>
    T& GetComponent()
    {
        return m_Entities->GetComponent<T>(m_Entity);
    }

    template<typename T, typename ... Args>
    T& AddComponent(Args&& ... args)
    {
        return m_Entities->AddComponent<T>(m_Entity, std::forward<Args>(args)...);
    }
private:
    Entity m_Entity;
    EntityManager* m_Entities;
};