export module Stack;

import std;
import IEnumerator;
import IEnumerable;

/**
 * @brief Шаблонный класс стека, реализующий интерфейс коллекции.
 *
 * @tparam T Тип хранимых элементов.
 *
 * @note Класс не является потокобезопасным.
 */
export template <typename T>
class Stack : public IEnumerable<T>
{
private:
    std::stack<T> _collection;

public:
    /**
     * @brief Получает перечислитель для обхода коллекции.
     * @return Умный указатель на IEnumerator<T>.
     */
    [[nodiscard]] std::unique_ptr<IEnumerator<T>> GetEnumerator() const override
    {
        return std::make_unique<Stack>(_collection);
    }

    /**
     * @brief Количество элементов в стеке.
     */
    [[nodiscard]] std::size_t Count() const override
    {
        return _collection.size();
    }

    /**
     * @brief Добавляет элемент в конец стека.
     * @param item Элемент для добавления.
     */
    void Push(T item)
    {
        _collection.push(item);
    }

    /**
     * @brief Извлекает и удаляет элемент из вершины стека.
     * @return Извлечённый элемент.
     * @throw std::out_of_range Если стек пуст.
     */
    [[nodiscard]] T Pop()
    {
        if (_collection.empty())
        {
            throw std::out_of_range("Stack::Pop: empty stack");
        }
        T item = std::move(_collection.top());
        _collection.pop();
        return item;
    }

    /**
     * @brief Возвращает элемент из вершины стека без его удаления.
     * @return Константная ссылка на элемент.
     * @throw std::out_of_range Если стек пуст.
     */
    [[nodiscard]] const T &Peek() const
    {
        if (_collection.empty())
        {
            throw std::out_of_range("Stack::Peek: empty stack");
        }
        return _collection.top();
    }
};
