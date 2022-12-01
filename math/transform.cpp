#include "transform.h"

Matrix4 transformToMat4(const Transform &_trans)
{
    // M = SRT

    // Matrix4 rotationMat4 = quatToMat4(_trans.rotation);

    // Matrix4 scaleMat4(_trans.scale.x, 0, 0, 0,
    //                   0, _trans.scale.y, 0, 0,
    //                   0, 0, _trans.scale.z, 0,
    //                   0, 0, 0, 1);

    // Matrix4 translateMat4(1, 0, 0, 0,
    //                       0, 1, 0, 0,
    //                       0, 0, 1, 0,
    //                       _trans.position.x, _trans.position.y, _trans.position.z, 1);

    // res = translateMat4 * rotationMat4 * scaleMat4;

    const float x = _trans.rotation.x;
    const float y = _trans.rotation.y;
    const float z = _trans.rotation.z;
    const float w = _trans.rotation.w;
    const float x2 = x + x;
    const float y2 = y + y;
    const float z2 = z + z;

    const float xx = x * x2;
    const float xy = x * y2;
    const float xz = x * z2;
    const float yy = y * y2;
    const float yz = y * z2;
    const float zz = z * z2;
    const float wx = w * x2;
    const float wy = w * y2;
    const float wz = w * z2;
    
    const float sx = _trans.scale.x;
    const float sy = _trans.scale.y;
    const float sz = _trans.scale.z;

    Matrix4 res;
    res.m[0] = (1 - (yy + zz)) * sx;
    res.m[1] = (xy + wz) * sx;
    res.m[2] = (xz - wy) * sx;
    res.m[3] = 0;
    res.m[4] = (xy - wz) * sy;
    res.m[5] = (1 - (xx + zz)) * sy;
    res.m[6] = (yz + wx) * sy;
    res.m[7] = 0;
    res.m[8] = (xz + wy) * sz;
    res.m[9] = (yz - wx) * sz;
    res.m[10] = (1 - (xx + yy)) * sz;
    res.m[11] = 0;
    res.m[12] = _trans.position.x;
    res.m[13] = _trans.position.y;
    res.m[14] = _trans.position.z;
    res.m[15] = 1;
    return res;
}