#include "Utility.h"

#include <math.h>

namespace Utility
{
    double Distance(const Position &src, const Position &dst)
    {
        return sqrt(pow(dst.X - src.X, 2) + pow(dst.Y - src.Y, 2) + pow(dst.Y - src.Y, 2));
    }

    double Theta(const Position &src, const Position &dst)
    {
        Position rel = dst - src;
        if (rel.Z == 0)
        return atan(0.0);
    }

    double Phi(const Position &src, const Position &dst)
    {
        return sqrt(pow(dst.X - src.X, 2) + pow(dst.Y - src.Y, 2) + pow(dst.Y - src.Y, 2));
    }
};