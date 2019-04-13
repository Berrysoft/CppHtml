#ifndef HTML_TREE_HPP
#define HTML_TREE_HPP

#include <vector>

namespace html
{
    template <typename T>
    class tree
    {
    private:
        using child_seq_t = std::vector<tree<T>>;

    public:
        using value_type = T;
        using reference = T&;
        using const_reference = const T&;
        using pointer = T*;
        using const_pointer = const T*;
        using child_type = tree<T>;
        using child_reference = child_type&;
        using child_const_reference = const child_type&;
        using child_iterator = typename child_seq_t::iterator;
        using child_const_iterator = typename child_seq_t::const_iterator;

    private:
        value_type m_data;
        child_seq_t m_children;

    public:
        constexpr reference data() noexcept { return m_data; }
        constexpr const_reference data() const noexcept { return m_data; }

        constexpr reference operator*() noexcept { return m_data; }
        constexpr pointer operator->() noexcept { return std::addressof(m_data); }

        constexpr const_reference operator*() const noexcept { return m_data; }
        constexpr const_pointer operator->() const noexcept { return std::addressof(m_data); }

        bool empty() const noexcept { return m_children.empty(); }
        std::size_t size() const noexcept { return m_children.size(); }

        child_reference front() { return m_children.front(); }
        child_const_reference front() const { return m_children.front(); }
        child_reference back() { return m_children.back(); }
        child_const_reference back() const { return m_children.back(); }

        child_iterator begin() noexcept { return m_children.begin(); }
        child_const_iterator begin() const noexcept { return m_children.begin(); }
        child_const_iterator cbegin() const noexcept { return m_children.cbegin(); }
        child_iterator end() noexcept { return m_children.end(); }
        child_const_iterator end() const noexcept { return m_children.end(); }
        child_const_iterator cend() const noexcept { return m_children.cend(); }

        void clear() noexcept { m_children.clear(); }
        void push_child(const child_type& child) { m_children.push_back(child); }
        void push_child(child_type&& child) { m_children.push_back(std::move(child)); }
        child_iterator insert_child(child_const_iterator pos, const child_type& child) { return m_children.insert(pos, child); }
        child_iterator insert_child(child_const_iterator pos, child_type&& child) { return m_children.insert(pos, std::move(child)); }
        child_iterator erase_child(child_const_iterator pos) { return m_children.erase(pos); }
        child_iterator erase_child(child_const_iterator first, child_const_iterator last) { return m_children.erase(first, last); }
    };
} // namespace html

#endif // !HTML_TREE_HPP
