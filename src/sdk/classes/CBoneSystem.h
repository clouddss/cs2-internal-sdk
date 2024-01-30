#pragma once

#include <cstdint>

#include "../../sdk/classes/CStrongHandle.h"
#include "../../sdk/helpers/CUtlSymbolLarge.hpp"
#include "../../sdk/helpers/BitFlag.h"
#include "../../sdk/helpers/modules.h"

enum EBoneFlags : uint32_t 
{
    FLAG_NO_BONE_FLAGS = 0x0,
    FLAG_BONEFLEXDRIVER = 0x4,
    FLAG_CLOTH = 0x8,
    FLAG_PHYSICS = 0x10,
    FLAG_ATTACHMENT = 0x20,
    FLAG_ANIMATION = 0x40,
    FLAG_MESH = 0x80,
    FLAG_HITBOX = 0x100,
    FLAG_BONE_USED_BY_VERTEX_LOD0 = 0x400,
    FLAG_BONE_USED_BY_VERTEX_LOD1 = 0x800,
    FLAG_BONE_USED_BY_VERTEX_LOD2 = 0x1000,
    FLAG_BONE_USED_BY_VERTEX_LOD3 = 0x2000,
    FLAG_BONE_USED_BY_VERTEX_LOD4 = 0x4000,
    FLAG_BONE_USED_BY_VERTEX_LOD5 = 0x8000,
    FLAG_BONE_USED_BY_VERTEX_LOD6 = 0x10000,
    FLAG_BONE_USED_BY_VERTEX_LOD7 = 0x20000,
    FLAG_BONE_MERGE_READ = 0x40000,
    FLAG_BONE_MERGE_WRITE = 0x80000,
    FLAG_ALL_BONE_FLAGS = 0xfffff,
    BLEND_PREALIGNED = 0x100000,
    FLAG_RIGIDLENGTH = 0x200000,
    FLAG_PROCEDURAL = 0x400000,
};

struct alignas(16) CBoneData 
{
    Vector position;
    float scale;
    Vector rotation;
};

class CModel 
{
public:
    BitFlag GetBoneFlags(const int index) 
    {
        using fn = int(__thiscall*)(void*, int); //int -> BitFlag?

        static auto addr = modules::client.pattern_scanner.scan("85 D2 78 16 3B 91").as();

        const auto& get_bone_flags = reinterpret_cast<fn>(addr);
       
        if (get_bone_flags)
            return BitFlag(get_bone_flags(this, index));

        return {};
    }

    int GetBoneParent(const int index) 
    {
        using fn = int(__thiscall*)(void*, int);

        static auto addr = modules::client.pattern_scanner.scan("85 D2 78 17 3B 91 70").as();

        const auto& get_bone_parent = reinterpret_cast<fn>(addr);

        if (get_bone_parent)
            return get_bone_parent(this, index);

        return {}; //MJ: Probably better to return -1 ?
    }

    const char* GetBoneName(const std::int32_t index) 
    {
        using fn = const char* (__thiscall*)(void*, int);

        static auto addr = modules::client.pattern_scanner.scan("85 D2 78 25 3B 91").as();

        const auto& get_bone_name = reinterpret_cast<fn>(addr);

        if (get_bone_name)
            return get_bone_name(this, index);

        return nullptr;
    }

public:
    std::uint8_t padding_0[0x170];
    std::int32_t BoneCount;
};

class CModelState 
{
public:
    std::uint8_t padding_0[0x80];
    CBoneData* bones;
    std::uint8_t padding_1[0x18];
    CStrongHandle<CModel> modelHandle;
    CUtlSymbolLarge modelName;
};

