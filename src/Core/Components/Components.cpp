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

#include "Core/Application/Runtime.h"
#include "Components.h"

namespace MxEngine
{
    void RegisterComponents()
    {
		Runtime::RegisterComponent<CameraController   >();
		Runtime::RegisterComponent<InputController    >();
		Runtime::RegisterComponent<RigidBody          >();
		Runtime::RegisterComponent<CharacterController>();
		Runtime::RegisterComponent<BoxCollider        >();
		Runtime::RegisterComponent<Script             >();
		Runtime::RegisterComponent<Behaviour          >();
		Runtime::RegisterComponent<InstanceFactory    >();
		Runtime::RegisterComponent<Instance           >();
		Runtime::RegisterComponent<Skybox             >();
		Runtime::RegisterComponent<DebugDraw          >();
		Runtime::RegisterComponent<MeshRenderer       >();
		Runtime::RegisterComponent<MeshSource         >();
		Runtime::RegisterComponent<MeshLOD            >();
		Runtime::RegisterComponent<ParticleSystem     >();
		Runtime::RegisterComponent<DirectionalLight   >();
		Runtime::RegisterComponent<PointLight         >();
		Runtime::RegisterComponent<SpotLight          >();
		Runtime::RegisterComponent<CameraEffects      >();
		Runtime::RegisterComponent<CameraSSR          >();
		Runtime::RegisterComponent<CameraSSGI         >();
		Runtime::RegisterComponent<CameraSSAO         >();
		Runtime::RegisterComponent<CameraToneMapping  >();
		Runtime::RegisterComponent<VRCameraController >();
		Runtime::RegisterComponent<AudioSource        >();
		Runtime::RegisterComponent<AudioListener      >();
		Runtime::RegisterComponent<SphereCollider     >();
		Runtime::RegisterComponent<CylinderCollider   >();
		Runtime::RegisterComponent<CapsuleCollider    >();
		Runtime::RegisterComponent<CompoundCollider   >();
    }
}