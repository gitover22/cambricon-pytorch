/*
All modification made by Cambricon Corporation: © 2022 Cambricon Corporation
All rights reserved.
All other contributions:
Copyright (c) 2014--2022, the respective contributors
All rights reserved.
For the list of contributors go to https://github.com/pytorch/pytorch/graphs/contributors
Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.
THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE
FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "aten/viewchain/viewNodeFuser.h"

namespace torch_mlu {

ViewNodeOptRegister& ViewNodeOptRegister::getInstance() {
  static ViewNodeOptRegister instance;
  return instance;
}

// For fusion optimization
BaseViewOp::baseViewOpPtr
ViewNodeOptRegister::runViewNodeFusionOpt(const std::vector<BaseViewOp::baseViewOpPtr>& other) {
  auto* registerRawPtr = getSpecificRawPtr(other.back()->type());
  TORCH_MLU_CHECK(registerRawPtr != nullptr, "Get Fusion register class ptr failed.");
  // Call op Opt func.
  return registerRawPtr->runViewNodeFusionOpt(other);
}

// For replace optimization
BaseViewOp::baseViewOpPtr
ViewNodeOptRegister::runViewNodeReplaceOpt(const BaseViewOp::baseViewOpPtr& ptr) {
  // Can't find any way to do view node fusion.
  auto* registerRawPtr = getSpecificRawPtr(ptr->type());
  if (registerRawPtr == nullptr) return ptr;
  // Call op Opt func.
  return registerRawPtr->runViewNodeReplaceOpt(ptr);
}

// For move optimization
std::vector<BaseViewOp::baseViewOpPtr>
ViewNodeOptRegister::runViewNodeMoveOpt(std::vector<BaseViewOp::baseViewOpPtr>& other,
                                        ViewsType type) {
  // Can't find any way to do view node fusion.
  auto* registerRawPtr = getSpecificRawPtr(type);
  if (registerRawPtr == nullptr) return other;
  // Call op Opt func.
  return registerRawPtr->runViewNodeMoveOpt(other);
}

}  // end namespace torch_mlu

