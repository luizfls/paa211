#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <iterator>
#include <numeric>
#include <set>
#include <string>
#include <unordered_set>
#include <utility>
#include <vector>

struct Pair
{
    unsigned int a, b;

    friend std::ostream& operator<<(std::ostream& os, const Pair& p)
    {
        os << p.a << '/' << p.b;
        return os;
    }
};

std::vector<Pair> load_file(std::string filename)
{
    std::ifstream ifs(filename);

    int n;
    ifs >> n;

    std::vector<unsigned int> ab;

    std::copy(std::istream_iterator<unsigned int>(ifs),
              std::istream_iterator<unsigned int>(),
              std::back_inserter(ab));

    std::vector<Pair> pairs;
    pairs.reserve(n + 1);

    auto it_a = ab.cbegin();
    auto it_b = std::next(it_a, n + 1);
    while(it_b != ab.cend())
    {
        auto a = *it_a++;
        auto b = *it_b++;
        pairs.push_back({a, b});
    }

    return pairs;
}

std::pair<double, std::unordered_set<std::size_t>>
pph(const std::vector<Pair>& vec)
{
    auto v = vec;
    unsigned int rn = v.front().a;
    unsigned int rd = v.front().b;
    double r = rn / double(rd);

    std::unordered_set<std::size_t> s({0});

    auto cmp = [](const Pair& p1, const Pair& p2)
               { return p1.a * p2.b < p2.a * p1.b; };

    std::vector<std::size_t> p(v.size());
    std::iota(p.begin(), p.end(), 0);
    std::sort(std::next(p.begin()), p.end(),
              [&](std::size_t i, std::size_t j)
              { return cmp(v[i], v[j]); });
    std::sort(std::next(v.begin()), v.end(), cmp);

    for(auto it = v.crbegin(); it != std::prev(v.crend()); ++it)
    {
        auto i = std::distance(v.cbegin(), it.base()) - 1;
        if((it->a / double(it->b)) > r)
        {
            rn += it->a;
            rd += it->b;
            r = rn / double(rd);
            s.insert(p[i]);
        }
        else
            break;
    }

    return {rn / double(rd), s};
}

template <typename T>
void print_collection(const T& a, char open, char close)
{
    std::cout << open;
    if(!a.empty())
    {
        std::cout << *a.begin();
        for(auto it = std::next(a.cbegin()); it != a.cend(); ++it)
        {
            std::cout << ", ";
            std::cout << *it;
        }
    }
    std::cout << close;
}

int main(int argc, char* argv[])
{
    auto input = load_file(argv[1]);

    if(argc <= 2)
    {
        std::cout << "input=";
        print_collection(input, '[', ']');
        std::cout << "\n";
    }

    auto [r, s] = pph(input);

    std::cout << "R=" << r << "; S=";
    std::set<unsigned int> ord_s(s.begin(), s.end());
    print_collection(ord_s, '(', ')');
    std::cout << "\n";

    return 0;
}
