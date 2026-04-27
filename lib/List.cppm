export module List;

import std;
import ICollection;
import IEnumerator;

export template <typename T>
class List final : public ICollection<T>
{
private:
	std::vector<T> _collection;

public:
	std::unique_ptr<IEnumerator<T>> GetEnumerator() const override
	{
		return std::make_unique<List>(_collection.begin(), _collection.end());
	}

	std::size_t Count() const override
	{
		return _collection.size();
	}

	void Add(const T &item) override
	{
		_collection.push_back(item);
	}

	[[nodiscard]] bool Remove(const T &item) override
	{
		return std::erase(_collection, item) > 0;
	}

	void Clear() override
	{
		_collection.clear();
	}

	bool Contains(const T &item) const override
	{
		return std::find(_collection.begin(), _collection.end(), item) != _collection.end();
	}

	[[nodiscard]] std::size_t Capacity() const
	{
		return _collection.capacity();
	}

	void SetCapacity(std::size_t capacity)
	{
		_collection.reserve(capacity);
	}

	[[nodiscard]] T &operator[](std::size_t index)
	{
		if (index >= _collection.size())
		{
			throw std::out_of_range("List::operator[]: index out of range");
		}
		return _collection[index];
	}

	[[nodiscard]] const T &operator[](std::size_t index) const
	{
		if (index >= _collection.size())
		{
			throw std::out_of_range("List::operator[]: index out of range");
		}
		return _collection[index];
	}

	void Insert(std::size_t index, const T &item)
	{
		if (index > _collection.size())
		{
			throw std::out_of_range("List::Insert: index out of range");
		}
		_collection.insert(_collection.begin() + index, item);
	}

	void RemoveAt(std::size_t index)
	{
		if (index >= _collection.size())
		{
			throw std::out_of_range("List::RemoveAt: index out of range");
		}
		_collection.erase(_collection.begin() + index);
	}
};