//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2014 Sandia Corporation.
//  Copyright 2014 UT-Battelle, LLC.
//  Copyright 2014. Los Alamos National Security
//
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================
#ifndef vtk_m_cont_internal_ArrayPortalFromIterators_h
#define vtk_m_cont_internal_ArrayPortalFromIterators_h

#include <vtkm/Types.h>
#include <vtkm/cont/ArrayPortal.h>
#include <vtkm/cont/Assert.h>

#include <iterator>

namespace vtkm {
namespace cont {
namespace internal {

/// This templated implementation of an ArrayPortal allows you to adapt a pair
/// of begin/end iterators to an ArrayPortal interface.
///
template<class IteratorT>
class ArrayPortalFromIterators
{
public:
  typedef typename std::iterator_traits<IteratorT>::value_type ValueType;

  VTKM_CONT_EXPORT ArrayPortalFromIterators() {  }

  VTKM_CONT_EXPORT
  ArrayPortalFromIterators(IteratorT begin, IteratorT end)
    : BeginIterator(begin), NumberOfValues(std::distance(begin, end))
  {
    VTKM_ASSERT_CONT(this->GetNumberOfValues() >= 0);
  }

  /// Copy constructor for any other ArrayPortalFromIterators with an iterator
  /// type that can be copied to this iterator type. This allows us to do any
  /// type casting that the iterators do (like the non-const to const cast).
  ///
  template<class OtherIteratorT>
  VTKM_CONT_EXPORT
  ArrayPortalFromIterators(const ArrayPortalFromIterators<OtherIteratorT> &src)
    : BeginIterator(src.GetRawIterator()), NumberOfValues(src.GetNumberOfValues())
  {  }

  VTKM_CONT_EXPORT
  vtkm::Id GetNumberOfValues() const
  {
    return this->NumberOfValues;
  }

  VTKM_CONT_EXPORT
  ValueType Get(vtkm::Id index) const
  {
    return *this->IteratorAt(index);
  }

  VTKM_CONT_EXPORT
  void Set(vtkm::Id index, const ValueType& value) const
  {
    *this->IteratorAt(index) = value;
  }

#ifndef VTKM_MSVC
  typedef IteratorT IteratorType;

  VTKM_CONT_EXPORT
  IteratorType GetIteratorBegin() const { return this->BeginIterator; }
#else // VTKM_MSVC
  // The MSVC compiler issues warnings if you use raw pointers with some
  // operations. To keep the compiler happy (and add some safety checks),
  // wrap the iterator in a check.
  typedef stdext::checked_array_iterator<IteratorT> IteratorType;

  VTKM_CONT_EXPORT
  IteratorType GetIteratorBegin() const {
    return IteratorType(this->BeginIterator, this->GetNumberOfValues());
  }
#endif // VTKM_MSVC

  VTKM_CONT_EXPORT
  IteratorType GetIteratorEnd() const {
    IteratorType iterator = this->GetIteratorBegin();
    std::advance(iterator, this->GetNumberOfValues());
    return iterator;
  }

  VTKM_CONT_EXPORT
  IteratorT GetRawIterator() const {
    return this->BeginIterator;
  }

private:
  IteratorT BeginIterator;
  vtkm::Id NumberOfValues;

  VTKM_CONT_EXPORT
  IteratorT IteratorAt(vtkm::Id index) const
  {
    VTKM_ASSERT_CONT(index >= 0);
    VTKM_ASSERT_CONT(index < this->GetNumberOfValues());

    return this->BeginIterator + index;
  }
};

}
}
} // namespace vtkm::cont::internal

#endif //vtk_m_cont_internal_ArrayPortalFromIterators_h
