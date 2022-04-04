#pragma once
#include <unordered_map>
#include <vector>

#include "Vortex/Graphics/GraphicsEnum.h"
#include "Vortex/Graphics/BufferLayout.h"

#include "Vortex/Common/HandleMap.h"
#include "Vortex/Common/HashedString.h"
#include "Vortex/Math/VortexMath.h"

namespace Vortex::Graphics {
	struct Texture;
	struct Buffer;
	struct VertexArray;
	struct Shader;
	struct FrameBuffer;
	struct TimerQuery;

	using RenderBackendHandle = BasicHandle<UInt32, 10, 4, 18>;

	using Handle = RenderBackendHandle::UnderlyingType;
	using RenderBackendMap = RenderBackendHandle::Map<
		Texture,
		Buffer,
		VertexArray,
		Shader,
		FrameBuffer,
		TimerQuery>;

	struct Texture {
		PixelFormat::Enum Format;
		UInt16 Size[3];
		UInt16 LayerCount;
		UInt8 BitsPerPixel;
		bool IsCubemap;

		UInt32 BackendID;
	};

	struct Buffer {
		BufferUsage::Enum BufferUsage;
		BufferLayout Layout;
		bool Mutable;
		SizeType Size;

		UInt32 BackendID;
	};

	struct VertexArray {
		Handle IndexBufferHandle;
		std::vector<Handle> VertexBufferHandles;
		bool Indexed;
		SizeType BufferBindIndex;
		Topology::Enum Topology;

		UInt32 BackendID;
	};

	struct Shader {
		std::unordered_map<HashedString, int> Locations;
		std::unordered_map<HashedString, ElementType::Enum> Types;
		std::unordered_map<HashedString, int> BindingPositions;

		UInt32 BackendID;
	};

	struct FrameBuffer {
		UInt16 Size[2];
		Handle Textures[FrameBufferAttachment::Count];

		UInt32 BackendID;
	};

	struct TimerQuery {
		enum {
			Stopped,
			Running,
			Waiting,
		} State;

		UInt32 BackendID;
	};

	class RenderBackend {
	public:
		virtual ~RenderBackend() = default;

	public:    // Buffer
		virtual Handle CreateBuffer(BufferUsage::Enum buffer_usage, const BufferLayout& buffer_layout, SizeType count, const void* data) = 0;
		virtual void UpdateBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, const void* data) = 0;
		virtual void GetBuffer(Handle buffer_handle, SizeType offset, SizeType data_size, void* data) = 0;
		virtual void DestroyBuffer(Handle buffer_handle) = 0;

	public:    // Vertex Array
		virtual Handle CreateVertexArray(Topology::Enum topology) = 0;
		virtual void SetIndexBuffer(Handle vertex_array_handle, Handle index_buffer_handle) = 0;
		virtual void AddVertexBuffer(Handle vertex_array_handle, Handle vertex_buffer_handle) = 0;
		virtual void Draw(Handle vertex_array_handle, SizeType count) const = 0;
		virtual void DrawInstanced(Handle vertex_array_handle, SizeType count, SizeType instance_count) const = 0;
		virtual void DestroyVertexArray(Handle vertex_array_handle) = 0;

	public:    // Texture
		virtual Handle CreateTexture2D(
			const UInt16* size,
			PixelFormat::Enum format,
			const void* pixels,
			TextureLODFilter::Enum min_filter,
			TextureLODFilter::Enum mag_filter,
			TextureWrap::Enum wrap_s,
			TextureWrap::Enum wrap_t,
			bool create_mipmap
		) = 0;
		virtual void UpdateTexture2D(Handle texture_handle, const void* pixels, bool create_mipmap) = 0;
		virtual void GetTexture(Handle texture_handle, UInt16 buffer_size, void* pixels) = 0;
		virtual void DestroyTexture(Handle texture_handle) = 0;

	public:    // Shader
		virtual Handle CreateShader(const char** sources, ShaderType::Enum* types, SizeType count) = 0;
		virtual void BindShader(Handle shader_handle) = 0;
		virtual void SetUniform(Handle shader_handle, HashedString name, const void* data, SizeType count) = 0;
		virtual void Dispatch(const Vortex::Int32* num_groups) = 0;
		virtual void DestroyShader(Handle shader_handle) = 0;

	public:    // Framebuffer
		virtual Handle CreateFrameBuffer(
			const UInt16* size,
			FrameBufferAttachment::Enum* attachments,
			SizeType count
		) = 0;
		virtual void BindFrameBuffer(Handle framebuffer_handle) = 0;
		virtual void BindBackBuffer() = 0;
		virtual void DestroyFrameBuffer(Handle framebuffer_handle) = 0;

	public:    // TimerQuery
		virtual Handle CreateTimerQuery() = 0;
		virtual void BeginTimerQuery(Handle timer_query_handle) = 0;
		virtual bool EndTimerQuery(Handle timer_query_handle, UInt32& value) = 0;
		virtual void DestroyTimerQuery(Handle timer_query_handle) = 0;

	public:
		virtual SizeType GetHardwareLimit(HardwareLimit::Enum type) const = 0;

		virtual void SetState(RenderStates::Enum state, bool active) = 0;
		virtual bool IsEnabled(RenderStates::Enum state) const = 0;
		virtual void SetDepthTest(DepthTesting::Enum depth_testing) const = 0;
		virtual void SetBlending(Blending::Enum blending) const = 0;

		virtual void SetScissor(const RectangleInt& dimension) const = 0;
		virtual void SetViewport(const RectangleInt& size) const = 0;

		virtual void ClearColor(const Color& color) const = 0;
		virtual void ClearDepth(float reset) const = 0;
		virtual void ClearStencil(UInt32 reset) const = 0;

#ifdef VORTEX_DEBUG
	protected:
		inline bool d_BufferChecks(Handle buffer_handle) const {
			bool out{true};
			out &= m_Datas.Is<Buffer>(buffer_handle);
			out &= m_Datas.Contains(buffer_handle);
			return out;
		}
		inline bool d_VertexArrayChecks(Handle vertex_array_handle) const {
			bool out{true};
			out &= m_Datas.Is<VertexArray>(vertex_array_handle);
			out &= m_Datas.Contains(vertex_array_handle);
			return out;
		}
		inline bool d_TextureChecks(Handle texture_handle) const {
			bool out{true};
			out &= m_Datas.Is<Texture>(texture_handle);
			out &= m_Datas.Contains(texture_handle);
			return out;
		}
		inline bool d_ShaderChecks(Handle shader_handle) const {
			bool out{true};
			out &= m_Datas.Is<Shader>(shader_handle);
			out &= m_Datas.Contains(shader_handle);
			return out;
		}
		inline bool d_FrameBufferChecks(Handle framebuffer_handle) const {
			bool out{true};
			out &= m_Datas.Is<FrameBuffer>(framebuffer_handle);
			out &= m_Datas.Contains(framebuffer_handle);
			return out;
		}
		inline bool d_TimerQueryChecks(Handle timer_query_handle) const {
			bool out{true};
			out &= m_Datas.Is<TimerQuery>(timer_query_handle);
			out &= m_Datas.Contains(timer_query_handle);
			return out;
		}
#endif

	public:
		inline bool Contains(Handle handle) const {
			return m_Datas.Contains(handle);
		};

		inline const Buffer& GetBuffer(Handle buffer_handle) const {
			VORTEX_ASSERT(d_BufferChecks(buffer_handle))
			return m_Datas.Get<Buffer>(buffer_handle);
		};
		inline const VertexArray& GetVertexArray(Handle vertex_array) const {
			VORTEX_ASSERT(d_BufferChecks(vertex_array))
			return m_Datas.Get<VertexArray>(vertex_array);
		};
		inline const Texture& GetTexture(Handle texture_handle) const {
			VORTEX_ASSERT(d_TextureChecks(texture_handle))
			return m_Datas.Get<Texture>(texture_handle);
		};
		inline const Shader& GetShader(Handle shader_handle) const {
			VORTEX_ASSERT(d_ShaderChecks(shader_handle))
			return m_Datas.Get<Shader>(shader_handle);
		};
		inline const FrameBuffer& GetFrameBuffer(Handle framebuffer_handle) const {
			VORTEX_ASSERT(d_FrameBufferChecks(framebuffer_handle))
			return m_Datas.Get<FrameBuffer>(framebuffer_handle);
		};
		inline const TimerQuery& GetTimerQuery(Handle timer_query_handle) const {
			VORTEX_ASSERT(d_TimerQueryChecks(timer_query_handle))
			return m_Datas.Get<TimerQuery>(timer_query_handle);
		};

		template<typename T>
		inline const T& Get(Handle handle) const {
			if constexpr(std::is_same_v<T, Buffer>) {
				return GetBuffer(handle);
			} else if constexpr(std::is_same_v<T, VertexArray>) {
				return GetVertexArray(handle);
			} else if constexpr(std::is_same_v<T, Texture>) {
				return GetTexture(handle);
			} else if constexpr(std::is_same_v<T, Shader>) {
				return GetShader(handle);
			} else if constexpr(std::is_same_v<T, FrameBuffer>) {
				return GetFrameBuffer(handle);
			} else if constexpr(std::is_same_v<T, TimerQuery>) {
				return GetTimerQuery(handle);
			}
		};

	protected:
		RenderBackendMap m_Datas;
	};
}