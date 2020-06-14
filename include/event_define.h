#pragma once

#include "dependence/functional.h"
#include "dependence/memory.h"
#include <cstdint>

class Event;

using event_id_t = uint32_t;
using event_t = dependence::shared_ptr<Event>;
using callback_t = dependence::function<void(void)>;
using tiker_t = dependence::function<uint32_t(void)>;

using event_pair = struct {
    event_id_t first;
    event_t second;
};