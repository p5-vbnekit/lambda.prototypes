#include <list>
#include <utility>
#include <filesystem>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <p5/lambda/utils/log.hxx>


namespace p5::lambda::utils::log {

void Type::write(
    Message::Text const &text,
    Message::Level const &level,
    Message::Location &&location
) noexcept(true) {
    if (! handler) return;
    auto message_ = Message{.text = text, .level = level};
    if ((Message::Level::Debug <= level) && (! message::location::unknown(location))) message_.location = ::std::move(location);
    try { handler(message_); } catch (...) {}
}

void Type::write(Message const &message) noexcept(true) {
    if (! handler) return;
    if (message.location && message::location::unknown(*(message.location))) {
        try { handler(Message{.text = message.text, .level = message.level}); } catch(...) {}
        return;
    }
    try { handler(message); } catch(...) {}
}

namespace message::formatter {

inline static auto location_(
    Location const &value,
    Path const &base,
    ::std::list<Output> &output
) noexcept(false) {
    auto text_ = Output{value.function_name()};
    if (! text_.empty()) output.push_back(text_);
    if ((text_ = value.file_name()).empty()) return;
    if (! base.empty()) {
        text_ = ::std::filesystem::relative(text_, base).string();
        if (text_.empty()) return;
    }
    auto const line_ = value.line();
    if (0 < line_) text_ = (::boost::format("%s#%u") % text_ % line_).str();
    output.push_back("@");
    output.push_back(text_);
}

} // namespace message::formatter

message::Formatter::Output message::Formatter::operator () (Input const &input) const noexcept(false) {
    ::std::list<Output> collector_;
    if (tag.empty()) {
        if (level) collector_.push_back((::boost::format("[%s]") % message::level::to_string(input.level)).str());
    }

    else if (level) collector_.push_back((::boost::format("[%s|%s]") % tag % message::level::to_string(input.level)).str());

    if (
        location && static_cast<bool>(input.location) && (! message::location::unknown(*(input.location)))
    ) message::formatter::location_(*(input.location), base, collector_);

    if (input.text.empty()) {
        if (! collector_.empty()) collector_.push_back("-");
        collector_.push_back("empty message");
        return ::boost::algorithm::join(collector_, " ");
    }

    return (::boost::format("%s: %s") % ::boost::algorithm::join(collector_, " ") % input.text).str();
}

} // namespace p5::lambda::utils::log
