export module ICollection;

import std;
import IEnumerable;

export template <typename T>
class ICollection : public IEnumerable<T>
{
public:
    virtual ~ICollection() = default;

    /// @brief Добавляет элемент в коллекцию
    virtual void Add(const T &item) = 0;

    /// @brief Удаляет первое вхождение элемента
    /// @return true, если элемент был найден и удалён
    virtual bool Remove(const T &item) = 0;

    /// @brief Удаляет все элементы
    virtual void Clear() = 0;

    /// @brief Возвращает количество элементов
    virtual std::size_t Count() const = 0;

    /// @brief Проверяет наличие элемента
    /// @note Требует, чтобы T поддерживал operator==
    virtual bool Contains(const T &item) const = 0;
};