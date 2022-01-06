#ifndef ECS_H
#define ECS_H

#include <bitset>
#include <vector>
#include <unordered_map>
#include <typeindex>

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
        Pool(int size=100) const {
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
        std::vector<IPool*> componentPools;
        
        // Vector of component signatures per entity, tells which component is
        // "on" for a given entity
        // vector index = entity id
        std::vector<Signature>  entityComponentSignatures;

        std::unordered_map<std::type_index, System*> system;

    public:
        Registry() = default;
        
};

template <typename TComponent>
void System::RequireComponent() {
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}
#endif