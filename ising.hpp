#pragma once

#include <vector>

#include <Eigen/Dense>
#include <unsupported/Eigen/KroneckerProduct>
#include <unsupported/Eigen/MatrixFunctions>


namespace ising {

Eigen::MatrixXcd kron_n(const std::vector<Eigen::MatrixXcd> &ops) {
  Eigen::MatrixXcd out = ops[0];
  for (size_t i = 1; i < ops.size(); ++i)
    out = Eigen::kroneckerProduct(out, ops[i]).eval();
  return out;
}

Eigen::MatrixXcd single_site_op(const Eigen::MatrixXcd &pauli,
                                int site,
                                int nqubits,
                                const Eigen::MatrixXcd &I1) {
  std::vector<Eigen::MatrixXcd> ops;
  ops.reserve(nqubits);
  for (int i = 0; i < nqubits; ++i) {
    if (i == site)
      ops.push_back(pauli);
    else
      ops.push_back(I1);
  }
  return kron_n(ops);
}

Eigen::MatrixXcd zz_op(const int site,
                       const int nqubits,
                       const Eigen::MatrixXcd &Z,
                       const Eigen::MatrixXcd &I1) {
  std::vector<Eigen::MatrixXcd> ops;
  ops.reserve(nqubits);
  for (int i = 0; i < nqubits; ++i) {
    if (i == site || i == (site + 1) % nqubits)
      ops.push_back(Z);
    else
      ops.push_back(I1);
  }
  return kron_n(ops);
}


Eigen::MatrixXcd trotter1st(const Eigen::MatrixXcd &H1,
                             const Eigen::MatrixXcd &H2,
                             const double delta) {
  auto U1 = (std::complex<double>(0.0, -1.0) * H1 * delta).exp();
  auto U2 = (std::complex<double>(0.0, -1.0) * H2 * delta).exp();
  return U2 * U1;
}

double magnetization_z(const Eigen::MatrixXcd &state,
                         const int nqubits,
                         const Eigen::MatrixXcd &Z,
                         const Eigen::MatrixXcd &I1) {
  double mz = 0.0;
  for (int i = 0; i < nqubits; ++i) {
    Eigen::MatrixXcd Z_i = single_site_op(Z, i, nqubits, I1);
    mz += (state.adjoint() * Z_i * state).value().real();
  }
  return mz;
}

double fidelity(const Eigen::MatrixXcd &state1,
                const Eigen::MatrixXcd &state2) {
  return std::norm((state1.adjoint() * state2).value());
};




}  // namespace ising


