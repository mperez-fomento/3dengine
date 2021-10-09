#ifndef maze_hpp
#define maze_hpp

#include "basix.hpp"
#include "cell.hpp"

class Maze : public sf::Drawable
{
public:
  Maze(Vector2i dimension);

  bool can_enter(int cell_num) const;
  CellType type(int cell_num);
  void change_type(int cell_num, CellType type);
  Vector2f center_of(int cell_num);
  int cell_num(Vector2f coordinates);
  void draw(sf::RenderTarget &win, sf::RenderStates states = sf::RenderStates::Default) const;
  bool check_collision(const Sprite &sprite);
  Vector2f cell_size() const;
  int exit() const;
  void set_exit(int cell);
  void remove_exit();
  bool inside_maze(Vector2i pos) const;
  vector<int> neighbours(int num) const;
  void update_levels(const map<int, int> &levels);
  void mark_cells(const vector<int> &cells, Color color);
  void mark_cells(const map<int, int> &cells, Color color);
  void mark_all_cells(Color color);
  ~Maze();

private:
  vector<Cell *> cells_;
  Vector2f topleft_;
  Vector2f cell_size_ = {100, 100};
  Vector2i dimension_ = {8, 8}; // {rows, cols}
  int exit_ = -1;
};

class PathComputer
{
public:
  PathComputer(Maze *maze);

  bool found() const;
  vector<int> frontier() const;
  map<int, int> level() const;
  vector<int> path() const;
  bool next_level();
  void reset(int start, int end);

private:
  bool found_{false};
  vector<int> frontier_;
  map<int, int> level_;
  map<int, int> parent_;
  vector<int> path_;
  int current_level_;
  int start_;
  int end_;
  Maze *maze_;
};

vector<int> compute_path(int start, int end, Maze *maze);

#endif