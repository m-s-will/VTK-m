//============================================================================
//  Copyright (c) Kitware, Inc.
//  All rights reserved.
//  See LICENSE.txt for details.
//  This software is distributed WITHOUT ANY WARRANTY; without even
//  the implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR
//  PURPOSE.  See the above copyright notice for more information.
//
//  Copyright 2018 National Technology & Engineering Solutions of Sandia, LLC (NTESS).
//  Copyright 2018 UT-Battelle, LLC.
//  Copyright 2018 Los Alamos National Security.
//
//  Under the terms of Contract DE-NA0003525 with NTESS,
//  the U.S. Government retains certain rights in this software.
//
//  Under the terms of Contract DE-AC52-06NA25396 with Los Alamos National
//  Laboratory (LANL), the U.S. Government retains certain rights in
//  this software.
//============================================================================
#ifndef vtk_m_exec_BoundaryState_h
#define vtk_m_exec_BoundaryState_h

#include <vtkm/Math.h>

namespace vtkm
{
namespace exec
{

/// \brief Provides a neighborhood's placement with respect to the mesh's boundary.
///
/// \c BoundaryState provides functionality for \c WorkletPointNeighborhood algorithms and Fetch's
/// to determine if they are operating on a point near the boundary. It allows you to query about
/// overlaps of the neighborhood and the mesh boundary. It also helps convert local neighborhood
/// ids to the corresponding location in the mesh.
///
/// This class is typically constructed using the \c Boundary tag in an \c ExecutionSignature.
/// There is little reason to construct this in user code.
///
struct BoundaryState
{
  VTKM_EXEC
  BoundaryState(const vtkm::Id3& ijk, const vtkm::Id3& pdims)
    : IJK(ijk)
    , PointDimensions(pdims)
  {
  }

  //@{
  /// Returns true if a neighborhood of the given radius is contained within the bounds of the cell
  /// set in the X, Y, or Z direction. Returns false if the neighborhood extends ouside of the
  /// boundary of the data in the X, Y, or Z direction.
  ///
  /// The radius defines the size of the neighborhood in terms of how far away it extends from the
  /// center. So if there is a radius of 1, the neighborhood extends 1 unit away from the center in
  /// each direction and is 3x3x3. If there is a radius of 2, the neighborhood extends 2 units for
  /// a size of 5x5x5.
  ///
  VTKM_EXEC bool InXBoundary(vtkm::IdComponent radius) const
  {
    return (((this->IJK[0] - radius) >= 0) && ((this->IJK[0] + radius) < this->PointDimensions[0]));
  }
  VTKM_EXEC bool InYBoundary(vtkm::IdComponent radius) const
  {
    return (((this->IJK[1] - radius) >= 0) && ((this->IJK[1] + radius) < this->PointDimensions[1]));
  }
  VTKM_EXEC bool InZBoundary(vtkm::IdComponent radius) const
  {
    return (((this->IJK[2] - radius) >= 0) && ((this->IJK[2] + radius) < this->PointDimensions[2]));
  }
  //@}

  /// Returns true if a neighborhood of the given radius is contained within the bounds
  /// of the cell set. Returns false if the neighborhood extends ouside of the boundary of the
  /// data.
  ///
  /// The radius defines the size of the neighborhood in terms of how far away it extends from the
  /// center. So if there is a radius of 1, the neighborhood extends 1 unit away from the center in
  /// each direction and is 3x3x3. If there is a radius of 2, the neighborhood extends 2 units for
  /// a size of 5x5x5.
  ///
  VTKM_EXEC bool InBoundary(vtkm::IdComponent radius) const
  {
    return this->InXBoundary(radius) && this->InYBoundary(radius) && this->InZBoundary(radius);
  }

  /// Returns the minimum neighborhood indices that are within the bounds of the data.
  ///
  VTKM_EXEC vtkm::Vec<vtkm::IdComponent, 3> MinNeighborIndices(vtkm::IdComponent radius) const
  {
    vtkm::Vec<vtkm::IdComponent, 3> minIndices;

    for (vtkm::IdComponent component = 0; component < 3; ++component)
    {
      if (this->IJK[component] >= radius)
      {
        minIndices[component] = -radius;
      }
      else
      {
        minIndices[component] = static_cast<vtkm::IdComponent>(-this->IJK[component]);
      }
    }

    return minIndices;
  }

  /// Returns the minimum neighborhood indices that are within the bounds of the data.
  ///
  VTKM_EXEC vtkm::Vec<vtkm::IdComponent, 3> MaxNeighborIndices(vtkm::IdComponent radius) const
  {
    vtkm::Vec<vtkm::IdComponent, 3> maxIndices;

    for (vtkm::IdComponent component = 0; component < 3; ++component)
    {
      if ((this->PointDimensions[component] - this->IJK[component] - 1) >= radius)
      {
        maxIndices[component] = radius;
      }
      else
      {
        maxIndices[component] = static_cast<vtkm::IdComponent>(this->PointDimensions[component] -
                                                               this->IJK[component] - 1);
      }
    }

    return maxIndices;
  }

  //todo: This needs to work with BoundaryConstantValue
  //todo: This needs to work with BoundaryPeroidic

  //@{
  /// Takes a local neighborhood index (in the ranges of -neighborhood size to neighborhood size)
  /// and returns the ijk of the equivalent point in the full data set. If the given value is out
  /// of range, the value is clamped to the nearest boundary. For example, if given a neighbor
  /// index that is past the minimum x range of the data, the index at the minimum x boundary is
  /// returned.
  ///
  VTKM_EXEC vtkm::Id3 NeighborIndexToFullIndexClamp(
    const vtkm::Vec<vtkm::IdComponent, 3>& neighbor) const
  {
    vtkm::Id3 fullIndex = this->IJK + neighbor;

    return vtkm::Max(vtkm::Id3(0), vtkm::Min(this->PointDimensions - vtkm::Id3(1), fullIndex));
  }

  VTKM_EXEC vtkm::Id3 NeighborIndexToFullIndexClamp(vtkm::IdComponent neighborI,
                                                    vtkm::IdComponent neighborJ,
                                                    vtkm::IdComponent neighborK) const
  {
    return this->NeighborIndexToFullIndexClamp(vtkm::make_Vec(neighborI, neighborJ, neighborK));
  }
  //@}

  //todo: This needs to work with BoundaryConstantValue
  //todo: This needs to work with BoundaryPeroidic

  //@{
  /// Takes a local neighborhood index (in the ranges of -neighborhood size to neighborhood size)
  /// and returns the flat index of the equivalent point in the full data set. If the given value
  /// is out of range, the value is clamped to the nearest boundary. For example, if given a
  /// neighbor index that is past the minimum x range of the data, the index at the minimum x
  /// boundary is returned.
  ///
  VTKM_EXEC vtkm::Id NeighborIndexToFlatIndexClamp(
    const vtkm::Vec<vtkm::IdComponent, 3>& neighbor) const
  {
    vtkm::Id3 full = this->NeighborIndexToFullIndexClamp(neighbor);

    return (full[2] * this->PointDimensions[1] + full[1]) * this->PointDimensions[0] + full[0];
  }

  VTKM_EXEC vtkm::Id NeighborIndexToFlatIndexClamp(vtkm::IdComponent neighborI,
                                                   vtkm::IdComponent neighborJ,
                                                   vtkm::IdComponent neighborK) const
  {
    return this->NeighborIndexToFlatIndexClamp(vtkm::make_Vec(neighborI, neighborJ, neighborK));
  }
  //@}

  vtkm::Id3 IJK;
  vtkm::Id3 PointDimensions;
};
}
} // namespace vtkm::exec

#endif //vtk_m_exec_BoundaryState_h
