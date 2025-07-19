#pragma once
#ifndef ECS_ASSERT
#define ECS_ASSERT(condition, msg) \
		if (!(condition)) { \
			std::cerr << "[ECS error]: " << msg << std::endl; \
			::abort(); \
		}
#endif
#ifndef ECS_INFO
#ifdef ECS_INFO_ENABLED
#define ECS_INFO(msg) std::cout << "[ECS info]: " << msg << "\n";
#else
#define ECS_INFO(msg);
#endif
#endif
#ifndef ECS_MSG
#define ECS_MSG(msg) std::cout << "[ECS]: " << msg << "\n";
#endif

#define ENTITY_INFO(id) \
		"['" << GetEntityTag(id) << "', ID: " << id << "]"

#define ECS_ASSERT_VALID_ENTITY(id) \
		ECS_ASSERT(id != NULL_ENTITY, "NULL_ENTITY cannot be operated on by the ECS") \
		ECS_ASSERT(id < maxEnity && id >= 0, "Invalid entity ID out of bounds: " + id);

#define ECS_ASSERT_ALIVE_ENTITY(id) \
		ECS_ASSERT(entityComponentMasks.Contains(id), "Attempting to access inactive entity with ID: " + id);
