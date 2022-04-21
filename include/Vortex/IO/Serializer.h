#pragma once
#include <pugixml.hpp>
#include <filesystem>

namespace Vortex {
	class Serializer {
		template<typename T>
		friend struct Serializable;

	public:
		inline void New() {
			doc.reset();
			m_CurrentNode = doc.append_child("Vortex");
		}
		inline void Load(const std::filesystem::path& path) { doc.load_file(path.c_str()); }
		inline void Save(const std::filesystem::path& path) const { doc.save_file(path.c_str()); }

	public:
		//returns true on success
		template<typename T>
		bool Serialize(const char* tag, const T& obj) {
			m_CurrentNode = m_CurrentNode.append_child(tag);
			bool out = Serializable<T>::Serialize(*this, obj);
			m_CurrentNode = m_CurrentNode.parent();

			return out;
		}
		//returns true on success
		template<typename T>
		bool Deserialize(const char* tag, T& obj) {
			m_CurrentNode = m_CurrentNode.child(tag);
			bool out = Serializable<T>::Deserialize(*this, obj);
			m_CurrentNode = m_CurrentNode.parent();

			return out;
		}

		template<typename T>
		bool SerializeArray(const char* tag, const T* obj, SizeType count) {
			m_CurrentNode = m_CurrentNode.append_child(tag);
			m_CurrentNode.append_attribute("count").set_value(count);

			bool out{true};
			for (SizeType i = 0; i < count; ++i) {
				m_CurrentNode = m_CurrentNode.append_child("element");
				out &= Serializable<T>::Serialize(*this, obj[i]);
				m_CurrentNode = m_CurrentNode.parent();
			}
			m_CurrentNode = m_CurrentNode.parent();

			return out;
		}
		template<typename T>
		SizeType DeserializeArray(const char* tag, T* obj, SizeType count) {
			m_CurrentNode = m_CurrentNode.child(tag);
			SizeType available_count = m_CurrentNode.attribute("count").as_uint();

			if (available_count > count) {
				available_count = count;
			}

			m_CurrentNode = m_CurrentNode.child("element");
			for (SizeType i = 0; i < available_count; ++i) {
				Serializable<T>::Deserialize(*this, obj[i]);
				m_CurrentNode = m_CurrentNode.next_sibling();
			}
			m_CurrentNode = m_CurrentNode.parent();

			return available_count;
		}

	protected:
		template<typename Type, typename ...T>
		using EIFSame = std::enable_if_t<(std::disjunction_v<std::is_same<Type, T>...>)>;

		template<typename Type>
		using EIFSupportedType = EIFSame<Type,
			Int32,
			UInt32,
			UInt64,
			bool,
			float,
			double,
			const char*>;

		template<typename T, typename = EIFSupportedType<T>>
		bool Set(const char* tag, const T& value) {
			auto attrib = m_CurrentNode.attribute(tag);
			if (!attrib) {
				attrib = m_CurrentNode.append_attribute(tag);
			}
			return attrib.set_value(value);
		}

		template<typename T, typename = EIFSupportedType<T>>
		bool Get(const char* tag, T& value) const {
			auto attrib = m_CurrentNode.attribute(tag);
			if (!attrib) {
				return false;
			}

			if constexpr(std::is_same_v<T, Int32>) {
				value = attrib.as_int();
			} else if constexpr(std::is_same_v<T, UInt32>) {
				value = attrib.as_uint();
			} else if constexpr(std::is_same_v<T, UInt64>) {
				value = attrib.as_ullong();
			} else if constexpr(std::is_same_v<T, bool>) {
				value = attrib.as_bool();
			} else if constexpr(std::is_same_v<T, float>) {
				value = attrib.as_float();
			} else if constexpr(std::is_same_v<T, double>) {
				value = attrib.as_double();
			} else if constexpr(std::is_same_v<T, const char*>) {
				value = attrib.as_string();
			}

			return true;
		}

	protected:
		pugi::xml_document doc;
		pugi::xml_node m_CurrentNode;
	};

	template<typename T>
	struct Serializable {
		static bool Serialize(Serializer& serializer, const T& obj) {
			return serializer.Set<T>("value", obj);
		}
		static bool Deserialize(const Serializer& serializer, T& obj) {
			return serializer.Get<T>("value", obj);
		}
	};
}