typedef char value_type;

class CircularBuffer {
    value_type * _buffer;
    size_t _head = 1;
    size_t _tail = 0;
    size_t _capacity;
    size_t _contents_size = 0;

    void increment_tail() {
        _tail++;
        _contents_size++;
        if (_tail >= _capacity) _tail = 0;
    }

    void decrement_tail() {
        if (_contents_size == 0) return;
        if (_tail == 0) _tail = _capacity - 1;
        else _tail--;
        _contents_size--;
    }

    void increment_head() {
        if (_contents_size == 0) return;
        _head++;
        _contents_size--;
        if (_head >= _capacity) _head = 0;
    }

    void decrement_head() {
        if (_head == 0) _head = _capacity - 1;
        else _head--;
        _contents_size++;
    }

public:
    CircularBuffer() {
        _buffer = nullptr;
        _capacity = 0;
    }

    ~CircularBuffer() {
        delete [] _buffer;
        _buffer = nullptr;
    }

    CircularBuffer(const CircularBuffer & cb) {
        _capacity = cb._capacity;
        _head = cb._head;
        _tail = cb._tail;
        _contents_size = cb._contents_size;
        _buffer = new value_type[_capacity];
        for (size_t i = 0; i < _capacity; i++) {
            _buffer[i] = cb._buffer[i];
        }
    }

    explicit CircularBuffer(int capacity) {
        _capacity = capacity;
        _buffer = new value_type[_capacity];
    }

    CircularBuffer(int capacity, const value_type & elem) {
        _capacity = capacity;
        _contents_size = capacity;
        _buffer = new value_type[_capacity];
        for (size_t i = 0; i < _capacity; i++) {
            _buffer[i] = elem;
        }
    }

    value_type & operator[](size_t index) {
        return _buffer[(_head + index) % _capacity];
    }

    const value_type & operator[](size_t index) const {
        return _buffer[(_head + index) % _capacity];
    }

    value_type & at(size_t index) {
        if (index >= _capacity) {
            throw std::out_of_range("Index is out of range!");
        }
        return this->operator[](index);
    }

    const value_type & at(size_t index) const {
        if (index >= _capacity) {
            throw std::out_of_range("Index is out of range!");
        }
        return this->operator[](index);
    }

    value_type & front() { return _buffer[_head]; }

    value_type & back() { return _buffer[_tail]; }

    const value_type & front() const { return _buffer[_head]; }

    const value_type & back() const { return _buffer[_tail]; }

    value_type * linearize() {
        if (empty()) return nullptr;
        while (_head--)
        {
            value_type x = _buffer[0];
            for (size_t i = 1; i < _capacity; i++) {
                _buffer[i - 1] = _buffer[i];
            }
            _buffer[_capacity - 1] = x;
        }
        _head = 0;
        _tail = _contents_size % _capacity;
        return &_buffer[0];
    }

    bool is_linearized() const {
        return !empty() && _head == 0;
    }

    void rotate(int new_begin) {
        auto* new_buffer = new value_type[_capacity];
        for (int i = new_begin; i < _contents_size; i++) {
            new_buffer[(_head + i - new_begin) % _capacity] = _buffer[(_head + i) % _capacity];
        }
        for (int i = 0; i < new_begin; i++) {
            new_buffer[(_head + _contents_size - new_begin + i) % _capacity] = _buffer[(_head + i) % _capacity];
        }
        delete [] _buffer;
        _buffer = new_buffer;
    }

    int size() const { return _contents_size; }

    bool empty() const { return _contents_size == 0; }

    bool full() const { return _contents_size == _capacity; }

    int reserve() const { return static_cast<int>(_capacity) - static_cast<int>(_contents_size); }

    int capacity() const { return _capacity; }

    //! Change the capacity of the <code>circular_buffer</code>.
    /*!
        \param new_capacity The new capacity.
        \throws Nothing
    */
    void set_capacity(int new_capacity) {
        if (new_capacity == _capacity) return;

        value_type* new_buffer = new value_type[new_capacity];//

        if (new_capacity > _capacity) {
            for (int i = 0; i < _contents_size; i++) {
                new_buffer[(_head + i) % new_capacity] = this->operator[](i);
            }
            _tail = (_head + _contents_size - 1) % new_capacity;
        }
        else {
            if (_contents_size > new_capacity) {//
                _head += (_contents_size - new_capacity);
                _head %= new_capacity;
                for (int i = 0; i < new_capacity; i++) {
                    new_buffer[(_head + i) % new_capacity] = this->operator[](i + _contents_size - new_capacity);
                }
            }
            else {
                for (int i = 0; i < _contents_size; i++) {
                    new_buffer[(_head + i) % new_capacity] = this->operator[](i);
                }
                _tail = (_head + _contents_size - 1) % new_capacity;
                _head %= new_capacity;
            }
        }

        _capacity = new_capacity;

        delete[] _buffer;
        _buffer = new_buffer;
    }

    void resize(int new_size, const value_type & item = value_type()) {
        if (_contents_size == new_size) return;
        size_t _contents_size_copy = _contents_size;
        if (new_size < _contents_size_copy) {
            for (int i = 0; i < _contents_size_copy - new_size; i++) {
                pop_front();
            }
        }
        else {
            for (int i = 0; i < new_size - _contents_size_copy; i++) {
                push_back(item);
            }
        }
    }

    CircularBuffer & operator=(const CircularBuffer & cb) {
        if (this != &cb) {
            _capacity = cb._capacity;
            _contents_size = cb._contents_size;
            _head = cb._head;
            _tail = cb._tail;

            delete [] _buffer;
            _buffer = new value_type[_capacity];
            for (size_t i = 0; i < _capacity; i++) {
                _buffer[i] = cb._buffer[i];
            }
        }
        return *this;
    }

    void swap(CircularBuffer & cb) {
        std::swap(this->_capacity, cb._capacity);
        std::swap(this->_head, cb._head);
        std::swap(this->_tail, cb._tail);
        std::swap(this->_contents_size, cb._contents_size);
        std::swap(this->_buffer, cb._buffer);
    }

    void push_back(const value_type & item = value_type()) {
        increment_tail();
        if (_contents_size > _capacity) increment_head();
        _buffer[_tail] = item;
    }

    void push_front(const value_type & item = value_type()) {
        decrement_head();
        if (_contents_size > _capacity) decrement_tail();
        _buffer[_head] = item;
    }

    void pop_back() { decrement_tail(); }

    void pop_front() { increment_head(); }

    void insert(int pos, const value_type & item = value_type()) {
        _buffer[(_head + pos) % _capacity] = item;
    }

    void erase(int first, int last) {
        auto* new_buffer = new value_type[_capacity];
        for (int i = 0; i < first; i++) {
            new_buffer[(_head + i) % _capacity] = _buffer[(_head + i) % _capacity];
        }
        if (last < _contents_size) {
            for (int i = first; i < _contents_size - last + 1; i++) {
                new_buffer[(_head + i) % _capacity] = _buffer[(_head + last + i - first) % _capacity];
            }
            _tail = (_head + _contents_size - last) % _contents_size;
        }
        else {
            _tail = (_head + first - 1) % _contents_size;
        }
        _contents_size -= (last - first);
        delete [] _buffer;
        _buffer = new_buffer;
    }

    void clear() {
        _head = 1;
        _tail = 0;
        _contents_size = 0;
    }

    friend bool operator==(const CircularBuffer & a, const CircularBuffer & b);
    friend bool operator!=(const CircularBuffer & a, const CircularBuffer & b);
};

bool operator==(const CircularBuffer & a, const CircularBuffer & b) {
    if (a._capacity == b._capacity && a._head == b._head && a._tail == b._tail && a._contents_size == b._contents_size) {
        for (size_t i = 0; i < a._contents_size; i++) {
            if (a[i] != b[i]) {
                return false;
            }
        }
        return true;
    }
    return false;
}

bool operator!=(const CircularBuffer & a, const CircularBuffer & b) {
    return !(a == b);
}

