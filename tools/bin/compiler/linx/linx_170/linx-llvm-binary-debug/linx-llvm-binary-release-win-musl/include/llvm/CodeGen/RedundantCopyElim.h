// ===- RedundantCopyElim.h - Redundant Copy Elimination -----------------------------*- C++ -*-=== //
//
// ===----------------------------------------------------------------------=== //
//
// \file This file contains the definition of the DAG scheduling mutation to
// eliminate redundant copy instructions.
//
// ===----------------------------------------------------------------------=== //

#ifndef LLVM_CODEGEN_REDUNDANTCOPYELIM_H
#define LLVM_CODEGEN_REDUNDANTCOPYELIM_H

#include <functional>
#include <memory>

namespace llvm {
class MachineInstr;
class ScheduleDAGMutation;
class TargetInstrInfo;
class TargetSubtargetInfo;

// Create a DAG scheduling mutation to eliminate redundant copy instructions.
std::unique_ptr<ScheduleDAGMutation>
createRedundantCopyElimDAGMutation();
} // end namespace llvm

#endif // LLVM_CODEGEN_REDUNDANTCOPYELIM_H
