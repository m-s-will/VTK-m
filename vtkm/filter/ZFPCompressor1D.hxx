//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2014 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2014 UT-Battelle, LLC.
//  Copyright 2014 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================

#include <vtkm/cont/ArrayHandleIndex.h>
#include <vtkm/cont/CellSetSingleType.h>
#include <vtkm/cont/CellSetStructured.h>
#include <vtkm/cont/DynamicArrayHandle.h>
#include <vtkm/cont/DynamicCellSet.h>
#include <vtkm/cont/ErrorFilterExecution.h>

#include <vtkm/worklet/DispatcherMapTopology.h>

namespace vtkm
{
namespace filter
{

namespace
{

template <typename CellSetList>
bool IsCellSetStructured(const vtkm::cont::DynamicCellSetBase<CellSetList>& cellset)
{
  if (cellset.template IsType<vtkm::cont::CellSetStructured<1>>())

  {
    return true;
  }
  return false;
}
} // anonymous namespace

//-----------------------------------------------------------------------------
inline VTKM_CONT ZFPCompressor1D::ZFPCompressor1D()
  : vtkm::filter::FilterField<ZFPCompressor1D>()
  , rate(0)
{
}


//-----------------------------------------------------------------------------
template <typename T, typename StorageType, typename DerivedPolicy>
inline VTKM_CONT vtkm::cont::DataSet ZFPCompressor1D::DoExecute(
  const vtkm::cont::DataSet& input,
  const vtkm::cont::ArrayHandle<T, StorageType>& field,
  const vtkm::filter::FieldMetadata& fieldMeta,
  const vtkm::filter::PolicyBase<DerivedPolicy>& policy)
{
  //  if (fieldMeta.IsPointField() == false)
  //  {
  //    throw vtkm::cont::ErrorFilterExecution("Point field expected.");
  //  }

  // Check the fields of the dataset to see what kinds of fields are present so
  // we can free the mapping arrays that won't be needed. A point field must
  // exist for this algorithm, so just check cells.
  const vtkm::Id numFields = input.GetNumberOfFields();
  bool hasCellFields = false;
  for (vtkm::Id fieldIdx = 0; fieldIdx < numFields && !hasCellFields; ++fieldIdx)
  {
    auto f = input.GetField(fieldIdx);
    if (f.GetAssociation() == vtkm::cont::Field::Association::CELL_SET)
    {
      hasCellFields = true;
    }
  }

  auto compressed = compressor.Compress(field, rate, field.GetNumberOfValues());

  vtkm::cont::DataSet dataset;
  vtkm::cont::Field compressedField(
    "compressed", vtkm::cont::Field::Association::POINTS, compressed);
  dataset.AddField(compressedField);
  return dataset;
}

//-----------------------------------------------------------------------------
template <typename T, typename StorageType, typename DerivedPolicy>
inline VTKM_CONT bool ZFPCompressor1D::DoMapField(
  vtkm::cont::DataSet& result,
  const vtkm::cont::ArrayHandle<T, StorageType>& input,
  const vtkm::filter::FieldMetadata& fieldMeta,
  const vtkm::filter::PolicyBase<DerivedPolicy>&)
{
  return false;
}
}
} // namespace vtkm::filter
