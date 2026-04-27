export module Queue;

import std;
import IEnumerator;
import IEnumerable;

/**
 * @brief Шаблонный класс очереди, реализующий интерфейс коллекции.
 *
 * @tparam T Тип хранимых элементов.
 *
 * @note Класс не является потокобезопасным.
 */
export template <typename T>
class Queue : public IEnumerable<T>
{
private:
    std::queue<T> _collection;

public:
    /**
     * @brief Получает перечислитель для обхода коллекции.
     * @return Умный указатель на IEnumerator<T>.
     */
    [[nodiscard]] std::unique_ptr<IEnumerator<T>> GetEnumerator() const override
    {
        return std::make_unique<Queue>(_collection);
    }

    /**
     * @brief Количество элементов в очереди.
     */
    [[nodiscard]] std::size_t Count() const override
    {
        return _collection.size();
    }

    /**
     * @brief Добавляет элемент в конец очереди.
     * @param item Элемент для добавления.
     */
    void Enqueue(T item)
    {
        _collection.push(item);
    }

    /**
     * @brief Извлекает и удаляет элемент из начала очереди.
     * @return Извлечённый элемент.
     * @throw std::out_of_range Если очередь пуста.
     */
    [[nodiscard]] T Dequeue()
    {
        if (_collection.empty())
        {
            throw std::out_of_range("Queue::Dequeue: empty queue");
        }
        T item = std::move(_collection.front());
        _collection.pop();
        return item;
    }

    /**
     * @brief Возвращает элемент из начала очереди без его удаления.
     * @return Константная ссылка на элемент.
     * @throw std::out_of_range Если очередь пуста.
     */
    [[nodiscard]] const T &Peek() const
    {
        if (_collection.empty())
        {
            throw std::out_of_range("Queue::Peek: empty queue");
        }
        return _collection.front();
    }
};