#include <vector>

#include "Netvars.h"
#include "../sdk.h"

namespace netvars
{
    void init()
    {
        std::vector<const char*> netvar_modules = { "client.dll", "engine2.dll", "schemasystem.dll" };

        for (const auto& module : netvar_modules)
        {
            const auto& netvar_class = g::schema_system->FindTypeScopeForModule(module);
            if (!netvar_class)
                continue;

            auto classes = netvar_class->GetClasses();

            for (const auto& class_binding : classes.GetElements())
            {
                const auto& class_info = netvar_class->FindDeclaredClass(class_binding->m_name);

                for (auto j = 0; j < class_info->m_fields_size; j++)
                {
                    const auto& field = &class_info->m_fields[j];

                    if (!field)
                        continue;

                    char name_hashed[256];

                    strcpy_s(name_hashed, class_binding->m_name);
                    strcat_s(name_hashed, "->");
                    strcat_s(name_hashed, field->m_name);

                    const auto hash = fnv::hash_runtime(name_hashed);

                    netvars_data[hash] = field->m_single_inheritance_offset;

                    //if (!strstr(class_binding->m_name, "CEntityInstance"));
                        //printf("DEBUG: %s->%s: 0x%p\n", class_binding->m_name, field->m_name, (uintptr_t)field->m_single_inheritance_offset);
                }
            }
        }
    }

    uintptr_t get_offset_by_hash_cached(const fnv::hash hash)
    {
        uintptr_t offset(0);

        if (!offset)
            offset = netvars_data[hash];

        return offset;
    }
}

