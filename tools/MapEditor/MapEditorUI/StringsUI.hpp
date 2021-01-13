#ifndef DE_STRINGS_H
#define DE_STRINGS_H

#include <string>

namespace DE {

class StringsUI {
public:
	std::string BoolFalse = "[ ]";
	std::string BoolTrue = "[X]";
	std::string FPS = "FPS:";
	std::string Tile = "Tile";
	std::string Zoom = "Zoom";
	std::string Brush = "Brush";
	std::string Save = "Save";
	std::string Collider = "Colliders";
	std::string Atlas = "Atlas (Tab)";
	std::string Play = "Play";
	std::string AddSprite = "Add Sprite";
	std::string Inspector = "INSPECTOR";
	std::string InspectorTilePosition = "pos";
	std::string InspectorTileX = "x";
	std::string InspectorTileY = "y";
	std::string InspectorTileCollider = "collider";
	std::string InspectorTileTag = "tag";
	std::string InspectorSize = "size";
	std::string InspectorSizeCollider = "size";
	std::string InspectorPosCollider = "offset";
	std::string Grid = "Grid";
};

} /* namespace DE */

#endif /* DE_STRINGS_H */
