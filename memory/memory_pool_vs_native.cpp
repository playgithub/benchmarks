#include <iostream>
#include <list>
#include <iomanip>
#include <chrono>


#define BOOST_ALL_DYN_LINK
#include <boost/pool/pool_alloc.hpp>

std::chrono::nanoseconds test_fast_pool_allocator(size_t total_count)
{
    typedef boost::fast_pool_allocator<int,
        boost::default_user_allocator_new_delete,
        boost::details::pool::default_mutex,
        64, 128> allocator;
    
    std::list<int, allocator> l;
    
    auto time_start = std::chrono::steady_clock::now();
    for (int i = 0; i < total_count; ++i)
        l.push_back(i);

    auto du = std::chrono::steady_clock::now() - time_start;

    l.clear();
    boost::singleton_pool<boost::fast_pool_allocator_tag, sizeof(int)>::
        purge_memory();

    return du;
}

std::chrono::nanoseconds test_native(size_t total_count)
{
    std::list<int> l;

    auto time_start = std::chrono::steady_clock::now();
    for (int i = 0; i < total_count; ++i)
        l.push_back(i);

    auto du = std::chrono::steady_clock::now() - time_start;

    l.clear();

    return du;
}

int main()
{
    constexpr size_t total_count = 100000000;
    
    auto du = test_fast_pool_allocator(total_count);
    size_t time_in_ms_for_pool = std::chrono::duration_cast<std::chrono::milliseconds>(du).count();
    std::cout
        << std::left
        << std::setw(30) << "boost::fast_pool_allocator: "
        << std::right << time_in_ms_for_pool
        << '\n';

    du = test_native(total_count);
    size_t time_in_ms_for_native = std::chrono::duration_cast<std::chrono::milliseconds>(du).count();
    std::cout
        << std::left
        << std::setw(30) << "native: "
        << std::right << time_in_ms_for_native
        << '\n';

    std::cout
        << "\nNormalized\n"
        << "------------------\n"
        << std::left << std::setw(10) << "pool: "
        << std::right << std::fixed << std::setprecision(3) << 1.0 << '\n'
        << std::left << std::setw(10) << "native: "
        << std::right << std::fixed << std::setprecision(3) << double(time_in_ms_for_native) / double(time_in_ms_for_pool)
        << std::endl;
}
