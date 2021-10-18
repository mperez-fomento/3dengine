#ifndef ENGINE3D_H_
#define ENGINE3D_H_

#include <string>
#include <vector>
#include "matrix.hpp"
#include "basix.hpp"

namespace e3d {
    const matrix::Matrix4x4 buildRotationMatrix(const double xrot, const double yrot, const double zrot) {
        matrix::Matrix4x4 xrm {};
        matrix::Matrix4x4 yrm {};
        matrix::Matrix4x4 zrm {};
        xrm[0][0] = 1;
        xrm[1][1] = cos(xrot);
        xrm[1][2] = -sin(xrot);
        xrm[2][1] = sin(xrot);
        xrm[2][2] = cos(xrot);
        xrm[3][3] = 1;
        yrm[0][0] = cos(yrot);
        yrm[0][2] = sin(yrot);
        yrm[1][1] = 1;
        yrm[2][0] = -sin(yrot);
        yrm[2][2] = cos(yrot);
        yrm[3][3] = 1;
        zrm[0][0] = cos(zrot);
        zrm[0][1] = -sin(zrot);
        zrm[1][0] = sin(zrot);
        zrm[1][1] = cos(zrot);
        zrm[2][2] = 1;
        zrm[3][3] = 1;
        return xrm * yrm * zrm;
    }

    const matrix::Matrix4x4 buildTraslationMatrix(const double dx, const double dy, const double dz) {
        matrix::Matrix4x4 tm {};
        tm[0][0] = 1;
        tm[1][1] = 1;
        tm[2][2] = 1;
        tm[3][0] = dx;
        tm[3][1] = dy;
        tm[3][2] = dz;
        tm[3][3] = 1;
        return tm;
    }

    class Camera {
        public:
            Camera(float fl, float ap, float n, float f, float field)
                : focal_length(fl), aperture(ap), near(n), far(f), fov(field)
            {
                setPosition(0, 0, 0);
                setRotation(0, 0, 0);
            }
            const matrix::Matrix4x4 projectionMatrix() const {
                matrix::Matrix4x4 pM {};
                pM[0][0] = 1 / tan(fov*3.141592f/360.0); // Considering a square canvas
                pM[1][1] = 1 / tan(fov*3.141592f/360.0);
                pM[2][2] = - far / (far - near);
                pM[3][2] = - far * near / (far - near);
                pM[2][3] = - 1;
                pM[3][3] = 0.0;
                return pM;
            }
            void setPosition(const double x, const double y, const double z) {
                position = matrix::Vector3{x, y, z};
                computeTraslationMatrix();
                computeCameraToWorldMatrix();
            }
            void setPosition(const matrix::Vector3& p) {
                setPosition(p[0], p[1], p[2]);
            }
            void move(const double x, const double y, const double z) {
                setPosition(position + matrix::Vector3{x, y, z});
            }
            void setRotation(const double x, const double y, const double z) {
                rotation = matrix::Vector3{x, y, z};
                computeRotationMatrix();
                computeCameraToWorldMatrix();
            }
            void setRotation(const matrix::Vector3& r) {
                setRotation(r[0], r[1], r[2]);
            }
            void rotate(const double x, const double y, const double z) {
                setRotation(rotation + matrix::Vector3{x, y, z});
            }
        public:
            float focal_length;
            float aperture; // Assume square aperture
            float near;
            float far;
            float fov;
            matrix::Vector3 position;
            matrix::Vector3 rotation;
            matrix::Matrix4x4 cameraToWorldMatrix;
            matrix::Matrix4x4 rotationMatrix;
            matrix::Matrix4x4 traslationMatrix;

            void computeRotationMatrix() {
                rotationMatrix = buildRotationMatrix(rotation[0], rotation[1], rotation[2]);
            }
            void computeTraslationMatrix() {
                traslationMatrix = buildTraslationMatrix(position[0], position[1], position[2]);
            }
            void computeCameraToWorldMatrix() {
                cameraToWorldMatrix = rotationMatrix * traslationMatrix;
            }
    };

    using Mesh = std::vector<matrix::Vector3>;

    const matrix::Vector3 transform(const matrix::Vector3& v, const matrix::Matrix4x4& m) {
        matrix::Vector4 transformed {};
        transformed = homogenize(v) * m;
        return matrix::Vector3{transformed[0], transformed[1], transformed[2]};
    }

    class Triangle {
        public:
            matrix::Vector3 a;
            matrix::Vector3 b;
            matrix::Vector3 c;
    };

    class Device {
        public:
            Device(Window& w, Camera c) : window {w}, camera {c} {
                aspectRatio = window.getSize().y/window.getSize().y;
            }

            void draw(const Mesh& mesh, const matrix::Matrix4x4 transformMatrix) {
                sf::VertexArray lines(sf::Lines, mesh.size());
                for (int i = 0; i < mesh.size(); ++i) {
                    lines[i].position = raster(transform(mesh[i], transformMatrix));
                }
                window.draw(lines);
            }

            void draw(const Triangle& triangle, const matrix::Matrix4x4& objectToWorldMatrix) {
                sf::VertexArray lines(sf::LineStrip, 3);
                matrix::Matrix4x4 transformMatrix = objectToWorldMatrix * camera.cameraToWorldMatrix;
                lines[0].position = raster(transform(triangle.a, transformMatrix));
                lines[1].position = raster(transform(triangle.b, transformMatrix));
                lines[2].position = raster(transform(triangle.c, transformMatrix));
                lines[3].position = lines[0].position;
                window.draw(lines);
            }

            Camera camera;

        private:
            Window& window;
            float aspectRatio;

            sf::Vector2f raster(matrix::Vector3 point) {
                matrix::Vector4 hpoint = normalize(homogenize(point) * camera.projectionMatrix());
                return sf::Vector2f((hpoint[0] + 1) * 0.5 * window.getSize().x,
                                    (1 - (hpoint[1] + 1) * 0.5) * window.getSize().y);
            }
    };

    class Poly {
        public:
            Poly() {
                move(0, 0, 0);
                rotate(0, 0, 0);
            }
            std::vector<Triangle> triangles;
            matrix::Vector3 position;
            matrix::Vector3 rotation;
            matrix::Matrix4x4 objectToWorldMatrix;
            void move(double x, double y, double z) {
                position = position + matrix::Vector3{x, y, z};
                traslationMatrix = computeTraslationMatrix();
                objectToWorldMatrix = computeObjectToWorldMatrix();
            }
            void setRotation(const matrix::Vector3& r) {
                rotation = r;
                rotationMatrix = computeRotationMatrix();
                objectToWorldMatrix = computeObjectToWorldMatrix();
            }
            void setRotation(double xrot, double yrot, double zrot) {
                setRotation(matrix::Vector3{xrot, yrot, zrot});
            }
            void rotate(double xrot, double yrot, double zrot) {
                setRotation(rotation + matrix::Vector3{xrot, yrot, zrot});
            }
             void draw(e3d::Device& dev) {
                for (auto triangle : triangles) {
                    dev.draw(triangle, objectToWorldMatrix);
                }
            }
        private:
            matrix::Matrix4x4 traslationMatrix;
            matrix::Matrix4x4 rotationMatrix;
            const matrix::Matrix4x4 computeTraslationMatrix() const {
                matrix::Matrix4x4 tm {};
                tm[0][0] = 1;
                tm[1][1] = 1;
                tm[2][2] = 1;
                tm[3][0] = position[0];
                tm[3][1] = position[1];
                tm[3][2] = position[2];
                tm[3][3] = 1;
                return tm;
            }
            const matrix::Matrix4x4 computeRotationMatrix() const {
                double xrot = rotation[0];
                double yrot = rotation[1];
                double zrot = rotation[2];
                matrix::Matrix4x4 xrm {};
                matrix::Matrix4x4 yrm {};
                matrix::Matrix4x4 zrm {};
                xrm[0][0] = 1;
                xrm[1][1] = cos(xrot);
                xrm[1][2] = -sin(xrot);
                xrm[2][1] = sin(xrot);
                xrm[2][2] = cos(xrot);
                xrm[3][3] = 1;
                yrm[0][0] = cos(yrot);
                yrm[0][2] = sin(yrot);
                yrm[1][1] = 1;
                yrm[2][0] = -sin(yrot);
                yrm[2][2] = cos(yrot);
                yrm[3][3] = 1;
                zrm[0][0] = cos(zrot);
                zrm[0][1] = -sin(zrot);
                zrm[1][0] = sin(zrot);
                zrm[1][1] = cos(zrot);
                zrm[2][2] = 1;
                zrm[3][3] = 1;
                return xrm * yrm * zrm;
            }
            const matrix::Matrix4x4 computeObjectToWorldMatrix() const {
                return rotationMatrix * traslationMatrix;
            }
     };

}

#endif // ENGINE3D_H_
