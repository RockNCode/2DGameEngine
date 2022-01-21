#ifndef RENDERSYSTEM_H
#define RENDERSYSTEM_H

#include "../ECS/ECS.h"
#include "../Components/TransformComponent.h"
#include "../Components/SpriteComponent.h"
#include "../AssetStore/AssetStore.h"

#include <SDL2/SDL.h>

class RenderSystem: public System {
    public:
        RenderSystem() {
            RequireComponent<TransformComponent>();
            RequireComponent<SpriteComponent>();
        }

        void Update(SDL_Renderer* renderer,std::unique_ptr<AssetStore>& assetStore) {
            // Sort all entities by zIndex
            struct RenderableEntity {
                TransformComponent transformComponent;
                SpriteComponent spriteComponent;
            };

            std::vector<RenderableEntity> renderableEntities;

            for (auto entity: GetSystemEntities() ) {
                RenderableEntity renderableEntity ;
                renderableEntity.transformComponent = entity.GetComponent<TransformComponent>();
                renderableEntity.spriteComponent = entity.GetComponent<SpriteComponent>();
                renderableEntities.emplace_back(renderableEntity);
            }

            std::sort(renderableEntities.begin(),renderableEntities.end(),
                []( const RenderableEntity &a, const RenderableEntity &b ) {
                    return a.spriteComponent.zIndex < b.spriteComponent.zIndex;
                }
            );
            // Loop all entities that the system is interested in
            for (auto entity: renderableEntities) {
                const auto transform = entity.transformComponent;
                const auto sprite = entity.spriteComponent;
                // Set the source rect
                SDL_Rect srcRect = sprite.srcRect;

                // Set the destination rect
                SDL_Rect dstRect = {
                    static_cast<int>(transform.position.x),
                    static_cast<int>(transform.position.y),
                    static_cast<int>(sprite.width * transform.scale.x),
                    static_cast<int>(sprite.height * transform.scale.y)
                };
                SDL_RenderCopyEx(renderer,
                    assetStore->GetTexture(sprite.assetId),
                    &srcRect,
                    &dstRect,
                    transform.rotation,
                    NULL,
                    SDL_FLIP_NONE
                    );
            }
        }
};

#endif
