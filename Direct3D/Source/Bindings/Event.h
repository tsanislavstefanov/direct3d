#pragma once

////////////////////////////////
// DELEGATE ////////////////////
////////////////////////////////

template<typename>
struct Delegate
{};

template<typename Result, typename ... Args>
struct Delegate<Result(Args...)>
{
    bool operator==(const Delegate<Result(Args...)>& other)
    {
        return Callback == other.Callback && Data == other.Data;
    }

    bool operator!=(const Delegate<Result(Args...)>& other)
    {
        return !((*this) == other);
    }

    Result operator()(Args ... args) const
    {
        return Callback(Data, std::forward<Args>(args)...);
    }

    template<typename Lambda>
    void Bind(Lambda& lambda)
    {
        Data = &lambda;
        Callback = [](void* data, Args ... args) -> Result {
            Lambda& lambda = *reinterpret_cast<Lambda*>(data);
            return std::invoke(lambda, std::forward<Args>(args)...);
        };
    }

    template<auto MemFunc, typename Object>
    void Bind(Object* object)
    {
        Data = object;
        Callback = [](void* data, Args ... args) -> Result {
            auto object = reinterpret_cast<Object*>(data);
            return std::invoke(MemFunc, object, std::forward<Args>(args)...);
        };
    }

    Result(*Callback)(void*, Args...) = nullptr;
    void* Data = nullptr;
};

////////////////////////////////
// SIGNAL //////////////////////
////////////////////////////////

template<typename ... Args>
class Signal
{
public:
    void operator()(Args ... args)
    {
        for (Callback& callback : m_Callbacks)
            callback(std::forward<Args>(args)...);
    }

    template<typename Lambda>
    void Connect(Lambda& lambda)
    {
        Callback callback = {};
        callback.Bind(lambda);
        m_Callbacks.push_back(callback);
    }

    template<auto MemFunc, typename Object>
    void Connect(Object* object)
    {
        Callback callback = {};
        callback.Bind<MemFunc>(object);
        m_Callbacks.push_back(callback);
    }

    template<typename Lambda>
    void Disconnect(Lambda& lambda)
    {
        Callback callback = {};
        callback.Bind(lambda);
        Disconnect(callback);
    }

    template<auto MemFunc, typename Object>
    void Disconnect(Object* object)
    {
        Callback callback = {};
        callback.Bind<MemFunc>(object);
        Disconnect(callback);
    }
private:
    using Callback = Delegate<void(Args...)>;

    void Disconnect(const Callback& callback)
    {
        const auto end = m_Callbacks.end();
        m_Callbacks.erase(std::remove(m_Callbacks.begin(), end, std::move(callback)), end);
    }

    std::vector<Callback> m_Callbacks = {};
};

////////////////////////////////
// ACTION(S) ///////////////////
////////////////////////////////

using Action = Signal<>;

////////////////////////////////
// EVENT DISPATCHER ////////////
////////////////////////////////

class EventDispatcher
{
public:
    template<typename Event>
    Signal<Event>& Subscribe()
    {
        const size_t index = GetTypeId<Event>();
        if (index >= m_Signals.size())
        {
            m_Signals.resize(index + 1);
            m_Signals[index] = std::make_shared<Signal<Event>>();
        }

        return *static_cast<Signal<Event>*>(m_Signals[index].get());
    }

    template<typename Event>
    void Publish(Event& event)
    {
        const size_t index = GetTypeId<Event>();
        if (index >= m_Signals.size())
            return;

        (*static_cast<Signal<Event>*>(m_Signals[index].get()))(event);
    }
private:
    static size_t GenerateTypeId();

    template<typename>
    static size_t GetTypeId()
    {
        static const size_t value = GenerateTypeId();
        return value;
    }

    std::vector<std::shared_ptr<void>> m_Signals = {};
};