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
		std::vector<T>::const_iterator _current;
		std::vector<T>::const_iterator _end;

		enum class State
		{
			BeforeFirst,
			Valid,
			AfterLast
		} _state = State::BeforeFirst;

		bool IsValid() const { return _state == State::Valid; }

	public:
		explicit ListEnumerator(std::vector<T>::const_iterator begin, std::vector<T>::const_iterator end) : _current(begin), _end(end) {}

		bool MoveNext() override
		{
			switch (_state)
			{
			case State::BeforeFirst:
				if (_current != _end)
				{
					_state = State::Valid;
					return true;
				}
				_state = State::AfterLast;
				return false;

			case State::Valid:
				++_current;
				if (_current != _end)
				{
					return true;
				}
				_state = State::AfterLast;
				[[fallthrough]];

			case State::AfterLast:
				return false;
			}
			return false; // unreachable
		}

		T &Current() override
		{
			if (!IsValid())
			{
				throw std::logic_error("ListEnumerator::Current: invalid position");
			}
			return const_cast<T &>(*_current);
		}

		const T &Current() const override
		{
			if (!IsValid())
			{
				throw std::logic_error("ListEnumerator::Current: invalid position");
			}
			return *_current;
		}
	};

public:
	std::unique_ptr<IEnumerator<T>> GetEnumerator() override
	{
		return std::make_unique<ListEnumerator>(_collection.begin(), _collection.end());
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