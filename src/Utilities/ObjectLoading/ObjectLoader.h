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

#include <sstream>

#include "Utilities/Math/Math.h"
#include "Core/BoundingObjects/AABB.h"
#include "Utilities/STL/MxHashMap.h"
#include "Utilities/STL/MxString.h"
#include "Utilities/STL/MxVector.h"
#include "Core/Resources/MeshData.h"
#include "Utilities/FileSystem/File.h"

namespace MxEngine
{
	/*!
	material info is a plain data structure, containing info how to load all resources of object material
	*/
	struct MaterialInfo
	{
		/*!
		name of material
		*/
		MxString Name;

		/*!
		albedo texture path
		*/
		FilePath AlbedoMap;
		/*!
		emmisive texture path
		*/
		FilePath EmissiveMap;
		/*!
		height texture path
		*/
		FilePath HeightMap;
		/*!
		normal texture path
		*/
		FilePath NormalMap;
		/*!
		ambient occlusion texture path
		*/
		FilePath AmbientOcclusionMap;
		/*!
		metallic texture path
		*/
		FilePath MetallicMap;
		/*!
		roughness texture path
		*/
		FilePath RoughnessMap;

		/*!
		transparency value
		*/
		float Transparency = 0.0f;
		/*!
		height displacement value
		*/
		float Displacement = 0.0f;
		/*!
		light emmision value
		*/
		float Emission = 0.0f;
		/*!
		object base color
		*/
		Vector3 BaseColor = MakeVector3(1.0f);
		/*!
		uv index scale
		*/
		Vector2 UVMultipliers = MakeVector2(1.0f);
		/*!
		metallic factor value
		*/
		float MetallicFactor = 0.0f;
		/*!
		roughness factor value
		*/
		float RoughnessFactor = 0.75f;
	};

	/*!
	mesh info is a temporary class which stores object model data. Later it is used to construct SubMesh class object
	*/
	struct MeshInfo
	{
		/*!
		name of mesh
		*/
		MxString name;
		/*!
		buffer containing all vertex data
		*/
		MxVector<Vertex> vertecies;
		/*
		face indicies to access buffer of verteces
		*/
		MxVector<uint32_t> indicies;		
		/*!
		mesh material pointer (to external table passed with MeshInfo inside ObjectInfo)
		*/
		MaterialInfo* material = nullptr;
		/*!
		has the mesh texture data (uv-coords) or not
		*/
		bool useTexture = false;
		/*!
		has the mesh normal data (and tangent space) or not
		*/
		bool useNormal = false;
	};

	using MaterialLibrary = MxVector<MaterialInfo>;

	/*!
	object info a special class which contains all data from which in-game object can be constructed
	it includes verteces, materials and precomputed bounding box. Also note that object is guaranteed to be aligned at (0, 0, 0), game world center
	*/
	struct ObjectInfo
	{
		/*!
		list of all materials. MeshInfo references each by pointer, so MaterialLibrary should not be altered after ObjectInfo construction
		*/
		MaterialLibrary materials;
		/*!
		list of all object meshes. For more info see MeshInfo documentation
		*/
		MxVector<MeshInfo> meshes;
	};

	/*!
	object loader is a special class which loads any type of file with object data into MxEngine compatible format (i.e. ObjectInfo)
	it supports same file types as Assimp library does, as it is base on it. For more info check documentation: https://github.com/assimp/assimp
	*/
	class ObjectLoader
	{
	public:
		/*
		loads object from disk by its file path
		\param path absoulute or relative to executable folder path to a file to load
		\returns ObjectInfo instance
		\warning this function is not thread safe
		*/
		static ObjectInfo Load(const FilePath& path);
		static MaterialLibrary LoadMaterials(const FilePath& path);
		static void DumpMaterials(const MaterialLibrary& materials, const FilePath& path);
	};
}