#ifndef VSERROR_H
#define VSERROR_H

#include <variant>
#include <system_error>
#include <optional>
#include <functional>
#include <type_traits>
#include <cassert>

/*Common interface to represent "result or error".
Type tc::err::Result and corresponding functions and meta-functions can be changed
if there is more suitable class than std::variant.
Can throw or simulate undefined behavior.
*/
namespace tc::err
{
    template<typename Value, typename Error = std::error_code>
    using Result = std::variant<Value, Error>;

    template<typename Value, typename Error>
    bool isError(const Result<Value, Error>& result) {
        return std::holds_alternative<Error>(result);
    }

    template<typename Result>
    using Value = std::variant_alternative_t<0, Result>;
    template<typename Result>
    using Error = std::variant_alternative_t<1, Result>;

    namespace ex
    {
        template<typename Value, typename Error>
        const Value& value(const Result<Value, Error>& result) {
            return std::get<Value>(result);
        }

        template<typename Value, typename Error>
        Value& value(Result<Value, Error>& result) {
            return std::get<Value>(result);
        }

        template<typename Value, typename Error>
        const Error& error(const Result<Value, Error>& result) {
            return std::get<Error>(result);
        }

        template<typename Value, typename Error>
        Error& error(Result<Value, Error>& result) {
            return std::get<Error>(result);
        }
    }

    inline namespace ub
    {
        template<typename Value, typename Error>
        const Value& value(const Result<Value, Error>& result)
        {
            std::optional<std::reference_wrapper<const Value>> res;
            try {
                res = std::get<Value>(result);
            }
            catch(...) {
                assert(false);
            }
            return res->get();
        }

        template<typename Value, typename Error>
        Value& value(Result<Value, Error>& result)
        {
            return const_cast<Value&>(value(static_cast<const Result<Value, Error>&>(result)));
        }

        template<typename Value, typename Error>
        const Error& error(const Result<Value, Error>& result)
        {
            std::optional<std::reference_wrapper<const Error>> res;
            try {
                res = std::get<Error>(result);
            }
            catch(...) {
                assert(false);
            }
            return res->get();
        }

        template<typename Value, typename Error>
        Error& error(Result<Value, Error>& result) {
            return const_cast<Error&>(error(static_cast<const Result<Value, Error>&>(result)));
        }
    }
}

#endif