#include "smart_ptr.h"

namespace dependence {

    template<class ptr_t>
    using shared_ptr = kypluk::reference_counter<ptr_t>;

    template<class ptr_t>
    using unique_ptr = kypluk::unique_ptr<ptr_t>;

}