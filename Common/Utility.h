#pragma once

namespace Utility
{
    double Distance(const Position &rel);
    double Theta(const Position &rel);
    double Phi(const Position &rel);
    double Distance(const Position &src, const Position &dst);
    double Theta(const Position &src, const Position &dst);
    double Phi(const Position &src, const Position &dst);

    double WhiteNoise(double value, double var);
};
