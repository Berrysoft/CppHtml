#ifndef HTML_UTILITY_HPP
#define HTML_UTILITY_HPP

#define PROP_GETSET(name, type, getv, setv)                         \
    constexpr type& name() & noexcept { return getv; }              \
    constexpr const type& name() const& noexcept { return getv; }   \
    constexpr type&& name() && noexcept { return std::move(getv); } \
    void name(const type& value) noexcept { setv = value; }         \
    void name(type&& value) noexcept { setv = std::move(value); }

#define PROP(name, type) PROP_GETSET(name, type, m_##name, m_##name)

#endif // !HTML_UTILITY_HPP
