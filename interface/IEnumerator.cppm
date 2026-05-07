export module IEnumerator;

export template <typename TValue>
class IEnumerator
{
public:
    virtual ~IEnumerator() = default;

    /// @brief Перемещает итератор на следующий элемент
    /// @return true, если итератор указывает на валидный элемент
    virtual bool MoveNext() = 0;

    /// @brief Возвращает текущий элемент (не продвигает итератор)
    /// @throw std::runtime_error если итератор не валиден
    virtual TValue &Current() = 0;

    /// @brief Const-версия Current()
    virtual const TValue &Current() const = 0;
};