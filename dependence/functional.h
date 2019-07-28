/* 
Author https://github.com/prograholic 
Original https://github.com/prograholic/blog/blob/master/cxx_function/main.cpp
*/

#pragma once

#include "memory.h"

namespace dependence
{
	template <typename UnusedType>
    class function;
	
    template <typename ReturnType, typename ...ArgumentTypes>
    class function <ReturnType (ArgumentTypes ...)> {
        public:

            typedef ReturnType signature_type (ArgumentTypes ...);

            function()
                : mInvoker()
            { }


            template <typename functionT>
            /// Uncomment this, and add corresponding assignment operator for `check4`
            ///explicit
            function(functionT f)
                : mInvoker(new free_function_holder<functionT>(f))
            { }

            template <typename functionType, typename ClassType>
            function(functionType ClassType::* f)
                : mInvoker(new member_function_holder<functionType, ArgumentTypes ...>(f))
            { }


            function(const function & other)
                : mInvoker(other.mInvoker->clone())
            { }

            function & operator = (const function & other) {
                mInvoker = other.mInvoker->clone();
            }

            ReturnType operator ()(ArgumentTypes ... args) {
                return mInvoker->invoke(args ...);
            }


        private:

            class function_holder_base {
                public:
                    function_holder_base() { }

                    virtual ~function_holder_base() { }

                    virtual ReturnType invoke(ArgumentTypes ... args) = 0;

                    virtual shared_ptr<function_holder_base> clone() = 0;

                private:
                    function_holder_base(const function_holder_base & );
                    void operator = (const function_holder_base &);
            };

            using invoker_t = shared_ptr<function_holder_base>;

            template <typename functionT>
            class free_function_holder : public function_holder_base {
                public:

                    free_function_holder(functionT func)
                        : function_holder_base(),
                        mfunction(func)
                    { }

                    virtual ReturnType invoke(ArgumentTypes ... args) {
                        return mfunction(args ...);
                    }

                    virtual invoker_t clone() {
                        return invoker_t(new free_function_holder(mfunction));
                    }

                private:
                    functionT mfunction;
            };


            template <typename functionType, typename ClassType, typename ... RestArgumentTypes>
            class member_function_holder : public function_holder_base {
                public:

                    using member_function_signature_t = functionType ClassType::*;

                    member_function_holder(member_function_signature_t f)
                        : mfunction(f)
                    {}

                    virtual ReturnType invoke(ClassType obj, RestArgumentTypes ... restArgs)
                    {
                        return (obj.*mfunction)(restArgs ...);
                    }

                    virtual invoker_t clone()
                    {
                        return invoker_t(new member_function_holder(mfunction));
                    }

                private:
                    member_function_signature_t mfunction;
            };

            invoker_t mInvoker;
    };
} // namespace dependence

