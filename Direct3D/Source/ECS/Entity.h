#pragma once

#include <algorithm>
#include <cassert>
#include <cstdint>
#include <memory>
#include <utility>
#include <vector>

////////////////////////////////
// ENTITY //////////////////////
////////////////////////////////

struct Entity
{
    static const Entity Invalid;

    Entity(size_t index, uint16_t version)
        :
        Id((index << 12) | version)
    {}

    bool operator==(const Entity& other) const;
    bool operator!=(const Entity& other) const;

    size_t GetIndex() const;
    uint16_t GetVersion() const;

    size_t Id;
};

////////////////////////////////
// COMPONENT ///////////////////
////////////////////////////////

class Component
{
public:
    template<typename>
    static size_t GetTypeId()
    {
        static const size_t type = GenerateTypeId();
        return type;
    }
private:
    static size_t GenerateTypeId();
};

////////////////////////////////
// SPARSE SET //////////////////
////////////////////////////////

class SparseSet
{
public:
    ////////////////////////////
    // ITERATOR ////////////////
    ////////////////////////////

    struct Iterator
    {
        size_t operator*() const
        {
            return (*Packed)[Index];
        }

        Iterator& operator++()
        {
            return ++Index, *this;
        }

        bool operator==(const Iterator& other) const
        {
            return Index == other.Index;
        }

        bool operator!=(const Iterator& other) const
        {
            return !((*this) == other);
        }

        const std::vector<size_t>* Packed = nullptr;
        size_t Index = 0;
    };

    SparseSet() = default;
    virtual ~SparseSet() = default;

    size_t operator[](size_t value);

    Iterator begin() const;
    Iterator end() const;

    size_t GetCapacity() const;
    size_t GetSize() const;

    bool IsEmpty() const;
    bool Contains(size_t value) const;
    void Add(size_t value);
    void Remove(size_t value);
    void RemoveAll();
private:
    std::vector<size_t> m_Sparse = {};
    std::vector<size_t> m_Packed = {};
};

////////////////////////////////
// COMPONENT MANAGER ///////////
////////////////////////////////

template<typename T>
class ComponentManager : public SparseSet
{
public:
    ComponentManager() = default;

    T& operator[](size_t entity)
    {
        const size_t index = SparseSet::operator[](entity);
        return *m_Components[index];
    }

    template<typename ... Args>
    T& Add(size_t entity, Args&& ... args)
    {
        SparseSet::Add(entity);
        auto component = std::make_unique<T>(std::forward<Args>(args)...);
        m_Components.push_back(std::move(component));
        return (*this)[entity];
    }

    void Remove(size_t entity)
    {
        const size_t offset = SparseSet::operator[](entity);
        SparseSet::Remove(entity);
        const auto item = m_Components.begin() + offset;
        m_Components.erase(item);
    }
private:
    std::vector<std::unique_ptr<T>> m_Components = {};
};

////////////////////////////////
// ENTITY QUERY ////////////////
////////////////////////////////

template<typename ... Types>
class EntityQuery
{
public:
    ////////////////////////////
    // ITERATOR ////////////////
    ////////////////////////////

    struct Iterator
    {
        Iterator(const SparseSet* candidate, std::vector<const SparseSet*> others, SparseSet::Iterator current)
            :
            Candidate(candidate),
            Others(others),
            Current(current),
            Last(Candidate->end())
        {
            if (Current != Last && !IsValid())
                ++(*this);
        }

        Iterator& operator++()
        {
            while (++Current != Last && !IsValid());
            return *this;
        }

        bool IsValid() const
        {
            const auto pred = [&](const SparseSet* other) {
                return other->Contains(*Current);
            };
            return std::all_of(Others.begin(), Others.end(), pred);
        }

        size_t operator*() const
        {
            return *Current;
        }

        bool operator==(const Iterator& other) const
        {
            return Current == other.Current;
        }

        bool operator!=(const Iterator& other) const
        {
            return !((*this) == other);
        }

        const SparseSet* Candidate;
        std::vector<const SparseSet*> Others;
        SparseSet::Iterator Current, Last;
    };

    EntityQuery(ComponentManager<Types>& ... components)
        :
        m_Components(&components...)
    {}

    Iterator begin() const
    {
        const SparseSet* candidate = GetCandidate();
        return { candidate, GetOthers(candidate), candidate->begin() };
    }

    Iterator end() const
    {
        const SparseSet* candidate = GetCandidate();
        return { candidate, GetOthers(candidate), candidate->end() };
    }

    template<typename T>
    bool Contains(uint32_t entity) const
    {
        const auto components = std::get<ComponentManager<T>*>(m_Components);
        return components->Contains(entity);
    }

    template<class T>
    T& GetComponent(uint32_t entity)
    {
        auto components = std::get<ComponentManager<T>*>(m_Components);
        assert(components->Contains(entity));
        return (*components)[entity];
    }

    template<class ... Types>
    std::tuple<Types&...> GetComponents(uint32_t entity)
    {
        assert(sizeof...(Types) != 0);
        assert(Contains<Types>(entity) && ...);
        return { GetComponent<Types>(entity) ... };
    }
private:
    const SparseSet* GetCandidate() const
    {
        return std::min(
            std::initializer_list<const SparseSet*>({
                [&]() {
                    const auto current = std::get<ComponentManager<Types>*>(m_Components);
                    return static_cast<const SparseSet*>(current);
                }() ...
            }),
            [](const SparseSet* lhs, const SparseSet* rhs) {
                return lhs->GetSize() < rhs->GetSize();
            }
        );
    }

    std::vector<const SparseSet*> GetOthers(const SparseSet* candidate) const
    {
        std::vector<const SparseSet*> result = {};
        (void)std::initializer_list<int>({
            [&]() {
                const auto current = std::get<ComponentManager<Types>*>(m_Components);
                if (current != candidate)
                    result.push_back(current);

                return 0;
            }() ...
        });
        return result;
    }

    const std::tuple<ComponentManager<Types>*...> m_Components;
};

////////////////////////////////
// ENTITY MANAGER //////////////
////////////////////////////////

class EntityManager
{
public:
    EntityManager() = default;

    const Entity& CreateEntity();
    void DestroyEntity(const Entity& entity);

    template<typename T, typename ... Args>
    T& AddComponent(const Entity& entity, Args&& ... args)
    {
        assert(IsEntityValid(entity));
        ComponentManager<T>& components = GetComponents<T>();
        const size_t index = entity.GetIndex();
        assert(!components.Contains(index));
        return components.Add(index, std::forward<Args>(args)...);
    }

    template<typename T>
    bool HasComponent(const Entity& entity)
    {
        assert(IsEntityValid(entity));
        ComponentManager<T>& components = GetComponents<T>();
        return components.Contains(entity.GetIndex());
    }

    template<typename T>
    T& GetComponent(const Entity& entity)
    {
        assert(IsEntityValid(entity));
        ComponentManager<T>& components = GetComponents<T>();
        return components[entity.GetIndex()];
    }

    template<typename T>
    void RemoveComponent(const Entity& entity)
    {
        assert(IsEntityValid(entity));
        ComponentManager<T>& components = GetComponents<T>();
        const size_t index = entity.GetIndex();
        assert(components.Contains(index));
        components.Remove(index);
    }

    bool IsEntityValid(const Entity& entity) const;

    template<typename ... Types>
    EntityQuery<Types...> SelectAll()
    {
        assert(sizeof...(Types) != 0);
        return { GetComponents<Types>() ... };
    }

    template<typename ... Types, typename F>
    void ForEach(const F& func)
    {
        assert(sizeof...(Types) != 0);
        EntityQuery<Types...> query = SelectAll<Types...>();
        for (const size_t entity : query)
            func(query.GetComponent<Types>(entity)...);
    }
private:
    const Entity& NewEntity();
    const Entity& RecycleEntity();

    template<typename T>
    ComponentManager<T>& GetComponents()
    {
        const size_t type = Component::GetTypeId<T>();
        if (type >= m_Components.size())
        {
            m_Components.resize(type + 1);
            m_Components[type] = std::make_shared<ComponentManager<T>>();
        }
        return *static_cast<ComponentManager<T>*>(m_Components[type].get());
    }

    std::vector<Entity> m_Entities = {};
    Entity m_Next = Entity::Invalid;
    std::vector<std::shared_ptr<void>> m_Components = {};
};