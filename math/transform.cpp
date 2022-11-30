#include "transform.h"

Matrix4 transformToMat4(const Transform &_trans)
{
    Matrix4 res;

    Matrix4 rotationMat4 = quatToMat4(_trans.rotation);
    res.cloumn[0].x = rotationMat4.cloumn[0].x * _trans.scale.x;
    res.cloumn[0].y = rotationMat4.cloumn[0].y * _trans.scale.y;
    res.cloumn[0].z = rotationMat4.cloumn[0].z * _trans.scale.z;
    res.cloumn[0].w = _trans.position.x;

    res.cloumn[1].x = rotationMat4.cloumn[1].x * _trans.scale.x;
    res.cloumn[1].y = rotationMat4.cloumn[1].y * _trans.scale.y;
    res.cloumn[1].z = rotationMat4.cloumn[1].z * _trans.scale.z;
    res.cloumn[1].w = _trans.position.y;

    res.cloumn[2].x = rotationMat4.cloumn[2].x * _trans.scale.x;
    res.cloumn[2].y = rotationMat4.cloumn[2].y * _trans.scale.y;
    res.cloumn[2].z = rotationMat4.cloumn[2].z * _trans.scale.z;
    res.cloumn[2].w = _trans.position.z;

    res.cloumn[3].x = 0;
    res.cloumn[3].y = 0;
    res.cloumn[3].z = 0;
    res.cloumn[3].w = 1;
    return res;
}