PROJECT(rask)

cmake_minimum_required(VERSION 2.8)

set( EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE} )

include_directories("." "tut-2009-08-30" "gmock-1.5.0/fused-src" "dicpp/include")
execute_process(COMMAND llvm-config --cppflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LIB_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config --libs all OUTPUT_VARIABLE LLVM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

set( CMAKE_CXX_FLAGS "${LLVM_CXX_FLAGS} -Wall" )
set( CMAKE_EXE_LINKER_FLAGS "${LLVM_LIB_FLAGS}" )

add_library(
    raskcore STATIC
    rask/Position.cpp
    rask/error/Message.cpp)

add_library(
    raskast STATIC
    rask/ast/Expression.cpp
    rask/ast/buildFunction.cpp
    rask/ast/buildTree.cpp
    rask/ast/buildVariableDecl.cpp
    rask/ast/buildFunctionCall.cpp
    rask/ast/buildFunctionDecl.cpp
    rask/ast/BuiltinFunctions.cpp
    rask/ast/buildExpression.cpp
    rask/ast/buildChainExpression.cpp
    rask/ast/buildReturn.cpp
    rask/ast/buildUnaryOperatorCall.cpp
    rask/Operators.cpp)

add_library(
    raskcst STATIC
    rask/cst/Parser.cpp
    rask/cst/parseFile.cpp)

add_library(
    raskcg STATIC
    rask/cg/CodeGenerator.cpp
    rask/cg/declBuiltinFunctions.cpp
    rask/cg/declFunction.cpp
    rask/cg/genFunction.cpp
    rask/cg/genFunctionCall.cpp
    rask/cg/genModule.cpp
    rask/cg/genVariableDecl.cpp
    rask/cg/genValue.cpp
    rask/cg/genReturn.cpp
    rask/cg/Prefixes.cpp
    rask/Operators.cpp)

add_library(
    gmock STATIC
    gmock-1.5.0/fused-src/gmock-gtest-all.cc
)

add_library(
    dicpp STATIC
    dicpp/lib/src/type_info.cpp
    dicpp/lib/src/scope.cpp
    dicpp/lib/src/scopes
    dicpp/lib/src/scopes/thread_local_singleton.cpp
    dicpp/lib/src/scopes/no_scope.cpp
    dicpp/lib/src/scopes/singleton.cpp
    dicpp/lib/src/demangle.cpp
    dicpp/lib/src/registry.cpp
    dicpp/lib/src/empty.cpp
    dicpp/lib/src/type_provider.cpp
)

ADD_EXECUTABLE(rask rask/main/rask.cpp)
ADD_EXECUTABLE(
    raskut
    rask/main/ut.cpp
    rask/test/GTestPrinter.cpp
    rask/ut/ast/ExpressionUT.cpp
    rask/ut/ast/ScopeUT.cpp
    rask/ut/nullUT.cpp
    rask/ut/ast/ConstantUT.cpp
    rask/ut/ast/operatorNameUT.cpp
    rask/ut/ast/CustomFunctionUT.cpp
    rask/ut/ast/buildFunctionUT.cpp
    rask/ut/cst/ParserUT.cpp
    rask/ut/error/LoggerUT.cpp
    rask/ut/error/MessageUT.cpp
    rask/ut/InputStreamUT.cpp
    rask/ut/PositionUT.cpp
    rask/ut/cg/genFunctionUT.cpp
    rask/ut/cg/genModuleUT.cpp
    rask/ut/cg/declBuiltinFunctionsUT.cpp
    rask/ut/ast/buildTreeUT.cpp
    rask/ut/ast/buildVariableDeclUT.cpp
    rask/ut/cg/genVariableDeclUT.cpp
    rask/ut/cg/genFunctionCallUT.cpp
    rask/ut/cg/SymbolTableUT.cpp
    rask/ut/ast/buildFunctionCallUT.cpp
    rask/ut/ast/FunctionTableUT.cpp
    rask/ut/ast/buildFunctionDeclUT.cpp
    rask/ut/ast/TreeUT.cpp
    rask/ut/cg/declFunctionUT.cpp
    rask/ut/ast/BuiltinFunctionUT.cpp
    rask/ut/ast/BuiltinFunctionsUT.cpp
    rask/ut/test/MockUT.cpp
    rask/ut/ast/buildChainExpressionUT.cpp
    rask/ut/ast/buildExpressionUT.cpp
    rask/ut/cg/genValueUT.cpp
    rask/ut/ast/buildReturnUT.cpp
    rask/ut/cg/genReturnUT.cpp
    rask/ut/ast/buildUnaryOperatorCallUT.cpp
    rask/test/Console.cpp
    rask/test/FunctionFactory.cpp
    rask/test/VariableDeclFactory.cpp
    rask/test/VariableFactory.cpp)

ADD_EXECUTABLE(raskc rask/main/raskc.cpp)

target_link_libraries(raskc raskcore raskast raskcst raskcg dicpp boost_program_options pthread ${LLVM_LIBS})
target_link_libraries(raskut raskcore raskast raskcst raskcg gmock pthread ${LLVM_LIBS})
target_link_libraries(rask ${LLVM_LIBS})

