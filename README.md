Реализация в C++ полиморфной иерархии контейнеров, например, как аналог иерархии коллекций в C#.

> [!WARNING]
> Каждый контейнер реализовывать как обёртку над соответствующим контейнером STL.

> [!TIP]
> По максимуму используйте исключения.

Ниже приведён упрощённый и адаптированный вариант требуемой иерархии из стандартной библиотеки C#.

Итератор [`IEnumerator<T>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerator-1):

```c#
interface IEnumerator<T> {
    public bool MoveNext();
    public T Current();
}
```

Интерфейс итерабельного объекта [`IEnumerable<T>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.ienumerable-1):

```c#
interface IEnumerable<T> {
    public IEnumerator<T> GetEnumerator();
}
```

Интерфейс коллекции [`ICollection<T>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.icollection-1):

```c#
interface ICollection<T> : IEnumerable<T> {
    public int Count();
    public void Add(T item);
    public void Clear();
    public bool Contains(T item);
    public bool Remove(T item);
}
```

Динамический массив [`List<T>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.list-1):

```c#
class List<T> : ICollection<T> {
    // ... (всё, что требуется, согласно реализуемым интерфейсам)

    public int Capacity();
    public void SetCapacity(int capacity);

    // operator[]
    public T this[int index] { get; set; }

    public void Insert(int index, T item);
    public void RemoveAt(int index);
}
```

Неупорядоченное множество [`HashSet<T>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.hashset-1):

```c#
class HashSet<T> : ICollection<T> {
    // ...

    public int Capacity();
    public void SetCapacity(int capacity);
}
```

> [!NOTE]
> Для элементов хеш-таблицы необходимо задать хеш-функцию и компаратор.
>
> В C# по дизайну метод [`GetHashCode`](https://learn.microsoft.com/en-us/dotnet/api/system.object.gethashcode) есть у любого объекта, а компаратор [`IEqualityComparer<T>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.iequalitycomparer-1) может быть задан с помощью соответствующей [перегрузки конструктора](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.hashset-1.-ctor?view=net-10.0#system-collections-generic-hashset-1-ctor%28system-collections-generic-iequalitycomparer%28%28-0%29%29%29).
>
> При реализации на C++ выберите один из двух вариантов:
>
> 1. _(Подход C++)_ Используйте дополнительные шаблонные параметры `Hash` и `KeyEqual` у класса хеш-таблицы, как в [`std::unordered_set`](https://en.cppreference.com/cpp/container/unordered_set).
> 2. _(Подход C#)_ Предоставьте соответствующую перегрузку конструктора с параметрами, позволяющими задать хеш-функцию и компаратор, и сохраните их в поля класса как [`std::function`](https://en.cppreference.com/cpp/utility/functional/function).

Неупорядоченный словарь [`Dictionary<TKey, TValue>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.dictionary-2):

```c#
class Dictionary<TKey, TValue> : ICollection<KeyValuePair<TKey, TValue>> {
    // ...

    public int Capacity();
    public void SetCapacity(int capacity);

    // operator[]
    public TValue this[TKey key] { get; set; }
```

Стек [`Stack<T>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.stack-1):

```c#
class Stack<T> : IEnumerable<T> {
    // ...

    public int Count();

    public void Push(T item);
    public T Peek();
    public T Pop();
}
```

Очередь [`Queue<T>`](https://learn.microsoft.com/en-us/dotnet/api/system.collections.generic.queue-1):

```c#
class Queue<T> : IEnumerable<T> {
    // ...

    public int Count();

    public T Dequeue();
    public T Peek();
    public void Enqueue(T item);
}
```
