export module HashSet;

import std;
import IEnumerator;
import ICollection;

export template <typename T, typename Hash = std::hash<T>, typename KeyEqual = std::equal_to<T>>
class HashSet : public ICollection<T>
{
private:
    std::unordered_set<T, Hash, KeyEqual> _collection;

    class HashSetEnumerator final : public IEnumerator<T>
    {
        // ...
    };

public:
    std::unique_ptr<IEnumerator<T>> GetEnumerator() override
    {
        return std::make_unique<HashSetEnumerator>(_collection);
    }

    void Add(const T &item) override
    {
        _collection.insert(item);
    }

    bool Remove(const T &item) override
    {
        return _collection.erase(item) > 0;
    }

    void Clear() override
    {
        _collection.clear();
    }

    [[nodiscard]] std::size_t Count() const override
    {
        return _collection.size();
    }

    bool Contains(const T &item) const override
    {
        return _collection.find(item) != _collection.end();
    }

    [[nodiscard]] std::size_t Capacity() const
    {
        return _collection.bucket_count();
    }

    void SetCapacity(std::size_t capacity)
    {
        _collection.reserve(capacity);
    }
};
