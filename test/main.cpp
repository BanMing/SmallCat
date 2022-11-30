#include <iostream>
#include "../math/matrix4.h"
#include "../math/vector3.h"
#include "../math/quaternion.h"
#include "../math/transform.h"

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

void transformTest()
{
    // https://www.andre-gaschler.com/rotationconverter/
    Vector3 scale(1.0f, 2.0f, 3.0f);
    Vector3 position(-10.8f, 0.0f, 5.3f);
    Quaternion rotation(0.2f, 0.2f, 0.0f, 1.0f);

    // First, extract the rotation basis of the transform
    Vector3 x = rotation * Vector3(1, 0, 0);
    Vector3 y = rotation * Vector3(0, 1, 0);
    Vector3 z = rotation * Vector3(0, 0, 1);

    // Next, scale the basis vectors
    x = x * scale.x;
    y = y * scale.y;
    z = z * scale.z;

    // Extract the position of the transform
    Vector3 p = position;

    // Create matrix
    Matrix4 oldMat4 = Matrix4(
        x.x, x.y, x.z, 0,  // X basis (& Scale)
        y.x, y.y, y.z, 0,  // Y basis (& scale)
        z.x, z.y, z.z, 0,  // Z basis (& scale)
        p.x, p.y, p.z, 1); // Position

    Transform trans(position, rotation, scale);
    
    Matrix4 transMat4 = transformToMat4(trans);
}
int main(int, char **)
{
    std::cout << "Hello, world!\n";
    VectorTest();
    matrixTest();
    quaternionTest();
    transformTest();
    return 0;
}