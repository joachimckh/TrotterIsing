#include <iostream>
#include <vector>
#include <fstream>

struct Complex {
    double real;
    double imag;
};

float mem_usage(int n) {
    std::vector<Complex> v;
    for (int i = 0; i < std::pow(2,n); ++i) {
        v.push_back({static_cast<double>(i), static_cast<double>(i)});
    }
    return sizeof(v) + v.size() * sizeof(Complex);
}
float convert_bytes(float bytes) {
    return bytes / (1024 * 1024); // Convert to megabytes
}

int main() {
  
  std::vector<double> res;
  for (int n=1; n<30; ++n) {
    auto m = mem_usage(n);
    std::cout << "Memory usage for" << n << " elements: " << convert_bytes(m) << " MB" << std::endl;
    res.push_back(convert_bytes(m));
  }

  std::ofstream f("mem.txt");
  for (auto &r : res) {
    f << r << std::endl;
  }

}
