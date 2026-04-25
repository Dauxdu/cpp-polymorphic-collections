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

    /**
     * @brief Вложенный класс-перечислитель для обхода элементов очереди.
     * @details Наследует IEnumerator<T>. Создаёт внутреннюю копию элементов
     *          в момент создания, обеспечивая семантику снимка.
     */
    class QueueEnumerator final : public IEnumerator<T>
    {
    private:
        std::vector<T> _items;
        std::ptrdiff_t _index = -1;

        bool IsValid() const
        {
            return _index >= 0 && _index < std::ssize(_items);
        }

    public:
        /**
         * @brief Конструктор перечислителя.
         * @param queue Исходная очередь, из которой создаётся снимок.
         */
        explicit QueueEnumerator(const std::queue<T> &queue)
        {
            std::queue<T> temp = queue;
            while (!temp.empty())
            {
                _items.push_back(std::move(temp.front()));
                temp.pop();
            }
        }

        /**
         * @brief Переходит к следующему элементу в перечислении.
         * @return true, если следующий элемент существует, иначе false.
         */
        bool MoveNext() override
        {
            ++_index;
            return _index < std::ssize(_items);
        }

        /**
         * @brief Получает текущий элемент в перечислении.
         * @return Ссылка на текущий элемент.
         * @throw std::out_of_range Если перечисление не находится в допустимой позиции.
         */
        T &Current() override
        {
            if (!IsValid())
            {
                throw std::logic_error("QueueEnumerator::Current: invalid position");
            }
            return _items[static_cast<std::size_t>(_index)];
        }

        /**
         * @brief Получает текущий элемент в перечислении (константная версия).
         * @return Константная ссылка на текущий элемент.
         * @throw std::out_of_range Если перечисление не находится в допустимой позиции.
         */
        const T &Current() const override
        {
            if (!IsValid())
            {
                throw std::logic_error("QueueEnumerator::Current: invalid position");
            }
            return _items[static_cast<std::size_t>(_index)];
        }
    };

public:
    /**
     * @brief Получает перечислитель для обхода коллекции.
     * @return Умный указатель на IEnumerator<T>.
     */
    [[nodiscard]] std::unique_ptr<IEnumerator<T>> GetEnumerator() override
    {
        return std::make_unique<QueueEnumerator>(_collection);
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