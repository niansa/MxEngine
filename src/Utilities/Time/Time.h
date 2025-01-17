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

#include <cstdint>
#include <cstddef>

namespace MxEngine
{
	using TimeStep = float;
	using SystemTime = int64_t;

	/*!
	time is a small utility class which uses application window timer to retrieve timestep since engine start
	*/
	struct Time
	{
		/*!
		gets current time of running game
		\returns timestep measured in milliseconds
		*/
		static TimeStep Current();
		/*!
		gets current time since engine start
		\returns timestep measured in milliseconds
		*/
		static TimeStep EngineCurrent();
		/*!
		sets current time since engine start
		\param time new current time
		*/
		static void SetCurrent(TimeStep time);
		/*!
		gets current system time (may not be fast, try avoiding calling it each frame)
		/returns system time (uses chrono std library)
		*/
		static SystemTime System();
		/*!
		gets time passed between frames
		\returns timestep measured in seconds
		*/
		static TimeStep Delta();
		/*!
		gets unscaled time passed since last frame
		\returns timestep measured in seconds
		*/
		static TimeStep UnscaledDelta();
		/*!
		gets average frames per second since last second
		\returns amount of frames per second
		*/
		static size_t FPS();
		/*!
		make current thread to go into sleep state
		\param seconds time in seconds
		*/
		static void Sleep(float seconds);
	};
}