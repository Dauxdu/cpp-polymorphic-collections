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
        std::queue<T>::container_type::const_iterator _it;
        std::queue<T>::container_type::const_iterator _end;

        enum class State
        {
            BeforeFirst,
            Valid,
            AfterLast
        } _state = State::BeforeFirst;

        bool IsValid() const { return _state == State::Valid; }

    public:
        /**
         * @brief Конструктор перечислителя.
         * @param queue Исходная очередь.
         */
        explicit QueueEnumerator(const std::queue<T> &queue) : _it(queue.c.begin()), _end(queue.c.end()) {}

        /**
         * @brief Переходит к следующему элементу.
         * @return true, если удалось перейти, false если достигнут конец.
         */
        bool MoveNext() override
        {
            switch (_state)
            {
            case State::BeforeFirst:
                if (_it != _end)
                {
                    _state = State::Valid;
                    return true;
                }
                _state = State::AfterLast;
                return false;

            case State::Valid:
                ++_it;
                if (_it != _end)
                {
                    return true;
                }
                _state = State::AfterLast;

            case State::AfterLast:
                return false;
            }
            return false;
        }

        /**
         * @brief Возвращает текущий элемент (non-const версия).
         * @throw std::logic_error если итератор не в валидном состоянии.
         */
        T &Current() override
        {
            if (!IsValid())
            {
                throw std::logic_error("QueueEnumerator::Current: enumerator not positioned");
            }
            return const_cast<T &>(*_it);
        }

        /**
         * @brief Возвращает текущий элемент (const версия).
         * @throw std::logic_error если итератор не в валидном состоянии.
         */
        const T &Current() const override
        {
            if (!IsValid())
            {
                throw std::logic_error("QueueEnumerator::Current: enumerator not positioned");
            }
            return *_it;
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