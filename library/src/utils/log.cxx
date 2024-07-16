#include <ranges>
#include <string>
#include <sstream>
#include <utility>
#include <filesystem>
#include <string_view>

#include <boost/format.hpp>

#include <p5/lambda/utils/log.hxx>
#include <p5/lambda/utils/generator.hxx>


namespace p5::lambda::utils::log {
namespace raii {
namespace context {

void dispatch(this_::Type &context) noexcept(true) {
    auto * const target_ = ::std::exchange(context.target, nullptr);
    if (! target_) return;
    target_->write(parent_::parent_::Message{
        .text = parent_::text::make(context.text),
        .level = context.level, .location = ::std::move(context.location)
    });
}

} // namespace context

this_::Type::~Type() noexcept(true) {
    if (context_.text) this_::context::dispatch(context_);
}

this_::Type::Type(Context &&context) noexcept(false): context_{[&context] {
    if (! context.target) throw ::std::invalid_argument{"empty target"};
    if (context.text) throw ::std::invalid_argument{"text not empty"};
    return ::std::move(context);
} ()} {}

} // namespace raii

namespace message {
namespace text {

// namespace walker::generator {
// namespace private_ {

// namespace parent_ = this_;
// namespace this_ = parent_::private_;

// using parent_::Type;
// using parent_::parent_::Source;
// using parent_::parent_::parent_::Chunk;

// inline static Type<Type<Chunk const>> common(
//     Source const &source
// ) noexcept(true) {
//     auto &&state_ = [&source_ = source] {
//         auto const * const pointer_ = [&source_] () -> ::std::decay_t<
//             decltype(source_())
//         > { return source_ ? source_() : nullptr; } ();
//         struct Result_ final {
//             ::std::decay_t<
//                 decltype(::std::end(pointer_->reference))
//             > end = {};
//             ::std::decay_t<
//                 decltype(::std::begin(pointer_->reference))
//             > begin = {};
//             ::std::decay_t<decltype(*begin)> skip = 0;
//             ::std::decay_t<decltype(pointer_)> pointer = nullptr;
//             ::std::decay_t<decltype(*pointer_)> holder = {};
//         };
//         if (pointer_) return Result_ {
//             .end = ::std::end(pointer_->reference),
//             .begin = ::std::begin(pointer_->reference),
//             .pointer = pointer_
//         };
//         return Result_{};
//     } ();
//     using Line_ = Type<Chunk const>;
//     return [
//         state_ = ::std::move(state_), &source_ = source, current_ = Line_{}
//     ] () mutable -> Line_ * {
//         if (! state_.pointer) return nullptr;
//         if (! current_) current_ = [&state_, &source_] () -> Chunk const * {
//             while (state_.end == state_.begin) {
//                 state_.pointer = source_();
//                 if (! state_.pointer) return nullptr;
//                 state_.end = ::std::end(state_.pointer->reference);
//                 state_.begin = ::std::begin(state_.pointer->reference);
//                 if (0 != state_.skip) {
//                     auto const skip_ = state_.skip;
//                     state_.skip = 0;
//                     if (skip_ == *(state_.begin)) state_.begin++;
//                     return nullptr;
//                 }
//                 if (0 == *(state_.begin)) {
//                     state_.end = state_.begin;
//                     continue;
//                 }
//             }
//             auto iterator_ = state_.begin;
//             do {
//                 if (0x00 == *iterator_) break;
//                 if (0x0a == *iterator_) { state_.skip = 0x0d; break; }
//                 if (0x0d == *iterator_) { state_.skip = 0x0a; break; }
//             } while (state_.end != (++iterator_));
//             if (state_.end == iterator_) {
//                 state_.begin = iterator_;
//                 return state_.pointer;
//             }
//             state_.holder.reference = {state_.begin, iterator_};
//             state_.holder.terminated = (0 == state_.skip);
//             if (state_.holder.terminated) state_.end = state_.begin = iterator_;
//             return &(state_.holder);
//         };
//         return &current_;
//     };
// }

// } // namespace private_

// Type<Factory<Line::Copy>::Line> Factory<Line::Copy>::operator () (
//     Source const &source
// ) const noexcept(true) {
//     auto &&source_ = private_::common(source);
//     return [
//         line_ = source_(),
//         source_ = ::std::move(source_),
//         current_ = ::std::string{}
//     ] () mutable -> ::std::string * {
//         if (! line_) return nullptr;
//         ::std::ostringstream stream_;
//         while (
//             auto const * const chunk_ = (*line_)()
//         ) stream_ << chunk_->reference;
//         line_ = source_();
//         current_ = ::std::move(stream_.str());
//         return &current_;
//     };
// }

// Type<Factory<Line::Chunk>::Line> Factory<Line::Chunk>::operator () (
//     Source const &source
// ) const noexcept(true) {
//     for (auto &line_: private_::common(source)) {
//         auto const end_ = ::std::end(line_);
//         auto iterator_ = ::std::begin(line_);
//         using Line_ = ::std::decay_t<decltype(*iterator_)>;
//         if (end_ == iterator_) { co_yield Line_{}; continue; }
//         auto &first_ = *iterator_;
//         if (end_ == (++iterator_)) {
//             if ((! terminate) || (first_.terminated)) {
//                 co_yield first_;
//                 continue;
//             }
//             auto buffer_ = ::std::string{first_.reference};
//             co_yield Line_{buffer_};
//             continue;
//         }
//         ::std::ostringstream buffer_;
//         buffer_ << first_.reference;
//         do buffer_ << (*iterator_).reference; while (end_ != (++iterator_));
//         co_yield Line_{buffer_.str()};
//     }
// }

// Type<Factory<Line::Chunks>::Line> Factory<Line::Chunks>::operator () (
//     Source const &source
// ) const noexcept(true) {
//     auto source_ = private_::common(source);
//     if (! terminate) return source_;
//     return [] (auto &source) -> ::std::decay_t<decltype(source_)> {
//         for (auto &line_: source) co_yield [
//             &source_ = line_
//         ] () -> ::std::decay_t<decltype(line_)> {
//             for (auto &chunk_: source_) {
//                 auto const &reference_ = chunk_.reference;
//                 if (chunk_.terminated) co_yield chunk_;
//                 else co_yield ::std::decay_t<decltype(chunk_)>{
//                     .reference = ::std::string{reference_}, .terminated = true
//                 };
//             }
//         } ();
//     } (source_);
// }
// } // namespace walker::generator

namespace driver {
namespace interface {

this_::Type::~Type() noexcept(true) {}

this_::Type::Type() noexcept(true) {}
this_::Type::Type(Type &&) noexcept(true) {}
this_::Type & this_::Type::operator = (Type &&) noexcept(true) { return *this; }

} // namespace interface
} // namespace driver
} // namespace text

namespace location {

::std::string this_::Type::to_string(
    ::std::string_view const &root
) const noexcept(false) {
    ::std::ostringstream stream_;
    using Text_ = ::std::decay_t<decltype(stream_.str())>;
    using Generator_ = parent_::parent_::parent_::generator::Coroutine<Text_>;
    ::std::ranges::move([this, &root] () -> Generator_ {
        auto text_ = Text_{function};
        if (! text_.empty()) co_yield ::std::move(text_);
        if ((text_ = file).empty()) co_return;
        if (! (root.empty() || (0 == root.at(0)))) {
            text_ = ::std::filesystem::relative(text_, root).string();
            if (text_.empty()) co_return;
        }
        if (0 < line) text_ = (::boost::format("%s#%u") % text_ % line).str();
        co_yield Text_{"|"};
        co_yield text_;
    } (), ::std::ostream_iterator<Text_>{stream_});
    return stream_.str();
}

} // namespace location
} // namespace message

namespace interface {

this_::Type::RAII this_::Type::write(
    Message::Level level, FinalToken_ &&, Message::Location &&location
) const noexcept(true) {
    auto context_ = this_::RAII::Context{.level = level, .target = this};
    if (Message::Level::Info < level) context_.location = ::std::move(location);
    return this_::RAII{::std::move(context_)};
}

this_::Type::Type::~Type() noexcept(true) = default;

} // namespace interface

void this_::Type::write_(Message &&message) const noexcept(true) {
    if (! handler) return;
    if (message.location && message::location::unknown(
        *(message.location)
    )) message.location.reset();
    try { handler(::std::move(message)); } catch(...) {}
}

} // namespace p5::lambda::utils::log
