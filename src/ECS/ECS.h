#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>
#include <set>
#include "../Logger/Logger.h"

const unsigned int MAX_COMPONENTS = 32;

///////////////////////////////////////////////////////////////////////////
// Signature
///////////////////////////////////////////////////////////////////////////
// We use a bitset to track which components an entity has and also helps
// track which entities a system is interested in
///////////////////////////////////////////////////////////////////////////

typedef std::bitset<MAX_COMPONENTS> Signature;

struct IComponent {
    protected:
        static int nextId;
};

// Used to assign a unique id to component type
template <typename T>
class Component : public IComponent {
    // returns unique id of Component<T>
    static int GetId () {
        static auto id = nextId++;
        return id;
    }
};

class Entity {
    private:
        int id;
    public:
        Entity(int id): id(id) {};
        Entity(const Entity& entity) = default;
        int GetId() const;
        Entity& operator = (const Entity& other) = default;
        bool operator == (const Entity& other) const { return id == other.id; }
        bool operator != (const Entity& other) const { return id != other.id; }
        bool operator > (const Entity& other) const { return id > other.id; }
        bool operator < (const Entity& other) const { return id > other.id; }

};

///////////////////////////////////////////////////////////////////////////
// System
///////////////////////////////////////////////////////////////////////////
// This system processes entities that contain a specific signature
///////////////////////////////////////////////////////////////////////////

class System {
    private:
        Signature componentSignature ;
        std::vector<Entity> entities;
    public:
        System() = default;
        ~System() = default;
        void AddEntityToSystem(Entity entity);
        void RemoveEntityFromSystem(Entity entity);
        std::vector<Entity> GetSystemEntities() const;
        const Signature& GetComponentSignature() const;

        // Defines the component type that entities must have to be considered
        // by the system
        template <typename TComponent> void RequireComponent();
};

/////////////////////////////////////////////////////////////////////////////
// Pool vector (contiguous data) of objects of type T
/////////////////////////////////////////////////////////////////////////////
class IPool{
    public:
        virtual ~IPool() {} ;
};
template <typename T>
class Pool : public IPool {
    private:
        std::vector<T> data;
    public:
        Pool(int size=100)  {
            data.resize(size);
        }
        virtual ~Pool() = default;

        bool isEmpty() const {
            return data.empty();
        }

        int GetSize() {
            return data.size();
        }

        void resize(int n) {
            data.resize(n);
        }

        void Clear() {
            data.clear();
        }

        void Add(T object) {
            data.push_back(object);
        }

        void Set(int index, T object) {
            data[index] = object;
        }

        T& Get (int index ) {
            return static_cast<T&>(data[index]);
        }

        T& operator [](unsigned int index) {
            return data[index];
        }
};

/////////////////////////////////////////////////////////////////////////////
// Manages the creation and destruction of entities adds systems and
// components
/////////////////////////////////////////////////////////////////////////////
class Registry {
    private:
        int numEntities = 0;
        // Vector of component pools, each pool has all the data for
        // a certain component type. 
        // Vector index = the component type id
        // Pool index = entity id
        std::vector<std::shared_ptr<IPool>> componentPools;
        
        // Vector of component signatures per entity, tells which component is
        // "on" for a given entity
        // vector index = entity id
        std::vector<Signature>  entityComponentSignatures;

        std::unordered_map<std::type_index, std::shared_ptr<System>> systems;

        // set of entities flagged to be added or removed in the next update.
        std::set<Entity> entitiesToBeAdded;
        std::set<Entity> entitiesToBeKilled;



    public:
        Registry() {
            Logger::Log("Registry created");
        };

        ~Registry() {
            Logger::Log("Registry destroyed");
        }

        Entity CreateEntity();

        void Update();

        // Component management
        template <typename TComponent, typename ...TArgs> void AddComponent(Entity entity, TArgs&& ...args);

        // Ask to remove component from an entity
        template <typename TComponent> void RemoveComponent(Entity entity);
        // Checks if an entity has component TComponent
        template <typename TComponent> bool HasComponent(Entity entity) const;

        // System functions
        template<typename TSystem, typename ...TArgs> void AddSystem(TArgs&& ...args);
        template<typename TSystem> void RemoveSystem();
        template<typename TSystem> bool HasSystem() const;
        template<typename TSystem> TSystem& GetSystem() const;

        // checks the component signature of an entity and add the entity to the systems
        // that are interested in it
        void AddEntityToSystems(Entity entity);
};

// Systems

template<typename TSystem, typename ...TArgs>
void Registry::AddSystem(TArgs&& ...args) {
    std::shared_ptr<TSystem> newSystem(std::make_shared<TSystem>(std::forward<TArgs>(args)...));
    systems.insert(std::make_pair(std::type_index(typeid(TSystem)),newSystem));
}

template<typename TSystem>
void Registry::RemoveSystem() {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    systems.erase(system);
}

template<typename TSystem>
bool Registry::HasSystem() const {
    return systems.find(std::type_index(typeid(TSystem))) != systems.end();
}

template<typename TSystem>
TSystem& Registry::GetSystem() const {
    auto system = systems.find(std::type_index(typeid(TSystem)));
    return *(std::static_pointer_cast<TSystem>(system->second));
}

/// Component
template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}

template <typename TComponent, typename ...TArgs>
void Registry::AddComponent(Entity entity, TArgs&& ...args) {
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    if(componentId >= componentPools.size() ) {
        componentPools.resize(componentId+1,nullptr);
    }

    if(componentPools[componentId] == nullptr) {
        std::shared_ptr<Pool<TComponent>> newComponentPool = std::make_shared<Pool<TComponent>>();
        componentPools[componentId] = newComponentPool;
    }

    std::shared_ptr<Pool<TComponent>> componentPool = std::static_pointer_cast<Pool<TComponent>>(componentPools[componentId]);

    if(entityId >= componentPool->GetSize()) {
        componentPool->resize(numEntities);
    }

    TComponent newComponent(std::forward<TArgs>(args)...);

    componentPool->Set(entityId,newComponent);

    entityComponentSignatures[entityId].set(componentId);
}

template <typename TComponent>
void Registry::RemoveComponent(Entity entity){
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();
    entityComponentSignatures[entityId].set(componentId,true);
}

template <typename TComponent>
bool Registry::HasComponent(Entity entity) const{
    const auto componentId = Component<TComponent>::GetId();
    const auto entityId = entity.GetId();

    return entityComponentSignatures[entityId].test(componentId);
}


#endif