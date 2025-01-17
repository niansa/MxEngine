// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditions and the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditions and the following disclaimer in the documentation
// and /or other materials provided with the distribution.
// 
// 3. Neither the name of the copyright holder nor the names of its
// contributors may be used to endorse or promote products derived from
// this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
// AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
// IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
// DISCLAIMED.IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
// FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
// DAMAGES(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
// CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
// OR TORT(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

#pragma once

#include "Utilities/Array/ArrayView.h"

namespace MxEngine
{
	class VertexBuffer;
	class IndexBuffer;
	struct VertexLayout;

	class VertexArray
	{
		using BindableId = unsigned int;

		BindableId id = 0;
		int attributeIndex = 0;
		void FreeVertexArray();
	public:
		VertexArray();
		~VertexArray();
		VertexArray(const VertexArray&) = delete;
		VertexArray(VertexArray&& array) noexcept;
		VertexArray& operator=(const VertexArray&) = delete;
		VertexArray& operator=(VertexArray&& array) noexcept;

		BindableId GetNativeHandle() const;
		void Bind() const;
		void Unbind() const;
		void AddVertexBuffer(const VertexBuffer& buffer, ArrayView<VertexLayout> layout);
		void AddInstancedVertexBuffer(const VertexBuffer& buffer, ArrayView<VertexLayout> layout);
		void PopBuffer(ArrayView<VertexLayout> layout);
		void LinkIndexBuffer(const IndexBuffer& buffer);
		int GetAttributeCount() const;
	};
}