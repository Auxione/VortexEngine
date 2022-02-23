#pragma once

/*
Usage:
class Klass: Vortex::Singleton<Klass> {
	using Singleton<Klass>::Singleton;
	friend class Vortex::Singleton<x>;
	Klass() = default;
	...
};

or

class VORTEX_SINGLETON(x)
	x() = default;
	...
};
*/
namespace Vortex {
	template<typename T>
	struct Singleton {
	public:
		using Type = Singleton<T>;

		static T& GetInstance() {
			static T instance;
			return instance;
		}

	protected:
		Singleton() = default;
		virtual ~Singleton() = default;

	public:
		Singleton& operator=(const Singleton&) = delete;
		Singleton(const Singleton&) = delete;
		Singleton(Singleton&&) noexcept = delete;
		Singleton& operator=(Singleton&&) noexcept = delete;
	};
}

#define VORTEX_SINGLETON(x) x final: public Vortex::Singleton<x> {    \
using Singleton<x>::Singleton;    \
friend class Vortex::Singleton<x>;
