#ifndef MOVEMENTSYSTEM_H
#define MOVEMENTSYSTEM_H

class MovementSystem : public System {
    public:
        MovementSystem() {

        }

        void Update() {
            for (auto entity : GetEntities()) {
                // Update entity position based on its velocity
            }
        }
};
#endif