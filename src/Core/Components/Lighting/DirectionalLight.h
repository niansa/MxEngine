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

#include "Platform/GraphicAPI.h"
#include "Utilities/ECS/Component.h"
#include "LightBase.h"

namespace MxEngine
{
    class MxObject;

    class DirectionalLight : public LightBase
    {
        MAKE_COMPONENT(DirectionalLight);
    public:
        constexpr static size_t TextureCount = 3;
    private:
        using TimerHandle = std::aligned_storage_t<sizeof(DirectionalLight::Handle)>;

        TimerHandle timerHandle;
        [[nodiscard]] const MxObject& GetUpdateTimerHandle() const;
    public:
        TextureHandle DepthMap;

        [[nodiscard]] bool IsFollowingViewport() const;
        void SetIsFollowingViewport(bool value);

        DirectionalLight();
        ~DirectionalLight();

        Vector3 Direction = MakeVector3(0.0f, 1.0f, 0.0f);
        Vector3 CascadeDirection{ 0.0f };
        std::array<float, TextureCount> Projections = { 15.0f, 150.0f, 1500.0f };

        [[nodiscard]] Matrix4x4 GetMatrix(const Vector3& center, size_t index) const;
        void FollowViewport();
    };
}