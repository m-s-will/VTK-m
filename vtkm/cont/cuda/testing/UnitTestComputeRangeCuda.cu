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
#include <vtkm/cont/cuda/DeviceAdapterCuda.h>

#include <vtkm/cont/cuda/internal/testing/Testing.h>
#include <vtkm/cont/testing/TestingComputeRange.h>

int UnitTestCudaComputeRange(int, char *[])
{
  int result = vtkm::cont::testing::TestingComputeRange
      <vtkm::cont::DeviceAdapterTagCuda>::Run();
  return vtkm::cont::cuda::internal::Testing::CheckCudaBeforeExit(result);
}
