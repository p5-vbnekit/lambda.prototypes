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


namespace this_ {

using Location = ::p5::lambda::utils::log::message::Location;

inline static auto make_location_text(
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
            line_ = (offset < line_) ? (line_ - static_cast<::std::decay_t<decltype(line_)>>(offset)) : 0;
            text_ = (::boost::format("%s#%u") % text_ % line_).str();
        }

        list_.push_back("@");
        list_.push_back(text_);
    }

    return ::boost::algorithm::join(list_, " ");
}

inline static auto make_prefix_text(
    ::std::string_view const &tag,
    ::std::string_view const &level
) noexcept(false) {
    return (::boost::format("[%s|%s]") % tag % level).str();
}

inline static auto & singleton() noexcept(false) {
    static auto instance_ = [] {
        struct Result_ final {
            ::std::string_view const tag = "utils.log test";
            ::std::string buffer = {};
            ::std::size_t counter = 0;
            ::p5::lambda::utils::Log log = {};
        };

        Result_ result_;

        result_.log >> [
            formatter_ = p5::lambda::utils::log::message::Formatter{
                .tag = result_.tag,
#ifdef P5_LAMBDA__BASE_PATH
                .base = P5_LAMBDA__BASE_PATH
#endif
            },
            &destination_ = result_.buffer
        ] (auto const &message) { destination_ = formatter_(message); };

        return result_;
    } ();

    return instance_;
}

} // namespace this_


BOOST_AUTO_TEST_CASE(write_case) {
    auto &singleton_ = this_::singleton();
    auto &log_ = singleton_.log;
    auto const &buffer_ = singleton_.buffer;

    using Message_ = ::std::decay_t<decltype(log_)>::Message;
    using Level_ = Message_::Level;

    log_.write<Level_::Developer>("Hello, world!");
    ::std::clog << buffer_ << ::std::endl << ::std::flush;

    auto const make_expected_ = [&singleton_] (
        Message_::Text const &level,
        Message_::Text const &text = {},
        Message_::Text const &location = {}
    ) {
        auto prefix_ = this_::make_prefix_text(singleton_.tag, level);
        if (! location.empty()) prefix_ = (::boost::format("%s %s") % prefix_ % location).str();
        if (text.empty()) return prefix_ + " - empty message";
        return (::boost::format("%s: %s") % prefix_ % text).str();
    };

    log_.write("");
    BOOST_CHECK(make_expected_("Info") == buffer_);

    auto const make_text_ = [&counter_ = singleton_.counter] {
        return "counter = " + ::std::to_string(counter_++);
    };

    auto text_ = make_text_();

    log_.write(text_);
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write(Message_{.text = (text_ = make_text_())});
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write(Message_{.text = (text_ = make_text_()), .level = Level_::Warning});
    BOOST_CHECK(make_expected_("Warning", text_) == buffer_);

    using Location_ = Message_::Location;

    log_.write(Message_{.text = (text_ = make_text_()), .level = Level_::Error, .location = Location_::current()});
    text_ = make_expected_("Error", text_, this_::make_location_text(1));
    BOOST_CHECK(text_ == buffer_);

    log_.write<Level_::Info>(text_ = make_text_());
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write<Level_::Warning>(text_ = make_text_(), Location_::current());
    BOOST_CHECK(make_expected_("Warning", text_) == buffer_);

    log_.write<Level_::Error>(text_ = make_text_(), Location_{});
    BOOST_CHECK(make_expected_("Error", text_) == buffer_);

    log_.write(Level_::Info, text_ = make_text_());
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write(text_ = make_text_(), Level_::Warning);
    BOOST_CHECK(make_expected_("Warning", text_) == buffer_);

    log_.write(text_ = make_text_(), Level_::Error, Location_::current());
    BOOST_CHECK(make_expected_("Error", text_) == buffer_);

    log_.write(Level_::Debug, text_ = make_text_(), Location_{});
    BOOST_CHECK(make_expected_("Debug", text_) == buffer_);

    log_.write(text_ = make_text_(), Level_::Developer, Location_{});
    BOOST_CHECK(make_expected_("Developer", text_) == buffer_);

    log_.write<Level_::Developer>(text_ = make_text_(), Location_{});
    BOOST_CHECK(make_expected_("Developer", text_) == buffer_);

    auto const location_ = Location_::current();

    log_.write(text_ = make_text_(), Level_::Info, Location_{location_});
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write(Level_::Info, text_ = make_text_(), Location_{location_});
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write(text_ = make_text_(), Level_::Debug, Location_{location_});
    text_ = make_expected_("Debug", text_, this_::make_location_text(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write(Level_::Developer, text_ = make_text_(), Location_{location_});
    text_ = make_expected_("Developer", text_, this_::make_location_text(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write<Level_::Debug>(text_ = make_text_(), Location_{location_});
    text_ = make_expected_("Debug", text_, this_::make_location_text(0, location_));
    BOOST_CHECK(text_ == buffer_);

    log_.write<Level_::Info>(text_ = make_text_(), Location_{location_});
    BOOST_CHECK(make_expected_("Info", text_) == buffer_);

    log_.write<Level_::Developer>(text_ = make_text_());
    text_ = make_expected_("Developer", text_, this_::make_location_text(1));
    BOOST_CHECK(text_ == buffer_);
}
