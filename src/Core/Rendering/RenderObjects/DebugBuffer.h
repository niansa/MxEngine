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

#include "Platform/GraphicAPI.h"

#pragma once

namespace MxEngine
{
	class AABB;
	class BoundingBox;
	class BoundingSphere;
	class Cone;
	class Cylinder;
	class Capsule;
	class Frustrum;
	class Line;
	class Rectangle;
	class Circle;

	class DebugBuffer
	{
		struct Point
		{
			Vector3 position;
			Vector4 color;
		};

		using FrontendStorage = MxVector<Point>;

		VertexBufferHandle VBO;
		VertexArrayHandle VAO;

		FrontendStorage storage;
	public:
		bool DrawAsScreenOverlay = false;

		void Init();
		void Submit(const Line& line, const Vector4& color);
		void Submit(const AABB& box, const Vector4& color);
		void Submit(const BoundingBox& box, const Vector4 color);
		void Submit(const BoundingSphere& sphere, const Vector4 color);
		void Submit(const Cone& cone, const Vector4& color);
		void Submit(const Frustrum& frustrum, const Vector4& color);
		void Submit(const Cylinder& cylinder, const Vector4& color);
		void Submit(const Capsule& capsule, const Vector4& color);
		void Submit(const Rectangle& rectangle, const Vector4& color);
		void Submit(const Circle& circle, const Vector4& color);

		void ClearBuffer(); 
		void SubmitBuffer();
		size_t GetSize() const;
		VertexArrayHandle GetVAO() const;
	};
}