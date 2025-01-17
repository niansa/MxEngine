#include "InstanceFactory.h"
#include "Core/Components/Rendering/MeshSource.h"
#include "Utilities/Profiler/Profiler.h"
#include "Core/Runtime/Reflection.h"

namespace MxEngine
{
    void InstanceFactory::InitMesh()
    {
        auto& object = MxObject::GetByComponent(*this);
        auto meshSource = object.GetComponent<MeshSource>();
        if (meshSource.IsValid())
        {
            auto& mesh = *meshSource->Mesh;
            auto modelBufferIndex = this->AddInstancedBuffer(mesh, this->GetModelData());
            (void)this->AddInstancedBuffer(mesh, this->GetNormalData());
            (void)this->AddInstancedBuffer(mesh, this->GetColorData());
            this->bufferIndex = modelBufferIndex; // others will be `bufferIndex + 1`, `bufferIndex + 2`
        }
    }

    void InstanceFactory::RemoveInstancedBuffer(Mesh& mesh, size_t index)
    {
        MX_ASSERT(mesh.GetInstancedBufferCount() == index + 1);
        mesh.PopInstancedBuffer();
    }

    void InstanceFactory::RemoveDanglingHandles()
    {
        for (auto& object : this->pool)
        {
            if (!object.IsValid() || !object->HasComponent<Instance>())
                this->pool.Deallocate(this->pool.IndexOf(object));
        }
    }

    void InstanceFactory::Destroy()
    {
        auto& object = MxObject::GetByComponent(*this);
        auto meshSource = object.GetComponent<MeshSource>();

        this->DestroyInstances();

        if (meshSource.IsValid() && meshSource->Mesh.IsValid())
        {
            auto& mesh = *meshSource->Mesh;
            this->RemoveInstancedBuffer(mesh, (size_t)this->bufferIndex + 2);
            this->RemoveInstancedBuffer(mesh, (size_t)this->bufferIndex + 1);
            this->RemoveInstancedBuffer(mesh, (size_t)this->bufferIndex + 0);
        }
    }

    void InstanceFactory::Init()
    {
        this->InitMesh();
    }

    void InstanceFactory::OnUpdate(float timeDelta)
    {
        this->RemoveDanglingHandles();
        if (!this->IsStatic) this->SendInstancesToGPU();
    }

    void InstanceFactory::SubmitInstances()
    {
        this->RemoveDanglingHandles();
        this->SendInstancesToGPU();
    }

    // see SceneSerializer.cpp
    void CloneInstanceInternal(const MxObject::Handle& origin, MxObject::Handle& target);

    MxObject::Handle InstanceFactory::Instanciate()
    {
        auto instance = MxObject::Create();
        auto object = MxObject::GetHandleByComponent(*this);

        this->pool.Allocate(instance);
        auto instanceComponent = instance->AddComponent<Instance>(object);
        CloneInstanceInternal(object, instance);
        return instance;
    }

    void InstanceFactory::DestroyInstances()
    {
        for (auto& object : this->pool)
        {
            MxObject::Destroy(object);
        }
        this->pool.Clear();
    }

    InstanceFactory::ModelData& InstanceFactory::GetModelData()
    {
        MAKE_SCOPE_PROFILER("Instancing::BufferModelData");

        this->models.resize(this->GetCount());
        auto model = this->models.begin();
        auto instance = this->GetInstancePool().begin();

        for (; model != this->models.end(); model++, instance++)
        {
            instance->GetUnchecked()->Transform.GetMatrix(*model);
        }
        if (this->models.empty()) this->models.emplace_back(0.0f);

        return this->models;
    }

    InstanceFactory::NormalData& InstanceFactory::GetNormalData()
    {
        MAKE_SCOPE_PROFILER("Instancing::BufferNormalData");

        this->normals.resize(this->GetCount());
        auto normal = this->normals.begin();
        auto model = this->models.begin();
        auto instance = this->GetInstancePool().begin();
        for (; normal != this->normals.end(); model++, normal++, instance++)
        {
            instance->GetUnchecked()->Transform.GetNormalMatrix(*model, *normal);
        }
        if (this->normals.empty()) this->normals.emplace_back(0.0f);

        return this->normals;
    }

    InstanceFactory::ColorData& InstanceFactory::GetColorData()
    {
        MAKE_SCOPE_PROFILER("Instancing::BufferColorData");

        this->colors.resize(this->GetCount());
        auto instance = this->GetInstancePool().begin();
        for (auto it = this->colors.begin(); it != this->colors.end(); it++, instance++)
        {
            *it = instance->GetUnchecked()->GetComponent<Instance>()->GetColor();
        }
        if (this->colors.empty()) this->colors.emplace_back(0.0f);

        return this->colors;
    }

    InstanceFactory::~InstanceFactory()
    {
        this->Destroy();
    }

    void InstanceFactory::SendInstancesToGPU()
    {
        auto& object = MxObject::GetByComponent(*this);
        auto meshSource = object.GetComponent<MeshSource>();
        if (meshSource.IsValid() && meshSource->Mesh.IsValid())
        {
            auto& mesh = *meshSource->Mesh;

            if ((uint16_t)mesh.GetInstancedBufferCount() < this->bufferIndex + 2)
            {
                this->InitMesh(); // MeshSource was updated, re-init mesh
            }
            else
            {
                this->BufferDataByIndex(mesh, (size_t)this->bufferIndex + 0, this->GetModelData());
                this->BufferDataByIndex(mesh, (size_t)this->bufferIndex + 1, this->GetNormalData());
                this->BufferDataByIndex(mesh, (size_t)this->bufferIndex + 2, this->GetColorData());
            }
        }
    }

    bool IsInstanced(const MxObject& object)
    {
        return object.HasComponent<InstanceFactory>();
    }

    bool IsInstanced(MxObject::Handle object)
    {
        return object.IsValid() && IsInstanced(*object);
    }

    bool IsInstance(const MxObject& object)
    {
        return object.HasComponent<Instance>();
    }

    bool IsInstance(MxObject::Handle object)
    {
        return object.IsValid() && IsInstance(*object);
    }

    MxObject::Handle Instanciate(MxObject& object)
    {
        auto instanceFactory = object.GetOrAddComponent<InstanceFactory>();
        return instanceFactory->Instanciate();
    }

    MxObject::Handle Instanciate(MxObject::Handle object)
    {
        return object.IsValid() ? Instanciate(*object) : MxObject::Handle{ };
    }

    MxObject::Handle GetInstanceParent(const MxObject& object)
    {
        auto instance = object.GetComponent<Instance>();
        return instance.IsValid() ? instance->GetParent() : MxObject::Handle{ };
    }

    MxObject::Handle GetInstanceParent(MxObject::Handle object)
    {
        return object.IsValid() ? GetInstanceParent(*object) : MxObject::Handle{ };
    }

    MXENGINE_REFLECT_TYPE
    {
        using GetPoolFunc = VectorPool<MxObject::Handle>& (InstanceFactory::*)();

        rttr::registration::class_<Instance>("Instance")
            (
                rttr::metadata(MetaInfo::FLAGS, MetaInfo::CLONE_COPY)
            )
            .property("color", &Instance::GetColor, &Instance::SetColor)
            (
                rttr::metadata(MetaInfo::FLAGS, MetaInfo::SERIALIZABLE | MetaInfo::EDITABLE),
                rttr::metadata(EditorInfo::INTERPRET_AS, InterpretAsInfo::COLOR)
            );

        rttr::registration::class_<InstanceFactory>("InstanceFactory")
            .constructor<>()
            .method("instanciate", &InstanceFactory::Instanciate)
            (
                rttr::metadata(MetaInfo::FLAGS, MetaInfo::EDITABLE)
            )
            .method("destroy insances", &InstanceFactory::DestroyInstances)
            (
                rttr::metadata(MetaInfo::FLAGS, MetaInfo::EDITABLE)
            )
            .property("is static", &InstanceFactory::IsStatic)
            (
                rttr::metadata(MetaInfo::FLAGS, MetaInfo::SERIALIZABLE | MetaInfo::EDITABLE)
            )
            .property_readonly("instance count", &InstanceFactory::GetCount)
            (
                rttr::metadata(MetaInfo::FLAGS, MetaInfo::EDITABLE)
            )
            .property_readonly("instances", (GetPoolFunc)&InstanceFactory::GetInstancePool)
            (
                rttr::metadata(MetaInfo::FLAGS, MetaInfo::SERIALIZABLE | MetaInfo::EDITABLE),
                rttr::metadata(EditorInfo::CUSTOM_VIEW, GUI::EditorExtra<InstanceFactory>),
                rttr::metadata(SerializeInfo::CUSTOM_SERIALIZE, SerializeExtra<InstanceFactory>),
                rttr::metadata(SerializeInfo::CUSTOM_DESERIALIZE, DeserializeExtra<InstanceFactory>)
            );
    }
}