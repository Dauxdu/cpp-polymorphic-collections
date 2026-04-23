export module Queue;

import std;
import IEnumerator;
import ICollection;

/**
 * @brief Шаблонный класс очереди, реализующий интерфейс коллекции.
 *
 * @tparam T Тип хранимых элементов. Должен быть копируемым/перемещаемым
 *           и поддерживать оператор `operator==` (для методов Contains/Remove).
 *
 * @details Класс предоставляет стандартные операции очереди (Enqueue, Dequeue, Peek),
 *          а также методы интерфейса ICollection<T>. Итерация осуществляется
 *          через снимок.
 *
 * @note Класс не является потокобезопасным.
 */
export template <typename T>
class Queue : public ICollection<T>
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
            if (_index < 0 || _index >= std::ssize(_items))
                throw std::out_of_range("Enumerator is not at a valid position");
            return _items[static_cast<std::size_t>(_index)];
        }

        /**
         * @brief Получает текущий элемент в перечислении (константная версия).
         * @return Константная ссылка на текущий элемент.
         * @throw std::out_of_range Если перечисление не находится в допустимой позиции.
         */
        const T &Current() const override
        {
            if (_index < 0 || _index >= std::ssize(_items))
                throw std::out_of_range("Enumerator is not at a valid position");
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
     * @brief Добавляет элемент в конец очереди.
     * @param item Добавляемый элемент.
     */
    void Enqueue(const T &item)
    {
        _collection.push(item);
    }

    /**
     * @brief Добавляет элемент в конец очереди.
     * @param item Добавляемый элемент.
     * @details Делегирует вызов методу Enqueue.
     */
    void Add(const T &item) override
    {
        Enqueue(item);
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
            throw std::out_of_range("Queue is empty");
        }
        T item = std::move(_collection.front());
        _collection.pop();
        return item;
    }

    /**
     * @brief Удаляет первое вхождение элемента из очереди.
     * @param item Удаляемый элемент.
     * @return true, если элемент был найден и удалён, иначе false.
     * @note Не удаляет дубликаты.
     */
    [[nodiscard]] bool Remove(const T &item) override
    {
        std::queue<T> temp;
        bool found = false;
        while (!_collection.empty())
        {
            if (!found && _collection.front() == item)
            {
                found = true;
                _collection.pop();
            }
            else
            {
                temp.push(std::move(_collection.front()));
                _collection.pop();
            }
        }
        _collection = std::move(temp);
        return found;
    }

    /**
     * @brief Очищает очередь.
     * @details Использует присваивание пустой очереди.
     */
    void Clear() noexcept override
    {
        _collection = std::queue<T>();
    }

    /**
     * @brief Возвращает количество элементов в очереди.
     * @return Количество элементов.
     */
    [[nodiscard]] std::size_t Count() const noexcept override
    {
        return _collection.size();
    }

    /**
     * @brief Возвращает элемент из начала очереди без его удаления.
     * @return Элемент в начале очереди.
     * @throw std::out_of_range Если очередь пуста.
     */
    [[nodiscard]] T Peek() const
    {
        if (_collection.empty())
        {
            throw std::out_of_range("Queue is empty");
        }
        return _collection.front();
    }

    /**
     * @brief Проверяет наличие элемента в очереди.
     * @param item Искомый элемент.
     * @return true, если элемент найден; иначе false.
     */
    [[nodiscard]] bool Contains(const T &item) const override
    {
        std::queue<T> temp = _collection;
        while (!temp.empty())
        {
            if (temp.front() == item)
                return true;
            temp.pop();
        }
        return false;
    }
};