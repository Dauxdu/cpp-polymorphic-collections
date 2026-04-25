export module HashSet;

import std;
import IEnumerator;
import ICollection;

/**
 * @brief Хэш-множество, реализующее интерфейс коллекции.
 * @tparam T Тип хранимых элементов.
 * @tparam Hash Функция хэширования (по умолчанию `std::hash<T>`).
 * @tparam KeyEqual Функция сравнения ключей на равенство (по умолчанию `std::equal_to<T>`).
 * @details Обёртка над `std::unordered_set`.
 */
export template <typename T, typename Hash = std::hash<T>, typename KeyEqual = std::equal_to<T>>
class HashSet : public ICollection<T>
{
private:
    std::unordered_set<T, Hash, KeyEqual> _collection;

    /**
     * @brief Перечислитель для последовательного обхода элементов `HashSet`.
     * @details Реализует конечный автомат состояний для безопасной итерации.
     *          Не допускает изменения коллекции во время обхода.
     */
    class HashSetEnumerator final : public IEnumerator<T>
    {
    private:
        /// Текущий итератор на элемент коллекции.
        std::unordered_set<T, Hash, KeyEqual>::const_iterator _it;
        /// Итератор, указывающий на конец коллекции.
        std::unordered_set<T, Hash, KeyEqual>::const_iterator _end;

        /**
         * @brief Состояние перечислителя.
         * @details Используется для контроля корректности вызовов `MoveNext()` и `Current()`.
         */
        enum class State
        {
            BeforeFirst, ///< Начальное состояние: перед первым элементом
            Valid,       ///< Перечислитель указывает на валидный элемент
            AfterLast    ///< Конечное состояние: после последнего элемента
        } _state = State::BeforeFirst;

        /**
         * @brief Проверяет, находится ли перечислитель в валидном состоянии.
         * @return `true`, если `_state == State::Valid`, иначе `false`.
         */
        bool IsValid() const
        {
            return _state == State::Valid;
        }

    public:
        /**
         * @brief Конструктор перечислителя.
         * @param set Ссылка на множество, по которому будет осуществляться обход.
         * @details Инициализирует итераторы на начало и конец коллекции,
         *          устанавливает начальное состояние `BeforeFirst`.
         */
        explicit HashSetEnumerator(const std::unordered_set<T, Hash, KeyEqual> &set) : _it(set.begin()), _end(set.end()) {}

        /**
         * @brief Перемещает перечислитель к следующему элементу.
         * @return `true`, если переход успешен и элемент доступен; `false`, если конец коллекции достигнут.
         * @details Реализует логику перехода между состояниями конечного автомата.
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
                return false;

            case State::AfterLast:
                return false;
            }
            return false;
        }

        /**
         * @brief Возвращает ссылку на текущий элемент (const версия).
         * @return Константная ссылка на текущий элемент.
         * @throws std::logic_error Если перечислитель не находится в состоянии `Valid`.
         */
        const T &Current() const override
        {
            if (!IsValid())
            {
                throw std::logic_error("HashSetEnumerator::Current: enumerator not positioned");
            }
            return *_it;
        }

        /**
         * @brief Возвращает ссылку на текущий элемент (non-const версия).
         * @return Неконстантная ссылка на текущий элемент.
         */
        T &Current() override
        {
            return const_cast<T &>(std::as_const(*this).Current());
        }
    };

public:
    /**
     * @brief Получает перечислитель для обхода коллекции.
     * @return Умный указатель на объект, реализующий `IEnumerator<T>`.
     * @details Каждый вызов создает новый независимый перечислитель,
     *          указывающий на начало текущей коллекции.
     */
    std::unique_ptr<IEnumerator<T>> GetEnumerator() override
    {
        return std::make_unique<HashSetEnumerator>(_collection);
    }

    /**
     * @brief Добавляет элемент в множество.
     * @param item Элемент для добавления.
     * @details Если элемент уже присутствует в множестве, операция игнорируется.
     */
    void Add(const T &item) override
    {
        _collection.insert(item);
    }

    /**
     * @brief Удаляет элемент из множества.
     * @param item Элемент для удаления.
     * @return `true`, если элемент был найден и удалён, `false`, если элемент отсутствовал.
     */
    bool Remove(const T &item) override
    {
        return _collection.erase(item) > 0;
    }

    /**
     * @brief Очищает множество, удаляя все элементы.
     * @details Ёмкость контейнера при этом сохраняется.
     */
    void Clear() override
    {
        _collection.clear();
    }

    /**
     * @brief Возвращает количество элементов в множестве.
     * @return Количество хранимых элементов.
     */
    [[nodiscard]] std::size_t Count() const override
    {
        return _collection.size();
    }

    /**
     * @brief Проверяет наличие элемента в множестве.
     * @param item Искомый элемент.
     * @return `true`, если элемент присутствует, иначе `false`.
     */
    bool Contains(const T &item) const override
    {
        return _collection.find(item) != _collection.end();
    }

    /**
     * @brief Возвращает текущую ёмкость хэш-таблицы (количество бакетов).
     * @return Количество бакетов во внутреннем `std::unordered_set`.
     */
    [[nodiscard]] std::size_t Capacity() const
    {
        return _collection.bucket_count();
    }

    /**
     * @brief Устанавливает минимальную ёмкость для хэш-таблицы.
     * @param capacity Желаемое количество бакетов.
     * @details Вызывает `reserve()` внутреннего контейнера. Если `capacity` меньше
     *          текущей ёмкости, операция может быть проигнорирована реализацией.
     */
    void SetCapacity(std::size_t capacity)
    {
        _collection.reserve(capacity);
    }
};