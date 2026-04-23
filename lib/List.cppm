export module List;

import std;
import ICollection;
import IEnumerator;

export template <typename T>
class List final : public ICollection<T>
{
private:
	std::vector<T> _collection;

	class ListEnumerator final : public IEnumerator<T>
	{
	private:
		T *_current = nullptr;

	public:
		explicit ListEnumerator() = default;

		explicit ListEnumerator(const T *source_ptr) noexcept
		{
			_current = source_ptr;
		}

		bool MoveNext() override
		{
			if (_current == nullptr)
			{
				return false;
			}
			else
			{
				++_current;
				return true;
			}
		}

		T &Current() override
		{
			if (_current == nullptr)
			{
				throw std::logic_error("Invalid enumerator state");
			}
			return *_current;
		}

		const T &Current() const override
		{
			if (_current == nullptr)
			{
				throw std::logic_error("Invalid enumerator state");
			}
			return *_current;
		}
	};

	ListEnumerator iterator();

public:
	std::size_t Count() const override
	{
		return _collection.size();
	}

	void Add(const T &item) override
	{
		_collection.push_back(item);
	}

	void Clear() override
	{
		_collection.clear();
	}

	bool Contains(const T &item) const override
	{
		return std::find(_collection.begin(), _collection.end(), item) != _collection.end();
	}

	[[nodiscard]] bool Remove(const T &item) override
	{
		//...
	}

	std::size_t Capacity() const
	{
		return _collection.capacity();
	}

	void SetCapacity(const std::size_t capacity)
	{
		_collection.reserve(capacity);
	}

	T &operator[](const std::size_t &index)
	{
		return &_collection[index];
	}

	const T &operator[](const std::size_t &index) const
	{
		return _collection[index];
	}
};

// int Count(); - size_type size() const noexept;
// void Add(T item); - void push_back(T item)
// void Clear(); - void clear()
// bool Contains(T item); find(v.begin(), v.end(), T item) != v.end()
// bool Remove(T item); find + erase

// int Capacity(); capacity()
// void SetCapacity(int capacity); v.reserve(int)
// T this[int index] { get; set; } - T operator[](int)
// void Insert(int index, T item); emplace (int index, T item)
// void RemoveAt(int index); erase(int index)