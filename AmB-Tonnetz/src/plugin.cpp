#include "plugin.hpp"
#include "rack.hpp"

using namespace rack;
// for reference:
// https://github.com/MarcBoule/MindMeldModular/blob/master/src/ShapeMaster/Widgets.hpp
Plugin *pluginInstance;

void init(Plugin *p) {
  pluginInstance = p;

  // Add modules here
  p->addModel(modelAmB_Tonnetz);

  // Any other plugin initialization...
  p->addModel(modelBaconTest);
}