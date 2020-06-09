# Tiger-Game
This is the final project for CSI 3344; Intro to Algorithms at Baylor University. It implements AI for both
men and the tiger in the [Tiger Game](https://en.wikipedia.org/wiki/Tiger_game).

![](https://upload.wikimedia.org/wikipedia/commons/thumb/8/8f/Tiger_game.svg/220px-Tiger_game.svg.png)

# Tiger Overview
The goal of the Tiger is not to eat the men, but rather to stay alive. To that end
the tiger AI will do everything in its power to avoid being encircled by the men. 
The idea stategy we found for the Tiger is for it to oscillate back and forth between two
points on the map. The Tiger attempts to always oscillate, and prefers to oscillate
along diagonals, because those give it the most chances for escape.

# Men Overview
**If the men play perfectly they will always win.** \
\
However, it is incredibly difficult to trap the tiger because the tiger can kill men by jumping them.
In order to trap the tiger, the men must advance the line forward in such a way that the tiger
can never get behind the advancing line. Once the Tiger is trapped in the top section of the map,
the men advance one by one into the pit until the tiger is trapped.

# Requirements
This project is written in C++11.