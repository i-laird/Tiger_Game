cmake_minimum_required(VERSION 3.10)
project(Tiger-Game)
project(WhatIsTHis)

set(CMAKE_CXX_STANDARD 11)

include_directories(.)
include_directories(Tiger-Game)

add_executable(WhatIsTHis
        driver.cpp
        function_api.h
        game_runner.cpp
        game_runner.h
        general_util.cpp
        general_util.h
        Men_Mover.cpp
        Men_Mover.h
        men_util.cpp
        men_util.h
        Specific_Move_Handler.cpp
        Specific_Move_Handler.h
        testing_util.cpp
        testing_util.h
        Transition.cpp
        Transition.h
        Unorderded_State.cpp
        Unorderded_State.h)


add_executable(Tiger-Game
        Tiger-Game/Board.cpp
        Tiger-Game/Board.h
        Tiger-Game/function_api.h
        Tiger-Game/game_runner.cpp
        Tiger-Game/game_runner.h
        Tiger-Game/main.cpp
        Tiger-Game/man_move_helper_functions.cpp
        Tiger-Game/man_move_helper_functions.h
        Tiger-Game/Man_Mover.cpp
        Tiger-Game/Man_Mover.h
        Tiger-Game/Play_Tiger.cpp
        Tiger-Game/Play_Tiger.h
)