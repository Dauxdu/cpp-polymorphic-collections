export module Dictionary;

import std;
import IEnumerator;
import ICollection;

export template <typename TKey, typename TValue, typename Hash = std::hash<TKey>, typename KeyEqual = std::equal_to<TKey>>
class Dictionary final : public ICollection<std::pair<const TKey, TValue>>
{
private:
    std::unordered_map<TKey, TValue, Hash, KeyEqual> _map;

public:
    bool Contains(const TKey &key) const
    {
        return _map.contains(key);
    }

    void Add(const std::pair<TKey, TValue> &item) override
    {
        if (Contains(item->first))
        {
            return;
        }
        _map.insert(item);
    }

    bool Remove(const TKey &key) override
    {
        if (!Contains(key))
        {
            return false;
        }
        _map.erase(key);
        return true;
    }

    void Clear() override
    {
        _map.clear();
    }

    std::size_t Count() const override
    {
        return _map.size();
    }

    std::size_t Capacity() const noexcept
    {
        return _map.bucket_count();
    }

    void SetCapacity(const std::size_t &capacity)
    {
        _map.reserve(capacity);
    }

    TValue &operator[](const TKey &key) const
    {
        if (!Contains(key))
        {
            throw std::out_of_range("Invalid key");
        }
        return _map[key];
    }

    TValue &operator[](const TKey &key)
    {
        if (!Contains(key))
        {
            throw std::out_of_range("Invalid key");
        }
        return _map[key];
    }
};