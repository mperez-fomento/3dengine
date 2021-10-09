#ifndef ENGINE3D_H_
#define ENGINE3D_H_

#include <string>
#include <vector>
#include "matrix.hpp"

namespace e3d {
    class Camera {
        public:
            matrix::Vector3 position;
            matrix::Vector3 target;
    };

    class Mesh {
        public:
            std::string name;
            std::vector<matrix::Vector3> vertices;
            matrix::Vector3 position;
            matrix::Vector3 rotation;
    };
}

#endif // ENGINE3D_H_
