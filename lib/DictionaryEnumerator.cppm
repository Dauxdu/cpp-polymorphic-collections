export module DictionaryEnumerator;

import std;
import IEnumerator;

export template <typename TKey, typename TValue, typename Hash = std::hash<TKey>, typename KeyEqual = std::equal_to<TKey>>
class DictionaryEnumerator final : public IEnumerator<std::pair<const TKey, TValue>>
{
private:
    bool _started = false;
    typename std::unordered_map<TKey, TValue, Hash, KeyEqual>::iterator _iter;
    typename std::unordered_map<TKey, TValue, Hash, KeyEqual>::iterator _end;

public:
    explicit DictionaryEnumerator(std::unordered_map<TKey, TValue, Hash, KeyEqual> &dictionary)
        : _iter(dictionary.begin()), _end(dictionary.end()), _started(false)
    {
    }

    bool MoveNext() override
    {
        if (!_started)
        {
            _started = true;
            return _iter != _end;
        }
        ++_iter;
        return _iter != _end;
    }

    std::pair<const TKey, TValue> &Current() override
    {
        if (!_started || _iter == _end)
        {
            throw std::logic_error("Invalid iterator");
        }
        return *_iter;
    }

    const std::pair<const TKey, TValue> &Current() const override
    {
        if (!_started || _iter == _end)
        {
            throw std::logic_error("Invalid iterator");
        }
        return *_iter;
    }
};