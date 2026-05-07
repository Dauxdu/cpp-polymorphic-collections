export module Dictionary;

import std;
import IEnumerator;
import ICollection;
import DictionaryEnumerator;

export template <typename TKey, typename TValue, typename Hash = std::hash<TKey>, typename KeyEqual = std::equal_to<TKey>>
class Dictionary final : public ICollection<std::pair<const TKey, TValue>>
{
private:
    std::unordered_map<TKey, TValue, Hash, KeyEqual> _dictionary;

public:
    bool Contains(const TKey &key) const
    {
        return _dictionary.contains(key);
    }

    void Add(const std::pair<const TKey, TValue> &item) override
    {
        auto [it, inserted] = _dictionary.insert(item);
        if (!inserted)
        {
            return;
        }
    }

    bool Remove(const TKey &key) override
    {
        return _dictionary.erase(key) > 0;
    }

    void Clear() override
    {
        _dictionary.clear();
    }

    std::size_t Count() const override
    {
        return _dictionary.size();
    }

    std::size_t Capacity() const noexcept
    {
        return _dictionary.bucket_count();
    }

    void SetCapacity(const std::size_t capacity)
    {
        _dictionary.reserve(capacity);
    }

    const TValue &operator[](const TKey &key) const
    {
        auto it = _dictionary.find(key);
        if (it == _dictionary.end())
        {
            throw std::out_of_range("Invalid key");
        }
        return it->second;
    }

    TValue &operator[](const TKey &key)
    {
        auto it = _dictionary.find(key);
        if (it == _dictionary.end())
        {
            throw std::out_of_range("Invalid key");
        }
        return it->second;
    }

    std::unique_ptr<IEnumerator<std::pair<const TKey, TValue>>> GetEnumerator() const
    {
        return std::make_unique<DictionaryEnumerator<TKey, TValue, Hash, KeyEqual>>(_dictionary);
    }
};