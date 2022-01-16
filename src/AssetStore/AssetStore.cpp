#include "AssetStore.h"
#include "../Logger/Logger.h"
AssetStore::AssetStore() {
    Logger::Log("Asset store constructor called");
}

AssetStore::~AssetStore() {
    Logger::Log("Asset store destructor called");
    
}