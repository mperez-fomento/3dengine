#ifndef cell_hpp
#define cell_hpp

#include "basix.hpp"

enum class CellType
{
    Wall,
    Corridor,
    Exit
};

class Cell : public Sprite
{
public:
    Cell(string image, CellType type);
    bool can_enter();
    CellType type() const;

private:
    CellType type_;
};

Cell *create_cell(CellType type, Vector2f position, Vector2f cell_size);

#endif