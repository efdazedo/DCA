// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Peter Staar (taa@zurich.ibm.com)
//
// This class symmetrizes single-particle Greens functions according to cluster symmetries,
// matsubara frequencies and band-index symmetries.

/*
 *  \section tau imaginary-time domain
 *
 *   \f{eqnarray*}{
 *     G(\tau) &=& -G(\tau+\beta)
 *   \f}
 *
 *  \section omega matsubara-frequency domain
 *
 *   \f{eqnarray*}{
 *     G(\varpi) &=& \overline{G(-\varpi)}
 *   \f}
 *
 *  \section r_and_k cluster domain
 *
 *   For each symmetry operation \f$\mathcal{S}\f$ of the cluster-domain, we have
 *
 *   \f{eqnarray*}{
 *     G(\vec{r}) &=& G(\mathcal{S}(\vec{r})) \\
 *     G(\vec{k}) &=& G(\mathcal{S}(\vec{k})) \\
 *   \f}
 */

#ifndef DCA_PHYS_DCA_STEP_SYMMETRIZATION_SYMMETRIZE_SINGLE_PARTICLE_FUNCTION_HPP
#define DCA_PHYS_DCA_STEP_SYMMETRIZATION_SYMMETRIZE_SINGLE_PARTICLE_FUNCTION_HPP

#include <cmath>
#include <complex>
#include <iostream>
#include <stdexcept>
#include <string>
#include <utility>

#include "dca/function/domains.hpp"
#include "dca/function/function.hpp"
#include "dca/phys/domains/cluster/cluster_definitions.hpp"
#include "dca/phys/domains/cluster/cluster_domain.hpp"
#include "dca/phys/domains/cluster/cluster_symmetry.hpp"
#include "dca/phys/domains/quantum/electron_band_domain.hpp"
#include "dca/phys/domains/quantum/electron_spin_domain.hpp"
#include "dca/phys/domains/time_and_frequency/frequency_domain.hpp"
#include "dca/phys/domains/time_and_frequency/frequency_domain_real_axis.hpp"
#include "dca/phys/domains/time_and_frequency/time_domain.hpp"
#include "dca/phys/domains/time_and_frequency/vertex_frequency_domain.hpp"

namespace dca {
namespace phys {
// dca::phys::

class symmetrize_single_particle_function {
public:
  using t = func::dmn_0<domains::time_domain>;
  using w = func::dmn_0<domains::frequency_domain>;
  using w_VERTEX = func::dmn_0<domains::vertex_frequency_domain<domains::COMPACT>>;
  using w_VERTEX_EXTENDED = func::dmn_0<domains::vertex_frequency_domain<domains::EXTENDED>>;
  using w_REAL = func::dmn_0<domains::frequency_domain_real_axis>;

  using b = func::dmn_0<domains::electron_band_domain>;
  using s = func::dmn_0<domains::electron_spin_domain>;
  using nu = func::dmn_variadic<b, s>;  // orbital-spin index

protected:
  template <typename scalartype, typename nu_dmn_t, typename f_dmn_0, typename f_dmn_1>
  static void execute(
      func::function<scalartype, func::dmn_variadic<nu_dmn_t, nu_dmn_t, f_dmn_0, f_dmn_1>>& f,
      func::function<int, func::dmn_variadic<nu_dmn_t, nu_dmn_t>>& H_symmetry, bool do_diff = false);

  template <typename scalartype, typename scalar_type, int D, domains::CLUSTER_NAMES N,
            domains::CLUSTER_SHAPE S>
  static void execute(
      func::function<scalartype,
                     func::dmn_0<domains::cluster_domain<scalar_type, D, N, domains::REAL_SPACE, S>>>& f,
      bool do_diff = false);

  template <typename scalartype, typename scalar_type, int D, domains::CLUSTER_NAMES N,
            domains::CLUSTER_SHAPE S>
  static void execute(
      func::function<
          scalartype,
          func::dmn_0<domains::cluster_domain<scalar_type, D, N, domains::MOMENTUM_SPACE, S>>>& f,
      bool do_diff = false);

  template <typename scalartype, typename f_dmn_0, typename f_dmn_1>
  static void execute(func::function<scalartype, func::dmn_variadic<b, b, f_dmn_0, f_dmn_1>>& f,
                      bool do_diff = false);

  template <typename scalartype, typename f_dmn_0>
  static void execute(func::function<scalartype, func::dmn_variadic<nu, nu, f_dmn_0>>& f,
                      bool do_diff = false);

  template <typename scalartype, typename f_dmn_0, typename f_dmn_1>
  static void execute(func::function<scalartype, func::dmn_variadic<nu, nu, f_dmn_0, f_dmn_1>>& f,
                      bool do_diff = false);

private:
  template <typename scalartype>
  static void difference(scalartype val, std::string function_name, std::string dmn_name);

  template <typename scalartype>
  static void difference(scalartype val0, scalartype val1, std::string function_name,
                         std::string dmn_name);

  template <typename scalartype, typename f_dmn_0, typename f_dmn_1>
  static void symmetrize_over_electron_spin(
      func::function<scalartype, func::dmn_variadic<nu, nu, f_dmn_0, f_dmn_1>>& f, bool do_diff);

  template <typename scalartype>
  static void execute(func::function<scalartype, t>& f, bool do_diff = false);

  template <typename scalartype>
  static void execute(func::function<scalartype, func::dmn_variadic<b, b, t>>& f,
                      bool do_diff = false);

  template <typename scalartype>
  static void execute(func::function<scalartype, w>& f, bool do_diff = false);

  template <typename scalartype>
  static void execute(func::function<scalartype, func::dmn_variadic<b, b, w>>& f,
                      bool do_diff = false);

  template <typename scalartype>
  static void execute(func::function<scalartype, w_REAL>& f, bool do_diff = false);

  template <typename scalartype>
  static void execute(func::function<scalartype, func::dmn_variadic<b, b, w_REAL>>& f,
                      bool do_diff = false);

  template <typename scalartype>
  static void execute(func::function<scalartype, w_VERTEX>& f, bool do_diff = false);

  template <typename scalartype>
  static void execute(func::function<scalartype, w_VERTEX_EXTENDED>& f, bool do_diff = false);

  template <typename scalartype, typename scalar_type, int D, domains::CLUSTER_NAMES N,
            domains::CLUSTER_SHAPE S>
  static void execute(
      func::function<scalartype,
                     func::dmn_variadic<b, b, func::dmn_0<domains::cluster_domain<
                                                  scalar_type, D, N, domains::REAL_SPACE, S>>>>& f,
      bool do_diff = false);

  template <typename scalartype, typename scalar_type, int D, domains::CLUSTER_NAMES N,
            domains::CLUSTER_SHAPE S>
  static void execute(
      func::function<scalartype,
                     func::dmn_variadic<b, b, func::dmn_0<domains::cluster_domain<
                                                  scalar_type, D, N, domains::MOMENTUM_SPACE, S>>>>& f,
      bool do_diff = false);
};

template <typename scalartype, typename nu_dmn_t, typename f_dmn_0, typename f_dmn_1>
void symmetrize_single_particle_function::execute(
    func::function<scalartype, func::dmn_variadic<nu_dmn_t, nu_dmn_t, f_dmn_0, f_dmn_1>>& f,
    func::function<int, func::dmn_variadic<nu_dmn_t, nu_dmn_t>>& /*H_symmetry*/, bool do_diff) {
  execute(f, do_diff);
}

template <typename scalartype, typename f_dmn_0, typename f_dmn_1>
void symmetrize_single_particle_function::execute(
    func::function<scalartype, func::dmn_variadic<b, b, f_dmn_0, f_dmn_1>>& f, bool do_diff) {
  {
    func::function<scalartype, f_dmn_0> f0(f.get_name());

    for (int nu_0 = 0; nu_0 < b::dmn_size(); ++nu_0) {
      for (int nu_1 = 0; nu_1 < b::dmn_size(); ++nu_1) {
        for (int ind_1 = 0; ind_1 < f_dmn_1::dmn_size(); ++ind_1) {
          for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ++ind_0)
            f0(ind_0) = f(nu_0, nu_1, ind_0, ind_1);

          symmetrize_single_particle_function::execute(f0, do_diff);

          for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ++ind_0)
            f(nu_0, nu_1, ind_0, ind_1) = f0(ind_0);
        }
      }
    }
  }

  {
    func::function<scalartype, f_dmn_1> f1(f.get_name());

    for (int nu_0 = 0; nu_0 < b::dmn_size(); ++nu_0) {
      for (int nu_1 = 0; nu_1 < b::dmn_size(); ++nu_1) {
        for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ++ind_0) {
          for (int ind_1 = 0; ind_1 < f_dmn_1::dmn_size(); ++ind_1)
            f1(ind_1) = f(nu_0, nu_1, ind_0, ind_1);

          symmetrize_single_particle_function::execute(f1, do_diff);

          for (int ind_1 = 0; ind_1 < f_dmn_1::dmn_size(); ++ind_1)
            f(nu_0, nu_1, ind_0, ind_1) = f1(ind_1);
        }
      }
    }
  }
}

template <typename scalartype, typename f_dmn_0>
void symmetrize_single_particle_function::execute(
    func::function<scalartype, func::dmn_variadic<nu, nu, f_dmn_0>>& f, bool do_diff) {
  func::function<scalartype, func::dmn_variadic<b, b, f_dmn_0>> f0(f.get_name());

  for (int spin_ind = 0; spin_ind < s::dmn_size(); ++spin_ind) {
    for (int b_0 = 0; b_0 < b::dmn_size(); ++b_0)
      for (int b_1 = 0; b_1 < b::dmn_size(); ++b_1)
        for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ++ind_0)
          f0(b_0, b_1, ind_0) = f(b_0, spin_ind, b_1, spin_ind, ind_0);

    symmetrize_single_particle_function::execute(f0, do_diff);

    for (int b_0 = 0; b_0 < b::dmn_size(); ++b_0)
      for (int b_1 = 0; b_1 < b::dmn_size(); ++b_1)
        for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ++ind_0)
          f(b_0, spin_ind, b_1, spin_ind, ind_0) = f0(b_0, b_1, ind_0);
  }
}

template <typename scalartype, typename f_dmn_0, typename f_dmn_1>
void symmetrize_single_particle_function::execute(
    func::function<scalartype, func::dmn_variadic<nu, nu, f_dmn_0, f_dmn_1>>& f, bool do_diff) {
  symmetrize_over_electron_spin(f, do_diff);

  {
    func::function<scalartype, func::dmn_variadic<b, b, f_dmn_0>> f0(f.get_name());

    for (int ind_1 = 0; ind_1 < f_dmn_1::dmn_size(); ++ind_1) {
      for (int spin_ind = 0; spin_ind < s::dmn_size(); ++spin_ind) {
        for (int b_0 = 0; b_0 < b::dmn_size(); ++b_0)
          for (int b_1 = 0; b_1 < b::dmn_size(); ++b_1)
            for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ++ind_0)
              f0(b_0, b_1, ind_0) = f(b_0, spin_ind, b_1, spin_ind, ind_0, ind_1);

        symmetrize_single_particle_function::execute(f0, do_diff);

        for (int b_0 = 0; b_0 < b::dmn_size(); ++b_0)
          for (int b_1 = 0; b_1 < b::dmn_size(); ++b_1)
            for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ++ind_0)
              f(b_0, spin_ind, b_1, spin_ind, ind_0, ind_1) = f0(b_0, b_1, ind_0);
      }
    }
  }

  {
    func::function<scalartype, func::dmn_variadic<b, b, f_dmn_1>> f1(f.get_name());

    for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ++ind_0) {
      for (int spin_ind = 0; spin_ind < s::dmn_size(); ++spin_ind) {
        for (int ind_1 = 0; ind_1 < f_dmn_1::dmn_size(); ++ind_1)
          for (int b_1 = 0; b_1 < b::dmn_size(); ++b_1)
            for (int b_0 = 0; b_0 < b::dmn_size(); ++b_0)
              f1(b_0, b_1, ind_1) = f(b_0, spin_ind, b_1, spin_ind, ind_0, ind_1);

        symmetrize_single_particle_function::execute(f1, do_diff);

        for (int ind_1 = 0; ind_1 < f_dmn_1::dmn_size(); ++ind_1)
          for (int b_1 = 0; b_1 < b::dmn_size(); ++b_1)
            for (int b_0 = 0; b_0 < b::dmn_size(); ++b_0)
              f(b_0, spin_ind, b_1, spin_ind, ind_0, ind_1) = f1(b_0, b_1, ind_1);
      }
    }
  }
}

template <typename scalartype, typename f_dmn_0, typename f_dmn_1>
void symmetrize_single_particle_function::symmetrize_over_electron_spin(
    func::function<scalartype, func::dmn_variadic<nu, nu, f_dmn_0, f_dmn_1>>& f, bool /*do_diff*/) {
  for (int ind_1 = 0; ind_1 < f_dmn_1::dmn_size(); ind_1++) {
    for (int ind_0 = 0; ind_0 < f_dmn_0::dmn_size(); ind_0++) {
      // spin-symmetry ... --> G_(e_UP, e_DN) == G_(e_DN, e_UP) == 0 !!
      for (int i = 0; i < b::dmn_size(); i++) {
        for (int j = 0; j < b::dmn_size(); j++) {
          f(i, 0, j, 1, ind_0, ind_1) = 0;
          f(i, 1, j, 0, ind_0, ind_1) = 0;

          scalartype tmp = (f(i, 0, j, 0, ind_0, ind_1) + f(i, 1, j, 1, ind_0, ind_1)) / 2.;

          f(i, 0, j, 0, ind_0, ind_1) = tmp;
          f(i, 1, j, 1, ind_0, ind_1) = tmp;
        }
      }
    }
  }
}

template <typename scalartype>
void symmetrize_single_particle_function::difference(scalartype val, std::string function_name,
                                                     std::string dmn_name) {
  if (std::abs(val) > 1.e-6) {
    std::cout << "difference detected in : " << dmn_name << "\t" << function_name << "\t"
              << std::abs(val) << "\n\n";
    // throw std::logic_error(__PRETTY_FUNCTION__);
  }
}

template <typename scalartype>
void symmetrize_single_particle_function::difference(scalartype val0, scalartype val1,
                                                     std::string function_name, std::string dmn_name) {
  if (abs(val0 - val1) > 1.e-6) {
    std::cout << "difference detected in : " << dmn_name << "\t" << function_name << "\t"
              << abs(val0 - val1) << "\n\n";
    // throw std::logic_error(__PRETTY_FUNCTION__);
  }
}

template <>
void symmetrize_single_particle_function::difference(float val, std::string function_name,
                                                     std::string dmn_name);

template <>
void symmetrize_single_particle_function::difference(float val0, float val1,
                                                     std::string function_name, std::string dmn_name);

template <>
void symmetrize_single_particle_function::difference(std::complex<float> val,
                                                     std::string function_name, std::string dmn_name);

template <>
void symmetrize_single_particle_function::difference(std::complex<float> val0,
                                                     std::complex<float> val1,
                                                     std::string function_name, std::string dmn_name);

template <typename scalartype>
void symmetrize_single_particle_function::execute(func::function<scalartype, t>& f, bool do_diff) {
  int shift = t::dmn_size() / 2;

  double max = 0;
  for (int i = 0; i < t::dmn_size() / 2; i++) {
    max = std::max(max, abs((f(i) + f(i + shift)) / 2.));

    scalartype tmp = (f(i) - f(i + shift)) / 2.;

    f(i) = tmp;
    f(i + shift) = -tmp;
  }

  if (do_diff)
    difference(max, f.get_name(), "tau-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype>
void symmetrize_single_particle_function::execute(
    func::function<scalartype, func::dmn_variadic<b, b, t>>& f, bool do_diff) {
  func::function<scalartype, func::dmn_variadic<b, b, t>> f_new;

  int t_0 = t::dmn_size() / 2;

  for (int t_ind = 0; t_ind < t::dmn_size() / 2; t_ind++) {
    for (int b0 = 0; b0 < b::dmn_size(); ++b0) {
      for (int b1 = 0; b1 < b::dmn_size(); ++b1) {
        scalartype tmp = (f(b0, b1, t_ind) - f(b1, b0, t_ind + t_0)) / 2.;

        f_new(b0, b1, t_ind) = tmp;
        f_new(b1, b0, t_ind + t_0) = -tmp;
      }
    }
  }

  double max = 0;
  for (int ind = 0; ind < f.size(); ++ind) {
    max = std::max(max, std::abs(f(ind) - f_new(ind)));

    f(ind) = f_new(ind);
  }

  if (do_diff)
    difference(max, f.get_name(), "t-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype>
void symmetrize_single_particle_function::execute(func::function<scalartype, w>& f, bool do_diff) {
  double max = 0;
  for (int i = 0; i < w::dmn_size() / 2; i++) {
    max = std::max(max, abs((f(i) - std::conj(f(w::dmn_size() - i - 1))) / 2.));

    scalartype tmp = (f(i) + std::conj(f(w::dmn_size() - i - 1))) / 2.;

    f(i) = tmp;
    f(w::dmn_size() - 1 - i) = std::conj(tmp);
  }

  if (do_diff)
    difference(max, f.get_name(), "w-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype>
void symmetrize_single_particle_function::execute(
    func::function<scalartype, func::dmn_variadic<b, b, w>>& f, bool do_diff) {
  func::function<scalartype, func::dmn_variadic<b, b, w>> f_new;

  int w_0 = w::dmn_size() - 1;

  for (int w_ind = 0; w_ind < w::dmn_size() / 2; ++w_ind) {
    for (int b0 = 0; b0 < b::dmn_size(); ++b0) {
      for (int b1 = 0; b1 < b::dmn_size(); ++b1) {
        scalartype tmp_0 = f(b0, b1, w_ind);
        scalartype tmp_1 = f(b1, b0, w_0 - w_ind);

        scalartype tmp = (tmp_0 + std::conj(tmp_1)) / 2.;

        f_new(b0, b1, w_ind) = tmp;
        f_new(b1, b0, w_0 - w_ind) = std::conj(tmp);
      }
    }
  }

  double max = 0;
  for (int ind = 0; ind < f.size(); ++ind) {
    max = std::max(max, abs(f(ind) - f_new(ind)));

    f(ind) = f_new(ind);
  }

  if (do_diff)
    difference(max, f.get_name(), "w-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype>
void symmetrize_single_particle_function::execute(func::function<scalartype, w_REAL>& /*f*/,
                                                  bool /*do_diff*/) {}

template <typename scalartype>
void symmetrize_single_particle_function::execute(
    func::function<scalartype, func::dmn_variadic<b, b, w_REAL>>& /*f*/, bool /*do_diff*/) {}

template <typename scalartype>
void symmetrize_single_particle_function::execute(func::function<scalartype, w_VERTEX>& f,
                                                  bool do_diff) {
  double max = 0;
  for (int i = 0; i < w_VERTEX::dmn_size() / 2; i++) {
    max = std::max(max, abs((f(i) - std::conj(f(w_VERTEX::dmn_size() - i - 1))) / 2.));

    scalartype tmp = (f(i) + std::conj(f(w_VERTEX::dmn_size() - i - 1))) / 2.;

    f(i) = tmp;
    f(w_VERTEX::dmn_size() - i - 1) = std::conj(tmp);
  }

  if (do_diff)
    difference(max, "w_VERTEX-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype>
void symmetrize_single_particle_function::execute(func::function<scalartype, w_VERTEX_EXTENDED>& f,
                                                  bool do_diff) {
  double max = 0;
  for (int i = 0; i < w_VERTEX_EXTENDED::dmn_size() / 2; i++) {
    max = std::max(max, abs((f(i) - std::conj(f(w_VERTEX_EXTENDED::dmn_size() - i - 1))) / 2.));

    scalartype tmp = (f(i) + std::conj(f(w_VERTEX_EXTENDED::dmn_size() - i - 1))) / 2.;

    f(i) = tmp;
    f(w_VERTEX_EXTENDED::dmn_size() - i - 1) = std::conj(tmp);
  }

  if (do_diff)
    difference(max, "w_VERTEX_EXTENDED-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype, typename scalar_type, int D, domains::CLUSTER_NAMES N,
          domains::CLUSTER_SHAPE S>
void symmetrize_single_particle_function::execute(
    func::function<scalartype,
                   func::dmn_0<domains::cluster_domain<scalar_type, D, N, domains::REAL_SPACE, S>>>& f,
    bool do_diff) {
  typedef domains::cluster_domain<scalar_type, D, N, domains::REAL_SPACE, S> r_cluster_type;
  typedef func::dmn_0<r_cluster_type> r_dmn_t;

  typedef
      typename domains::cluster_symmetry<r_cluster_type>::sym_super_cell_dmn_t sym_super_cell_dmn_t;

  static func::function<std::pair<int, int>,
                        func::dmn_variadic<func::dmn_variadic<r_dmn_t, b>, sym_super_cell_dmn_t>>&
      r_symmetry_matrix = domains::cluster_symmetry<r_cluster_type>::get_symmetry_matrix();

  static func::function<scalartype, r_dmn_t> f_new;

  f_new = scalartype(0.);

  for (int S_ind = 0; S_ind < sym_super_cell_dmn_t::dmn_size(); ++S_ind) {
    for (int r_ind = 0; r_ind < r_dmn_t::dmn_size(); ++r_ind) {
      int R_new_ind = r_symmetry_matrix(r_ind, 0, S_ind).first;

      f_new(r_ind) += f(R_new_ind);
    }
  }

  if (sym_super_cell_dmn_t::dmn_size() > 0)
    f_new /= double(sym_super_cell_dmn_t::dmn_size());
  else
    throw std::logic_error(__FUNCTION__);

  double max = 0;
  for (int ind = 0; ind < f.size(); ++ind) {
    max = std::max(max, std::abs(f(ind) - f_new(ind)));

    f(ind) = f_new(ind);
  }

  if (do_diff)
    difference(max, f.get_name(), "r-cluster-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype, typename scalar_type, int D, domains::CLUSTER_NAMES N,
          domains::CLUSTER_SHAPE S>
void symmetrize_single_particle_function::execute(
    func::function<scalartype,
                   func::dmn_variadic<b, b, func::dmn_0<domains::cluster_domain<
                                                scalar_type, D, N, domains::REAL_SPACE, S>>>>& f,
    bool do_diff) {
  typedef domains::cluster_domain<scalar_type, D, N, domains::REAL_SPACE, S> r_cluster_type;
  typedef func::dmn_0<r_cluster_type> r_dmn_t;

  typedef
      typename domains::cluster_symmetry<r_cluster_type>::sym_super_cell_dmn_t sym_super_cell_dmn_t;

  static func::function<std::pair<int, int>,
                        func::dmn_variadic<func::dmn_variadic<r_dmn_t, b>, sym_super_cell_dmn_t>>&
      r_symmetry_matrix = domains::cluster_symmetry<r_cluster_type>::get_symmetry_matrix();

  static func::function<scalartype, func::dmn_variadic<b, b, r_dmn_t>> f_new;

  f_new = scalartype(0.);

  for (int S_ind = 0; S_ind < sym_super_cell_dmn_t::dmn_size(); ++S_ind) {
    for (int b0 = 0; b0 < b::dmn_size(); ++b0) {
      for (int b1 = 0; b1 < b::dmn_size(); ++b1) {
        for (int r_ind = 0; r_ind < r_dmn_t::dmn_size(); ++r_ind) {
          int R_new_ind = r_symmetry_matrix(r_ind, 0, S_ind).first;

          int b0_new = r_symmetry_matrix(0, b0, S_ind).second;
          int b1_new = r_symmetry_matrix(r_ind, b1, S_ind).second;

          f_new(b0, b1, r_ind) += f(b0_new, b1_new, R_new_ind);
        }
      }
    }
  }

  if (sym_super_cell_dmn_t::dmn_size() > 0)
    f_new /= double(sym_super_cell_dmn_t::dmn_size());
  else
    throw std::logic_error(__FUNCTION__);

  double max = 0;
  for (int ind = 0; ind < f.size(); ++ind) {
    max = std::max(max, std::abs(f(ind) - f_new(ind)));

    f(ind) = f_new(ind);
  }

  if (do_diff)
    difference(max, f.get_name(), "r-cluster-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype, typename scalar_type, int D, domains::CLUSTER_NAMES N,
          domains::CLUSTER_SHAPE S>
void symmetrize_single_particle_function::execute(
    func::function<scalartype,
                   func::dmn_0<domains::cluster_domain<scalar_type, D, N, domains::MOMENTUM_SPACE, S>>>& f,
    bool do_diff) {
  typedef domains::cluster_domain<scalar_type, D, N, domains::MOMENTUM_SPACE, S> k_cluster_type;
  typedef func::dmn_0<k_cluster_type> k_dmn_t;

  typedef
      typename domains::cluster_symmetry<k_cluster_type>::sym_super_cell_dmn_t sym_super_cell_dmn_t;

  static func::function<std::pair<int, int>,
                        func::dmn_variadic<func::dmn_variadic<k_dmn_t, b>, sym_super_cell_dmn_t>>&
      k_symmetry_matrix = domains::cluster_symmetry<k_cluster_type>::get_symmetry_matrix();

  static func::function<scalartype, k_dmn_t> f_new;

  f_new = scalartype(0.);

  for (int S_ind = 0; S_ind < sym_super_cell_dmn_t::dmn_size(); ++S_ind) {
    for (int k_ind = 0; k_ind < k_dmn_t::dmn_size(); ++k_ind) {
      int K_new_ind = k_symmetry_matrix(k_ind, 0, S_ind).first;

      f_new(k_ind) += f(K_new_ind);
    }
  }

  if (sym_super_cell_dmn_t::dmn_size() > 0)
    f_new /= double(sym_super_cell_dmn_t::dmn_size());
  else
    throw std::logic_error(__FUNCTION__);

  double max = 0;
  for (int ind = 0; ind < f.size(); ++ind) {
    max = std::max(max, abs(f(ind) - f_new(ind)));

    f(ind) = f_new(ind);
  }

  if (do_diff)
    difference(max, f.get_name(), "k-cluster-domain of the function : " + f.get_name() + "\n");
}

template <typename scalartype, typename scalar_type, int D, domains::CLUSTER_NAMES N,
          domains::CLUSTER_SHAPE S>
void symmetrize_single_particle_function::execute(
    func::function<scalartype,
                   func::dmn_variadic<b, b, func::dmn_0<domains::cluster_domain<
                                                scalar_type, D, N, domains::MOMENTUM_SPACE, S>>>>& f,
    bool do_diff) {
  typedef domains::cluster_domain<scalar_type, D, N, domains::MOMENTUM_SPACE, S> k_cluster_type;
  typedef func::dmn_0<k_cluster_type> k_dmn_t;

  typedef
      typename domains::cluster_symmetry<k_cluster_type>::sym_super_cell_dmn_t sym_super_cell_dmn_t;

  static func::function<std::pair<int, int>,
                        func::dmn_variadic<func::dmn_variadic<k_dmn_t, b>, sym_super_cell_dmn_t>>&
      k_symmetry_matrix = domains::cluster_symmetry<k_cluster_type>::get_symmetry_matrix();

  static func::function<scalartype, func::dmn_variadic<b, b, k_dmn_t>> f_new;

  f_new = scalartype(0.);

  for (int S_ind = 0; S_ind < sym_super_cell_dmn_t::dmn_size(); ++S_ind) {
    for (int b0 = 0; b0 < b::dmn_size(); ++b0) {
      for (int b1 = 0; b1 < b::dmn_size(); ++b1) {
        for (int k_ind = 0; k_ind < k_dmn_t::dmn_size(); ++k_ind) {
          int K_new_ind = k_symmetry_matrix(k_ind, b0, S_ind).first;  // FIXME: b0 -> b1

          int b0_new = k_symmetry_matrix(0, b0, S_ind).second;
          int b1_new = k_symmetry_matrix(k_ind, b1, S_ind).second;

          f_new(b0, b1, k_ind) += f(b0_new, b1_new, K_new_ind);
        }
      }
    }
  }

  f_new /= double(sym_super_cell_dmn_t::dmn_size());

  double max = 0;
  for (int ind = 0; ind < f.size(); ++ind) {
    max = std::max(max, std::abs(f(ind) - f_new(ind)));

    f(ind) = f_new(ind);
  }

  if (do_diff)
    difference(max, f.get_name(), "k-clusterdomain of the function : " + f.get_name() + "\n");
}

}  // phys
}  // dca

#endif  // DCA_PHYS_DCA_STEP_SYMMETRIZATION_SYMMETRIZE_SINGLE_PARTICLE_FUNCTION_HPP
