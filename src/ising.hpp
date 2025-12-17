#pragma once

#include <vector>

#include <Eigen/Dense>
#include <unsupported/Eigen/KroneckerProduct>
#include <unsupported/Eigen/MatrixFunctions>

namespace ising {
using mat2 = Eigen::Matrix2cd;
using matcd = Eigen::MatrixXcd;

const auto I = (mat2() << 1., 0., 0., 1.).finished();
const auto X = (mat2() << 0., 1., 1., 0.).finished();
const auto Z = (mat2() << 1., 0., 0., -1.).finished();

matcd kron_n(const std::vector<matcd> &ops) {
  matcd out = ops[0];
  for (size_t i = 1; i < ops.size(); ++i)
    out = Eigen::kroneckerProduct(out, ops[i]).eval();
  return out;
}

matcd single_site_op(const matcd &pauli, int site, int nqubits,
                     const matcd &I1) {
  std::vector<matcd> ops;
  ops.reserve(nqubits);
  for (int i = 0; i < nqubits; ++i) {
    if (i == site)
      ops.push_back(pauli);
    else
      ops.push_back(I1);
  }
  return kron_n(ops);
}

matcd zz_op(const int site, const int nqubits, const matcd &Z,
            const matcd &I1) {
  std::vector<matcd> ops;
  ops.reserve(nqubits);
  for (int i = 0; i < nqubits; ++i) {
    if (i == site || i == (site + 1) % nqubits)
      ops.push_back(Z);
    else
      ops.push_back(I1);
  }
  return kron_n(ops);
}

matcd trotter1st(const matcd &H1, const matcd &H2, const double delta) {
  auto U1 = (std::complex<double>(0.0, -1.0) * H1 * delta).exp();
  auto U2 = (std::complex<double>(0.0, -1.0) * H2 * delta).exp();
  return U1 * U2;
}

double magnetization_z(const matcd &state, const int nqubits, const matcd &Z,
                       const matcd &I1) {
  double mz = 0.0;
  for (int i = 0; i < nqubits; ++i) {
    matcd Z_i = single_site_op(Z, i, nqubits, I1);
    mz += (state.adjoint() * Z_i * state).value().real();
  }
  return mz;
}

double fidelity(const matcd &state1, const matcd &state2) {
  return std::norm((state1.adjoint() * state2).value());
};

} // namespace ising
