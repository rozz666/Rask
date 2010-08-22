PROJECT(rask)

set( EXECUTABLE_OUTPUT_PATH ../bin/${CMAKE_BUILD_TYPE} )

include_directories("." "tut-2009-08-30")

add_library(raskcore STATIC rask/ast/parseFunction.cpp rask/cg/genFunction.cpp rask/cst/parseFile.cpp rask/error/Message.cpp rask/Position.cpp)
ADD_EXECUTABLE(rask rask/main/rask.cpp )
ADD_EXECUTABLE(raskut rask/main/ut.cpp rask/ut/ast/FunctionUT.cpp rask/ut/ast/parseFunctionUT.cpp rask/ut/cg/genFunctionUT.cpp rask/ut/cst/parseFileUT.cpp rask/ut/error/LoggerUT.cpp rask/ut/error/MessageUT.cpp rask/ut/InputStreamUT.cpp rask/ut/PositionUT.cpp rask/test/Console.cpp)
ADD_EXECUTABLE(raskc rask/main/raskc.cpp)

target_link_libraries(raskc raskcore boost_program_options pthread)
target_link_libraries(raskut raskcore pthread)
target_link_libraries(rask raskcore)
