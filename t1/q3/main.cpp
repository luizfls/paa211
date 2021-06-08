#include <algorithm>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

template <typename T>
void print_vector(const std::vector<T>& v)
{
    std::cout << "[";
    if(!v.empty())
    {
        std::cout << v.front();
        for(auto it = std::next(v.cbegin()); it != v.cend(); ++it)
        {
            std::cout << ", ";
            std::cout << *it;
        }
    }
    std::cout << "]";
}

bool increment(std::vector<bool>& bs)
{
    bool carry = true;
    for(auto it = bs.rbegin(); it != bs.rend(); ++it)
    {
        *it = !(*it);
        if(*it)
        {
            carry = false;
            break;
        }
    }
    return !carry;
}

bool decrement(std::vector<bool>& bs)
{
    bool carry = true;
    for(auto it = bs.rbegin(); it != bs.rend(); ++it)
    {
        *it = !(*it);
        if(!(*it))
        {
            carry = false;
            break;
        }
    }
    return !carry;
}

void test_flask(const std::vector<bool>& bs, int k)
{
    std::cout << "Entrada: ";
    print_vector(bs);
    std::cout << "\n";

    k = std::min(k, static_cast<int>(bs.size()));

    std::vector<std::vector<bool>> sections(k);
    std::cout << "Partição: ";
    for(std::size_t i = 0; i < k; ++i)
    {
        sections[i].assign(std::next(bs.begin(), i * bs.size() / k),
                           std::next(bs.begin(), (i + 1) * bs.size() / k));
        print_vector(sections[i]);
    }
    std::cout << "\n";

    std::vector<bool> answer;
    answer.reserve(bs.size());
    for(const auto& section : sections)
    {
        std::vector<bool> current(section.size(), false);
        unsigned int falls = 0;
        while(current <= section)
        {
            ++falls;
            auto success = increment(current);
            if(!success) break;
        }
        decrement(current);
        std::cout << "Após " << falls << " quedas, um frasco quebrou\n";
        answer.insert(answer.end(), current.begin(), current.end());
    }

    std::cout << "Resposta: ";
    print_vector(answer);
    std::cout << "\n";

    std::cout << ">> " << (answer == bs ? "CORRETO" : "ERRADO") << "!\n";

    std::cout << "\n";
}

int main(int argc, char* argv[])
{
    std::ifstream infile(argv[1]);
    std::string line;
    std::getline(infile, line);
    std::istringstream iss(line);
    int nbits;
    iss >> nbits;
    while(std::getline(infile, line))
    {
        std::vector<bool> bs;
        bs.reserve(nbits);
        for(auto c : line)
            bs.emplace_back(c == '1');
        test_flask(bs, std::stoi(argv[2]));
    }

    return 0;
}
