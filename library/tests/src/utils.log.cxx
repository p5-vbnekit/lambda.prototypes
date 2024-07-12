#define BOOST_TEST_MODULE utils.log
#include <boost/test/unit_test.hpp>

#include <cstdint>

#include <list>
#include <string>
#include <iostream>
#include <filesystem>
#include <type_traits>
#include <string_view>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <p5/lambda/utils/log.hxx>


namespace utils_ = ::p5::lambda::utils;

namespace this_ {

using Location = ::utils_::log::message::Location;

inline static auto make_location_text_(
    ::std::size_t offset = 0,
    Location const &location = Location::current()
) noexcept(false) {
    ::std::list<::std::string> list_;

    auto text_ = ::std::string{location.function_name()};
    if (! text_.empty()) list_.push_back(text_);

    if (! (text_ = location.file_name()).empty()) {
#ifdef P5_LAMBDA__BASE_PATH
        text_ = ::std::filesystem::relative(text_, P5_LAMBDA__BASE_PATH).string();
#endif
        auto line_ = location.line();
        if (0 < line_) {
            line_ = (offset < line_) ? (line_ - static_cast<
                ::std::decay_t<decltype(line_)>
            >(offset)) : 0;
            text_ = (::boost::format("%s#%u") % text_ % line_).str();
        }

        list_.push_back("|");
        list_.push_back(text_);
    }

    return ::boost::algorithm::join(list_, "");
}

inline static auto make_prefix_text_(
    ::std::string_view const &tag,
    ::std::string_view const &level
) noexcept(false) { return (::boost::format("[%s|%s]") % tag % level).str(); }

inline static auto & singleton_() noexcept(false) {
    static auto instance_ = [] {
        struct Result_ final {
            ::std::string_view const tag = "utils.log test";
            ::std::string buffer = {};
            ::std::size_t counter = 0;
            ::utils_::Log log = {};
        } result_;

//         auto &&formatter_ = [&tag_ = result_.tag] (auto const &message) {
//             ::std::list<::std::string> buffer_;
//             auto const size_ = [&buffer_, &text_ = message.text] {
//                 auto counter_ = static_cast<
//                     ::std::decay_t<decltype(buffer_.size())>
//                 >(0);
//                 for (auto &line_: [&text_] () {
//                     namespace walker_ = ::utils_::log::message::text::walker;
//                     using Line_ = walker_::policy::line::types::Copy;
//                     struct Policy_ final { using line = Line_; };
//                     return ::utils_::generator::coroutine::from_function(
//                         walker_::make(text_, Policy_{})
//                     );
//                 } ()) { counter_++; buffer_.emplace_back(::std::move(line_)); }
//                 return counter_;
//             } ();
//             if constexpr (true) {
//                 ::std::ostringstream stream_;
//                 namespace message_ = ::utils_::log::message;
//                 stream_ << make_prefix_text_(
//                     tag_, message_::level::to_string(message.level)
//                 );
//                 if (message.location) {
//                     stream_ << " " << message_::location::to_string(
//                         *(message.location)
// #ifdef P5_LAMBDA__BASE_PATH
//                         , P5_LAMBDA__BASE_PATH
// #endif
//                     );
//                 }
//                 if (1 < size_) stream_ << ":";
//                 else if (0 < size_) {
//                     if (! buffer_.front().empty()) stream_ << ": ";
//                     stream_ << ::std::move(buffer_.front());
//                     buffer_.pop_front();
//                 }
//                 buffer_.emplace_front(::std::move(stream_.str()));
//             };
//             if (! (1 < size_)) return ::std::move(buffer_.front());
//             static auto const new_line_ = [] {
//                 ::std::ostringstream stream_;
//                 stream_ << ::std::endl;
//                 return ::std::move(stream_.str());
//             } ();
//             return ::boost::algorithm::join(::std::move(buffer_), new_line_);
//         };

//         result_.log >> [
//             &destination_ = result_.buffer, formatter_ = ::std::move(formatter_)
//         ] (auto const &message) { destination_ = formatter_(message); };

        result_.log >> [
            &tag_ = result_.tag, &destination_ = result_.buffer
        ] (auto const &message) {
            auto prefix_ = this_::make_prefix_text_(
                tag_, ::utils_::log::message::level::to_string(message.level)
            );
            if (message.location) prefix_ = (
                ::boost::format("%s %s") % prefix_ %
                ::utils_::log::message::location::to_string(
                    *(message.location)
#ifdef P5_LAMBDA__BASE_PATH
                    , P5_LAMBDA__BASE_PATH
#endif
                )
            ).str();
            ::std::ostringstream stream_;
            for (auto const &chunk: *(message.text)) stream_ << chunk.reference;
            destination_ = stream_.str();
            if (destination_.empty()) destination_ = ::std::move(prefix_);
            else destination_ = (
                ::boost::format("%s: %s") % ::std::move(prefix_) %
                ::std::move(destination_)
            ).str();
        };

        return result_;
    } ();

    return instance_;
}

} // namespace this_


BOOST_AUTO_TEST_CASE(write_case) {
    auto &singleton_ = this_::singleton_();
    auto &log_ = singleton_.log;
    auto const &buffer_ = singleton_.buffer;

    using Message_ = ::std::decay_t<decltype(log_)>::Message;
    using Level_ = Message_::Level;

    log_.write(::std::make_tuple(
        "testing", ::std::make_tuple(" log ", "utility")
    ));
    ::std::clog << buffer_ << ::std::endl << ::std::flush;

    log_.write() << "this message" << " triggered by " << "RAII object";
    ::std::clog << buffer_ << ::std::endl << ::std::flush;

    auto const make_expected_ = [&singleton_] (
        ::std::string_view const &level,
        ::std::string_view const &text = {},
        ::std::string_view const &location = {}
    ) {
        auto prefix_ = this_::make_prefix_text_(singleton_.tag, level);
        if (! location.empty()) prefix_ = (
            ::boost::format("%s %s") % prefix_ % location
        ).str();
        if (text.empty() || (0 == text.at(0))) return prefix_;
        return (::boost::format("%s: %s") % prefix_ % text).str();
    };

    log_.write("");
    BOOST_CHECK(make_expected_("Info") == buffer_);

    auto const make_text_ = [&counter_ = singleton_.counter] {
        return "counter = " + ::std::to_string(counter_++);
    };

    auto text_ = make_text_();
    namespace message_text_ = ::utils_::log::message::text;

    log_.write(text_);
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    text_ = make_text_();
    log_.write(Message_{.text = message_text_::make(text_)});
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write(Message_{
        .text = message_text_::make(text_),
        .level = Level_::Warning
    });
    BOOST_CHECK(make_expected_("Warning", text_) == buffer_);

    using Location_ = Message_::Location;

    log_.write(Message_{
        .text = message_text_::make(text_),
        .level = Level_::Error,
        .location = Location_::current()
    });
    text_ = make_expected_("Error", text_, this_::make_location_text_(2));
    BOOST_CHECK(text_ == buffer_);

    log_.write<Level_::Info>(text_ = make_text_());
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write<Level_::Warning>(text_ = make_text_(), Location_::current());
    BOOST_CHECK(make_expected_("Warning", text_, this_::make_location_text_(1)) == buffer_);

    log_.write<Level_::Error>(text_ = make_text_(), Location_{});
    BOOST_CHECK(make_expected_("Error", text_) == buffer_);

    log_.write(Level_::Info, text_ = make_text_());
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write(text_ = make_text_(), Level_::Warning);
    BOOST_CHECK(make_expected_("Warning", text_) == buffer_);

    log_.write(text_ = make_text_(), Level_::Error, Location_::current());
    BOOST_CHECK(make_expected_("Error", text_, this_::make_location_text_(1)) == buffer_);

    log_.write(Level_::Debug, text_ = make_text_(), Location_{});
    BOOST_CHECK(make_expected_("Debug", text_) == buffer_);

    log_.write(text_ = make_text_(), Level_::Developer, Location_{});
    BOOST_CHECK(make_expected_("Developer", text_) == buffer_);

    log_.write<Level_::Developer>(text_ = make_text_(), Location_{});
    BOOST_CHECK(make_expected_("Developer", text_) == buffer_);

    auto const location_ = Location_::current();

    log_.write(text_ = make_text_(), Level_::Info, Location_{location_});
    text_ = make_expected_("Info", text_, this_::make_location_text_(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write(Level_::Info, text_ = make_text_(), Location_{location_});
    text_ = make_expected_("Info", text_, this_::make_location_text_(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write(text_ = make_text_(), Level_::Debug, Location_{location_});
    text_ = make_expected_("Debug", text_, this_::make_location_text_(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write(Level_::Developer, text_ = make_text_(), Location_{location_});
    text_ = make_expected_("Developer", text_, this_::make_location_text_(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write<Level_::Debug>(text_ = make_text_(), Location_{location_});
    text_ = make_expected_("Debug", text_, this_::make_location_text_(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write<Level_::Info>(text_ = make_text_(), Location_{location_});
    text_ = make_expected_("Info", text_, this_::make_location_text_(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write<Level_::Developer>(text_ = make_text_());
    text_ = make_expected_("Developer", text_, this_::make_location_text_(1));
    BOOST_CHECK(text_ == buffer_);

    log_.write(Level_::Info) << (text_ = make_text_());
    text_ = make_expected_("Info", text_);
    BOOST_CHECK(text_ == buffer_);

    log_.write(Level_::Info, Location_::current()) << (text_ = make_text_());
    text_ = make_expected_("Info", text_, this_::make_location_text_(1));
    BOOST_CHECK(text_ == buffer_);

    log_.write<Level_::Info>(location_) << (text_ = make_text_());
    text_ = make_expected_("Info", text_, this_::make_location_text_(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write(Level_::Developer) << (text_ = make_text_());
    text_ = make_expected_("Developer", text_, this_::make_location_text_(1));
    BOOST_CHECK(text_ == buffer_);

    log_.write(Level_::Developer) << "log utility test finished";
    ::std::clog << buffer_ << ::std::endl << ::std::flush;
}
