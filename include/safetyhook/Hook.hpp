#pragma once

#include <cstdint>
#include <memory>
#include <vector>

namespace safetyhook {
class Factory;

class Hook final {
public:
    ~Hook();

    auto target() const { return m_target; }
    auto destination() const { return m_destination; }
    auto trampoline() const { return m_trampoline; }
    auto ok() const { return m_trampoline != 0; }

    template <typename T> T* original() const { return (T*)m_trampoline; }

    template <typename RetT = void, typename... Args> auto call(Args... args) {
        return ((RetT(*)(Args...))m_trampoline)(args...);
    }

    template <typename RetT = void, typename... Args> auto thiscall(Args... args) {
        return ((RetT(__thiscall*)(Args...))m_trampoline)(args...);
    }

    template <typename RetT = void, typename... Args> auto stdcall(Args... args) {
        return ((RetT(__stdcall*)(Args...))m_trampoline)(args...);
    }

private:
    friend Factory;

    std::shared_ptr<Factory> m_factory;
    uintptr_t m_target{};
    uintptr_t m_destination{};
    uintptr_t m_trampoline{};
    size_t m_trampoline_size{};
    size_t m_trampoline_allocation_size{};
    std::vector<uint8_t> m_original_bytes{};

    Hook() = delete;
    Hook(const Hook&) = delete;
    Hook(Hook&&) = delete;
    Hook(std::shared_ptr<Factory> factory, uintptr_t target, uintptr_t destination);
};
} // namespace safetyhook