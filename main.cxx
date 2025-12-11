////#include <pybind11/pybind11.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <Eigen/Dense>
#include <unsupported/Eigen/KroneckerProduct>
#include <unsupported/Eigen/MatrixFunctions>
#include "ising.hpp"


struct params {
  static int nqubits;
  static double M;
  static double t;
  static double delta;
  static double g;
};
int params::nqubits = 6;
double params::M = 200;
double params::t = 40.0;
double params::delta = params::t / params::M;
double params::g = 1.0;
   
int main(int argc, char** argv) {

  // in file params.txt read params::nqubits, M, t, g provided as arguments
  if (argc < 1) {
    std::cout << "Usage: " << argv[0] << "params.txt" << std::endl;
  }
  std::ifstream param_file(argv[1]);
  param_file >> params::nqubits >> params::M >> params::t >> params::g;
  params::delta = params::t / params::M;
  float dimension = std::pow(2, params::nqubits);


  using namespace ising;
  Eigen::Matrix2cd I;
  I << 1, 0,
       0, 1;

  Eigen::Matrix2cd X;
  X << 0, 1,
       1, 0;

  Eigen::Matrix2cd Z;
  Z << 1, 0,
       0,-1;


  Eigen::MatrixXcd H = Eigen::MatrixXcd::Zero(dimension, dimension);
  for (int i = 0; i < params::nqubits; ++i) {
    H += zz_op(i, params::nqubits, Z, I);
  }
  for (int i = 0; i < params::nqubits; ++i) {
    H += params::g * single_site_op(X, i, params::nqubits, I);
  }
  // eigenvalues and eigenvectors
  Eigen::SelfAdjointEigenSolver<Eigen::MatrixXcd> es(H);
  Eigen::MatrixXcd U_exact = es.eigenvectors() *
                             (es.eigenvalues().array() * (std::complex<double>(0.0, -1.0) * params::delta)).exp().matrix().asDiagonal() *
                             es.eigenvectors().adjoint();

  
  Eigen::MatrixXcd H1 = Eigen::MatrixXcd::Zero(dimension, dimension);
  for (int i = 0; i < params::nqubits; ++i) {
    H1 += zz_op(i, params::nqubits, Z, I);
  }
  Eigen::MatrixXcd H2 = Eigen::MatrixXcd::Zero(dimension, dimension);
  for (int i = 0; i < params::nqubits; ++i) {
    H2 += params::g * single_site_op(X, i, params::nqubits, I);
  }
  Eigen::MatrixXcd U_trotter = trotter1st(H1, H2, params::delta);

  Eigen::VectorXcd state = Eigen::VectorXcd::Zero(dimension);
  state(0) = 1.0;
  auto stateT = state;

  std::vector<double> y_exact{magnetization_z(state, params::nqubits, Z, I)};
  std::vector<double> y_trotter{magnetization_z(state, params::nqubits, Z, I)};

  for (int step=0; step < params::M; ++step) {
    state = U_exact * state;
    double mz = magnetization_z(state, params::nqubits, Z, I);
    y_exact.push_back(mz);

    stateT = U_trotter * stateT;
    y_trotter.push_back(magnetization_z(stateT, params::nqubits, Z, I));
  }


  std::ofstream f("magnetization.txt");
  for (size_t i = 0; i < y_exact.size(); ++i)
    f << y_exact[i] << " " << y_trotter[i] << std::endl;



  return 0;
};
