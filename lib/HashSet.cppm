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

public:
    /**
     * @brief Получает перечислитель для обхода коллекции.
     * @return Умный указатель на объект, реализующий `IEnumerator<T>`.
     * @details Каждый вызов создает новый независимый перечислитель,
     *          указывающий на начало текущей коллекции.
     */
    std::unique_ptr<IEnumerator<T>> GetEnumerator() const override
    {
        return std::make_unique<HashSet>(_collection);
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