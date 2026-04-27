export module IEnumerable;

import std;
import IEnumerator;

export template <typename T>
class IEnumerable
{
public:
    virtual ~IEnumerable() = default;

    /// @brief Получает итератор для обхода коллекции
    virtual std::unique_ptr<IEnumerator<T>> GetEnumerator() const = 0;
};