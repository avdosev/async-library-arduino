#include "kypluk/kyplukSmartPtr.h"

namespace dependence {

    template<class ptr_t>
    using shared_ptr = kypluk::SharedPtr<ptr_t>;

    template<class ptr_t>
    using unique_ptr = kypluk::UniquePtr<ptr_t>;

}