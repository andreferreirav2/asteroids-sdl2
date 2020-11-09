#pragma once
#include "../../engine/ecs/Component.h"

// Attach to Entities that are supposed to flip boundaries (reach the end and appear on the other side)
struct Boundless : public Component {};