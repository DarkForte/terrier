//===----------------------------------------------------------------------===//
//
//                         Peloton
//
// parameter_translator.h
//
// Identification: src/include/execution/expression/parameter_translator.h
//
// Copyright (c) 2015-2017, Carnegie Mellon University Database Group
//
//===----------------------------------------------------------------------===//

#pragma once

#include "execution/expression/expression_translator.h"

namespace terrier::execution {

namespace expression {
class ParameterValueExpression;
}  // namespace planner



//===----------------------------------------------------------------------===//
// A parameter expression translator just produces the LLVM value version of the
// constant value within.
//===----------------------------------------------------------------------===//
class ParameterTranslator : public ExpressionTranslator {
 public:
  ParameterTranslator(const expression::ParameterValueExpression &exp,
                      CompilationContext &ctx);

  // Produce the value that is the result of codegen-ing the expression
  codegen::Value DeriveValue(CodeGen &codegen,
                             RowBatch::Row &row) const override;
};


}  // namespace terrier::execution