// Copyright (C) 2018 ETH Zurich
// Copyright (C) 2018 UT-Battelle, LLC
// All rights reserved.
//
// See LICENSE for terms of usage.
// See CITATION.md for citation guidelines, if DCA++ is used for scientific publications.
//
// Author: Giovanni Balduzzi(gbalduzz@itp.phys.ethz.ch)
//
// This file provides a lightweight proxy to access blocks of a matrix. The underlying matrix must
// not be destroyed while this object is in use. A const and a non const version are provided.

#ifndef DCA_LINALG_MATRIX_VIEW_HPP
#define DCA_LINALG_MATRIX_VIEW_HPP

#include <cassert>
#include <iostream>
#include <memory>
#include <stdexcept>

#include "dca/linalg/device_type.hpp"
#include "dca/util/cuda_definitions.hpp"

namespace dca {
namespace linalg {
// dca::linalg::

template <typename ScalarType, DeviceType device_name = linalg::CPU>
class MatrixView {
public:
  using Pair = std::pair<int, int>;
  MatrixView(ScalarType* data, Pair size, int ld);
  MatrixView(ScalarType* data, int size, int ld);
  MatrixView(ScalarType* data, int size);
  template <template <typename, DeviceType> class Matrix>
  MatrixView(Matrix<ScalarType, device_name>& mat);
  template <template <typename, DeviceType> class Matrix>
  MatrixView(Matrix<ScalarType, device_name>& mat, int offset_i, int offset_j);
  template <template <typename, DeviceType> class Matrix>
  MatrixView(Matrix<ScalarType, device_name>& mat, int offset_i, int offset_j, int ni, int nj);

  template <template <typename, DeviceType> class Matrix>
  MatrixView& operator=(const Matrix<ScalarType, device_name>& rhs);

  MatrixView& operator=(const MatrixView<ScalarType, device_name>& rhs);

  void print(std::ostream& out = std::cout) const;

  __DEVICE__ __HOST__ inline int leadingDimension() const {
    return ldm_;
  }
  __DEVICE__ __HOST__ inline int ld() const {
    return leadingDimension();
  }

  __DEVICE__ __HOST__ std::pair<int, int> size() const {
    return size_;
  }
  __DEVICE__ __HOST__ ScalarType* ptr() {
    return ptr_;
  }
  __DEVICE__ __HOST__ const ScalarType* ptr() const {
    return ptr_;
  }
  __DEVICE__ __HOST__ ScalarType* ptr(int i, int j) {
    assert(0 <= i && i <= size_.first);
    assert(0 <= j && j <= size_.second);
    return ptr_ + leadingDimension() * j + i;
  }
  __DEVICE__ __HOST__ const ScalarType* ptr(int i, int j) const {
    assert(0 <= i && i <= size_.first);
    assert(0 <= j && j <= size_.second);
    return ptr_ + leadingDimension() * j + i;
  }
  __DEVICE__ __HOST__ int nrRows() const {
    return size_.first;
  }
  __DEVICE__ __HOST__ int nrCols() const {
    return size_.second;
  }
  bool is_square() const {
    return size_.first == size_.second;
  }
  __DEVICE__ __HOST__ ScalarType& operator()(int i, int j) {
    assert(0 <= i && i < size_.first);
    assert(0 <= j && j < size_.second);
    return ptr_[i + j * ldm_];
  }
  __DEVICE__ __HOST__ const ScalarType& operator()(int i, int j) const {
    assert(0 <= i && i < size_.first);
    assert(0 <= j && j < size_.second);
    return ptr_[i + j * ldm_];
  }

private:
  ScalarType* const ptr_;
  const int ldm_;
  const std::pair<int, int> size_;
};

template <typename ScalarType, DeviceType device_t>
MatrixView<ScalarType, device_t>::MatrixView(ScalarType* const data, const Pair size, const int ld)
    : ptr_(data), ldm_(ld), size_(size) {}

template <typename ScalarType, DeviceType device_t>
MatrixView<ScalarType, device_t>::MatrixView(ScalarType* const data, const int size, const int ld)
    : ptr_(data), ldm_(ld), size_(std::make_pair(size, size)) {}

template <typename ScalarType, DeviceType device_t>
MatrixView<ScalarType, device_t>::MatrixView(ScalarType* const data, const int size)
    : ptr_(data), ldm_(size), size_(std::make_pair(size, size)) {}

template <typename ScalarType, DeviceType device_t>
template <template <typename, DeviceType> class Matrix>
MatrixView<ScalarType, device_t>::MatrixView(Matrix<ScalarType, device_t>& mat)
    : ptr_(mat.ptr()), ldm_(mat.leadingDimension()), size_(mat.size()) {}

template <typename ScalarType, DeviceType device_t>
template <template <typename, DeviceType> class Matrix>
MatrixView<ScalarType, device_t>::MatrixView(Matrix<ScalarType, device_t>& mat, int offset_i,
                                             int offset_j)
    : MatrixView(mat, offset_i, offset_j, mat.nrRows() - offset_i, mat.nrCols() - offset_j) {
  assert(offset_i < mat.nrCols());
  assert(offset_j < mat.nrRows());
}

template <typename ScalarType, DeviceType device_t>
template <template <typename, DeviceType> class Matrix>
MatrixView<ScalarType, device_t>::MatrixView(Matrix<ScalarType, device_t>& mat, int offset_i,
                                             int offset_j, int ni, int nj)
    : ptr_(mat.ptr(offset_i, offset_j)), ldm_(mat.leadingDimension()), size_(std::make_pair(ni, nj)) {
  assert(ni + offset_i <= mat.nrRows());
  assert(nj + offset_j <= mat.nrCols());
}

template <typename ScalarType, DeviceType device_t>
template <template <typename, DeviceType> class Matrix>
MatrixView<ScalarType, device_t>& MatrixView<ScalarType, device_t>::operator=(
    const Matrix<ScalarType, device_t>& rhs) {
  assert(nrCols() == rhs.nrCols() and nrRows() == rhs.nrRows());
  for (int j = 0; j < nrCols(); ++j)
    for (int i = 0; i < nrRows(); ++i)
      (*this)(i, j) = rhs(i, j);
  return *this;
}

template <typename ScalarType, DeviceType device_t>
MatrixView<ScalarType, device_t>& MatrixView<ScalarType, device_t>::operator=(
    const MatrixView<ScalarType, device_t>& rhs) {
  assert(nrCols() == rhs.nrCols() and nrRows() == rhs.nrRows());
  for (int j = 0; j < nrCols(); ++j)
    for (int i = 0; i < nrRows(); ++i)
      (*this)(i, j) = rhs(i, j);
  return *this;
}

template <typename ScalarType, DeviceType device_t>
void MatrixView<ScalarType, device_t>::print(std::ostream& out) const {
  out << "\tMatrix view:\n";
  out << "Size: \t" << size_.first << ", " << size_.second << "\n";

  for (int j = 0; j < nrRows(); ++j) {
    for (int i = 0; i < nrRows(); ++i)
      out << (*this)(i, j) << "\t";
    out << "\n";
  }
  out << "\n" << std::endl;
}

// Methods for producing a MatrixView object from a const reference.
// Note: always assign to a const object to maintain const-correctness.
template <typename ScalarType, DeviceType device_t, template <typename, DeviceType> class Matrix>
MatrixView<ScalarType, device_t> makeViewFromConst(const Matrix<ScalarType, device_t>& m) {
  auto& non_const_ref = const_cast<Matrix<ScalarType, device_t>&>(m);
  return MatrixView<ScalarType, device_t>(non_const_ref);
}

template <typename ScalarType, DeviceType device_t, template <typename, DeviceType> class Matrix>
MatrixView<ScalarType, device_t> makeViewFromConst(const Matrix<ScalarType, device_t>& m, int off_i,
                                                   int off_j) {
  auto& non_const_ref = const_cast<Matrix<ScalarType, device_t>&>(m);
  return MatrixView<ScalarType, device_t>(non_const_ref, off_i, off_j);
}

template <typename ScalarType, DeviceType device_t, template <typename, DeviceType> class Matrix>
MatrixView<ScalarType, device_t> makeViewFromConst(const Matrix<ScalarType, device_t>& m, int off_i,
                                                   int off_j, int n_i, int n_j) {
  auto& non_const_ref = const_cast<Matrix<ScalarType, device_t>&>(m);
  return MatrixView<ScalarType, device_t>(non_const_ref, off_i, off_j, n_i, n_j);
}

}  // namespace linalg
}  // namespace dca

#endif  // DCA_LINALG_MATRIX_VIEW_HPP
