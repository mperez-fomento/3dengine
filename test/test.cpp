#include <catch2/catch_test_macros.hpp>
#include "matrix.hpp"
#include "engine3d.hpp"

TEST_CASE("Rows can be checked for equality", "[columns]") {
    matrix::Row<3> c {1, 2, 3};
    matrix::Row<3> d {4, 5, 6};
    matrix::Row<3> f {1, 2, 3};
    REQUIRE(c == c);
    REQUIRE(c == f);
    REQUIRE(c != d);
}

TEST_CASE("Rows can be multiplied by scalars", "[columns]") {
    matrix::Row<3> a {1, 2, 3};
    REQUIRE(2*a == matrix::Row<3>{2, 4, 6});
    REQUIRE(0.5*a == matrix::Row<3>{0.5, 1, 1.5});
}

TEST_CASE("Rows can be added", "[columns]") {
    matrix::Row<3> a {1, 2, 3};
    matrix::Row<3> b {4, 5, 6};
    REQUIRE(a + b == matrix::Row<3>{5, 7, 9});
    REQUIRE(a - b == matrix::Row<3>{-3, -3, -3});
}

TEST_CASE("Dot product of two columns is a scalar", "[columns]") {
    matrix::Row<3> a {1, 2, 3};
    matrix::Row<3> b {4, 5, 6};
    REQUIRE(a*b == 32);
}

TEST_CASE("Rows can be negated", "[columns]") {
    matrix::Row<3> a {1, 2, 3};
    REQUIRE(-a == matrix::Row<3>{-1, -2, -3});
}

TEST_CASE("Matrices can be multiplied by a scalar", "[matrix]") {
    matrix::Matrix<3, 3> a {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    REQUIRE(2*a == matrix::Matrix<3, 3>{{{2, 4, 6}, {8, 10, 12}, {14, 16, 18}}});
}

TEST_CASE("Matrices can be added", "[matrix]") {
    matrix::Matrix<2, 2> a {{{1, 2}, {4, 5}}};
    matrix::Matrix<2, 2> b {{{1, 1}, {1, 1}}};
    REQUIRE(a + b == matrix::Matrix<2, 2>{{{2, 3}, {5, 6}}});
}

TEST_CASE("Matrices can be multiplied", "[matrix]") {
    matrix::Matrix<2, 2> a {{{1, 2}, {3, 4}}};
    matrix::Matrix<2, 2> b {{{1, 1}, {1, 1}}};
    matrix::Matrix<2, 3> c {{{1, 2, 3}, {4, 5, 6}}};
    REQUIRE(a * b == matrix::Matrix<2, 2>{{{3, 3}, {7, 7}}});
    REQUIRE(b * a == matrix::Matrix<2, 2>{{{4, 6}, {4, 6}}});
    REQUIRE(b * c == matrix::Matrix<2, 3>{{{5, 7, 9}, {5, 7, 9}}});
}

TEST_CASE("Matrices can be transposed", "[matrix]") {
    matrix::Matrix<2, 2> a {{{1, 2}, {3, 4}}};
    REQUIRE(transpose(a) == matrix::Matrix<2, 2>{{{1, 3}, {2, 4}}});
}

TEST_CASE("Rows can be multiplied with matrices", "[row],[matrix]") {
    matrix::Row<3> r {1, 2, 3};
    matrix::Matrix<3, 3> m {{{1, 1, 1}, {1, 0, 1}, {-1, 0, 1}}};
    REQUIRE(r * m == matrix::Row<3>{0, 1, 6});
}

TEST_CASE("Identity matrix times any matrix or row is that matrix or row", "[matrix]") {
    matrix::Matrix<3, 3> m {{{1, 2, 3}, {4, 5, 6}, {7, 8, 9}}};
    matrix::Row<3> r {1, 2 ,3};
    REQUIRE(matrix::I<3>() * m == m);
    REQUIRE(m * matrix::I<3>() == m);
    REQUIRE(r * matrix::I<3>() == r);
}

TEST_CASE("Homegeneous vectors can be normalized", "[row]") {
    matrix::Vector4 v {2, 4, 6, 2};
    matrix::Vector4 w {2, 4, 6, 0};
    matrix::Vector4 x {2, 4, 6, 1};
    REQUIRE(normalize(v) == matrix::Vector4{1, 2, 3, 1});
    REQUIRE(normalize(w) == matrix::Vector4{2, 4, 6, 0});
    REQUIRE(normalize(x) == matrix::Vector4{2, 4, 6, 1});
}

TEST_CASE("3D vectors can be transformed into homogeneous coordinate vectors", "[row]") {
    matrix::Vector3 v {1, 2, 3};
    REQUIRE(homogenize(v) == matrix::Vector4{1, 2, 3, 1});
}

TEST_CASE("Object-to-World matrix is correctly computed", "[poly]") {
    e3d::Poly frontTri;
    frontTri.triangles.push_back({{-1, -1, 0}, {0, 1, 0}, {1, -1, 0}});
    e3d::Poly sideTri;
    sideTri.triangles.push_back({{0, -1, -1}, {0, 1, 0}, {0, -1, -1}});
    e3d::Poly horTri;
    horTri.triangles.push_back({{-1, 0, -1}, {0, 0, 1}, {1, 0, -1}});

    SECTION("Without moving or rotating, o2w matrix is the identity matrix") {
        REQUIRE(frontTri.objectToWorldMatrix - matrix::I<4>() == 2*matrix::I<4>());
    }
}
