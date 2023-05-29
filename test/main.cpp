#include <iostream>
#include <cmath>
#include "../math/matrix4.h"
#include "../math/vector3.h"
#include "../math/quaternion.h"
#include "../math/transform.h"
#include "../animation/animation_clip.h"
#include "../gltf/gltf_loader.h"

void VectorTest()
{
    Vector3 a(1, 2, 3);
    Vector3 b(2, 3, 4);
    float dotValue = dot(a, b);
    Vector3 AcrossB = cross(a, b);
    Vector3 BcrossA = cross(b, a);
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
    Matrix4 rotationMat4 = quatToMat4(rotation);
    Quaternion mat4Quat = mat4ToQuat(rotationMat4);

    Matrix4 transMat4 = transformToMat4(trans);
    Quaternion mat4Quat1 = mat4ToQuat(transMat4);
    Transform mat4Trans = mat4ToTransform(transMat4);
}

void gltfTest()
{
    // cgltf_data *data = loadGLTFFile("../assets/test.gltf");
    cgltf_data *data = loadGLTFFile("../assets/Woman.gltf");
    std::vector<AnimationClip> clips = loadAnimationClips(data);
    printf("Clips Size: %zu\n", clips.size());
    for (size_t i = 0; i < clips.size(); i++)
    {
        printf("clip name: %s looping: %d\n", clips[i].getName().c_str(), clips[i].isLooping());
        // for (size_t j = 0; j < clips[i].size(); j++)
        // {
        //     AnimatedJoint &joint = clips[i][j];
        //     printf("    joint name:%s id:%zu\n", joint.m_jointName.c_str(), joint.m_jointID);
        // }
    }

    freeGLTFFile(data);
}

int main(int, char **)
{
    std::cout << "Hello, world!\n";
    // VectorTest();
    // matrixTest();
    // quaternionTest();
    // transformTest();
    gltfTest();
    // std::cout << fmod(3, 2) << std::endl;
    // std::cout << fmod(3.2, 2.1) << std::endl;
    // std::cout << fmod(33.2, 2.1) << std::endl;
    return 0;
}