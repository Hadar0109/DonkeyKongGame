#pragma once

enum class Keys { LEFT = 'a', UP = 'w', RIGHT = 'd', DOWN = 'x', STAY = 's', ESC = 27, HAMMER = 'p' };
enum class Direction { POSITIVE = 1, NEGATIVE = -1, STAY = 0 };
enum class Boundaries { MAX_X = 80, MAX_Y = 25 };
enum class Characters { LADDER = 'H', BARREL = 'O', MARIO = '@', DONKEY_KONG = '&', PAULINE = '$', AIR = ' ', GHOST = 'x', HAMMER = 'p' , WALL = 'Q', LEGEND = 'L', BIG_GHOST = 'X' };
enum class FloorCharacters : char { PREV = '=', RIGHT = '>', LEFT = '<' };
enum class MenuType { MAIN_MENU, INSTRUCTIONS, PAUSE_MENU, SCREEN_OPTIONS};
enum class ResultValue { DISQUALIFIED, FINISHED, NO_RESULT };

