#pragma once
#include "Vortex/Memory/Memory.h"
#include "Vortex/Memory/HeapAllocator.h"

namespace Vortex {
	template<typename T>
	struct List {
		struct Node {
			T Data;
			Node* Next;

			template<typename ... Args>
			explicit Node(Args&& ... args): Data{std::forward<Args>(args)...}, Next{nullptr} {}
		};

	public:
		List()
			: m_Size{0}, m_Head{nullptr} {
		};

		List(const List<T>& other)
			: m_Size{other.m_Size}, m_Head{nullptr} {

			Node* new_head = nullptr;
			Node* current_node = other.m_Head;
			while (current_node != nullptr) {
				auto* copied_node = GetHeapAllocator()->New<Node>(current_node->Data);
				copied_node->Next = new_head;

				current_node = current_node->Next;
				new_head = copied_node;
			}
			m_Head = new_head;
		};

		List(List<T>&& other) noexcept
			: m_Size{other.m_Size}, m_Head{other.m_Head} {
			other.m_Head = nullptr;
			other.m_Size = 0;
		};

		~List() {
			while (!Empty()) {
				PopFront();
			}
		};

	public:
		constexpr bool operator==(const List<T>& other) {
			return m_Head == other.m_Head && m_Size == other.m_Size;
		}
		constexpr bool operator!=(const List<T>& other) {
			return m_Head != other.m_Head || m_Size != other.m_Size;
		}

		constexpr List<T>& operator=(const List<T>& other) = delete;

		constexpr List<T>& operator=(List<T>&& other) noexcept {
			if (*this == other) {
				return *this;
			}
			auto* new_head = other.m_Head;
			auto new_size = other.m_Size;

			other.m_Head = nullptr;
			other.m_Size = 0;

			Clear();
			m_Size = new_size;
			m_Head = new_head;

			return *this;
		}

	public:
		void PushFront(const T& value) {
			Node* new_node = GetHeapAllocator()->New<Node>(value);

			new_node->Next = m_Head;
			m_Head = new_node;

			++m_Size;
		}

		template<typename ... Args>
		void EmplaceFront(Args&& ...args) {
			Node* new_node = GetHeapAllocator()->New<Node>(std::forward<Args>(args)...);

			new_node->Next = m_Head;
			m_Head = new_node;

			++m_Size;
		}

		void PopFront() {
			VORTEX_ASSERT(m_Head != nullptr)

			auto* next_node = m_Head->Next;
			GetHeapAllocator()->Delete(m_Head);
			m_Head = next_node;
			--m_Size;
		}

		constexpr T& Front() {
			VORTEX_ASSERT(m_Head != nullptr)
			return m_Head;
		}
		constexpr const T& Front() const {
			VORTEX_ASSERT(m_Head != nullptr)
			return m_Head;
		}

		void Erase(const T& value) {
			if (Front() == value) {
				PopFront();
				return;
			}

			Node* current_node = m_Head;
			while (current_node->Next != nullptr) {
				if (current_node->Next->Data == value) {
					current_node->Next = current_node->Next->Next;

					GetHeapAllocator()->Delete(current_node->Next);
					--m_Size;
					return;
				} else {
					current_node = current_node->Next;
				}
			}
		}

		void EraseAll(const T& value) {
			if (Front() == value) { PopFront(); }

			Node* current_node = m_Head;
			while (current_node->Next != nullptr) {
				if (current_node->Next->Data == value) {
					current_node->Next = current_node->Next->Next;

					GetHeapAllocator()->Delete(current_node->Next);
					--m_Size;
				} else {
					current_node = current_node->Next;
				}
			}
		}

		void Clear() {
			Node* current_node = m_Head;
			while (current_node != nullptr) {
				auto* next_node = current_node->Next;
				GetHeapAllocator()->Delete(current_node);
				current_node = next_node;
			}
			m_Head = nullptr;
			m_Size = 0;
		};

		bool Contains(const T& value) const {
			Node* current_node = m_Head;

			while (current_node != nullptr) {
				if (current_node->Data == value) {
					return true;
				}
				current_node = current_node->Next;
			}
			return false;
		}

		constexpr bool Size() const { return m_Size; }
		constexpr bool Empty() const { return m_Head == nullptr; }

	protected:
		SizeType m_Size;
		Node* m_Head;

	public:
		struct Iterator {
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::forward_iterator_tag;

		public:
			constexpr explicit Iterator(Node* node): m_Node(node) {}
			constexpr Iterator(const Iterator&) = default;
			constexpr Iterator(Iterator&&) noexcept = default;
			virtual ~Iterator() = default;

		public:
			constexpr Iterator& operator=(const Iterator&) = default;
			constexpr Iterator& operator=(Iterator&&) noexcept = default;

		public:
			constexpr Iterator& operator++() {
				m_Node = m_Node->Next;
				return *this;
			}
			constexpr const Iterator operator++(int) {
				return Iterator{m_Node->Next};
			}

		public:
			constexpr bool operator==(const Iterator& other) { return m_Node == other.m_Node; };
			constexpr bool operator!=(const Iterator& other) { return m_Node != other.m_Node; };

			reference operator*() const { return m_Node->Data; }
			pointer operator->() { return &m_Node->Data; }

		protected:
			Node* m_Node;
		};

		struct ConstIterator {
			using difference_type = ptrdiff_t;
			using value_type = T;
			using pointer = T*;
			using reference = T&;
			using iterator_category = std::forward_iterator_tag;

		public:
			constexpr explicit ConstIterator(Node* node): m_Node(node) {}
			constexpr ConstIterator(const ConstIterator&) = default;
			constexpr ConstIterator(ConstIterator&&) noexcept = default;
			virtual ~ConstIterator() = default;

		public:
			constexpr ConstIterator& operator=(const ConstIterator&) = default;
			constexpr ConstIterator& operator=(ConstIterator&&) noexcept = default;

		public:
			constexpr ConstIterator& operator++() const {
				m_Node = m_Node->Next;
				return *this;
			}
			constexpr const ConstIterator operator++(int) const {
				return Iterator{m_Node->Next};
			}

		public:
			constexpr bool operator==(const ConstIterator& other) { return m_Node == other.m_Node; };
			constexpr bool operator!=(const ConstIterator& other) { return m_Node != other.m_Node; };

			reference operator*() const { return m_Node->Data; }
			pointer operator->() const { return &m_Node->Data; }

		protected:
			mutable Node* m_Node;
		};

	public:
		using iterator = Iterator;
		using const_iterator = ConstIterator;

	public:
		iterator begin() const { return iterator{m_Head}; }
		iterator end() const { return iterator{nullptr}; }
		const_iterator cbegin() const { return const_iterator{m_Head}; }
		const_iterator cend() const { return const_iterator{nullptr}; }
	};
}