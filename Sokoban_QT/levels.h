#ifndef LEVELS_H
#define LEVELS_H


class Levels
{
public:
    Levels();
    // 1 wall 2 player 3 box 4 check 5 ready box 0 b_wall
    const int level [4][8][8]{
        {   {1,1,1,1,1,1,1,1},
            {1,0,1,0,0,1,0,1},
            {1,0,1,0,0,1,4,1},
            {1,0,1,1,1,1,0,1},
            {1,0,0,0,3,0,0,1},
            {1,0,0,2,0,0,0,1},
            {1,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1}
        },
        {   {1,1,1,1,1,1,1,1},
            {1,0,1,0,0,1,0,1},
            {1,4,1,0,0,1,4,1},
            {1,0,1,1,1,1,0,1},
            {1,0,3,0,3,0,0,1},
            {1,0,3,2,0,0,0,1},
            {1,4,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1}
        },
        {   {1,1,1,1,1,1,1,1},
            {1,0,1,0,0,1,0,1},
            {1,0,1,0,4,1,0,1},
            {1,4,1,1,0,1,4,1},
            {1,0,3,0,3,3,0,1},
            {1,0,0,2,0,0,0,1},
            {1,0,0,0,0,0,0,1},
            {1,1,1,1,1,1,1,1}
        },
        {   {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0},
            {0,0,0,0,0,0,0,0}
        }

    };
};

#endif // LEVELS_H
