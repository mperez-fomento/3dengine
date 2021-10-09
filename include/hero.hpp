#ifndef hero_hpp
#define hero_hpp

#include "basix.hpp"
#include "maze.hpp"

class Hero : public AnimatedSprite
{
public:
  Hero(Maze *maze, int initial_pos, int destination);
  void follow_path(vector<int> path);
  void update(Time delta);
  

private:
  int pos_ = 0;
  int destination_ = 0;
  Maze *maze_;
  stack<int> path_;
  float velocity_ = 0.005;
  Vector2f speed_;

  bool at_pos(int cell_num);
};

#endif