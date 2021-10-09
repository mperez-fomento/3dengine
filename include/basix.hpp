/** \file basix.hpp
 * \brief Makes graphics easy for learners.
 *
 * Wraps SFML to make it easy for learners the development of a graphical
 * application. Provides several utility functions, a central resource
 * manager, a class for animated sprites, and some collision detection
 * functions.
 *
 * Utility functions:
 * - to_string(t): converts an object 't' to a string
 * - error(s): shows error message 's' and finishes the program
 * - error(s, s2): shows error message 's'+'s2' and finishes the program
 * - keep_window_open(): shows 'Please enter a character to exit' and waits for that char
 * - keep_winwow_open(s): shows message 's' and waits for a char from the user
 * - now(): returns the current time and date
 * - pow2(base): returns 'base' squared
 * - random_int(min, max): returns a random int between 'min' and 'max'
 * - random_double(min, max): returns a random double between 'min' and 'max'
 * - sort(c): sorts the elements of 'c' (a container) using the '<' operator
 * - sort(c, p): sorts the elements of 'c' (a container) using the predicate 'p'
 * - find(c, v): searches for value 'v' in container 'c'; returns an iterator
 * - find_if(c, v, p): searches for value 'v' among the elements of 'c' for which 'p' is true
 */

#ifndef ezsfml_hpp
#define ezsfml_hpp

#include <algorithm>
#include <array>
#include <assert.h>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <forward_list>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <list>
#include <map>
#include <memory>
#include <random>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>

using namespace std;

namespace basix
{

inline void printv(vector<int> v)
{
  for (auto const x : v)
  {
    cout << x << " ";
  }
  cout << "\n";
}

// Converts an object of any type to a string
template <class T>
string to_string(const T &t)
{
  ostringstream os;
  os << t;
  return os.str();
}

struct Exit : runtime_error
{
  Exit() : runtime_error("Exit") {}
};

// error() simply disguises throws:
inline void error(const string &s)
{
  throw runtime_error(s);
}

inline void error(const string &s, const string &s2)
{
  error(s + s2);
}

inline void error(const string &s, int i)
{
  ostringstream os;
  os << s << ": " << i;
  error(os.str());
}

template <class T>
char *as_bytes(T &i) // needed for binary I/O
{
  void *addr = &i; // get the address of the first byte
  // of memory used to store the object
  return static_cast<char *>(addr); // treat that memory as bytes
}

// Shows a message on the console and waits for the user to enter a character
// Useful to keep the console open to see the results of our program
inline void keep_window_open()
{
  cin.clear();
  cin.ignore(cin.rdbuf()->in_avail(), '\n');
  getchar();
  return;
}

inline void keep_window_open(string s)
{
  if (s == "")
    return;
  cin.clear();
  cin.ignore(120, '\n');
  for (;;)
  {
    cout << "Please enter " << s << " to exit\n";
    string ss;
    while (cin >> ss && ss != s)
      cout << "Please enter " << s << " to exit\n";
    return;
  }
}

// Returns the system time
inline time_t now()
{
  return time(nullptr);
}

// Returns 'base' squared
inline double pow2(double base)
{
  return base * base;
}

const int DEFAULT_MIN_RANDOM_INT = 0;
const int DEFAULT_MAX_RANDOM_INT = 10;

// Returns a random int between 'min' and 'max'
// If 'min' and 'max' are not specified, the range is 0..10

inline int random_int(int min = DEFAULT_MIN_RANDOM_INT, int max = DEFAULT_MAX_RANDOM_INT)
{
  if (min <= max)
  {
    std::uniform_int_distribution<int> distribution(min, max);
    std::random_device rd;
    std::mt19937 random_generator(rd());
    return distribution(random_generator);
  }
  else
  {
    throw "random_int: lower bound is greater than upper bound";
  }
}

const double DEFAULT_MIN_RANDOM_DOUBLE = 0.0;
const double DEFAULT_MAX_RANDOM_DOUBLE = 1.0;

// Returns a random double between 'min' and 'max'
// If 'min' and 'max' are not specified, the range is 0..10

inline double random_double(double min = DEFAULT_MIN_RANDOM_DOUBLE, double max = DEFAULT_MAX_RANDOM_DOUBLE)
{
  if (min <= max)
  {
    std::uniform_real_distribution<double> distribution(min, max);
    std::random_device rd;
    std::mt19937 random_generator(rd());
    return distribution(random_generator);
  }
  else
  {
    throw "random_double: lower bound is greater than upper bound";
  }
}

template <typename C>
using Value_type = typename C::value_type;

template <typename C>
using Iterator = typename C::iterator;

// Sorts the elements of a Container 'c' using the '<' operator
template <typename C>
// requires Container<C>()
void sort(C &c)
{
  std::sort(c.begin(), c.end());
}

// Sorts the elements of a Container 'c' using 'p' as predicate
template <typename C, typename Pred>
// requires Container<C>() && Binary_Predicate<Value_type<C>>()
void sort(C &c, Pred p)
{
  std::sort(c.begin(), c.end(), p);
}

// Finds the value 'v' in Container 'c'
// Returns an iterator to the element found
template <typename C, typename Val>
// requires Container<C>() && Equality_comparable<C,Val>()
Iterator<C> find(C &c, Val v)
{
  return std::find(c.begin(), c.end(), v);
}

// Finds the value 'v' among the elements of Container 'c' for which 'p' is true
template <typename C, typename Pred>
// requires Container<C>() && Predicate<Pred,Value_type<C>>()
Iterator<C> find_if(C &c, Pred p)
{
  return std::find_if(c.begin(), c.end(), p);
}

// Avoid namespace prefixing and other renamings
using sf::Clock;
using sf::Color;
using sf::Event;
using sf::Font;
using sf::Keyboard;
using sf::milliseconds;
using sf::Mouse;
using sf::Music;
using sf::seconds;
using sf::Texture;
using sf::Time;
using sf::View;
using Vector = sf::Vector2<double>;
using sf::Vector2f;
using sf::Vector2i;

// Resource holders

template <typename Resource, typename Identifier>
class Resource_holder
{
 public:
  void load(Identifier id, const std::string &filename);
  Resource &get(Identifier id);
  const Resource &get(Identifier id) const;

 private:
  std::map<Identifier, std::unique_ptr<Resource>> _resource_map;
};

template <typename Resource, typename Identifier>
inline void Resource_holder<Resource, Identifier>::load(Identifier id,
                                                        const std::string &filename)
{
  std::unique_ptr<Resource> resource(new Resource());
  if (!resource->loadFromFile(filename))
    throw std::runtime_error("Failed to load " + filename);
  auto inserted = _resource_map.insert(std::make_pair(id, std::move(resource)));
}

template <typename Resource, typename Identifier>
inline Resource &Resource_holder<Resource, Identifier>::get(Identifier id)
{
  auto found = _resource_map.find(id);
  assert(found != _resource_map.end());
  return *found->second;
}

template <typename Resource, typename Identifier>
inline const Resource &Resource_holder<Resource, Identifier>::get(Identifier id) const
{
  auto found = _resource_map.find(id);
  assert(found != _resource_map.end());
  return *found->second;
}

using Identifier = std::string;

static Resource_holder<Texture, Identifier> texture_holder{};
static Resource_holder<Font, Identifier> font_holder{};
static Resource_holder<sf::SoundBuffer, Identifier> sound_buffer_holder{};

// Window class

class Point : public sf::Vector2f
{
 public:
  Point(const double x, const double y);
  Point(const sf::Vector2f vf);
  Point(const sf::Vector2i vi);
};

inline Point::Point(const double x, const double y)
    : sf::Vector2f(float(x), float(y))
{
}

inline Point::Point(const sf::Vector2f v)
    : sf::Vector2f(v)
{
}

inline Point::Point(const sf::Vector2i vi)
    : sf::Vector2f(vi)
{
}

inline double distance(const Point a, const Point b)
{
  return sqrt((b.x - a.x) * (b.x - a.x) + (b.y - a.y) * (b.y - a.y));
}

inline float city_distance(Vector2f p, Vector2f q)
{
  return max(abs(p.x - q.x), abs(p.y - q.y));
}


class Window : public sf::RenderWindow
{
 public:
  Window(const int width, const int height, const std::string &title);
  void clear(const Color &color = Color(255, 255, 255, 0));
  bool contains(const Point &point,
                const int hmargin = 0, const int vmargin = 0) const;
  bool contains(const double x, const double y,
                const int hmargin = 0, const int vmargin = 0) const;
};

const int DEFAULT_WINDOW_WIDTH = 800;
const int DEFAULT_WINDOW_HEIGHT = 600;

inline Window::Window(const int width = DEFAULT_WINDOW_WIDTH,
                      const int height = DEFAULT_WINDOW_HEIGHT,
                      const std::string &title = "")
    : sf::RenderWindow(sf::VideoMode(width, height), title)
{
}

inline void Window::clear(const Color &color)
{
  sf::RenderWindow::clear(color);
}

inline bool Window::contains(const Point &point,
                      const int hmargin,
                      const int vmargin) const
{
  Vector window_size{getSize()};
  sf::Vector2i pixel = mapCoordsToPixel(point);
  return pixel.x > hmargin && pixel.x < window_size.x - hmargin && pixel.y > vmargin && pixel.y < window_size.y - vmargin;
}

inline bool Window::contains(const double x, const double y,
                      const int hmargin, const int vmargin) const
{
  return contains(Point{x, y}, hmargin, vmargin);
}

class FullScreenWindow : public Window {
 public:
  FullScreenWindow(const std::string title = "");
};

inline FullScreenWindow::FullScreenWindow(const std::string title)
    : Window()
{
  vector<sf::VideoMode> modes = sf::VideoMode::getFullscreenModes();
  sf::VideoMode mode {sf::VideoMode::getDesktopMode()};
  if (mode.isValid()) {
    this->create(mode, title, sf::Style::Fullscreen);
  }
  else {
    for (std::size_t i = 0; i < modes.size(); ++i) {
      mode = modes[i];
      if (mode.isValid()) {
        break;
      }
    }
    this->create(mode, title, sf::Style::Fullscreen);
  }
}

// Classes and functions to add graphic elements (sprites, circles, boxes, text,...)

class Sprite : public sf::Sprite
{
 public:
  Sprite();
  Sprite(const std::string &filename);
  bool contains(const Point point) const;
  bool contains(const double x, const double y) const;
};

inline Sprite::Sprite() : sf::Sprite() {}

inline Sprite::Sprite(const std::string &filename)
{
  texture_holder.load(filename, filename);
  setTexture(texture_holder.get(filename), true);
  setOrigin(getLocalBounds().width / 2,
            getLocalBounds().height / 2);
}

inline bool Sprite::contains(const Point point) const
{
  return getGlobalBounds().contains(point);
}

inline bool Sprite::contains(const double x, const double y) const
{
  return contains(Point{x, y});
}

inline Sprite create_sprite(const std::string &filename)
{
  return Sprite(filename);
}

// Overload for backwards compatibility. First argument unused.
inline Sprite create_sprite(Window &, const std::string &filename)
{
  return create_sprite(filename);
}

// Animated Sprite

enum class AnimationState
{
  Playing,
  Stopped,
  Paused
};

struct Animation
{
  std::string id;
  std::string sprite_sheet;
  sf::Vector2i sheet_dimensions;
  sf::Vector2i frame_size;
  Time frame_duration;
  sf::IntRect operator[](const int index) const;
  sf::IntRect operator[](const Time elapsed) const;
  int frame_number() const;
  Time duration() const;
};

inline sf::IntRect Animation::operator[](const int index) const
{
  int x = index % sheet_dimensions.x;
  int y = index / sheet_dimensions.x;
  return sf::IntRect{sf::Vector2i{x * frame_size.x, y * frame_size.y}, frame_size};
}

inline sf::IntRect Animation::operator[](const Time elapsed) const
{
  int index = elapsed.asMilliseconds() / frame_duration.asMilliseconds();
  if (index >= frame_number())
  {
    index = frame_number() - 1;
  }
  return (*this)[index];
}

inline int Animation::frame_number() const
{
  return sheet_dimensions.x * sheet_dimensions.y;
}

inline Time Animation::duration() const
{
  return sf::milliseconds(frame_duration.asMilliseconds() * frame_number());
}

class AnimatedSprite : public Sprite
{
 public:
  AnimatedSprite();
  AnimatedSprite(const std::string &id,
                 const std::string &sprite_sheet,
                 const sf::Vector2i sheet_dimensions,
                 const Time frame_duration);
  void addAnimation(const std::string &id,
                    const std::string &sprite_sheet,
                    const sf::Vector2i sheet_dimensions,
                    const Time frame_duration);
  void activateAnimation(const std::string &id);
  void play(const std::string &id, bool loop = false);
  void stop();
  void pause();
  void resume();
  void update(const Time elapsed_time);
  bool isPlayingAnimation() const;
  bool isPlayingAnimation(const std::string& id) const;
  std::string getPlayingAnimation() const;
  std::string getActiveAnimation() const;

 private:
  std::map<std::string, Animation> animations;
  AnimationState state = AnimationState::Stopped;
  std::string activeAnimation;
  Time elapsedSinceStart;
  sf::IntRect frame;
  bool loop{false};
};

inline AnimatedSprite::AnimatedSprite() {}

inline AnimatedSprite::AnimatedSprite(const std::string &id,
                               const std::string &sprite_sheet,
                               const sf::Vector2i sheet_dimensions,
                               const Time frame_duration)
    : Sprite(),
      state{AnimationState::Stopped},
      elapsedSinceStart{sf::milliseconds(0)},
      frame{getTextureRect()}
{
  addAnimation(id, sprite_sheet, sheet_dimensions, frame_duration);
}

inline void AnimatedSprite::addAnimation(const std::string &id,
                                  const std::string &sprite_sheet,
                                  const sf::Vector2i sheet_dimensions,
                                  const Time frame_duration)
{
  texture_holder.load(sprite_sheet, sprite_sheet);
  Texture &sheet{texture_holder.get(sprite_sheet)};
  sf::Vector2u sheet_size = sheet.getSize();
  int frame_size_x = sheet_size.x / sheet_dimensions.x;
  int frame_size_y = sheet_size.y / sheet_dimensions.y;
  animations[id] = Animation{id,
    sprite_sheet,
    sheet_dimensions,
    sf::Vector2i{frame_size_x, frame_size_y},
    frame_duration};
}

inline void AnimatedSprite::activateAnimation(const std::string &id)
{
  state = AnimationState::Stopped;
  activeAnimation = id;
  elapsedSinceStart = sf::milliseconds(0);
  frame = sf::IntRect{{0, 0}, animations[id].frame_size};
  setTexture(texture_holder.get(animations[id].sprite_sheet));
  setOrigin(0, 0);
  setTextureRect(frame);
  setOrigin(animations[id].frame_size.x / 2, animations[id].frame_size.y / 2);
}

inline void AnimatedSprite::play(const std::string &id, bool repeat)
{
  activeAnimation = id;
  elapsedSinceStart = sf::milliseconds(0);
  frame = sf::IntRect{{0, 0}, animations[id].frame_size};
  setTexture(texture_holder.get(animations[id].sprite_sheet));
  setOrigin(0, 0);
  setTextureRect(frame);
  setOrigin(animations[id].frame_size.x / 2, animations[id].frame_size.y / 2);
  state = AnimationState::Playing;
  loop = repeat;
}

inline void AnimatedSprite::stop()
{
  state = AnimationState::Stopped;
  loop = false;
}

inline void AnimatedSprite::pause()
{
  if (state == AnimationState::Playing)
  {
    state = AnimationState::Paused;
  }
}

inline void AnimatedSprite::resume()
{
  if (state == AnimationState::Paused)
  {
    state = AnimationState::Playing;
  }
}

inline void AnimatedSprite::update(const Time elapsed_time)
{
  if (activeAnimation != "")
  {
    Animation curAnim = animations[activeAnimation];
    if (state == AnimationState::Playing)
    {
      elapsedSinceStart += elapsed_time;
      if (elapsedSinceStart > curAnim.duration())
      {
        if (loop)
        {
          elapsedSinceStart = sf::milliseconds(0);
        }
        else
        {
          stop();
        }
      }
    }
    frame = curAnim[elapsedSinceStart];
    setTextureRect(frame);
  }
}

inline bool AnimatedSprite::isPlayingAnimation() const
{
  return state == AnimationState::Playing;
}

inline std::string AnimatedSprite::getPlayingAnimation() const
{
  if (isPlayingAnimation())
  {
    return activeAnimation;
  }
  else
  {
    return "";
  }
}

inline bool AnimatedSprite::isPlayingAnimation(const std::string& id) const
{
  return state == AnimationState::Playing && activeAnimation == id;
}

inline std::string AnimatedSprite::getActiveAnimation() const
{
  return activeAnimation;
}

const std::string DEFAULT_FONT = "UbuntuMono-R.ttf";
const Identifier DEFAULT_FONT_ID = "default";
const int DEFAULT_FONT_SIZE = 16;
const Color DEFAULT_TEXT_COLOR = Color(0, 0, 0);

class Text : public sf::Text
{
 public:
  Text(const std::string &text_string = "",
       const int font_size = DEFAULT_FONT_SIZE,
       const Identifier font = DEFAULT_FONT);
  bool contains(const Point point) const;
  bool contains(const double x, const double y) const;
};

inline Text::Text(const std::string &text_string,
           const int font_size,
           const Identifier font)
{
  setString(text_string);
  font_holder.load(font, font);
  setFont(font_holder.get(font));
  setCharacterSize(font_size);
  setFillColor(DEFAULT_TEXT_COLOR);
}

inline bool Text::contains(const Point point) const
{
  return getGlobalBounds().contains(point);
}

inline bool Text::contains(const double x, const double y) const
{
  return contains(Point{x, y});
}

inline Text create_text(const std::string &text_string = "",
                        const int font_size = DEFAULT_FONT_SIZE,
                        const Identifier font = DEFAULT_FONT)
{
  Text text(text_string, font_size, font);
  return text;
}

// Overload for backwards compatibility. First argument unused.
inline Text create_text(Window &,
                        const std::string &text_string = "",
                        const int font_size = DEFAULT_FONT_SIZE,
                        const Identifier font = DEFAULT_FONT)
{
  return create_text(text_string, font_size, font);
}

inline void wait_for(const int ms)
{
  sf::sleep(sf::milliseconds(ms));
}

class Sound : public sf::Sound
{
public:
  Sound(const std::string &filename);
};

inline Sound::Sound(const std::string &filename) : sf::Sound()
{
  sound_buffer_holder.load(filename, filename);
  setBuffer(sound_buffer_holder.get(filename));
}

inline Sound create_sound(const std::string &filename)
{
  return Sound(filename);
}

// Overload for backwards compatibility. First argument unused.
inline Sound create_sound(Window &, const std::string &filename)
{
  return create_sound(filename);
}

class Circle : public sf::CircleShape
{
 public:
  Circle(const double radius, const Point center = Point{0, 0});
  bool contains(const Point &point) const;
  bool contains(const double x, const double y) const;
};

inline Circle::Circle(const double radius, const Point center)
    : sf::CircleShape(float(radius))
{
  setOrigin(Point{radius, radius});
  move(center);
}

inline bool Circle::contains(const Point &point) const
{
  return distance(point, getPosition()) < getRadius();
}

inline bool Circle::contains(const double x, const double y) const
{
  return contains(Point{x, y});
}

inline Circle create_circle(const double radius, const Point center)
{
  return Circle(radius, center);
}

// Overload for backwards compatibility. First argument unused.
inline Circle create_circle(Window &, const double radius, const Point center)
{
  return create_circle(radius, center);
}

class Rectangle : public sf::RectangleShape
{
 public:
  Rectangle(const double width, const double height, const Point center);
};

inline Rectangle::Rectangle(const double width = 0,
                     const double height = 0,
                     const Point center = Point{0, 0})
    : sf::RectangleShape(sf::Vector2f{float(width), float(height)})
{
  setOrigin(Point{width / 2, height / 2});
  move(center);
}

inline Rectangle create_rectangle(const double width,
                                  const double height,
                                  const Point center)
{
  return Rectangle{width, height, center};
}

// Overload for backwards compatibility. First argument unused.
inline Rectangle create_rectangle(Window &,
                                  const double width,
                                  const double height,
                                  const Point center)
{
  return create_rectangle(width, height, center);
}

inline bool is_inside_window(const Point point,
                      const Window &window,
                      int hmargin = 0,
                      int vmargin = 0)
{
  Vector window_size{window.getSize()};
  return point.x > hmargin && point.x < window_size.x - hmargin && point.y > vmargin && point.y < window_size.y - vmargin;
}

inline bool is_inside_rectangle(const Point point, const Point topleft, const Point bottomright)
{
  return point.x > topleft.x && point.x < bottomright.x && point.y > topleft.y && point.y < bottomright.y;
}

inline bool is_inside_rectangle(const Point point, const Rectangle &rectangle)
{
  return rectangle.getGlobalBounds().contains(point);
}

} // namespace basix

namespace Collision
{
class BitmaskManager
{
 public:
  ~BitmaskManager()
  {
    std::map<const sf::Texture *, sf::Uint8 *>::const_iterator end = Bitmasks.end();
    for (std::map<const sf::Texture *, sf::Uint8 *>::const_iterator iter = Bitmasks.begin(); iter != end; iter++)
      delete[] iter->second;
  }

  sf::Uint8 GetPixel(const sf::Uint8 *mask, const sf::Texture *tex, unsigned int x, unsigned int y)
  {
    if (x > tex->getSize().x || y > tex->getSize().y)
      return 0;

    return mask[x + y * tex->getSize().x];
  }

  sf::Uint8 *GetMask(const sf::Texture *tex)
  {
    sf::Uint8 *mask;
    std::map<const sf::Texture *, sf::Uint8 *>::iterator pair = Bitmasks.find(tex);
    if (pair == Bitmasks.end())
    {
      sf::Image img = tex->copyToImage();
      mask = CreateMask(tex, img);
    }
    else
      mask = pair->second;

    return mask;
  }

  sf::Uint8 *CreateMask(const sf::Texture *tex, const sf::Image &img)
  {
    sf::Uint8 *mask = new sf::Uint8[tex->getSize().y * tex->getSize().x];

    for (unsigned int y = 0; y < tex->getSize().y; y++)
    {
      for (unsigned int x = 0; x < tex->getSize().x; x++)
        mask[x + y * tex->getSize().x] = img.getPixel(x, y).a;
    }

    Bitmasks.insert(std::pair<const sf::Texture *, sf::Uint8 *>(tex, mask));

    return mask;
  }

 private:
  std::map<const sf::Texture *, sf::Uint8 *> Bitmasks;
};

static BitmaskManager Bitmasks;

inline bool PixelPerfectTest(const sf::Sprite &Object1, const sf::Sprite &Object2, sf::Uint8 AlphaLimit)
{
  sf::FloatRect Intersection;
  if (Object1.getGlobalBounds().intersects(Object2.getGlobalBounds(), Intersection))
  {
    sf::IntRect O1SubRect = Object1.getTextureRect();
    sf::IntRect O2SubRect = Object2.getTextureRect();

    sf::Uint8 *mask1 = Bitmasks.GetMask(Object1.getTexture());
    sf::Uint8 *mask2 = Bitmasks.GetMask(Object2.getTexture());

    // Loop through our pixels
    for (int i = Intersection.left; i < Intersection.left + Intersection.width; i++)
    {
      for (int j = Intersection.top; j < Intersection.top + Intersection.height; j++)
      {

        sf::Vector2f o1v = Object1.getInverseTransform().transformPoint(i, j);
        sf::Vector2f o2v = Object2.getInverseTransform().transformPoint(i, j);

        // Make sure pixels fall within the sprite's subrect
        if (o1v.x > 0 && o1v.y > 0 && o2v.x > 0 && o2v.y > 0 &&
            o1v.x < O1SubRect.width && o1v.y < O1SubRect.height &&
            o2v.x < O2SubRect.width && o2v.y < O2SubRect.height)
        {

          if (Bitmasks.GetPixel(mask1, Object1.getTexture(), (int)(o1v.x) + O1SubRect.left, (int)(o1v.y) + O1SubRect.top) > AlphaLimit &&
              Bitmasks.GetPixel(mask2, Object2.getTexture(), (int)(o2v.x) + O2SubRect.left, (int)(o2v.y) + O2SubRect.top) > AlphaLimit)
            return true;
        }
      }
    }
  }
  return false;
}

inline bool CreateTextureAndBitmask(sf::Texture &LoadInto, const std::string &Filename)
{
  sf::Image img;
  if (!img.loadFromFile(Filename))
    return false;
  if (!LoadInto.loadFromImage(img))
    return false;

  Bitmasks.CreateMask(&LoadInto, img);
  return true;
}

inline sf::Vector2f GetSpriteCenter(const sf::Sprite &Object)
{
  sf::FloatRect AABB = Object.getGlobalBounds();
  return sf::Vector2f(AABB.left + AABB.width / 2.f, AABB.top + AABB.height / 2.f);
}

inline sf::Vector2f GetSpriteSize(const sf::Sprite &Object)
{
  sf::IntRect OriginalSize = Object.getTextureRect();
  sf::Vector2f Scale = Object.getScale();
  return sf::Vector2f(OriginalSize.width * Scale.x, OriginalSize.height * Scale.y);
}

inline bool CircleTest(const sf::Sprite &Object1, const sf::Sprite &Object2)
{
  sf::Vector2f Obj1Size = GetSpriteSize(Object1);
  sf::Vector2f Obj2Size = GetSpriteSize(Object2);
  float Radius1 = (Obj1Size.x + Obj1Size.y) / 4;
  float Radius2 = (Obj2Size.x + Obj2Size.y) / 4;

  sf::Vector2f Distance = GetSpriteCenter(Object1) - GetSpriteCenter(Object2);

  return (Distance.x * Distance.x + Distance.y * Distance.y <= (Radius1 + Radius2) * (Radius1 + Radius2));
}

class OrientedBoundingBox // Used in the BoundingBoxTest
{
 public:
  OrientedBoundingBox(const sf::Sprite &Object) // Calculate the four points of the OBB from a transformed (scaled, rotated...) sprite
  {
    sf::Transform trans = Object.getTransform();
    sf::IntRect local = Object.getTextureRect();
    Points[0] = trans.transformPoint(0.f, 0.f);
    Points[1] = trans.transformPoint(local.width, 0.f);
    Points[2] = trans.transformPoint(local.width, local.height);
    Points[3] = trans.transformPoint(0.f, local.height);
  }

  sf::Vector2f Points[4];

  void ProjectOntoAxis(const sf::Vector2f &Axis, float &Min, float &Max) // Project all four points of the OBB onto the given axis and return the dotproducts of the two outermost points
  {
    Min = (Points[0].x * Axis.x + Points[0].y * Axis.y);
    Max = Min;
    for (int j = 1; j < 4; j++)
    {
      float Projection = (Points[j].x * Axis.x + Points[j].y * Axis.y);

      if (Projection < Min)
        Min = Projection;
      if (Projection > Max)
        Max = Projection;
    }
  }
};

inline bool BoundingBoxTest(const sf::Sprite &Object1, const sf::Sprite &Object2)
{
  OrientedBoundingBox OBB1(Object1);
  OrientedBoundingBox OBB2(Object2);

  // Create the four distinct axes that are perpendicular to the edges of the two rectangles
  sf::Vector2f Axes[4] = {
    sf::Vector2f(OBB1.Points[1].x - OBB1.Points[0].x,
                 OBB1.Points[1].y - OBB1.Points[0].y),
    sf::Vector2f(OBB1.Points[1].x - OBB1.Points[2].x,
                 OBB1.Points[1].y - OBB1.Points[2].y),
    sf::Vector2f(OBB2.Points[0].x - OBB2.Points[3].x,
                 OBB2.Points[0].y - OBB2.Points[3].y),
    sf::Vector2f(OBB2.Points[0].x - OBB2.Points[1].x,
                 OBB2.Points[0].y - OBB2.Points[1].y)};

  for (int i = 0; i < 4; i++) // For each axis...
  {
    float MinOBB1, MaxOBB1, MinOBB2, MaxOBB2;

    // ... project the points of both OBBs onto the axis ...
    OBB1.ProjectOntoAxis(Axes[i], MinOBB1, MaxOBB1);
    OBB2.ProjectOntoAxis(Axes[i], MinOBB2, MaxOBB2);

    // ... and check whether the outermost projected points of both OBBs overlap.
    // If this is not the case, the Separating Axis Theorem states that there can be no collision between the rectangles
    if (!((MinOBB2 <= MaxOBB1) && (MaxOBB2 >= MinOBB1)))
      return false;
  }
  return true;
}
} // namespace Collision

using namespace basix;

#endif
