//============================================================================
//
// This file is part of the Thea toolkit.
//
// This software is distributed under the BSD license, as detailed in the
// accompanying LICENSE.txt file. Portions are derived from other works:
// their respective licenses and copyright information are reproduced in
// LICENSE.txt and/or in the relevant source files.
//
// Author: Siddhartha Chaudhuri
// First version: 2009
//
//============================================================================

#ifndef __Thea_VariantMatrix_hpp__
#define __Thea_VariantMatrix_hpp__

#include "Common.hpp"
#include "IOStream.hpp"
#include "MatrixWrapper.hpp"
#include "MatVec.hpp"
#include "Serializable.hpp"
#include "SparseMatrixWrapper.hpp"
#include "SparseMatVec.hpp"
#include <type_traits>

namespace Thea {

#define THEA_VARMAT_CHECK_DENSE(fn_name) \
  { alwaysAssertM(isDense(), "VariantMatrix: " fn_name "() can only be called for a dense matrix"); }

#define THEA_VARMAT_CHECK_SPARSE(fn_name) \
  { alwaysAssertM(isSparse(), "VariantMatrix: " fn_name "() can only be called for a sparse matrix"); }

/** A container holding either a dense or a sparse matrix (or null). */
template < typename T = Real, typename DenseMatrixT = MatrixX<T>, typename SparseMatrixT = SparseColumnMatrix<T> >
class VariantMatrix : public AbstractDenseMatrix<T>, public AbstractCompressedSparseMatrix<T>, public Serializable
{
  private:
    static_assert(std::is_base_of< Eigen::DenseBase<DenseMatrixT>, DenseMatrixT >::value,
                  "VariantMatrix: DenseMatrixT is not a valid dense matrix");

    static_assert(std::is_base_of< Eigen::SparseMatrixBase<SparseMatrixT>, SparseMatrixT >::value,
                  "VariantMatrix: SparseMatrixT is not a valid sparse matrix");

    static_assert(std::is_same<typename DenseMatrixT::value_type, T>::value,
                  "VariantMatrix: Dense matrix must have T as element type");

    static_assert(std::is_same<typename SparseMatrixT::value_type, T>::value,
                  "VariantMatrix: Sparse matrix must have T as element type");

  public:
    /** Types of core matrices (enum class). */
    struct Type
    {
      /** Enumeration values. */
      enum Value
      {
        NONE   = 0,  ///< No valid matrix.
        DENSE  = 1,  ///< Dense matrix.
        SPARSE = 2,  ///< Sparse matrix.
      };

      THEA_ENUM_CLASS_BODY(Type)

      THEA_ENUM_CLASS_STRINGS_BEGIN(Type)
        THEA_ENUM_CLASS_STRING(NONE,    "none")
        THEA_ENUM_CLASS_STRING(DENSE,   "dense")
        THEA_ENUM_CLASS_STRING(SPARSE,  "sparse")
      THEA_ENUM_CLASS_STRINGS_END(Type)

    }; // struct Type

    using typename AbstractDenseMatrix<T>::Value;
    using typename AbstractDenseMatrix<T>::value_type;

    typedef DenseMatrixT   Dense;   ///< Dense matrix type.
    typedef SparseMatrixT  Sparse;  ///< Sparse matrix type.

    /** Default constructor. */
    VariantMatrix() : type(Type::NONE), dense_wrapper(&dense), sparse_wrapper(&sparse) {}

    /** Get the type of the matrix (dense, sparse, or invalid). */
    Type getType() const { return type; }

    /** Set the type of the matrix (dense, sparse, or invalid). */
    void setType(Type type_) const { type = type_; }

    /** Check if the object stores a valid matrix (may be zero-sized) or not. */
    bool isValid() const { return type != Type::NONE; }

    /** Check if the object stores a dense matrix (may be zero-sized) or not. */
    bool isDense() const { return type == Type::DENSE; }

    /** Check if the object stores a sparse matrix (may be zero-sized) or not. */
    bool isSparse() const { return type == Type::SPARSE; }

    /** Get the underlying dense matrix. */
    Dense const & getDense() const { THEA_VARMAT_CHECK_DENSE("getDense"); return dense; }

    /** Get the underlying dense matrix. */
    Dense & getDense() { THEA_VARMAT_CHECK_DENSE("getDense"); return dense; }

    /** Get the underlying sparse matrix. */
    Sparse const & getSparse() const { THEA_VARMAT_CHECK_SPARSE("getSparse"); return sparse; }

    /** Get the underlying sparse matrix. */
    Sparse & getSparse() { THEA_VARMAT_CHECK_SPARSE("getSparse"); return sparse; }

    /** Clear all matrix data. */
    void clear()
    {
      dense.resize(0, 0);
      sparse.resize(0, 0); sparse.data.squeeze();
      type = Type::NONE;
    }

    // Functions from AbstractMatrix
    int64 rows() const { return isDense() ? dense_wrapper.rows() : (isSparse() ? sparse_wrapper.rows() : 0); }
    int64 cols() const { return isDense() ? dense_wrapper.cols() : (isSparse() ? sparse_wrapper.cols() : 0); }
    void setZero() { if (isDense()) { dense_wrapper.setZero(); } else if (isSparse()) { sparse_wrapper.setZero(); } }

    int8 isResizable() const
    {
      return isDense() ? dense_wrapper.isResizable() : (isSparse() ? sparse_wrapper.isResizable() : 0);
    }

    int8 resize(int64 nrows, int64 ncols)
    {
      if (isDense())
        return dense_wrapper.resize(nrows, ncols);
      else if (isSparse())
        return sparse_wrapper.resize(nrows, ncols);
      else
        return 0;
    }

    // Functions from AbstractAddressableMatrix
    Value const & at(int64 row, int64 col) const { THEA_VARMAT_CHECK_DENSE("at"); return dense_wrapper.at(row, col); }
    Value & mutableAt(int64 row, int64 col) { THEA_VARMAT_CHECK_DENSE("mutableAt"); return dense_wrapper.mutableAt(row, col); }

    // Functions from RowOrColumnMajorMatrix
    int8 isRowMajor() const { return isDense() ? dense_wrapper.isRowMajor() : (isSparse() ? sparse_wrapper.isRowMajor() : 0); }
    int8 isColumnMajor() const
    { return isDense() ? dense_wrapper.isColumnMajor() : (isSparse() ? sparse_wrapper.isColumnMajor() : 0); }

    // Functions from AbstractDenseMatrix
    Value const * data() const { THEA_VARMAT_CHECK_DENSE("data"); return dense_wrapper.data(); }
    Value * data() { THEA_VARMAT_CHECK_DENSE("data");  return dense_wrapper.data(); }
    void fill(Value const & value) { THEA_VARMAT_CHECK_DENSE("fill");  dense_wrapper.fill(value); }
    void getRow(int64 row, Value * values) const { THEA_VARMAT_CHECK_DENSE("getRow"); dense_wrapper.getRow(row, values); }
    void setRow(int64 row, Value const * values) { THEA_VARMAT_CHECK_DENSE("setRow"); dense_wrapper.setRow(row, values); }
    void getColumn(int64 col, Value * values) const
    { THEA_VARMAT_CHECK_DENSE("getColumn"); dense_wrapper.getColumn(col, values); }
    void setColumn(int64 col, Value const * values)
    { THEA_VARMAT_CHECK_DENSE("setColumn"); dense_wrapper.setColumn(col, values); }

    // Functions from AbstractSparseMatrix
    int64 numStoredElements() const
    { THEA_VARMAT_CHECK_SPARSE("numStoredElements"); return sparse_wrapper.numStoredElements(); }

    // Functions from AbstractCompressedSparseMatrix
    int64 innerSize() const { THEA_VARMAT_CHECK_SPARSE("innerSize"); return sparse_wrapper.innerSize(); }
    int64 outerSize() const { THEA_VARMAT_CHECK_SPARSE("outerSize"); return sparse_wrapper.outerSize(); }
    int8 isFullyCompressed() const { THEA_VARMAT_CHECK_SPARSE("isFullyCompressed"); return sparse_wrapper.isFullyCompressed(); }
    int32 getInnerIndexType() const
    { THEA_VARMAT_CHECK_SPARSE("getInnerIndexType"); return sparse_wrapper.getInnerIndexType(); }
    int32 getOuterIndexType() const
    { THEA_VARMAT_CHECK_SPARSE("getOuterIndexType"); return sparse_wrapper.getOuterIndexType(); }
    int32 getNonZeroCountType() const
    { THEA_VARMAT_CHECK_SPARSE("getNonZeroCountType"); return sparse_wrapper.getNonZeroCountType(); }
    void const * getInnerIndices() const
    { THEA_VARMAT_CHECK_SPARSE("getInnerIndices"); return sparse_wrapper.getInnerIndices(); }
    void * getInnerIndices() { THEA_VARMAT_CHECK_SPARSE("getInnerIndices"); return sparse_wrapper.getInnerIndices(); }
    void const * getOuterIndices() const
    { THEA_VARMAT_CHECK_SPARSE("getOuterIndices"); return sparse_wrapper.getOuterIndices(); }
    void * getOuterIndices() { THEA_VARMAT_CHECK_SPARSE("getOuterIndices"); return sparse_wrapper.getOuterIndices(); }
    void const * getNonZeroCounts() const
    { THEA_VARMAT_CHECK_SPARSE("getNonZeroCounts"); return sparse_wrapper.getNonZeroCounts(); }
    void * getNonZeroCounts() { THEA_VARMAT_CHECK_SPARSE("getNonZeroCounts"); return sparse_wrapper.getNonZeroCounts(); }
    Value const * getValues() const { THEA_VARMAT_CHECK_SPARSE("getValues"); return sparse_wrapper.getValues(); }
    Value * getValues() { THEA_VARMAT_CHECK_SPARSE("getValues"); return sparse_wrapper.getValues(); }

    // Type-casting functions
    AbstractAddressableMatrix<Value> const * asAddressable() const { return isDense() ? this : nullptr; }
    AbstractAddressableMatrix<Value> * asAddressable() { return isDense() ? this : nullptr; }
    AbstractDenseMatrix<Value> const * asDense() const { return isDense() ? this : nullptr; }
    AbstractDenseMatrix<Value> * asDense() { return isDense() ? this : nullptr; }
    AbstractSparseMatrix<Value> const * asSparse() const { return isSparse() ? this : nullptr; }
    AbstractSparseMatrix<Value> * asSparse() { return isSparse() ? this : nullptr; }
    AbstractCompressedSparseMatrix<Value> const * asCompressed() const { return isSparse() ? this : nullptr; }
    AbstractCompressedSparseMatrix<Value> * asCompressed() { return isSparse() ? this : nullptr; }

    // Functions from Serializable
    void read(BinaryInputStream & input, Codec const & codec = Codec_AUTO(), bool read_block_header = false)
    {
      if (codec == Codec_AUTO())
      {
        auto which = input.readMatrix(read_block_header, dense, sparse);
        type = (which == 0 ? Type::DENSE : Type::SPARSE);
      }
      else
      {
        // Decide based on the type of codec supplied
        if (dynamic_cast< MatrixCodec<Dense> const * >(&codec))
        {
          input.readMatrix(read_block_header, dense, codec);
          type = Type::DENSE;
        }
        else if (dynamic_cast< MatrixCodec<Sparse> const * >(&codec))
        {
          input.readMatrix(read_block_header, sparse, codec);
          type = Type::SPARSE;
        }
        else
          throw Error("VariantMatrix: Codec is not valid for either the dense or sparse matrix types");
      }
    }

    void write(BinaryOutputStream & output, Codec const & codec = Codec_AUTO(), bool write_block_header = false) const
    {
      if (isDense())
        output.writeMatrix(dense, codec, write_block_header);
      else if (isSparse())
        output.writeMatrix(sparse, codec, write_block_header);
      else
        throw Error("VariantMatrix: Cannot write an invalid matrix");
    }

  private:
    Type                         type;            ///< Is the matrix dense or sparse?
    Dense                        dense;           ///< The dense matrix, if any.
    Sparse                       sparse;          ///< The sparse matrix, if any.
    MatrixWrapper<Dense>         dense_wrapper;   ///< Wrapper for the dense matrix.
    SparseMatrixWrapper<Sparse>  sparse_wrapper;  ///< Wrapper for the sparse matrix.

}; // struct VariantMatrix

#undef THEA_VARMAT_CHECK_DENSE
#undef THEA_VARMAT_CHECK_SPARSE

} // namespace Thea

#endif
