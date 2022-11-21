#include <iostream>
#include "../math/matrix4.h"
#include "../math/vector3.h"
#include "../math/quaternion.h"

void VectorTest()
{
    Vector3 test(1, 2, 3);
}

void matrixTest()
{

    Matrix4 x(2, 0, 0, 0,
              0, 3, 0, 0,
              0, 2, 3, 0,
              0, 0, 0, 4);
    Matrix4 y(3, 2, 0, 0,
              0, 0, 1, 0,
              0, 2, 2, 0,
              3, 0, 3, 0);

    Matrix4 c = x * y;
    Matrix4 t(3, 0, 1, 0,
              0, 4, 0, 3,
              3, 0, 3, 0,
              0, 2, 0, 2);
    Vector3 v(1, 2, 3);
    Vector4 v4 = t * Vector4(1, 2, 3, 1);
    Vector3 p = transformPoint(t, v);
    Vector3 v1 = transformVector(t, v);
    Matrix4 z = transposed(x);

    Matrix4 z1 = Matrix4(
        x.xx, x.yx, x.zx, x.tx,
        x.xy, x.yy, x.zy, x.ty,
        x.xz, x.yz, x.zz, x.tz,
        x.xw, x.yw, x.zw, x.tw);
    Matrix4 inver = inverse(t);
}

void quaternionTest()
{
    Quaternion q(1, 2, 3, 4);
}

int main(int, char **)
{
    std::cout << "Hello, world!\n";
    VectorTest();
    matrixTest();
    quaternionTest();
    return 0;
}