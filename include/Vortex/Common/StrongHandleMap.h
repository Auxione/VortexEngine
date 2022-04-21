#pragma once

namespace Vortex {
	template<
		typename IDType_,
		IDType_ IDBitSize_,
		IDType_ GenerationBitSize_
	>
	struct BasicStrongHandle {
		using IDType = IDType_;
		using Packer = IntegerPacker<IDType, IDBitSize_, GenerationBitSize_>;

		constexpr static IDType EncodeID(IDType index, IDType generation) {
			IDType id{0};
			Packer::template Pack<0>(id, index);
			Packer::template Pack<1>(id, generation);
			return id;
		}
		constexpr static IDType DecodeIndex(IDType id) { return Packer::template Unpack<0>(id); }
		constexpr static IDType DecodeGeneration(IDType id) { return Packer::template Unpack<1>(id); }

		template<typename T>
		struct Handle {
			using Type = T;

		public:
			IDType id;

		public:
			constexpr friend bool operator==(Handle<T> rhs, Handle<T> lhs) { return rhs.id == lhs.id; }
			constexpr friend bool operator!=(Handle<T> rhs, Handle<T> lhs) { return rhs.id != lhs.id; }
		};

		template<typename ... T>
		struct Map {
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
			using GenerationContainerType = std::vector<IDType>;
			using FreelistType = std::priority_queue<
				IDType,
				std::deque<IDType>,
				std::greater<>
			>;

			constexpr static IDType NullID{0};

		protected:
			template<typename Type>
			constexpr static bool ContainsType() { return std::disjunction_v<std::is_same<T, Type>...>; }

		public:
			inline Map()
				: m_Values(),
				  m_Generations(),
				  m_FreeList(),
				  m_NextFreeIndex{1} {

				VORTEX_STATIC_ASSERT_MSG(std::disjunction_v<std::is_trivial<T>...>, "Types must be trivial")
				VORTEX_STATIC_ASSERT_MSG(std::disjunction_v<std::is_trivially_copyable<T>...>, "Types must be trivially copyable")

				m_Values.emplace(m_Values.begin() + NullID, StoredType{});
				m_Generations.emplace(m_Generations.begin() + NullID, 0);

#ifdef VORTEX_DEBUG
				d_ActiveIDs.emplace(NullID);
#endif
			}

		public:
			template<typename Type, typename = std::enable_if_t<ContainsType<Type>()>>
			Handle<Type> Insert(const Type& value) {
				IDType index;
				if (m_FreeList.empty()) {
					VORTEX_ASSERT(m_NextFreeIndex < Packer::template GetMaxValue<0>())
					index = m_NextFreeIndex;
					++m_NextFreeIndex;
				} else {
					index = m_FreeList.top();
					m_FreeList.pop();
				}

				IDType generation;
				if (index < m_Generations.size()) {
					generation = m_Generations[index];
				} else {
					generation = 0;
					m_Generations.emplace(m_Generations.begin() + index, IDType{0});
				}

				if constexpr(TypeRegistrySize == 1) {
					m_Values.insert(m_Values.begin() + index, value);
				} else {
					m_Values.insert(m_Values.begin() + index, Type{value});
				}

				IDType id = EncodeID(index, generation);
#ifdef VORTEX_DEBUG
				d_ActiveIDs.emplace(id);
				++d_Size;
#endif
				return Handle<Type>{id};
			}

			template<typename Type, typename = std::enable_if_t<ContainsType<Type>()>>
			inline void Destroy(Handle<Type> handle) noexcept {
				auto index = DecodeIndex(handle.id);
				auto generation = DecodeGeneration(handle.id);

				//if handle is valid just increment generation and add it to free list
				auto stored_gen = m_Generations[index];
				if (stored_gen == generation) {
					++stored_gen;
					if (stored_gen >= Packer::template GetMaxValue<2>()) {
						stored_gen = 0;
					}
					m_FreeList.emplace(index);
					m_Generations[index] = stored_gen;

#ifdef VORTEX_DEBUG
					d_ActiveIDs.erase(handle);
					--d_Size;
#endif
				}
			}

			template<typename Type, typename = std::enable_if_t<ContainsType<Type>()>>
			inline bool Contains(Handle<Type> handle) noexcept {
				auto index = DecodeIndex(handle.id);
				auto generation = DecodeGeneration(handle.id);

				if (index == NullID || index >= m_Generations.size()) {
					return false;
				} else {
					return m_Generations[index] == generation;
				}
			}

		public:
			template<typename Type, typename = std::enable_if_t<ContainsType<Type>()>>
			inline Type& Get(Handle<Type> handle) noexcept {
				VORTEX_ASSERT(Contains<Type>(handle))

				auto index = DecodeIndex(handle.id);

				if constexpr(TypeRegistrySize == 1) {
					return m_Values[index];
				} else {
					return std::get<Type>(m_Values[index]);
				}
			}
			template<typename Type, typename = std::enable_if_t<ContainsType<Type>()>>
			inline const Type& Get(Handle<Type> handle) const noexcept {
				VORTEX_ASSERT(Contains(handle))

				auto index = DecodeIndex(handle.id);

				if constexpr(TypeRegistrySize == 1) {
					return m_Values[index];
				} else {
					return std::get<Type>(m_Values[index]);
				}
			}

		protected:
			ValueStorageType m_Values;
			GenerationContainerType m_Generations;
			FreelistType m_FreeList;
			IDType m_NextFreeIndex;

#ifdef VORTEX_DEBUG
		public:
			std::set<IDType> d_ActiveIDs{};
			Vortex::SizeType d_Size{1};
#endif
		};
	};

	using StrongHandle = BasicStrongHandle<UInt32, 24, 8>;
	template<typename ... T>
	using StrongHandleMap = StrongHandle::Map<T...>;
}

template<typename T>
struct std::hash<Vortex::StrongHandle::Handle<T>> {
	std::size_t operator()(const Vortex::StrongHandle::Handle<T>& s) const noexcept {
		return std::hash<typename Vortex::StrongHandle::IDType>{}(s.id);
	}
};
