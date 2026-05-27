
#include <cstdint>
#include <cstdlib>
#include <memory>
#include <stdexcept>
#include <algorithm>

namespace utk
{
    template<typename T>
    class vector{
        public:
            vector( int size, T value );

            vector( int size );

            /*
                All copy constructors (what about assignments?)
                take in const&.
                Cannot take:
                    by value (no ref) since we need a copy by value to exist
                    in the first place, for construction of `const vector other`. 
                    For assignment, a = b, we have either
                        - copy from `const T&` : this can also use copy-swap.
                        - copy by value `T` (extra copy) but then swap with `*this` 
                        intead of copying again. This is called copy-and-swap.
                        Benefits:  Note that when we accept a T, argument
                        has a property that if expr is lvalue: `other` is constructed
                        by a copy constructor, for rvalue, `other` construced by move ctor.
                        This in olden times, would provide an alternative for move constructor.
                        cannot take const here cuz we cant move from const inside the assignment fn.
                    
            */

            vector( const vector& other );

            vector( vector&& other);

            vector& operator=(const vector& other);

            vector& operator=( vector&& other);

            ~vector();

            void push_back( T value );

            void pop_back();

            T& operator [](uint64_t index);

            const T& operator[](uint64_t index) const;

            [[nodiscard]] inline const T* begin() const
            {
                return data;
            }
            
            [[nodiscard]] inline const T* end() const{
                return data + size;
            }

        private:
            T* data;
            uint64_t size;
            uint64_t capacity;
    };

    //constructor
    template<typename T>
    vector<T>::vector( int size, T value )
    {
        if( size <= 0 )
        {
            throw std::runtime_error("Size cannot be < 0");
        }

        this->size = size;

        uint64_t malloc_size = 1;

        while(malloc_size < size)
        {
            malloc_size <<= 1LL;
        }

        malloc_size = std::max(malloc_size, static_cast<uint64_t>(size));

        data = static_cast<T*>(operator new(sizeof(T)*malloc_size));
        capacity = malloc_size; //assign after allocation for safety
        std::uninitialized_fill(data, data + size, value);
    }

    //ctor 2
    template<typename T>
    vector<T>::vector(int size) : vector(size, T{}) 
    {
        // pass
    }

    //copy ctor
    template<typename T>
    vector<T>::vector( const vector& other )
    {
        size = other.size;
        data = static_cast<T*>(operator new(sizeof(T)*other.capacity));
        std::uninitialized_copy(
            other.data, //from
            other.data + other.size, //from end
            data
        );
        
        capacity = other.capacity;
    }

    //move ctor
    template <typename T>
    vector<T>::vector(vector&& other)
    {
        size = other.size;
        capacity = other.capacity;
        data = other.data;
        other.size = 0;
        other.data = nullptr;
        other.capacity = 0;
    }

    //copy-assignment
    template<typename T>
    vector<T>& vector<T>::operator=(const vector& other)
    {
        if (this == &other)
            return *this;
    
        if ( capacity < other.size )
        {
            T* newData = static_cast<T*>(operator new(sizeof(T)*other.capacity));

            try{
                std::uninitialized_copy(
                    other.data,
                    other.data + other.size,
                    newData
                );
            }
            catch(...)
            {
                operator delete(newData);
                throw;
            }
            // Destroy old elements
            std::destroy(data, data + size);

            // Free old storage
            operator delete(data);

            // Commit new state
            data = newData;
            capacity = other.capacity;
        }
        else{
            // Key point to be noted, if we use assignment on an
            // uninitialized buffer [ T ][ T ][ T ][ raw ][ raw ]
            // there is no element at index 4,5 that we can assign to.

            size_t overlap = std::min(size, other.size);

            // Assign existing elements
            for (size_t i = 0; i < overlap; ++i)
            {
                data[i] = other.data[i]; // can use assignment on data[i] here
            }

            // Construct additional elements if needed
            if (other.size > size)
            {
                std::uninitialized_copy(
                    other.data + size,
                    other.data + other.size,
                    data + size
                );
            }

            // Destroy extra elements if shrinking
            else if (size > other.size)
            {
                std::destroy(
                    data + other.size,
                    data + size
                );
            }
        }
        size = other.size;
        return *this;
    }

    //move-assignment 
    template<typename T>
    vector<T>& vector<T>::operator=(vector<T>&& other)
    {
        if(this == &other)
        {
            return *this;
        }

        std::destroy(data, data + size); //invariant: only first size objects are constructed objects
        operator delete (data);

        this->size = other.size;
        this->data = other.data;
        this->capacity = other.capacity;

        other.size = 0;
        other.data = nullptr;
        other.capacity = 0;

        return *this;
    }

    template<typename T>
    vector<T>::~vector()
    {
        std::destroy(data, data + size); //invariant: only first size objects are constructed objects
        operator delete (data);
        this->capacity = 0;
        this->size = 0;
    }

}