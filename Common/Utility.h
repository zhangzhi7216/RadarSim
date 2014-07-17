#pragma once

namespace Utility
{
    __declspec(dllexport) double Distance(const Position &rel);
    __declspec(dllexport) double Theta(const Position &rel);
    __declspec(dllexport) double Phi(const Position &rel);
    __declspec(dllexport) double Distance(const Position &src, const Position &dst);
    __declspec(dllexport) double Theta(const Position &src, const Position &dst);
    __declspec(dllexport) double Phi(const Position &src, const Position &dst);
    __declspec(dllexport) Position Rel(double dis, double theta, double phi);

    __declspec(dllexport) double WhiteNoise(double value, double var);
    __declspec(dllexport) double ColorNoise(double value, double var);
    __declspec(dllexport) double MultNoise(double value, double var);
};
