#pragma once
#include <unordered_map>
#include <queue>
#include <tuple>
#include <type_traits>
#include <variant>

#include "Vortex/Debug/Assert.h"

namespace Vortex {
	template<typename ... T>
	struct HandleMap {
		using HandleType = UInt32;

		VORTEX_STATIC_ASSERT(std::is_integral_v<HandleType>)

		using VariantType = std::variant<T...>;
		using ValueContainerType = std::vector<VariantType>;
		using GenerationContainerType = std::vector<HandleType>;
		using FreelistType = std::queue<HandleType>;
		using TypeRegistry = std::tuple<T...>;

		constexpr static HandleType IDBitSize{10};
		constexpr static HandleType TypeBitSize{4};
		constexpr static HandleType GenerationBitSize{18};

		//VORTEX_STATIC_ASSERT(IDBitSize + TypeBitSize + GenerationBitSize == sizeof(HandleType))

		constexpr static HandleType NullHandle{0};

		constexpr static HandleType IDBitStart{0};
		constexpr static HandleType TypeBitStart{IDBitSize};
		constexpr static HandleType GenerationBitStart{TypeBitStart + IDBitSize};

		constexpr static HandleType IDMaxValue{(1 << IDBitSize) - 1};
		constexpr static HandleType TypeMaxValue{(1 << TypeBitSize) - 1};
		constexpr static HandleType GenerationMaxValue{(1 << GenerationBitSize) - 1};

		VORTEX_STATIC_ASSERT(std::tuple_size_v<TypeRegistry> < TypeMaxValue)

	protected:
		template<typename Value, SizeType Index = 0>
		constexpr static SizeType GetFirstIndex_R() {
			VORTEX_STATIC_ASSERT_MSG(Index < std::tuple_size_v<TypeRegistry>, "Type does in registry!")
			using CurrentType = std::tuple_element_t<Index, TypeRegistry>;

			if constexpr(std::is_same_v<CurrentType, Value>) {
				return Index;
			} else {
				return GetFirstIndex_R<Value, Index + 1>();
			}
		}

		template<typename Type, int Index = 0>
		constexpr static bool ContainsType_r() {
			if constexpr(Index < std::tuple_size_v<TypeRegistry>) {
				using CurrentType = std::tuple_element_t<Index, TypeRegistry>;

				if constexpr(std::is_same_v<Type, CurrentType>) {
					return true;
				} else {
					return ContainsType_r<Type, Index + 1>();
				}
			}
			return false;
		}

	public:
		inline HandleMap()
			: m_Values(IDMaxValue),
			  m_Generations(IDMaxValue),
			  m_FreeList{} {

			for (HandleType i = 0; i < IDMaxValue; ++i) {
				if (i == NullHandle) {
					continue;
				}
				m_FreeList.emplace(HandleType{IDMaxValue - 1 - i});
				m_Generations.emplace_back(0);
			}
			m_Values.emplace(m_Values.begin() + NullHandle, VariantType{});
		}
		inline HandleMap(const HandleMap&) = default;
		inline HandleMap(HandleMap&&) noexcept = default;

		virtual ~HandleMap() = default;

	public:
		inline HandleMap& operator=(const HandleMap&) = default;
		inline HandleMap& operator=(HandleMap&&) noexcept = default;

	public:
		template<typename Type>
		inline HandleType Insert(Type value) noexcept {
			VORTEX_STATIC_ASSERT(ContainsType_r<Type>())
			VORTEX_ASSERT(!m_FreeList.empty())

			HandleType id = m_FreeList.front();
			m_FreeList.pop();

			m_Values.insert(m_Values.begin() + id, VariantType{value});

			auto generation = m_Generations[id];
			constexpr static auto type_index = GetFirstIndex_R<Type>();
			auto handle = CreateHandle(id, type_index, generation);

			return handle;
		}

		inline bool Destroy(HandleType handle) noexcept {
			auto generation = ExtractGeneration(handle);
			auto id = ExtractID(handle);

			//if handle is valid just increment generation and add it to free list
			if (generation == m_Generations[id]) {
				++m_Generations[id];
				if (m_Generations[id] > GenerationMaxValue) {
					m_Generations[id] = 0;
				}
				m_FreeList.emplace(id);

				return true;
			}
			return false;
		}

	public:
		inline bool Contains(HandleType handle) const noexcept {
			if (handle == NullHandle) {
				return false;
			} else {
				auto generation = ExtractGeneration(handle);
				auto id = ExtractID(handle);
				return m_Generations[id] == generation;
			}
		}

		template<typename Type>
		inline bool Is(HandleType handle) const noexcept {
			VORTEX_STATIC_ASSERT(ContainsType_r<Type>())
			if (handle == NullHandle) {
				return false;
			} else {
				VORTEX_ASSERT(Contains(handle))
				constexpr static auto type_index = GetFirstIndex_R<Type>();
				return ExtractType(handle) == type_index;
			}
		}

		template<typename Type>
		inline Type& Get(HandleType handle) noexcept {
			VORTEX_STATIC_ASSERT(ContainsType_r<Type>())
			VORTEX_ASSERT(Contains(handle))
			auto id = ExtractID(handle);
			return std::get<Type>(m_Values[id]);
		}
		template<typename Type>
		inline const Type& Get(HandleType handle) const noexcept {
			VORTEX_STATIC_ASSERT(ContainsType_r<Type>())
			VORTEX_ASSERT(Contains(handle))
			auto id = ExtractID(handle);
			return std::get<Type>(m_Values[id]);
		}

		template<typename Type>
		inline Type* GetIf(HandleType handle) noexcept {
			VORTEX_STATIC_ASSERT(ContainsType_r<Type>())

			if (!Contains(handle)) { return nullptr; }

			auto id = ExtractID(handle);
			auto& value = m_Values[id];
			return std::get_if<Type>(&value);
		}
		template<typename Type>
		inline const Type* GetIf(HandleType handle) const noexcept {
			VORTEX_STATIC_ASSERT(ContainsType_r<Type>())

			if (!Contains(handle)) { return nullptr; }

			auto id = ExtractID(handle);
			const auto& value = m_Values[id];
			return std::get_if<Type>(&value);
		}

	protected:
		inline static HandleType CreateHandle(HandleType id, HandleType type, HandleType generation) {
			HandleType handle{0};
			handle |= (id & IDMaxValue) << IDBitStart;
			handle |= (type & TypeMaxValue) << TypeBitStart;
			handle |= (generation & GenerationMaxValue) << GenerationBitStart;
			return handle;
		}
		constexpr static HandleType ExtractID(HandleType handle) {
			return (handle >> IDBitStart) & IDMaxValue;
		}
		constexpr static HandleType ExtractType(HandleType handle) {
			return (handle >> TypeBitStart) & TypeMaxValue;
		}
		constexpr static HandleType ExtractGeneration(HandleType handle) {
			return (handle >> GenerationBitStart) & GenerationMaxValue;
		}

	protected:
		ValueContainerType m_Values;
		GenerationContainerType m_Generations;
		FreelistType m_FreeList;
	};
}