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
    private:
        std::unordered_set<T, Hash, KeyEqual>::const_iterator _it;
        std::unordered_set<T, Hash, KeyEqual>::const_iterator _end;

        enum class State
        {
            BeforeFirst,
            Valid,
            AfterLast
        } _state = State::BeforeFirst;

        bool IsValid() const
        {
            return _state == State::Valid;
        }

    public:
        explicit HashSetEnumerator(const std::unordered_set<T, Hash, KeyEqual> &set) : _it(set.begin()), _end(set.end()) {}

        bool MoveNext() override
        {
            switch (_state)
            {
            case State::BeforeFirst:
                if (_it != _end)
                {
                    _state = State::Valid;
                    return true;
                }
                _state = State::AfterLast;
                return false;

            case State::Valid:
                ++_it;
                if (_it != _end)
                {
                    return true;
                }
                _state = State::AfterLast;
                return false;

            case State::AfterLast:
                return false;
            }
            return false;
        }

        const T &Current() const override
        {
            if (!IsValid())
            {
                throw std::logic_error("HashSetEnumerator::Current: enumerator not positioned");
            }
            return *_it;
        }

        T &Current() override
        {
            return const_cast<T &>(std::as_const(*this).Current());
        }
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
