#include <acheron/engine/math/quaternion.h>

#include <math.h>

#define QUAT_INDEX(y, x) (y*4+x)

// http://mathinfo.univ-reims.fr/IMG/pdf/Rotating_Objects_Using_Quaternions.pdf

void quaternion_from_mat4(quaternion_t *quat, mat4_t *mat) {
    int nxt[] = {1, 2, 0};
    
    float tr = mat->val[QUAT_INDEX(0, 0)] + mat->val[QUAT_INDEX(1, 1)] + mat->val[QUAT_INDEX(2, 2)];
    // check diagonal
    if (tr > 0.0f) {
        float s = sqrt(tr+1.0f);
        quat->w = s/2.0f;
        s = 0.5f/s;
        quat->x = (mat->val[QUAT_INDEX(1, 2)]-mat->val[QUAT_INDEX(2, 1)]) * s;
        quat->y = (mat->val[QUAT_INDEX(2, 0)]-mat->val[QUAT_INDEX(0, 2)]) * s;
        quat->z = (mat->val[QUAT_INDEX(0, 1)]-mat->val[QUAT_INDEX(1, 0)]) * s;
    }
    else {
        float q[4];
        // negative diagonal
        int i = 0;
        if (mat->val[QUAT_INDEX(1, 1)] > mat->val[QUAT_INDEX(0, 0)])
            i = 1;
        if (mat->val[QUAT_INDEX(2, 2)] > mat->val[QUAT_INDEX(i, i)])
            i = 2;
        int j, k;
        j = nxt[i];
        k = nxt[j];
        float s = sqrt(mat->val[QUAT_INDEX(i, i)]-(mat->val[QUAT_INDEX(j, j)]+mat->val[QUAT_INDEX(k, k)])+1.0f);
        q[i] = s*0.5f;
        if (s != 0.0f)
            s = 0.5f/s;
        q[3] = (mat->val[QUAT_INDEX(j, k)]-mat->val[QUAT_INDEX(k, j)])*s;
        q[j] = (mat->val[QUAT_INDEX(i, j)]-mat->val[QUAT_INDEX(j, i)])*s;
        q[k] = (mat->val[QUAT_INDEX(i, k)]-mat->val[QUAT_INDEX(k, i)])*s;
        quat->x = q[0];
        quat->y = q[1];
        quat->z = q[2];
        quat->w = q[3];
    }
}

void quaternion_to_mat4(quaternion_t *quat, mat4_t *mat) {
    float xx, xy, xz;
    float yy, yz, zz;
    float wx, wy, wz;
    float x2, y2;
    float z2;
    
    x2 = quat->x + quat->x; y2 = quat->y + quat->y;
    z2 = quat->z + quat->z;
    xx = quat->x * x2; xy = quat->x * y2; xz = quat->x * z2;
    yy = quat->y * y2; yz = quat->y * z2; zz = quat->z * z2;
    wx = quat->w * x2; wy = quat->w * y2; wz = quat->w * z2;
    
    
    mat->val[QUAT_INDEX(0, 0)] = 1.0f-(yy+zz); mat->val[QUAT_INDEX(1, 0)] = xy-wz;
    mat->val[QUAT_INDEX(2, 0)] = xz + wy; mat->val[QUAT_INDEX(3, 0)] = 0.0; 
    mat->val[QUAT_INDEX(0, 1)] = xy + wz; mat->val[QUAT_INDEX(1, 1)] = 1.0 - (xx + zz); 
    mat->val[QUAT_INDEX(2, 1)] = yz - wx; mat->val[QUAT_INDEX(3, 1)] = 0.0;
    mat->val[QUAT_INDEX(0, 2)] = xz - wy; mat->val[QUAT_INDEX(1, 2)] = yz + wx; 
    mat->val[QUAT_INDEX(2, 2)] = 1.0 - (xx + yy); mat->val[QUAT_INDEX(3, 2)] = 0.0;
    
    mat->val[QUAT_INDEX(0, 3)] = 0; mat->val[QUAT_INDEX(1, 3)] = 0; 
    mat->val[QUAT_INDEX(2, 3)] = 0; mat->val[QUAT_INDEX(3, 3)] = 1;
}

void quaternion_convert_from_euler(quaternion_t *quat, ar_vector3f_t euler) {
    float cr, cp, cy, sr, sp, sy, cpcy, spsy;
    float roll = euler.x;
    float pitch = euler.y;
    float yaw = euler.z;
    
    cr = cos(roll/2);
    cp = cos(pitch/2);
    cy = cos(yaw/2);
    
    sr = sin(roll/2);
    sp = sin(pitch/2);
    sy = sin(yaw/2);
    
    cpcy = cp*cy;
    spsy = sp*sy;
    
    quat->w = cr*cpcy+sr*spsy;
    quat->x = sr*cpcy-cr*spsy;
    quat->y = cr*sp*cy+sr*cp*sy;
    quat->z = cr*cp*sy-sr*sp*cy;
}

quaternion_t quaternion_mul(quaternion_t *q0, quaternion_t *q1) {
    quaternion_t res;
    float a, b, c, d, e, f, g, h;
    a = (q0->w+q0->x)*(q1->w+q1->x);
    b = (q0->z-q0->y)*(q1->y+q1->z);
    c = (q0->w-q0->x)*(q1->y+q1->z);
    d = (q0->y-q0->z)*(q1->w+q1->x);
    e = (q0->x-q0->z)*(q1->x+q1->y);
    f = (q0->x-q0->z)*(q1->x+q1->y);
    g = (q0->w-q0->y)*(q1->w+q1->z);
    h = (q0->w-q0->y)*(q1->w+q1->z);
    
    res.w = b+(-e-f+g+h)/2;
    res.x = a-(e+f+g+h)/2;
    res.y = c+(e-f+g-h)/2;
    res.z = d+(e-f-g+h)/2;
    
    return res;
}

