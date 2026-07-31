#pragma once

#include <appstate.h>
#include <bone.h>
#include <raylib.h>
#include <sprite.h>

#include <cfloat>
#include <cmath>

namespace SIK {
namespace Math {
Vector2 V2Sub(Vector2 a, Vector2 b);
Vector2 V2Add(Vector2 a, Vector2 b);
Vector2 V2Scale(Vector2 a, float s);
float V2Len(Vector2 a);
Vector2 V2Norm(Vector2 a);

Vector2 V2Rotate(Vector2 v, float rad);
float NormalizeAngle(float a);

float PointSegmentDist(Vector2 p, Vector2 a, Vector2 b, float* outT = nullptr);

Vector2 WorldToLocalOfParent(AppState& state, Vector2 worldPoint,
                             int parentIdx);
bool HitTestBody(AppState& state, Vector2 worldPoint, float boneRadius,
                 SelectionType& outType, int& outIndex);

void UpdateSkeleton(AppState& state);
void SolveIK(AppState& state, int effectorIdx, Vector2 target,
             int iterations = 1);

}  // namespace Math
}  // namespace SIK