#include "stdafx.h"
#include "Scene.h"

Actor Scene::CreateActor()
{
    const Entity& entity = m_Entities.CreateEntity();
    return { entity, &m_Entities };
}

void Scene::Update()
{

}