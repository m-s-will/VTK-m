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

#ifndef vtk_m_filter_DataSetFilter_h
#define vtk_m_filter_DataSetFilter_h

#include <vtkm/cont/CoordinateSystem.h>
#include <vtkm/cont/DataSet.h>
#include <vtkm/cont/DynamicCellSet.h>
#include <vtkm/cont/Field.h>
#include <vtkm/cont/MultiBlock.h>
#include <vtkm/cont/RuntimeDeviceTracker.h>

#include <vtkm/filter/Filter.h>
#include <vtkm/filter/PolicyBase.h>
#include <vtkm/filter/Result.h>

namespace vtkm
{
namespace filter
{

template <class Derived>
class FilterDataSet : public vtkm::filter::Filter<Derived>
{
public:
  VTKM_CONT
  FilterDataSet();

  VTKM_CONT
  ~FilterDataSet();

  VTKM_CONT
  void SetActiveCellSetIndex(vtkm::Id index) { this->CellSetIndex = index; }

  VTKM_CONT
  vtkm::Id GetActiveCellSetIndex() const { return this->CellSetIndex; }

  VTKM_CONT
  void SetActiveCoordinateSystem(vtkm::Id index) { this->CoordinateSystemIndex = index; }

  VTKM_CONT
  vtkm::Id GetActiveCoordinateSystemIndex() const { return this->CoordinateSystemIndex; }

  //From the field we can extract the association component
  // ASSOC_ANY -> unable to map
  // ASSOC_WHOLE_MESH -> (I think this is points)
  // ASSOC_POINTS -> map using point mapping
  // ASSOC_CELL_SET -> how do we map this?
  // ASSOC_LOGICAL_DIM -> unable to map?
  VTKM_CONT
  bool MapFieldOntoOutput(Result& result, const vtkm::cont::Field& field);

  template <typename DerivedPolicy>
  VTKM_CONT bool MapFieldOntoOutput(Result& result,
                                    const vtkm::cont::Field& field,
                                    const vtkm::filter::PolicyBase<DerivedPolicy>& policy);

private:
  template <typename DerivedPolicy>
  VTKM_CONT Result PrepareForExecution(const vtkm::cont::DataSet& input,
                                       const vtkm::filter::PolicyBase<DerivedPolicy>& policy);

  vtkm::Id CellSetIndex;
  vtkm::Id CoordinateSystemIndex;

  friend class vtkm::filter::Filter<Derived>;
};
}
} // namespace vtkm::filter

#include <vtkm/filter/FilterDataSet.hxx>

#endif // vtk_m_filter_DataSetFilter_h
