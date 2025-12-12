#include "ising.hpp"
#include <fstream>
#include <iostream>
#include <chrono>

struct params {
  inline static int nqubits = 6;
  inline static double M = 200.0;
  inline static double t = 40.0;
  inline static double g = 1.0;

  static double delta() { return t / M; }
};

int main(int argc, char **argv) {
  if (argc < 2) {
    std::cout << "Usage: " << argv[0] << " " << "params.txt" << std::endl;
  }
  std::ifstream param_file(argv[1]);
  std::string key;
  param_file >> key >> params::nqubits;
  param_file >> key >> params::M;
  param_file >> key >> params::t;
  param_file >> key >> params::g;
  float dimension = std::pow(2, params::nqubits);

  using ising::I, ising::X, ising::Z;

  auto start = std::chrono::high_resolution_clock::now();

  Eigen::MatrixXcd H = Eigen::MatrixXcd::Zero(dimension, dimension);
  for (int i = 0; i < params::nqubits; ++i) {
    H += ising::zz_op(i, params::nqubits, Z, I);
  }
  for (int i = 0; i < params::nqubits; ++i) {
    H += params::g * ising::single_site_op(X, i, params::nqubits, I);
  }
  // eigenvalues and eigenvectors
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXcd> es(H);
  Eigen::MatrixXcd U_exact =
      es.eigenvectors() *
      (es.eigenvalues().array() *
       (std::complex<double>(0.0, -1.0) * params::delta()))
          .exp()
          .matrix()
          .asDiagonal() *
      es.eigenvectors().adjoint();

  Eigen::MatrixXcd H1 = Eigen::MatrixXcd::Zero(dimension, dimension);
  for (int i = 0; i < params::nqubits; ++i) {
    H1 += ising::zz_op(i, params::nqubits, Z, I);
  }
  Eigen::MatrixXcd H2 = Eigen::MatrixXcd::Zero(dimension, dimension);
  for (int i = 0; i < params::nqubits; ++i) {
    H2 += params::g * ising::single_site_op(X, i, params::nqubits, I);
  }
  Eigen::MatrixXcd U_trotter = ising::trotter1st(H1, H2, params::delta());

  Eigen::VectorXcd state = Eigen::VectorXcd::Zero(dimension);
  state(0) = 1.0;
  auto stateT = state;

  std::vector<double> y_exact{
      ising::magnetization_z(state, params::nqubits, Z, I)};
  std::vector<double> y_trotter{
      ising::magnetization_z(state, params::nqubits, Z, I)};
  std::vector<double> v_fidelity{ising::fidelity(state, stateT)};

  for (int step = 0; step < params::M; ++step) {
    state = U_exact * state;
    double mz = ising::magnetization_z(state, params::nqubits, Z, I);
    y_exact.push_back(mz);

    stateT = U_trotter * stateT;
    y_trotter.push_back(ising::magnetization_z(stateT, params::nqubits, Z, I));

    v_fidelity.push_back(ising::fidelity(state, stateT));
  }
  auto end = std::chrono::high_resolution_clock::now();
  auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
  std::cout << "Runtime : " << duration.count() << " ms" << std::endl;

  std::ofstream f("magnetization.txt");
  for (size_t i = 0; i < y_exact.size(); ++i)
    f << y_exact[i] << " " << y_trotter[i] << " " << v_fidelity[i] << std::endl;

  return 0;
};
