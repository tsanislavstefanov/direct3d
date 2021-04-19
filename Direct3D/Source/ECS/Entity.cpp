#include "stdafx.h"
#include "Entity.h"

////////////////////////////////
// ENTITY //////////////////////
////////////////////////////////

const Entity Entity::Invalid = { SIZE_MAX >> 12, 0 };

bool Entity::operator==(const Entity& other) const
{
    return Id == other.Id;
}

bool Entity::operator!=(const Entity& other) const
{
    return !((*this) == other);
}

size_t Entity::GetIndex() const
{
    return Id >> 12;
}

uint16_t Entity::GetVersion() const
{
    return Id | 0xfff;
}

////////////////////////////////
// COMPONENT ///////////////////
////////////////////////////////

size_t Component::GenerateTypeId()
{
    static size_t next = 0;
    return next++;
}

////////////////////////////////
// SPARSE SET //////////////////
////////////////////////////////

size_t SparseSet::operator[](size_t value)
{
    assert(Contains(value));
    return m_Sparse[value];
}

SparseSet::Iterator SparseSet::begin() const
{
    return { &m_Packed, 0 };
}

SparseSet::Iterator SparseSet::end() const
{
    return { &m_Packed, m_Packed.size() };
}

size_t SparseSet::GetCapacity() const
{
    return m_Sparse.capacity();
}

size_t SparseSet::GetSize() const
{
    return m_Packed.size();
}

bool SparseSet::IsEmpty() const
{
    return m_Packed.empty();
}

bool SparseSet::Contains(size_t value) const
{
    return value < m_Sparse.size()
        && m_Sparse[value] < m_Packed.size()
        && m_Packed[m_Sparse[value]] == value;
}

void SparseSet::Add(size_t value)
{
    if (value >= m_Sparse.size())
        m_Sparse.resize(value + 1);

    m_Sparse[value] = m_Packed.size();
    m_Packed.push_back(value);
}

void SparseSet::Remove(size_t value)
{
    assert(Contains(value));
    const size_t back = m_Packed.back();
    m_Sparse[back] = m_Sparse[value];
    m_Packed[m_Sparse[value]] = back;
    m_Packed.pop_back();
    const Entity& invalid = Entity::Invalid;
    m_Sparse[value] = invalid.GetIndex();
}

void SparseSet::RemoveAll()
{
    m_Sparse.clear();
    m_Packed.clear();
}

////////////////////////////////
// ENTITY MANAGER //////////////
////////////////////////////////

const Entity& EntityManager::CreateEntity()
{
    return m_Next == Entity::Invalid ? NewEntity() : RecycleEntity();
}

void EntityManager::DestroyEntity(const Entity& entity)
{
    assert(IsEntityValid(entity));
    size_t index = entity.GetIndex();
    uint16_t version = m_Next.GetVersion();
    m_Next = { index, version };
    Entity& current = m_Entities[index];
    index = current.GetIndex(), version = current.GetVersion();
    current = { index, ++version };
}

bool EntityManager::IsEntityValid(const Entity& entity) const
{
    if (entity == Entity::Invalid)
        return false;

    const size_t index = entity.GetIndex();
    return index < m_Entities.size() && m_Entities[index] == entity;
}

const Entity& EntityManager::NewEntity()
{
    const size_t index = m_Entities.size();
    m_Entities.push_back({ index, 0 });
    const Entity& entity = m_Entities[index];
    assert(entity != Entity::Invalid);
    return entity;
}

const Entity& EntityManager::RecycleEntity()
{
    size_t index = m_Next.GetIndex();
    Entity& entity = m_Entities[index];
    uint16_t version = entity.GetVersion();
    entity = { index, version };
    index = entity.GetIndex(), version = m_Next.GetVersion();
    m_Next = { index, version };
    assert(entity != Entity::Invalid);
    return entity;
}