#ifndef CIRCULARBUFFER_TI_CIRCULARBUFFER_H
#define CIRCULARBUFFER_TI_CIRCULARBUFFER_H

template<typename value_type>

class CircularBuffer {
    value_type * _buffer;
    size_t _head = 1; //индекс начала буфера в массиве _buffer
    size_t _tail = 0; //индекс конца буфера в массиве _buffer
    size_t _capacity; //вместимость (размер массива _buffer)
    size_t _contents_size = 0; //сколько уже заполнено

    //сдвинуть "хвост" вправо (новый элемент в конце буфера)
    void increment_tail() {
        _tail++;
        _contents_size++;
        if (_tail >= _capacity) _tail = 0; //если индекс хвоста выйдет за пределы массива, значит конец - в начале массива
    }

    //сдвинуть "хвост" влево (убрать элемент с конца буфера)
    void decrement_tail() {
        if (_contents_size == 0) return;
        if (_tail == 0) _tail = _capacity - 1; //если хвост был в начале и мы уменьшаем, то теперь хвост указывает на конец массива
        else _tail--;
        _contents_size--;
    }

    //сдвинуть "голову" вправо (убрать элемент с начала буфера)
    void increment_head() {
        if (_contents_size == 0) return;
        _head++;
        _contents_size--;
        if (_head >= _capacity) _head = 0; //если индекс головы выйдет за пределы массива, значит начало - в начале массива
    }

    //сдвинуть "голову" влево (новый элемент в начале буфера)
    void decrement_head() {
        if (_head == 0) _head = _capacity - 1; //если голова была в начале и мы уменьшаем, то теперь голова указывает на конец массива
        else _head--;
        _contents_size++;
    }

    class Iterator {
        size_t index; // индекс элемента в буфере
        bool last; //является ли элементом, идущим после последнего
        CircularBuffer* parent; // родитель, по которому проходим итератором
    public:
        Iterator(size_t index, CircularBuffer* parent) : index(index), parent (parent) {
            last = index >= parent->size();  // если индекс больше количества эл-в, то = true
        }
        // возвращаем измененый итератор
        Iterator &operator++(){
            if (index < parent->size()) //если не дошли до конца буфера
                index ++;
            if (index >= parent->size()) // когда достигнем конца
                last = true; // говорим, что это конец буфера
            return *this;
        }
        //озвращаем неизмененный итератор
        Iterator operator++(int i){
            Iterator it = *this;
            ++(*this);
            return it;
        }

        bool operator==(const Iterator &it) const {
            return this->parent == it.parent && (this->last == it.last || this->index == it.index);
        }

        bool operator!=(const Iterator &it) {
            return !(*this == it);
        }

        // получение эл-та по индексу
        value_type operator *(){
            return parent->at(index);
        }

    };




public:
    CircularBuffer(){ // создали пустой буфер
        _buffer = nullptr;
        _capacity = 0;
    }

    ~CircularBuffer(){
        delete [] _buffer;
        _buffer = nullptr;
    }

    CircularBuffer(const CircularBuffer & cb){
        _capacity = cb._capacity;
        _head = cb._head;
        _tail = cb._tail;
        _contents_size = cb._contents_size;
        _buffer = new value_type [_capacity];
        for (size_t i = 0; i < _capacity; i++) {
            _buffer[i] = cb._buffer[i];
        }
    }

    //Конструирует буфер заданной ёмкости.
    explicit CircularBuffer(int capacity){
        _capacity = capacity;
        _buffer = new value_type [_capacity];
    }

    //Конструирует буфер заданной ёмкости, целиком заполняет его элементом elem.
    CircularBuffer(int capacity, const value_type & elem){
        _capacity = capacity;
        _contents_size = capacity;
        _buffer = new value_type [_capacity];
        for (size_t i = 0; i < _capacity; i++){
            _buffer[i] = elem;
        }
    }

    //Доступ по индексу. Не проверяют правильность индекса.
    value_type & operator[](size_t index){
        return  _buffer[(_head+index) % _capacity];
    }

    const value_type & operator[](size_t index) const{
        return _buffer[(_head + index) % _capacity];
    }

    //Доступ по индексу. Методы бросают исключение в случае неверного индекса.
    value_type & at(size_t index){
        if (index >= _capacity)
            throw std::out_of_range ("index is out range");
        return this->operator[](index);
    }

    const value_type & at(size_t index) const{
        if (index >= _capacity)
            throw std::out_of_range ("index is out range");
        return this->operator[](index);
    }

    //Ссылка на первый элемент.
    value_type & front(){
        return _buffer[_head];
    }

    //Ссылка на последний элемент.
    value_type & back(){
        return _buffer[_tail];
    }

    const value_type & front() const{
        return _buffer[_head];
    }

    const value_type & back() const{
        return _buffer[_tail];
    }

    //Линеаризация - сдвинуть кольцевой буфер так, что его первый элемент
    //переместится в начало аллоцированной памяти. Возвращает указатель
    //на первый элемент.
    //т.е. делаем так, чтобы 1 эл-т буфера оказался в начале памяти массива _buffer
    value_type * linearize(){
        if (empty())
            return nullptr;
        while (_head--){
            value_type x = _buffer[0];
            for (size_t i = 1; i < _capacity; i++){
                _buffer [i-1] = _buffer [i];
            }
            _buffer [_capacity - 1] = x;
            _head = 0;
            _tail = _contents_size % _capacity;
            return &_buffer[0];
        }
    }

    //Проверяет, является ли буфер линеаризованным.
    bool is_linearized() const{
        return !empty() && _head == 0;
    }

    //Сдвигает буфер так, что по нулевому индексу окажется элемент
    //с индексом new_begin.
    void rotate(int new_begin){
        CircularBuffer tmp (_capacity);
        for (int i = new_begin; i < _contents_size; i++) {
            tmp.push_back(this->at(i));
        }
        for (int i = 0; i < new_begin; ++i) {
            tmp.push_back(this->at(i));
        }
        this->swap(tmp);
    }

    //Количество элементов, хранящихся в буфере.
    int size() const{
        return _contents_size;
    }

    bool empty() const{
        return _contents_size == 0;
    }

    //true, если size() == capacity().
    bool full() const{
        return _capacity == _contents_size;
    }

    //Количество свободных ячеек в буфере.
    int reserve() const{
        return static_cast<int>(_capacity) - static_cast<int>(_contents_size);
    }

    //ёмкость буфера
    int capacity() const{
        return _capacity;
    }

    //установить новую вместимоть, если все эл-ты не влезли, отбрасываем последние
    void set_capacity(int new_capacity){
        CircularBuffer tmp(new_capacity);
        int new_content_size = std::min (new_capacity, static_cast<int>(_contents_size));
        for (int i = 0; i < new_content_size; i++) {
            tmp.push_back(this->at(i));
        }
        this ->swap(tmp);
    }

    //Изменяет размер буфера.
    //В случае расширения, новые элементы заполняются элементом item.
    void resize(int new_size, const value_type & item = value_type()){
        CircularBuffer tmp (_capacity);
        int min = std::min(new_size, static_cast<int>(_capacity));
        for (int i = 0; i < min; i++) {
            if (i>= _contents_size)
                tmp.push_back(item);
            else
                tmp.push_back(this->at(i));
        }
        this->swap(tmp);
    }

    //Оператор присваивания.
    CircularBuffer & operator=(const CircularBuffer & cb){
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

    //Обменивает содержимое буфера с буфером cb.
    void swap(CircularBuffer & cb){
        std::swap(this->_capacity, cb._capacity);
        std::swap(this->_head, cb._head);
        std::swap(this->_tail, cb._tail);
        std::swap(this->_contents_size, cb._contents_size);
        std::swap(this->_buffer, cb._buffer);
    }

    //добавляет элемент в конец буфера.
    void push_back(const value_type & item = value_type()){
        increment_tail();
        if (_contents_size > _capacity) increment_head();
        _buffer[_tail] = item;
    }

    //добавляет элемент в начало
    void push_front(const value_type & item = value_type()){
        decrement_head();
        if (_contents_size > _capacity) decrement_tail();
        _buffer[_head] = item;
    }


    //удаляет последний элемент буфера.
    void pop_back(){
        decrement_tail();
    }

    //удаляет первый элемент буфера.
    void pop_front(){
        increment_head();
    }

    //Вставляет элемент item по индексу pos. Ёмкость буфера остается неизменной.
    void insert(int pos, const value_type & item = value_type()){
        if (pos >= _contents_size){
            push_back(item);
            return;
        }
        CircularBuffer tmp(_capacity);
        for (int i = 0; i < pos; i++) {
            tmp.push_back(this->at());
        }
        for (int i = pos; i < _contents_size; i++) {
            tmp.push_back(this->at());
        }
        this->swap(tmp);
    }

    //Удаляет элементы из буфера в интервале [first, last).
    void erase(int first, int last){
        CircularBuffer tmp (_capacity);
        for (int i = 0; i < first; i++) {
            tmp.push_back(this->at(i));
        }
        for (int i = last; i < _contents_size; i++) {
            tmp.push_back(this->at(i));
        }
        this->swap(tmp);
    }

    //Очищает буфер.
    void clear(){
        _head = 1;
        _tail = 0;
        _contents_size = 0;
    }

    //итератор начало
    Iterator begin() {
        return Iterator(0, this);
    }

//итератор конец
    Iterator end() {
        return Iterator(size(), this);
    }

    bool operator==(const CircularBuffer & b){
        if (this->_capacity == b._capacity && this->_head == b._head && this->_tail == b._tail && this->_contents_size == b._contents_size){
            for (size_t i = 0; i < this->_contents_size; i++){
                if (this->at(i) != b.at(i))
                    return false;
            }
            return true;
        }
        return false;
    }

    bool operator!=(const CircularBuffer & b){
        return !(*this == b);
    }
};





#endif //CIRCULARBUFFER_TI_CIRCULARBUFFER_H
