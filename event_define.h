#pragma once

/*
    !!! if you has realy stl !!!
    замени нижние дефайны и все будет работать, к тому же скорее всего гораздо стабильнее и быстрее
*/

#include "dependence/functional.h"
#include "dependence/memory.h"

class Event;

using event_id_t = uint32_t;
using event_t = dependence::shared_ptr<Event>;
using callback_t = dependence::function<void(void)>;

// std::pair<event_id_t, event_t>
using event_pair = struct {
    event_id_t first;
    event_t second;
};