PROJECT(rask)

cmake_minimum_required(VERSION 2.8)

set( EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE} )

include_directories("." "tut-2009-08-30")
execute_process(COMMAND llvm-config --cppflags OUTPUT_VARIABLE LLVM_CXX_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config --ldflags OUTPUT_VARIABLE LLVM_LIB_FLAGS OUTPUT_STRIP_TRAILING_WHITESPACE)
execute_process(COMMAND llvm-config --libs all OUTPUT_VARIABLE LLVM_LIBS OUTPUT_STRIP_TRAILING_WHITESPACE)

set( CMAKE_CXX_FLAGS "${LLVM_CXX_FLAGS} -Wall" )
set( CMAKE_EXE_LINKER_FLAGS "${LLVM_LIB_FLAGS}" )

add_library(raskcore STATIC rask/cg/genFunction.cpp rask/ast/buildFunction.cpp rask/cst/parseFile.cpp rask/error/Message.cpp rask/Position.cpp rask/cg/genModule.cpp rask/cg/declBuiltinFunctions.cpp rask/ast/buildTree.cpp rask/ast/buildVariableDecl.cpp rask/cg/genVariableDecl.cpp rask/cg/genFunctionCall.cpp rask/ast/buildFunctionCall.cpp rask/ast/buildFunctionDecl.cpp rask/cg/declFunction.cpp rask/ast/BuiltinFunctions.cpp rask/ast/buildExpression.cpp rask/cg/genValue.cpp rask/ast/buildReturn.cpp rask/cg/genReturn.cpp)
ADD_EXECUTABLE(rask rask/main/rask.cpp)
ADD_EXECUTABLE(raskut rask/main/ut.cpp rask/ut/ast/CustomFunctionUT.cpp rask/ut/ast/buildFunctionUT.cpp rask/ut/cst/parseFileUT.cpp rask/ut/error/LoggerUT.cpp rask/ut/error/MessageUT.cpp rask/ut/InputStreamUT.cpp rask/ut/PositionUT.cpp rask/ut/cg/genFunctionUT.cpp rask/test/Console.cpp rask/ut/cg/genModuleUT.cpp rask/ut/cg/declBuiltinFunctionsUT.cpp rask/ut/ast/buildTreeUT.cpp rask/ut/ast/buildVariableDeclUT.cpp rask/ut/cg/genVariableDeclUT.cpp rask/ut/cg/genFunctionCallUT.cpp rask/ut/cg/SymbolTableUT.cpp rask/ut/ast/buildFunctionCallUT.cpp rask/ut/ast/SymbolTableUT.cpp rask/ut/ast/buildFunctionDeclUT.cpp rask/ut/ast/TreeUT.cpp rask/ut/cg/declFunctionUT.cpp rask/ut/ast/BuiltinFunctionUT.cpp rask/ut/ast/BuiltinFunctionsUT.cpp rask/ut/test/MockUT.cpp rask/ut/ast/buildExpressionUT.cpp rask/ut/cg/genValueUT.cpp rask/test/FunctionFactory.cpp rask/ut/ast/buildReturnUT.cpp rask/test/VariableDeclFactory.cpp rask/ut/cg/genReturnUT.cpp rask/test/VariableFactory.cpp)
ADD_EXECUTABLE(raskc rask/main/raskc.cpp)

target_link_libraries(raskc raskcore boost_program_options pthread ${LLVM_LIBS})
target_link_libraries(raskut raskcore pthread ${LLVM_LIBS})
target_link_libraries(rask raskcore ${LLVM_LIBS})

