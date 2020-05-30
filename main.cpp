#include <iostream>
#include <functional>

struct Decorator
{
    template<typename Function>
    static auto make(Function&& func)
    {
#if __cplusplus == 202002 //c++20
        return [&] <typename ...Args> (Args && ...args) {
            std::cout<<"Do something " << std::endl;
            return std::invoke(std::forward<Function>(func), args...);
        };
#elif __cplusplus == 201703L //c++17
        return [&] (auto && ...args) {
            std::cout<<"Do something " << std::endl;
            return std::invoke(std::forward<Function>(func), args...);
        };
#elif __cplusplus == 201402L //c++14
        return [&] (auto && ...args) {
            std::cout<<"Do something " << std::endl;
            return std::__invoke(std::forward<Function>(func), args...);
        };
#else
#error "define specific c++ standard manually"
#endif
    }
};

int myFunc(int a, int b)
{
    std::cout<<a << " " << b << std::endl;
    return a + b;
}

class MyClass
{
public:
    MyClass(int data) : m_data(data) {}
    void func()
    {
        std::cout<<"Class hello" << " " << m_data << std::endl;
    }
private:
    int m_data;
};


int main() {
    std::cout<<__cplusplus<<std::endl;
    auto decorated_1 = Decorator::make(myFunc);
    decorated_1(1,2);

    auto my_lambda = [](int a, int b){ std::cout<<a<<" " <<b<<std::endl;};
    auto decorated_2 = Decorator::make(my_lambda);
    decorated_2(3,4);

    int (*ptr)(int, int) = myFunc;
    auto decorated_3 = Decorator::make(ptr);
    decorated_3(5,6);

    std::function<void(int, int)> fun = myFunc;
    auto decorated_4 = Decorator::make(fun);
    decorated_4(7,8);

    auto decorated_5 = Decorator::make(decorated_4);
    decorated_5(9, 10);

    auto decorated_6 = Decorator::make(&MyClass::func);
    decorated_6(MyClass(10));

    return 0;
}
