cmake_minimum_required(VERSION 3.7.1)
project(Tiger-Game)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -Wall -Wextra -pedantic")

add_executable(Tiger-Game
        driver.cpp
        function_api.h
        game_runner.cpp
        game_runner.h
        general_util.cpp
        general_util.h
        Men_Mover.h
        men_util.cpp
        men_util.h
        Specific_Move_Handler.cpp
        Specific_Move_Handler.h
        testing_util.cpp
        testing_util.h
        Transition.cpp
        Transition.h
        Unordered_State.cpp
        Unordered_State.h
        Smart_Mover.cpp
        Smart_Mover.h
        Men_Mover.cpp)

