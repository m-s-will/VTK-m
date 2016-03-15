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
//  Copyright 2014 Los Alamos National Security.
//
//  Under the terms of Contract DE-AC04-94AL85000 with Sandia Corporation,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================

#ifndef vtk_m_filter_DataSetFilter_h
#define vtk_m_filter_DataSetFilter_h

#include <vtkm/cont/DataSet.h>
#include <vtkm/cont/DynamicCellSet.h>
#include <vtkm/cont/CoordinateSystem.h>
#include <vtkm/cont/Field.h>

#include <vtkm/filter/PolicyBase.h>
#include <vtkm/filter/internal/RuntimeDeviceTracker.h>

namespace vtkm {
namespace filter {

class DataSetResult
{
public:
  VTKM_CONT_EXPORT
  DataSetResult(): Valid(false), Data()
    { }

  VTKM_CONT_EXPORT
  DataSetResult(const  vtkm::cont::DataSet& ds): Valid(true), Data(ds)
    { }

  VTKM_CONT_EXPORT
  bool IsValid() const { return this->Valid; }

  VTKM_CONT_EXPORT
  const vtkm::cont::DataSet& GetDataSet() const { return this->Data; }

  VTKM_CONT_EXPORT
  vtkm::cont::DataSet& GetDataSet() { return this->Data; }

private:
  bool Valid;
  vtkm::cont::DataSet Data;
};



template<class Derived>
class DataSetFilter
{
public:
  VTKM_CONT_EXPORT
  DataSetFilter();

  VTKM_CONT_EXPORT
  void SetActiveCellSet(vtkm::Id index)
    { this->CellSetIndex = index; }

  VTKM_CONT_EXPORT
  vtkm::Id GetActiveCellSetIndex() const
    { return this->CellSetIndex; }

  VTKM_CONT_EXPORT
  void SetActiveCoordinateSystem(vtkm::Id index)
    { this->CoordinateSystemIndex = index; }

  VTKM_CONT_EXPORT
  vtkm::Id GetActiveCoordinateSystemIndex() const
    { return this->CoordinateSystemIndex; }

  VTKM_CONT_EXPORT
  DataSetResult Execute(const vtkm::cont::DataSet &input);

  template<typename DerivedPolicy>
  VTKM_CONT_EXPORT
  DataSetResult Execute(const vtkm::cont::DataSet &input,
                        const vtkm::filter::PolicyBase<DerivedPolicy>& policy );

  //From the field we can extract the association component
  // ASSOC_ANY -> unable to map
  // ASSOC_WHOLE_MESH -> (I think this is points)
  // ASSOC_POINTS -> map using point mapping
  // ASSOC_CELL_SET -> how do we map this?
  // ASSOC_LOGICAL_DIM -> unable to map?
  VTKM_CONT_EXPORT
  bool MapFieldOntoOutput(DataSetResult& result,
                          const vtkm::cont::Field& field);

  template<typename DerivedPolicy>
  VTKM_CONT_EXPORT
  bool MapFieldOntoOutput(DataSetResult& result,
                          const vtkm::cont::Field& field,
                          const vtkm::filter::PolicyBase<DerivedPolicy>& policy);

private:
  template<typename DerivedPolicy>
  VTKM_CONT_EXPORT
  DataSetResult PrepareForExecution(const vtkm::cont::DataSet& input,
                                    const vtkm::filter::PolicyBase<DerivedPolicy>& policy);

  std::string OutputFieldName;
  vtkm::Id CellSetIndex;
  vtkm::Id CoordinateSystemIndex;
  vtkm::filter::internal::RuntimeDeviceTracker Tracker;
};

}
} // namespace vtkm::filter


#include <vtkm/filter/DataSetFilter.hxx>

#endif // vtk_m_filter_DataSetFilter_h