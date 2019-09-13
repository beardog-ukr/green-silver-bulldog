#include "TiledMapLoader.h"

#include "MetaTileCode.h"
#include "NineNutsLogger.h"

#include <cmath>                                 // fmod,

const std::string metaLayerName = "meta1-layer"; // TODO: this one is duplicated
                                                 // in TiledMapKeeper

// =============================================================================
// =============================================================================

TiledMapLoader::TiledMapLoader() {
  workNode = nullptr;
}

// =============================================================================

TiledMapLoader::~TiledMapLoader() {
  // nothing to do here, but ensure it's called
  log("%s: obj destroyed", __func__);
}

// =============================================================================

bool TiledMapLoader::findStartPositions() {
  bool result = true;

  TMXLayer *const metaLayer = workNode->getLayer(metaLayerName);

  if (metaLayer == nullptr) {
    return false;
  }

  // perform Search
  dogStartX = 10; // TODO remove
  dogStartY = 12;

  bool dogPosFound    = false;
  bool farmerPosFound = false;

  const Size mapSize = workNode->getMapSize();

  for (int tileX = 0; tileX < mapSize.width; tileX++) {
    for (int tileY = 0; tileY < mapSize.height; tileY++) {
      const int   tileGid = metaLayer->getTileGIDAt(Vec2(tileX, tileY));
      const Value prop    = workNode->getPropertiesForGID(tileGid);

      if (prop.isNull()) {
        continue;
      }

      const ValueMap vm   = prop.asValueMap();
      const auto     frez = vm.find("MetaTileCode");

      if (frez == vm.end()) {
        continue;
      }

      const int metaCode = frez->second.asInt();

      switch (metaCode) {
      case MTC_FFD_FARMER_START:

        if (farmerPosFound) {
          log("%s: additional farmer pos found at %d:%d", __func__, tileX, tileY);
        }

        farmerPosFound = true;
        farmerStartX   = tileX;
        farmerStartY   = mapSize.height - tileY - 1;
        break;

      case MTC_FFD_DOG_START:

        if (dogPosFound) {
          log("%s: additional dog pos found at %d:%d", __func__, tileX, tileY);
        }

        dogPosFound = true;
        dogStartX   = tileX;
        dogStartY   = mapSize.height - tileY - 1;
        break;

        // Note there is no suitable default action here
        // default:
      }

      if (dogPosFound && farmerPosFound) {
        break;
      }
    }

    if (dogPosFound && farmerPosFound) {
      break;
    }
  }

  return dogPosFound && farmerPosFound;
}

// =============================================================================

int TiledMapLoader::getFarmerStartX() const {
  return farmerStartX;
}

// =============================================================================

int TiledMapLoader::getFarmerStartY() const {
  return farmerStartY;
}

// =============================================================================

int TiledMapLoader::getDogStartX() const {
  return dogStartX;
}

// =============================================================================

int TiledMapLoader::getDogStartY() const {
  return dogStartY;
}

// =============================================================================

TMXTiledMap * TiledMapLoader::loadFile(const std::string mapFilename)
{
  workNode = TMXTiledMap::create(mapFilename);

  if (workNode == nullptr)  {
    log("failed to load tiled map.");
    return nullptr;
  }
  else {
    log("map loaded...");
  }

  // --- hide "meta" layer  --------------------
  TMXLayer *layer = workNode->getLayer(metaLayerName);

  if (layer != nullptr) {
    layer->setVisible(false);
  }
  else {
    log("%s: Warning: '%s' layer not found in '%s'", __func__,
        metaLayerName.c_str(), mapFilename.c_str());
  }

  // --- Search for starting positions --------------------
  if (!findStartPositions())  {
    log("%s: failed to find starting positions", __func__);

    // return nullptr;
  }

  // --- finally
  return workNode;
}
