
/**
this gives us the C++ "placement new" operator, which arduino is missing by default
Usage:

    char buffer[sizeof(MyType)];
    MyType* pmine = new(buffer) MyType();

Ie, allows the new operator to use pre-allocated memory
*/
void* operator new(size_t size, void* ptr)
{
    return ptr;
}