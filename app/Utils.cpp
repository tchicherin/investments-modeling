#include "Utils.h"
#include <random>

std::mt19937 Rng::g_{ std::random_device{}() };
std::once_flag Rng::init_flag_;
