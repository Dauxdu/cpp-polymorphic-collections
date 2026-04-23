export module IEnumerator;

export template <typename T>
class IEnumerator
{
public:
    using value_type = T;

    virtual ~IEnumerator() = default;

    /// @brief Перемещает итератор на следующий элемент
    /// @return true, если итератор указывает на валидный элемент
    virtual bool MoveNext() = 0;

    /// @brief Возвращает текущий элемент (не продвигает итератор)
    /// @throw std::runtime_error если итератор не валиден
    virtual T &Current() = 0;

    /// @brief Const-версия Current()
    virtual const T &Current() const = 0;
};