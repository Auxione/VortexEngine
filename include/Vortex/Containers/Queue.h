#pragma once
#include "Vortex/Memory/HeapAllocator.h"

namespace Vortex {
	template<typename T>
	struct Queue {
		struct Node {
			Node* Next;
			Byte Data[sizeof(T)];
		};

	public:
		Queue()
			: m_Size{0}, m_Head{nullptr}, m_Tail{nullptr} {
		};

		Queue(const Queue<T>& other) = delete;

		Queue(Queue<T>&& other) noexcept
			: m_Size{0}, m_Head{other.m_Head}, m_Tail{other.m_Tail} {
			other.m_Tail = nullptr;
			other.m_Head = nullptr;
		};

		~Queue() {
			while (m_Head != nullptr) {
				Pop();
			}
		};

	public:
		bool operator==(const Queue<T>& other) const { return m_Head == other.m_Head && m_Tail == other.m_Tail; }
		bool operator!=(const Queue<T>& other) const { return m_Head != other.m_Head || m_Tail != other.m_Tail; }

		constexpr Queue<T>& operator=(const Queue<T>& other) = delete;

		Queue<T>& operator=(Queue<T>&& other) noexcept {
			if (*this != other) {
				auto* head = other.m_Head;
				auto* tail = other.m_Tail;
				auto size = other.m_Size;

				other.m_Tail = nullptr;
				other.m_Head = nullptr;

				Clear();
				m_Head = head;
				m_Tail = tail;
				m_Size = size;
			}
			return *this;
		}

	public:
		void Push(const T& value) {
			Node* temp = m_Tail;
			Node* pushed_node = GetHeapAllocator()->New<Node>();

			pushed_node->Next = nullptr;
			std::memcpy(pushed_node->Data, &value, sizeof(T));

			if (m_Tail != nullptr) {
				temp->Next = pushed_node;
			} else {
				m_Head = pushed_node;
			}
			m_Tail = pushed_node;
			++m_Size;
		}

		template<typename ... Args>
		void Emplace(Args&& ... args) {
			Node* temp = m_Tail;
			Node* pushed_node = GetHeapAllocator()->New<Node>();
			pushed_node->Next = nullptr;

			new(&pushed_node->Data) T(std::forward<Args>(args)...);

			if (m_Tail != nullptr) {
				temp->Next = pushed_node;
			} else {
				m_Head = pushed_node;
			}
			m_Tail = pushed_node;
			++m_Size;
		}

		void Pop() {
			if (m_Head != nullptr) {
				Node* old_node = m_Head;
				m_Head = old_node->Next;

				if (m_Head == nullptr) { m_Tail = nullptr; }

				GetHeapAllocator()->Delete(old_node);
			}
			--m_Size;
		}

		void Clear() {
			while (m_Tail != nullptr) {
				Pop();
			}
			m_Size = 0;
		}

	public:
		T& Front() { return *reinterpret_cast<T*>(m_Head->Data); }
		const T& Front() const { return *reinterpret_cast<T*>(m_Head->Data); }

	public:
		bool Empty() const { return m_Tail == nullptr; }
		SizeType Size() const { return m_Size; }

	protected:
		SizeType m_Size;
		Node* m_Head;
		Node* m_Tail;
	};
}