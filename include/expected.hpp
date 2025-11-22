namespace kstd {
    template <typename success, typename error>
    union expected {
        success S;
        error E;
    };
};