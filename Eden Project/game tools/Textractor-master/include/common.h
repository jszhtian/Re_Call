#pragma once

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <string>
#include <vector>
#include <deque>
#include <array>
#include <unordered_map>
#include <unordered_set>
#include <functional>
#include <algorithm>
#include <regex>
#include <memory>
#include <optional>
#include <thread>
#include <mutex>
#include <shared_mutex>
#include <atomic>
#include <filesystem>
#include <cstdint>
#include <cassert>

#ifdef _WIN64
constexpr bool x64 = true;
#else
constexpr bool x64 = false;
#endif

template <typename T, typename... Xs>
struct ArrayImpl { using type = std::tuple<T, Xs...>[]; };
template <typename T>
struct ArrayImpl<T> { using type = T[]; };
template <typename... Ts>
using Array = typename ArrayImpl<Ts...>::type;

template <auto F> using Functor = std::integral_constant<std::remove_reference_t<decltype(F)>, F>;

template <typename V>
struct Identity { V operator()(V v) const { return v; } };

struct PermissivePointer
{
	template <typename T>
	operator T*() { return (T*)p; }
	void* p;
};

template <typename HandleCloser = Functor<CloseHandle>>
class AutoHandle
{
public:
	AutoHandle(HANDLE h) : h(h) {}
	operator HANDLE() { return h.get(); }
	PHANDLE operator&() { static_assert(sizeof(*this) == sizeof(HANDLE)); assert(!h); return (PHANDLE)this; }
	operator bool() { return h.get() != NULL && h.get() != INVALID_HANDLE_VALUE; }

private:
	struct HandleCleaner { void operator()(void* h) { if (h != INVALID_HANDLE_VALUE) HandleCloser()(PermissivePointer{ h }); } };
	std::unique_ptr<void, HandleCleaner> h;
};

template<typename T, typename M = std::mutex>
class Synchronized
{
public:
	template <typename... Args>
	Synchronized(Args&&... args) : contents(std::forward<Args>(args)...) {}

	struct Locker
	{
		T* operator->() { return &contents; }
		std::unique_lock<M> lock;
		T& contents;
	};

	Locker Acquire() { return { std::unique_lock(m), contents }; }
	Locker operator->() { return Acquire(); }

private:
	T contents;
	M m;
};

static struct
{
	BYTE DUMMY[100];
	template <typename T> 
	operator T*() { static_assert(sizeof(T) < sizeof(DUMMY)); return (T*)DUMMY; }
} DUMMY;

template <typename T>
inline auto FormatArg(T arg) { return arg; }

template <typename C>
inline auto FormatArg(const std::basic_string<C>& arg) { return arg.c_str(); }

#pragma warning(push)
#pragma warning(disable: 4996)
template <typename... Args>
inline std::string FormatString(const char* format, const Args&... args)
{
	std::string buffer(snprintf(nullptr, 0, format, FormatArg(args)...), '\0');
	sprintf(buffer.data(), format, FormatArg(args)...);
	return buffer;
}

template <typename... Args>
inline std::wstring FormatString(const wchar_t* format, const Args&... args)
{
	std::wstring buffer(_snwprintf(nullptr, 0, format, FormatArg(args)...), L'\0');
	_swprintf(buffer.data(), format, FormatArg(args)...);
	return buffer;
}
#pragma warning(pop)

inline std::optional<std::wstring> StringToWideString(const std::string& text, UINT encoding)
{
	std::vector<wchar_t> buffer(text.size() + 1);
	if (int length = MultiByteToWideChar(encoding, 0, text.c_str(), text.size() + 1, buffer.data(), buffer.size()))
		return std::wstring(buffer.data(), length - 1);
	return {};
}

inline std::wstring StringToWideString(const std::string& text)
{
	std::vector<wchar_t> buffer(text.size() + 1);
	MultiByteToWideChar(CP_UTF8, 0, text.c_str(), -1, buffer.data(), buffer.size());
	return buffer.data();
}

inline std::string WideStringToString(const std::wstring& text)
{
	std::vector<char> buffer((text.size() + 1) * 4);
	WideCharToMultiByte(CP_UTF8, 0, text.c_str(), -1, buffer.data(), buffer.size(), nullptr, nullptr);
	return buffer.data();
}

template <typename... Args>
inline void TEXTRACTOR_MESSAGE(const wchar_t* format, const Args&... args) { MessageBoxW(NULL, FormatString(format, args...).c_str(), L"Textractor", MB_OK); }

#ifdef _DEBUG
#define TEST(...) static auto _ = CreateThread(nullptr, 0, [](auto) { __VA_ARGS__; return 0UL; }, NULL, 0, nullptr); 
#else
#define TEST(...)
#endif
