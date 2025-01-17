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

#include "MeshRenderer.h"
#include "Utilities/ObjectLoading/ObjectLoader.h"
#include "Core/Resources/AssetManager.h"
#include "Core/Runtime/Reflection.h"

namespace MxEngine
{
	void MakeTexture(TextureHandle& currentTexture, MxHashMap<StringId, TextureHandle>& textures, const FilePath& path, TextureFormat format)
	{
		if (!path.empty()) 
		{
			auto id = MakeStringId(path.string());
			if (textures.find(id) == textures.end())
			{
				textures[id] = GraphicFactory::Create<Texture>(path, format);
			}
			currentTexture = textures[id];
		}
	}

	MaterialHandle ConvertMaterial(const MaterialInfo& mat, MxHashMap<StringId, TextureHandle>& textures)
	{
		auto materialResource = ResourceFactory::Create<Material>();
		auto& material = *materialResource;

		MakeTexture(material.AlbedoMap, textures, mat.AlbedoMap, TextureFormat::RGBA);
		MakeTexture(material.EmissiveMap, textures, mat.EmissiveMap, TextureFormat::R);
		MakeTexture(material.HeightMap, textures, mat.HeightMap, TextureFormat::R);
		MakeTexture(material.NormalMap, textures, mat.NormalMap, TextureFormat::RG);
		MakeTexture(material.MetallicMap, textures, mat.MetallicMap, TextureFormat::R);
		MakeTexture(material.RoughnessMap, textures, mat.RoughnessMap, TextureFormat::R);
		MakeTexture(material.AmbientOcclusionMap, textures, mat.AmbientOcclusionMap, TextureFormat::R);

		material.Emission = mat.Emission;
		material.Transparency = mat.Transparency;
		material.BaseColor = mat.BaseColor;
		material.MetallicFactor = mat.MetallicFactor;
		material.RoughnessFactor = mat.RoughnessFactor;
		material.UVMultipliers = mat.UVMultipliers;
		material.Name = mat.Name;

		return materialResource;
	}

	MeshRenderer::MeshRenderer()
		: Materials(1, ResourceFactory::Create<Material>()) { }

	MeshRenderer::MeshRenderer(MaterialRef material)
		: Materials(1, std::move(material)) { }

	MeshRenderer::MeshRenderer(MaterialArray materials)
		: Materials(std::move(materials)) { }

	MeshRenderer& MeshRenderer::operator=(MaterialRef material)
	{
		this->Materials = MaterialArray{ 1, material };
		return *this;
	}

	MeshRenderer& MeshRenderer::operator=(MaterialArray materials)
	{
		this->Materials = std::move(materials);
		return *this;
	}

	MeshRenderer::MaterialRef MeshRenderer::GetMaterial() const
	{
		MX_ASSERT(!Materials.empty()); 
		return this->Materials[0];
	}

	MeshRenderer::MaterialArray MeshRenderer::LoadMaterials(const FilePath& path)
	{
		MaterialArray materials;
		MxHashMap<StringId, TextureHandle> textures;
		auto matlibExtenstion = MeshRenderer::GetMaterialFileExtenstion();
		FilePath actualPath = path;
		if (path.extension() != matlibExtenstion)
			actualPath = path.native() + matlibExtenstion.native();

		auto materialLibrary = ObjectLoader::LoadMaterials(actualPath);

		materials.resize(materialLibrary.size());
		for (size_t i = 0; i < materialLibrary.size(); i++)
		{
			materials[i] = ConvertMaterial(materialLibrary[i], textures);
		}

		return materials;
	}

	const static FilePath materialFileExtenstion = ".mx_matlib";
	const FilePath& MeshRenderer::GetMaterialFileExtenstion()
	{
		return materialFileExtenstion;
	}

	MXENGINE_REFLECT_TYPE
	{
		rttr::registration::class_<MeshRenderer>("MeshRenderer")
			(
				rttr::metadata(MetaInfo::FLAGS, MetaInfo::CLONE_COPY)
			)
			.constructor<>()
			.method("load materials", &MeshRenderer::GetMaterial)
			(
				rttr::metadata(MetaInfo::FLAGS, MetaInfo::EDITABLE),
				rttr::metadata(EditorInfo::CUSTOM_VIEW, GUI::EditorExtra<MeshRenderer>)
			)
			.property("materials", &MeshRenderer::Materials)
			(
				rttr::metadata(MetaInfo::FLAGS, MetaInfo::SERIALIZABLE | MetaInfo::EDITABLE)
			);
	}
}