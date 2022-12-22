/////////////////////////////////////////////////////////////////////////////
// Authored by Jeong-Mo Hong for CSE4060 course at Dongguk University CSE  //
// jeongmo.hong@gmail.com                                                  //
// Do whatever you want license.                                           //
/////////////////////////////////////////////////////////////////////////////

#include "MatrixMN.h"
#include <assert.h>

template class MatrixMN<float>;
template class MatrixMN<double>;

//template void MatrixMN<float>::multiply(const VectorND<float>& vector, VectorND<float>& result) const;
//template void MatrixMN<float>::multiplyTransposed(const VectorND<float>& vector, VectorND<float>& result) const;
//
//template void MatrixMN<double>::multiply(const VectorND<double>& vector, VectorND<double>& result) const;
//template void MatrixMN<double>::multiplyTransposed(const VectorND<double>& vector, VectorND<double>& result) const;
