export module List;

// int Count(); - size_type size() const noexept;
// void Add(T item); - void push_back(T item)
// void Clear(); - void clear()
// bool Contains(T item); find(v.begin(), v.end(), T item) != v.end()
// bool Remove(T item); find + erase

// int Capacity(); capacity()
// void SetCapacity(int capacity); v.reserve(int)
// T this[int index] { get; set; } - T operator[](int)
// void Insert(int index, T item); emplace (int index, T item)
// void RemoveAt(int index); erase(int index)