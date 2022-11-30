#include "transform.h"

Matrix4 transformToMat4(const Transform &_trans)
{
    // M = SRT
    Matrix4 res;

    Matrix4 rotationMat4 = quatToMat4(_trans.rotation);
    Matrix4 scaleMat4(_trans.scale.x, 0, 0, 0,
                      0, _trans.scale.y, 0, 0,
                      0, 0, _trans.scale.z, 0,
                      0, 0, 0, 1);
    Matrix4 translateMat4(1, 0, 0, 0,
                          0, 1, 0, 0,
                          0, 0, 1, 0,
                          _trans.position.x, _trans.position.y, _trans.position.z, 1);
    res = translateMat4 * rotationMat4 * scaleMat4;
    Matrix4 res1 = scaleMat4 * rotationMat4 * translateMat4;

    // res.cloumn[0].x = rotationMat4.cloumn[0].x * _trans.scale.x;
    // res.cloumn[0].y = rotationMat4.cloumn[0].y * _trans.scale.y;
    // res.cloumn[0].z = rotationMat4.cloumn[0].z * _trans.scale.z;
    // res.cloumn[0].w = 0;

    // res.cloumn[1].x = rotationMat4.cloumn[1].x * _trans.scale.x;
    // res.cloumn[1].y = rotationMat4.cloumn[1].y * _trans.scale.y;
    // res.cloumn[1].z = rotationMat4.cloumn[1].z * _trans.scale.z;
    // res.cloumn[1].w = 0;

    // res.cloumn[2].x = rotationMat4.cloumn[2].x * _trans.scale.x;
    // res.cloumn[2].y = rotationMat4.cloumn[2].y * _trans.scale.y;
    // res.cloumn[2].z = rotationMat4.cloumn[2].z * _trans.scale.z;
    // res.cloumn[2].w = 0;

    // res.cloumn[3].x = _trans.position.x;
    // res.cloumn[3].y = _trans.position.y;
    // res.cloumn[3].z = _trans.position.z;
    // res.cloumn[3].w = 1;
    return res;
}