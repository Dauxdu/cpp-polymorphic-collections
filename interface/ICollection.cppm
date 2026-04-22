export module ICollection;

import std;
import IEnumerable;

export template <typename T>
class ICollection : public IEnumerable<T>
{
public:
    virtual ~ICollection() = default;

    /// @brief Возвращает количество элементов
    virtual std::size_t Count() const = 0;

    /// @brief Добавляет элемент в коллекцию
    virtual void Add(const T &_item) = 0;

    /// @brief Удаляет все элементы
    virtual void Clear() = 0;

    /// @brief Проверяет наличие элемента
    /// @note Требует, чтобы T поддерживал operator==
    virtual bool Contains(const T &_item) const = 0;

    /// @brief Удаляет первое вхождение элемента
    /// @return true, если элемент был найден и удалён
    virtual bool Remove(const T &_item) = 0;
};