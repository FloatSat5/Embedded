// Rishav (2022/05/16)

#ifndef _LANCZOS_EULER_H_
#define _LANCZOS_EULER_H_

#include "vector.h"
#include "matrix.h"

namespace lnz
{
    class Euler : public Vector<3>
    {
    public:
        Euler(const float (&array)[3]);
        Euler(const Euler &q);
        Euler(const Vector<3> &q);
        Euler();
        ~Euler() {}

        void operator=(const float (&array)[3]);
        void operator=(const Euler &q);

        Matrix<3, 3> get_dcm();
    };

    inline Euler::Euler(const float (&array)[3])
    {
        for (uint8_t n = 0; n < 3; n++)
        {
            V[n] = array[n];
        }
    }

    inline Euler::Euler(const Euler &q)
    {
        for (uint8_t n = 0; n < 3; n++)
        {
            V[n] = q.V[n];
        }
    }

    inline Euler::Euler(const Vector<3> &v)
    {
        for (uint8_t n = 0; n < 3; n++)
        {
            V[n] = v.V[n];
        }
    }

    inline Euler::Euler()
    {
        V[0] = 1;
        for (uint8_t n = 1; n < 3; n++)
        {
            V[n] = 0.0;
        }
    }

    inline void Euler::operator=(const float (&array)[3])
    {
        for (uint8_t n = 0; n < 3; n++)
        {
            V[n] = array[n];
        }
    }

    inline void Euler::operator=(const Euler &q)
    {
        for (uint8_t n = 0; n < 3; n++)
        {
            V[n] = q.V[n];
        }
    }

    inline Matrix<3, 3> Euler::get_dcm()
    {
        Matrix<3, 3> Q;
        float yaw = V[0];
        float pitch = V[1];
        float roll = V[2];

        float cy = cos(yaw);
        float cp = cos(pitch);
        float cr = cos(roll);
        float sy = sin(yaw);
        float sp = sin(pitch);
        float sr = sin(roll);

        Q.set(0, 0, cp * cy);
        Q.set(0, 1, cp * sy);
        Q.set(0, 2, -sp);
        Q.set(1, 0, sr * sp * cy - cr * sy);
        Q.set(1, 1, sr * sp * sy + cr * cy);
        Q.set(1, 2, sr * cp);
        Q.set(2, 0, cr * sp * cy + sr * sy);
        Q.set(2, 1, cr * sp * sy - sr * cy);
        Q.set(2, 2, cr * cp);

        return Q;
    }
}

#endif // euler.h
