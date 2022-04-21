#pragma once
#include <queue>
#include <tuple>
#include <type_traits>
#include <variant>
#include <functional>

#include "Vortex/Common/IntegerPacker.h"

#ifdef VORTEX_DEBUG
#include <set>

#include "Vortex/Debug/Assert.h"
#endif

namespace Vortex {
	template<
		typename UnderlyingType_,
		UnderlyingType_ IDBitSize_,
		UnderlyingType_ TypeBitSize_,
		UnderlyingType_ GenerationBitSize_
	>
	struct BasicHandle {
		VORTEX_STATIC_ASSERT(std::is_integral_v<UnderlyingType_>)
		VORTEX_STATIC_ASSERT(
			IDBitSize_ + TypeBitSize_ + GenerationBitSize_ <= std::numeric_limits<UnderlyingType_>::digits
		)

		using UnderlyingType = UnderlyingType_;
		using Packer = IntegerPacker<UnderlyingType, IDBitSize_, TypeBitSize_, GenerationBitSize_>;

	protected:
		constexpr static UnderlyingType EncodeHandle(UnderlyingType id, UnderlyingType type, UnderlyingType generation) {
			UnderlyingType handle{0};
			Packer::template Pack<0>(handle, id);
			Packer::template Pack<1>(handle, type);
			Packer::template Pack<2>(handle, generation);
			return handle;
		}
		constexpr static UnderlyingType DecodeID(UnderlyingType handle) { return Packer::template Unpack<0>(handle); }
		constexpr static UnderlyingType DecodeType(UnderlyingType handle) { return Packer::template Unpack<1>(handle); }
		constexpr static UnderlyingType DecodeGeneration(UnderlyingType handle) { return Packer::template Unpack<2>(handle); }

	public:

		// Stores data in a contiguous block of memory.
		// Creates an associative mapping between a handle and the data.
		// O(1) insertions
		// O(1) lookups
		// O(n)? removals
		template<typename ... T>
		struct Map {
			using Handle = UnderlyingType;

			// == Type registry ==
			using TypeRegistry = std::tuple<T...>;
			template<SizeType i>
			using TypeRegistryElement = std::tuple_element_t<i, TypeRegistry>;
			constexpr static SizeType TypeRegistrySize = std::tuple_size_v<TypeRegistry>;
			using TypeRegistryFirstElement = TypeRegistryElement<0>;

			using StoredType = std::conditional_t<TypeRegistrySize == 1,
				TypeRegistryFirstElement,
				std::variant<T...>
			>;
			using ValueStorageType = std::vector<StoredType>;
			using GenerationContainerType = std::vector<Handle>;
			using FreelistType = std::priority_queue<
				Handle,
				std::deque<Handle>,
				std::greater<>
			>;

			constexpr static Handle NullHandle{0};

		protected:
			template<typename Value, SizeType Index = 0>
			constexpr static SizeType GetFirstTypeIndex_r() {
				VORTEX_STATIC_ASSERT_MSG(Index < TypeRegistrySize, "Type does in registry!")
				using CurrentType = TypeRegistryElement<Index>;

				if constexpr(std::is_same_v<CurrentType, Value>) {
					return Index;
				} else {
					return GetFirstTypeIndex_r<Value, Index + 1>();
				}
			}

			template<typename Type>
			constexpr static bool ContainsType() { return std::disjunction_v<std::is_same<T, Type>...>; }

		public:
			inline Map()
				: m_Values(),
				  m_Generations(),
				  m_FreeList(),
				  m_NextFreeID{1} {

				VORTEX_STATIC_ASSERT(TypeRegistrySize < Packer::template GetMaxValue<1>())
				VORTEX_STATIC_ASSERT_MSG(std::disjunction_v<std::is_trivial<T>...>, "Passed types must be trivial")
				VORTEX_STATIC_ASSERT_MSG(std::disjunction_v<std::is_trivially_copyable<T>...>, "Passed types must be trivially copyable")

				m_Values.emplace(m_Values.begin() + NullHandle, StoredType{});
				m_Generations.emplace(m_Generations.begin() + NullHandle, 0);

#ifdef VORTEX_DEBUG
				d_ActiveIDs.emplace(NullHandle);
#endif
			}
			inline Map(const Map&) = default;
			inline Map(Map&&) noexcept = default;

			virtual ~Map() = default;

		public:
			inline Map& operator=(const Map&) = default;
			inline Map& operator=(Map&&) noexcept = default;

			template<SizeType Count = TypeRegistrySize, typename = typename std::enable_if_t<Count == 1>::type>
			inline auto& operator[](Handle handle) { return Get<TypeRegistryFirstElement>(handle); }

			template<SizeType Count = TypeRegistrySize, typename = typename std::enable_if_t<Count == 1>::type>
			inline const auto& operator[](Handle handle) const { return Get<TypeRegistryFirstElement>(handle); }

		public:
			template<typename Type = TypeRegistryFirstElement>
			Handle Insert(const Type& value) {
				VORTEX_STATIC_ASSERT(ContainsType<Type>())

				Handle id;
				if (m_FreeList.empty()) {
					VORTEX_ASSERT(m_NextFreeID < Packer::template GetMaxValue<0>())
					id = m_NextFreeID;
					++m_NextFreeID;
				} else {
					id = m_FreeList.top();
					m_FreeList.pop();
				}

				constexpr static auto type_index = GetFirstTypeIndex_r<Type>();

				Handle generation;
				if (id < m_Generations.size()) {
					generation = m_Generations[id];
				} else {
					generation = 0;
					m_Generations.emplace(m_Generations.begin() + id, Handle{0});
				}

				if constexpr(TypeRegistrySize == 1) {
					m_Values.insert(m_Values.begin() + id, value);
				} else {
					m_Values.insert(m_Values.begin() + id, StoredType{value});
				}

				auto handle = EncodeHandle(id, type_index, generation);

#ifdef VORTEX_DEBUG
				d_ActiveIDs.emplace(handle);
				++d_Size;
#endif
				return handle;
			}

			inline void Destroy(Handle handle) noexcept {
				auto generation = DecodeGeneration(handle);
				auto id = DecodeID(handle);

				//if handle is valid just increment generation and add it to free list
				auto stored_gen = m_Generations[id];
				if (stored_gen == generation) {
					++stored_gen;
					if (stored_gen >= Packer::template GetMaxValue<2>()) {
						stored_gen = 0;
					}
					m_FreeList.emplace(id);
					m_Generations[id] = stored_gen;

#ifdef VORTEX_DEBUG
					d_ActiveIDs.erase(handle);
					--d_Size;
#endif
				}
			}

		public:
			[[nodiscard]] inline bool Contains(Handle handle) const noexcept {
				auto generation = DecodeGeneration(handle);
				auto id = DecodeID(handle);

				if (handle == NullHandle
					|| id >= m_Generations.size()) {
					return false;
				} else {
					return m_Generations[id] == generation;
				}
			}

			template<typename Type>
			inline bool Is(Handle handle) const noexcept {
				VORTEX_STATIC_ASSERT(ContainsType<Type>())

				auto id = DecodeID(handle);
				auto type = DecodeType(handle);
				auto generation = DecodeGeneration(handle);

				if (handle == NullHandle
					|| id >= m_Generations.size()
					|| generation != m_Generations[id]) {
					return false;
				} else {
					auto internal_tye = GetFirstTypeIndex_r<Type>();
					return type == internal_tye;
				}
			}

		public:
			template<typename Type = TypeRegistryFirstElement>
			inline Type& Get(Handle handle) noexcept {
				VORTEX_STATIC_ASSERT(ContainsType<Type>())
				VORTEX_ASSERT(Is<Type>(handle))
				VORTEX_ASSERT(Contains(handle))
				auto id = DecodeID(handle);

				if constexpr(TypeRegistrySize == 1) {
					return m_Values[id];
				} else {
					return std::get<Type>(m_Values[id]);
				}
			}
			template<typename Type = TypeRegistryFirstElement>
			inline const Type& Get(Handle handle) const noexcept {
				VORTEX_STATIC_ASSERT(ContainsType<Type>())
				VORTEX_ASSERT(Is<Type>(handle))
				VORTEX_ASSERT(Contains(handle))
				auto id = DecodeID(handle);

				if constexpr(TypeRegistrySize == 1) {
					return m_Values[id];
				} else {
					return std::get<Type>(m_Values[id]);
				}
			}

			template<typename Type = TypeRegistryFirstElement>
			inline Type* GetIf(Handle handle) noexcept {
				VORTEX_STATIC_ASSERT(ContainsType<Type>())

				if (!Contains(handle)) { return nullptr; }

				auto id = DecodeID(handle);

				if constexpr(TypeRegistrySize == 1) {
					return m_Values.data() + id;
				} else {
					return std::get<Type>(m_Values.data() + id);
				}
			}
			template<typename Type = TypeRegistryFirstElement>
			inline const Type* GetIf(Handle handle) const noexcept {
				VORTEX_STATIC_ASSERT(ContainsType<Type>())

				if (!Contains(handle)) { return nullptr; }

				auto id = DecodeID(handle);

				if constexpr(TypeRegistrySize == 1) {
					return m_Values.data() + id;
				} else {
					return std::get<Type>(m_Values.data() + id);
				}
			}

		protected:
			ValueStorageType m_Values;
			GenerationContainerType m_Generations;
			FreelistType m_FreeList;
			Handle m_NextFreeID;

#ifdef VORTEX_DEBUG
		public:
			std::set<Handle> d_ActiveIDs{};
			Vortex::SizeType d_Size{1};
#endif
		};
	};

	template<typename ... T>
	using HandleMap = BasicHandle<UInt32, 10, 4, 18>::Map<T...>;
}