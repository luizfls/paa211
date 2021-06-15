#include <filesystem>
#include <fstream>
#include <iomanip>
#include <random>
#include <string>

namespace fs = std::filesystem;

void generate_input(std::string filename, int n)
{
    std::ofstream ofs(filename);

    ofs << n;

    auto generate_terms = [n, &ofs](unsigned int max)
    {
        std::mt19937 engine(std::random_device{}());
        std::uniform_int_distribution<unsigned int> dist(0, max);

        for(int i = 0; i <= n; ++i)
        {
            if(i % 10 == 0)
                ofs << '\n';
            ofs << ' ' << std::setw(4) << dist(engine);
        }
    };

    generate_terms(500);
    generate_terms(1000);

    ofs << '\n';
}

int main(int argc, char* argv[])
{
    int n = std::stoi(argv[1]);

    std::string prefix = "input_" + std::to_string(n) + "_";
    int diff = 1;
    std::string ext = ".dat";
    while(fs::exists(prefix + std::to_string(diff) + ext))
        ++diff;

    auto filename = prefix + std::to_string(diff) + ext;
    generate_input(filename, n);

    return 0;
}

