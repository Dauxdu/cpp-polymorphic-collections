export module Queue;

import std;
import IEnumerator;
import ICollection;

export template <typename T>
class Queue : public ICollection<T>
{
private:
    std::queue<T> _collection;

    class QueueEnumerator final : public IEnumerator<T>
    {
    private:
        std::vector<T> _items;
        std::ptrdiff_t _index = -1;

    public:
        explicit QueueEnumerator(const std::queue<T> &queue)
        {
            std::queue<T> temp = queue;
            while (!temp.empty())
            {
                _items.push_back(std::move(temp.front()));
                temp.pop();
            }
        }

        bool MoveNext() override
        {
            ++_index;
            return _index < std::ssize(_items);
        }

        T &Current() override
        {
            if (_index < 0 || _index >= std::ssize(_items))
                throw std::out_of_range("Enumerator is not at a valid position");
            return _items[static_cast<std::size_t>(_index)];
        }

        const T &Current() const override
        {
            if (_index < 0 || _index >= std::ssize(_items))
                throw std::out_of_range("Enumerator is not at a valid position");
            return _items[static_cast<std::size_t>(_index)];
        }
    };

public:
    std::unique_ptr<IEnumerator<T>> GetEnumerator() override
    {
        return std::make_unique<QueueEnumerator>(_collection);
    }

    void Add(const T &item) override
    {
        Enqueue(item);
    }

    bool Remove(const T &item) override
    {
        std::queue<T> temp;
        bool found = false;
        while (!_collection.empty())
        {
            if (!found && _collection.front() == item)
            {
                found = true;
                _collection.pop();
            }
            else
            {
                temp.push(std::move(_collection.front()));
                _collection.pop();
            }
        }
        _collection = std::move(temp);
        return found;
    }

    void Clear() noexcept override
    {
        _collection = std::queue<T>();
    }

    std::size_t Count() const noexcept override
    {
        return _collection.size();
    }

    bool Contains(const T &item) const override
    {
        std::queue<T> temp = _collection;
        while (!temp.empty())
        {
            if (temp.front() == item)
                return true;
            temp.pop();
        }
        return false;
    }

    void Enqueue(const T &item)
    {
        _collection.push(item);
    }

    T Dequeue()
    {
        if (_collection.empty())
        {
            throw std::out_of_range("Queue is empty");
        }
        T item = std::move(_collection.front());
        _collection.pop();
        return item;
    }

    T Peek() const
    {
        if (_collection.empty())
        {
            throw std::out_of_range("Queue is empty");
        }
        return _collection.front();
    }
};