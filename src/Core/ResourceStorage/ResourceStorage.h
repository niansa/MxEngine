// Copyright(c) 2019 - 2020, #Momo
// All rights reserved.
// 
// Redistributionand use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are met :
// 
// 1. Redistributions of source code must retain the above copyright notice, this
// list of conditionsand the following disclaimer.
// 
// 2. Redistributions in binary form must reproduce the above copyright notice,
// this list of conditionsand the following disclaimer in the documentation
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

#include <unordered_map>
#include <string>

#include "Utilities/Memory/Memory.h"
#include "Utilities/Logger/Logger.h"

namespace MxEngine
{
    template<typename T, typename Key = std::string>
    class ResourceStorage
    {
        using BaseStorage = std::unordered_map<Key, UniqueRef<T>>;

        BaseStorage storage;
    public:
        BaseStorage& GetStorage() { return storage; }
        const BaseStorage& GetStorage() const { return storage; }

        T* Get(const Key& key)
        {
            auto it = this->storage.find(key);
            if (it == this->storage.end()) return nullptr;
            return it->second.get();
        }

        bool Exists(const Key& key)
        {
            return this->storage.find(key) != this->storage.end();
        }

        T* Add(const Key& key, UniqueRef<T> value)
        {
            Logger::Instance().Debug("MxEngine::ResourceStorage", "adding resource: " + key);
            T* ret = value.get();
            this->storage[key] = std::move(value);
            return ret;
        }

        void Delete(const Key& key)
        {
            if (this->Exists(key))
            {
                this->storage.erase(key);
                Logger::Instance().Debug("MxEngine::ResourceStorage", "deleting resource: " + key);
            }
        }
    };
}