#include <iostream>
#include <sstream>
#include <functional>
#include <iomanip>
#include <chrono>

#ifdef _DEBUG
const char * build_info = "DEBUG";
#else
const char * build_info = "RELEASE";
#endif // _DEBUG

constexpr size_t loop_count = 100000000;

long long int  i;

struct B
{
    virtual void do_someting() = 0;
};

struct T : public B
{
    void do_something_by_non_virtual_function()
    {
        i *= 2;
        i /= 2;
        i += 2;

    }

    virtual void do_someting()
    {
        i *= 2;
        i /= 2;
        i += 2;
    }
};

std::chrono::nanoseconds test_non_virtual_function()
{
    auto time_start = std::chrono::steady_clock::now();
    T t;
    for (size_t i = 0; i < loop_count; ++i)
        t.do_something_by_non_virtual_function();
    return std::chrono::steady_clock::now() - time_start;
}

std::chrono::nanoseconds test_virtual_funciton_call_by_self()
{
    auto time_start = std::chrono::steady_clock::now();
    T t;
    for (size_t i = 0; i < loop_count; ++i)
        t.do_someting();
    return std::chrono::steady_clock::now() - time_start;
}

std::chrono::nanoseconds test_virtual_funciton_call_by_base_class()
{
    auto time_start = std::chrono::steady_clock::now();
    B * t = new T();
    for (size_t i = 0; i < loop_count; ++i)
    {
        t->do_someting();
    }
    return std::chrono::steady_clock::now() - time_start;
}

std::chrono::nanoseconds test_bind_outside_loop()
{
    auto time_start = std::chrono::steady_clock::now();
    T t;
    std::function<void()> func = std::bind(&T::do_someting, &t);
    for (size_t i = 0; i < loop_count; ++i)
    {
        func();
    }
    return std::chrono::steady_clock::now() - time_start;
}

std::chrono::nanoseconds test_bind_inside_loop()
{
    auto time_start = std::chrono::steady_clock::now();
    T t;
    for (size_t i = 0; i < loop_count; ++i)
    {
        std::function<void()> func = std::bind(&T::do_someting, &t);
        func();
    }
    return std::chrono::steady_clock::now() - time_start;
}

int main()
{
    std::cout << "test_non_virtual_function" << std::endl;
    size_t dur_non_virtual_function = std::chrono::duration_cast<std::chrono::milliseconds>(test_non_virtual_function()).count();

    std::cout << "test_virtual_funciton_call_by_self" << std::endl;
    size_t dur_virtual_funciton_call_by_self = std::chrono::duration_cast<std::chrono::milliseconds>(test_virtual_funciton_call_by_self()).count();

    std::cout << "test_virtual_funciton_call_by_base_class" << std::endl;
    size_t dur_virtual_funciton_call_by_base_class = std::chrono::duration_cast<std::chrono::milliseconds>(test_virtual_funciton_call_by_base_class()).count();

    std::cout << "test_bind_outside_loop" << std::endl;
    size_t dur_bind_outside_loop = std::chrono::duration_cast<std::chrono::milliseconds>(test_bind_outside_loop()).count();

    std::cout << "test_bind_inside_loop" << std::endl;
    size_t dur_bind_inside_loop = std::chrono::duration_cast<std::chrono::milliseconds>(test_bind_inside_loop()).count();

    constexpr size_t title_width = 40;
    constexpr size_t value_width = 8;

    std::ostringstream oss_output;
    oss_output << "\nTest report (build info: " << build_info << ")\n-----------------------\n\n";

    oss_output << "original result\n\n";

    oss_output << std::left << std::setw(title_width) << "non_virtual_function:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_non_virtual_function << '\n';
    oss_output << std::left << std::setw(title_width) << "virtual_funciton_call_by_self:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_virtual_funciton_call_by_self << '\n';
    oss_output << std::left << std::setw(title_width) << "virtual_funciton_call_by_base_class:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_virtual_funciton_call_by_base_class << '\n';
    oss_output << std::left << std::setw(title_width) << "bind_outside_loop:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_bind_outside_loop << '\n';
    oss_output << std::left << std::setw(title_width) << "bind_inside_loop:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_bind_inside_loop << '\n';

    double base = dur_non_virtual_function;
    double dur_non_virtual_function_normalized = 1.0;
    double dur_virtual_funciton_call_by_self_normalized = double(dur_virtual_funciton_call_by_self) / base;
    double dur_virtual_funciton_call_by_base_class_normalized = double(dur_virtual_funciton_call_by_base_class) / base;
    double dur_bind_outside_loop_normalized = double(dur_bind_outside_loop) / base;
    double dur_bind_inside_loop_normalized = double(dur_bind_inside_loop) / base;

    oss_output << "\nnormalized result\n\n";

    oss_output << std::left << std::setw(title_width) << "non_virtual_function:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_non_virtual_function_normalized << '\n';
    oss_output << std::left << std::setw(title_width) << "virtual_funciton_call_by_self:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_virtual_funciton_call_by_self_normalized << '\n';
    oss_output << std::left << std::setw(title_width) << "virtual_funciton_call_by_base_class:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_virtual_funciton_call_by_base_class_normalized << '\n';
    oss_output << std::left << std::setw(title_width) << "bind_outside_loop:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_bind_outside_loop_normalized << '\n';
    oss_output << std::left << std::setw(title_width) << "bind_inside_loop:" << std::right << std::fixed << std::setprecision(3) << std::setw(value_width) << dur_bind_inside_loop_normalized << '\n';

    std::cout << oss_output.str() << std::endl;

    system("pasue");

    return 0;
}
