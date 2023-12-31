#define NVOICES 1
/* ------------------------------------------------------------
author: "Oli Larkin (contact@olilarkin.co.uk)"
copyright: "Oliver Larkin"
name: "tambura", "Tambura"
version: "1.0"
Code generated with Faust 2.69.9 (https://faust.grame.fr)
Compilation options: -a
/usr/local/share/faust/vcvrack/template/src/FaustModule.cpp -lang cpp -i -os0
-fpga-mem 10000 -ct 1 -cn tambura -es 1 -mcd 16 -single -ftz 0
------------------------------------------------------------ */

#ifndef __tambura_H__
#define __tambura_H__

/************************************************************************
 IMPORTANT NOTE : this file contains two clearly delimited sections :
 the ARCHITECTURE section (in two parts) and the USER section. Each section
 is governed by its own copyright and license. Please check individually
 each section for license and copyright information.
 *************************************************************************/

/*******************BEGIN ARCHITECTURE SECTION (part 1/2)****************/

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2020 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This Architecture section is free software; you can redistribute it
 and/or modify it under the terms of the GNU General Public License
 as published by the Free Software Foundation; either version 3 of
 the License, or (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.

 You should have received a copy of the GNU General Public License
 along with this program; If not, see <http://www.gnu.org/licenses/>.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.

 ************************************************************************
 ************************************************************************/

// #include "faust/dsp/dsp.h"
// dsp.h file is included and dsp file is renamed to rack_dsp to avoid namespace
// conflicts. Then 'faust -scn rack_dsp' is used in faust2vcvrack.

#include <algorithm>
#include <assert.h>
#include <functional>
#include <iostream>
#include <string>
#include <vector>

// Rack needed ressources
#include <rack.hpp>
using namespace rack;
static Plugin *pluginInstance;

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#ifdef _WIN32
typedef unsigned int uint
#endif

    /*
    // Pixels are just a weird unit, defined by VCV Rack as 1/75 inches, or
    0.338(6) mm
    */

    using namespace std;

struct UI;
struct Meta;

/**
 * Signal processor definition.
 */

class rack_dsp {

public:
  rack_dsp() {}
  virtual ~rack_dsp() {}

  virtual int getNumInputs() = 0;
  virtual int getNumOutputs() = 0;
  virtual void buildUserInterface(UI *ui_interface) = 0;
  virtual int getSampleRate() = 0;
  virtual void init(int sample_rate) = 0;
  virtual void instanceInit(int sample_rate) = 0;
  virtual void instanceConstants(int sample_rate) = 0;
  virtual void instanceResetUserInterface() = 0;
  virtual void instanceClear() = 0;
  virtual rack_dsp *clone() = 0;
  virtual void metadata(Meta *m) = 0;
  virtual void compute(int count, FAUSTFLOAT **inputs,
                       FAUSTFLOAT **outputs) = 0;
  virtual void compute(double /*date_usec*/, int count, FAUSTFLOAT **inputs,
                       FAUSTFLOAT **outputs) {
    compute(count, inputs, outputs);
  }
};

template <typename REAL> struct one_sample_dsp : public rack_dsp {

  int *iZone;
  REAL *fZone;

  one_sample_dsp() : iZone(nullptr), fZone(nullptr) {}

  one_sample_dsp(int *icontrol, REAL *fcontrol)
      : iZone(icontrol), fZone(fcontrol) {
    assert(false);
  }

  virtual ~one_sample_dsp() {
    delete[] iZone;
    delete[] fZone;
  }

  void initControl() {
    iZone = new int[getNumIntControls()];
    fZone = new FAUSTFLOAT[getNumRealControls()];
  }

  virtual int getNumIntControls() = 0;
  virtual int getNumRealControls() = 0;

  virtual void control(int *iControl, REAL *fControl) = 0;

  virtual void compute(FAUSTFLOAT *inputs, FAUSTFLOAT *outputs, int *iControl,
                       REAL *fControl) = 0;

  virtual void compute(int count, FAUSTFLOAT **inputs_aux,
                       FAUSTFLOAT **outputs_aux) {}

  virtual void compute(double date_usec, int count, FAUSTFLOAT **inputs,
                       FAUSTFLOAT **outputs) {
    compute(count, inputs, outputs);
  }
};

/************************** BEGIN DecoratorUI.h **************************
 FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
*************************************************************************/

#ifndef Decorator_UI_H
#define Decorator_UI_H

/************************** BEGIN UI.h *****************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __UI_H__
#define __UI_H__

/************************************************************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***************************************************************************/

#ifndef __export__
#define __export__

// Version as a global string
#define FAUSTVERSION "2.69.9"

// Version as separated [major,minor,patch] values
#define FAUSTMAJORVERSION 2
#define FAUSTMINORVERSION 69
#define FAUSTPATCHVERSION 9

// Use FAUST_API for code that is part of the external API but is also compiled
// in faust and libfaust Use LIBFAUST_API for code that is compiled in faust and
// libfaust

#ifdef _WIN32
#pragma warning(disable : 4251)
#ifdef FAUST_EXE
#define FAUST_API
#define LIBFAUST_API
#elif FAUST_LIB
#define FAUST_API __declspec(dllexport)
#define LIBFAUST_API __declspec(dllexport)
#else
#define FAUST_API
#define LIBFAUST_API
#endif
#else
#ifdef FAUST_EXE
#define FAUST_API
#define LIBFAUST_API
#else
#define FAUST_API __attribute__((visibility("default")))
#define LIBFAUST_API __attribute__((visibility("default")))
#endif
#endif

#endif

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

/*******************************************************************************
 * UI : Faust DSP User Interface
 * User Interface as expected by the buildUserInterface() method of a DSP.
 * This abstract class contains only the method that the Faust compiler can
 * generate to describe a DSP user interface.
 ******************************************************************************/

struct Soundfile;

template <typename REAL> struct FAUST_API UIReal {

  UIReal() {}
  virtual ~UIReal() {}

  // -- widget's layouts

  virtual void openTabBox(const char *label) = 0;
  virtual void openHorizontalBox(const char *label) = 0;
  virtual void openVerticalBox(const char *label) = 0;
  virtual void closeBox() = 0;

  // -- active widgets

  virtual void addButton(const char *label, REAL *zone) = 0;
  virtual void addCheckButton(const char *label, REAL *zone) = 0;
  virtual void addVerticalSlider(const char *label, REAL *zone, REAL init,
                                 REAL min, REAL max, REAL step) = 0;
  virtual void addHorizontalSlider(const char *label, REAL *zone, REAL init,
                                   REAL min, REAL max, REAL step) = 0;
  virtual void addNumEntry(const char *label, REAL *zone, REAL init, REAL min,
                           REAL max, REAL step) = 0;

  // -- passive widgets

  virtual void addHorizontalBargraph(const char *label, REAL *zone, REAL min,
                                     REAL max) = 0;
  virtual void addVerticalBargraph(const char *label, REAL *zone, REAL min,
                                   REAL max) = 0;

  // -- soundfiles

  virtual void addSoundfile(const char *label, const char *filename,
                            Soundfile **sf_zone) = 0;

  // -- metadata declarations

  virtual void declare(REAL * /*zone*/, const char * /*key*/,
                       const char * /*val*/) {}

  // To be used by LLVM client
  virtual int sizeOfFAUSTFLOAT() { return sizeof(FAUSTFLOAT); }
};

struct FAUST_API UI : public UIReal<FAUSTFLOAT> {
  UI() {}
  virtual ~UI() {}
};

#endif
/**************************  END  UI.h **************************/

//----------------------------------------------------------------
//  Generic UI empty implementation
//----------------------------------------------------------------

class FAUST_API GenericUI : public UI {

public:
  GenericUI() {}
  virtual ~GenericUI() {}

  // -- widget's layouts
  virtual void openTabBox(const char *label) {}
  virtual void openHorizontalBox(const char *label) {}
  virtual void openVerticalBox(const char *label) {}
  virtual void closeBox() {}

  // -- active widgets
  virtual void addButton(const char *label, FAUSTFLOAT *zone) {}
  virtual void addCheckButton(const char *label, FAUSTFLOAT *zone) {}
  virtual void addVerticalSlider(const char *label, FAUSTFLOAT *zone,
                                 FAUSTFLOAT init, FAUSTFLOAT min,
                                 FAUSTFLOAT max, FAUSTFLOAT step) {}
  virtual void addHorizontalSlider(const char *label, FAUSTFLOAT *zone,
                                   FAUSTFLOAT init, FAUSTFLOAT min,
                                   FAUSTFLOAT max, FAUSTFLOAT step) {}
  virtual void addNumEntry(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                           FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {}

  // -- passive widgets
  virtual void addHorizontalBargraph(const char *label, FAUSTFLOAT *zone,
                                     FAUSTFLOAT min, FAUSTFLOAT max) {}
  virtual void addVerticalBargraph(const char *label, FAUSTFLOAT *zone,
                                   FAUSTFLOAT min, FAUSTFLOAT max) {}

  // -- soundfiles
  virtual void addSoundfile(const char *label, const char *soundpath,
                            Soundfile **sf_zone) {}

  virtual void declare(FAUSTFLOAT *zone, const char *key, const char *val) {}
};

//----------------------------------------------------------------
//  Generic UI decorator
//----------------------------------------------------------------

class FAUST_API DecoratorUI : public UI {

protected:
  UI *fUI;

public:
  DecoratorUI(UI *ui = 0) : fUI(ui) {}
  virtual ~DecoratorUI() { delete fUI; }

  // -- widget's layouts
  virtual void openTabBox(const char *label) { fUI->openTabBox(label); }
  virtual void openHorizontalBox(const char *label) {
    fUI->openHorizontalBox(label);
  }
  virtual void openVerticalBox(const char *label) {
    fUI->openVerticalBox(label);
  }
  virtual void closeBox() { fUI->closeBox(); }

  // -- active widgets
  virtual void addButton(const char *label, FAUSTFLOAT *zone) {
    fUI->addButton(label, zone);
  }
  virtual void addCheckButton(const char *label, FAUSTFLOAT *zone) {
    fUI->addCheckButton(label, zone);
  }
  virtual void addVerticalSlider(const char *label, FAUSTFLOAT *zone,
                                 FAUSTFLOAT init, FAUSTFLOAT min,
                                 FAUSTFLOAT max, FAUSTFLOAT step) {
    fUI->addVerticalSlider(label, zone, init, min, max, step);
  }
  virtual void addHorizontalSlider(const char *label, FAUSTFLOAT *zone,
                                   FAUSTFLOAT init, FAUSTFLOAT min,
                                   FAUSTFLOAT max, FAUSTFLOAT step) {
    fUI->addHorizontalSlider(label, zone, init, min, max, step);
  }
  virtual void addNumEntry(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                           FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    fUI->addNumEntry(label, zone, init, min, max, step);
  }

  // -- passive widgets
  virtual void addHorizontalBargraph(const char *label, FAUSTFLOAT *zone,
                                     FAUSTFLOAT min, FAUSTFLOAT max) {
    fUI->addHorizontalBargraph(label, zone, min, max);
  }
  virtual void addVerticalBargraph(const char *label, FAUSTFLOAT *zone,
                                   FAUSTFLOAT min, FAUSTFLOAT max) {
    fUI->addVerticalBargraph(label, zone, min, max);
  }

  // -- soundfiles
  virtual void addSoundfile(const char *label, const char *filename,
                            Soundfile **sf_zone) {
    fUI->addSoundfile(label, filename, sf_zone);
  }

  virtual void declare(FAUSTFLOAT *zone, const char *key, const char *val) {
    fUI->declare(zone, key, val);
  }
};

// Defined here to simplify header #include inclusion
class FAUST_API SoundUIInterface : public GenericUI {};

#endif
/**************************  END  DecoratorUI.h **************************/
/************************** BEGIN MapUI.h ******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ***********************************************************************/

#ifndef FAUST_MAPUI_H
#define FAUST_MAPUI_H

#include <map>
#include <stdio.h>
#include <string>
#include <vector>

/************************** BEGIN PathBuilder.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __PathBuilder__
#define __PathBuilder__

#include <algorithm>
#include <map>
#include <regex>
#include <set>
#include <string>
#include <vector>

/*******************************************************************************
 * PathBuilder : Faust User Interface
 * Helper class to build complete hierarchical path for UI items.
 ******************************************************************************/

class FAUST_API PathBuilder {

protected:
  std::vector<std::string> fControlsLevel;
  std::vector<std::string> fFullPaths;
  std::map<std::string, std::string>
      fFull2Short; // filled by computeShortNames()

  /**
   * @brief check if a character is acceptable for an ID
   *
   * @param c
   * @return true is the character is acceptable for an ID
   */
  bool isIDChar(char c) const {
    return ((c >= 'a') && (c <= 'z')) || ((c >= 'A') && (c <= 'Z')) ||
           ((c >= '0') && (c <= '9'));
  }

  /**
   * @brief remove all "/0x00" parts
   *
   * @param src
   * @return modified string
   */
  std::string remove0x00(const std::string &src) const {
    return std::regex_replace(src, std::regex("/0x00"), "");
  }

  /**
   * @brief replace all non ID char with '_' (one '_' may replace several non ID
   * char)
   *
   * @param src
   * @return modified string
   */
  std::string str2ID(const std::string &src) const {
    std::string dst;
    bool need_underscore = false;
    for (char c : src) {
      if (isIDChar(c) || (c == '/')) {
        if (need_underscore) {
          dst.push_back('_');
          need_underscore = false;
        }
        dst.push_back(c);
      } else {
        need_underscore = true;
      }
    }
    return dst;
  }

  /**
   * @brief Keep only the last n slash-parts
   *
   * @param src
   * @param n : 1 indicates the last slash-part
   * @return modified string
   */
  std::string cut(const std::string &src, int n) const {
    std::string rdst;
    for (int i = int(src.length()) - 1; i >= 0; i--) {
      char c = src[i];
      if (c != '/') {
        rdst.push_back(c);
      } else if (n == 1) {
        std::string dst;
        for (int j = int(rdst.length()) - 1; j >= 0; j--) {
          dst.push_back(rdst[j]);
        }
        return dst;
      } else {
        n--;
        rdst.push_back(c);
      }
    }
    return src;
  }

  void addFullPath(const std::string &label) {
    fFullPaths.push_back(buildPath(label));
  }

  /**
   * @brief Compute the mapping between full path and short names
   */
  void computeShortNames() {
    std::vector<std::string>
        uniquePaths; // all full paths transformed but made unique with a prefix
    std::map<std::string, std::string>
        unique2full; // all full paths transformed but made unique with a prefix
    char num_buffer[16];
    int pnum = 0;

    for (const auto &s : fFullPaths) {
      // Using snprintf since Teensy does not have the std::to_string function
      snprintf(num_buffer, 16, "%d", pnum++);
      std::string u = "/P" + std::string(num_buffer) + str2ID(remove0x00(s));
      uniquePaths.push_back(u);
      unique2full[u] = s; // remember the full path associated to a unique path
    }

    std::map<std::string, int> uniquePath2level; // map path to level
    for (const auto &s : uniquePaths)
      uniquePath2level[s] = 1; // we init all levels to 1
    bool have_collisions = true;

    while (have_collisions) {
      // compute collision list
      std::set<std::string> collisionSet;
      std::map<std::string, std::string> short2full;
      have_collisions = false;
      for (const auto &it : uniquePath2level) {
        std::string u = it.first;
        int n = it.second;
        std::string shortName = cut(u, n);
        auto p = short2full.find(shortName);
        if (p == short2full.end()) {
          // no collision
          short2full[shortName] = u;
        } else {
          // we have a collision, add the two paths to the collision set
          have_collisions = true;
          collisionSet.insert(u);
          collisionSet.insert(p->second);
        }
      }
      for (const auto &s : collisionSet)
        uniquePath2level[s]++; // increase level of colliding path
    }

    for (const auto &it : uniquePath2level) {
      std::string u = it.first;
      int n = it.second;
      std::string shortName = replaceCharList(cut(u, n), {'/'}, '_');
      fFull2Short[unique2full[u]] = shortName;
    }
  }

  std::string replaceCharList(const std::string &str,
                              const std::vector<char> &ch1, char ch2) {
    auto beg = ch1.begin();
    auto end = ch1.end();
    std::string res = str;
    for (size_t i = 0; i < str.length(); ++i) {
      if (std::find(beg, end, str[i]) != end)
        res[i] = ch2;
    }
    return res;
  }

public:
  PathBuilder() {}
  virtual ~PathBuilder() {}

  // Return true for the first level of groups
  bool pushLabel(const std::string &label) {
    fControlsLevel.push_back(label);
    return fControlsLevel.size() == 1;
  }

  // Return true for the last level of groups
  bool popLabel() {
    fControlsLevel.pop_back();
    return fControlsLevel.size() == 0;
  }

  // Return a complete path built from a label
  std::string buildPath(const std::string &label) {
    std::string res = "/";
    for (size_t i = 0; i < fControlsLevel.size(); i++) {
      res = res + fControlsLevel[i] + "/";
    }
    res += label;
    return replaceCharList(
        res, {' ', '#', '*', ',', '?', '[', ']', '{', '}', '(', ')'}, '_');
  }

  // Assuming shortnames have been built, return the shortname from a label
  std::string buildShortname(const std::string &label) {
    return (hasShortname()) ? fFull2Short[buildPath(label)] : "";
  }

  bool hasShortname() { return fFull2Short.size() > 0; }
};

#endif // __PathBuilder__
/**************************  END  PathBuilder.h **************************/

/*******************************************************************************
 * MapUI : Faust User Interface.
 *
 * This class creates:
 * - a map of 'labels' and zones for each UI item.
 * - a map of unique 'shortname' (built so that they never collide) and zones
 *for each UI item
 * - a map of complete hierarchical 'paths' and zones for each UI item
 *
 * Simple 'labels', 'shortname' and complete 'paths' (to fully discriminate
 *between possible same 'labels' at different location in the UI hierachy) can
 *be used to access a given parameter.
 ******************************************************************************/

class FAUST_API MapUI : public UI, public PathBuilder {

protected:
  // Label zone map
  std::map<std::string, FAUSTFLOAT *> fLabelZoneMap;

  // Shortname zone map
  std::map<std::string, FAUSTFLOAT *> fShortnameZoneMap;

  // Full path map
  std::map<std::string, FAUSTFLOAT *> fPathZoneMap;

  void addZoneLabel(const std::string &label, FAUSTFLOAT *zone) {
    std::string path = buildPath(label);
    fFullPaths.push_back(path);
    fPathZoneMap[path] = zone;
    fLabelZoneMap[label] = zone;
  }

public:
  MapUI() {}
  virtual ~MapUI() {}

  // -- widget's layouts
  void openTabBox(const char *label) { pushLabel(label); }
  void openHorizontalBox(const char *label) { pushLabel(label); }
  void openVerticalBox(const char *label) { pushLabel(label); }
  void closeBox() {
    if (popLabel()) {
      // Shortnames can be computed when all fullnames are known
      computeShortNames();
      // Fill 'shortname' map
      for (const auto &it : fFullPaths) {
        fShortnameZoneMap[fFull2Short[it]] = fPathZoneMap[it];
      }
    }
  }

  // -- active widgets
  void addButton(const char *label, FAUSTFLOAT *zone) {
    addZoneLabel(label, zone);
  }
  void addCheckButton(const char *label, FAUSTFLOAT *zone) {
    addZoneLabel(label, zone);
  }
  void addVerticalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                         FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step) {
    addZoneLabel(label, zone);
  }
  void addHorizontalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                           FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step) {
    addZoneLabel(label, zone);
  }
  void addNumEntry(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                   FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step) {
    addZoneLabel(label, zone);
  }

  // -- passive widgets
  void addHorizontalBargraph(const char *label, FAUSTFLOAT *zone,
                             FAUSTFLOAT fmin, FAUSTFLOAT fmax) {
    addZoneLabel(label, zone);
  }
  void addVerticalBargraph(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT fmin,
                           FAUSTFLOAT fmax) {
    addZoneLabel(label, zone);
  }

  // -- soundfiles
  virtual void addSoundfile(const char *label, const char *filename,
                            Soundfile **sf_zone) {}

  // -- metadata declarations
  virtual void declare(FAUSTFLOAT *zone, const char *key, const char *val) {}

  //-------------------------------------------------------------------------------
  // Public API
  //-------------------------------------------------------------------------------

  /**
   * Set the param value.
   *
   * @param str - the UI parameter label/shortname/path
   * @param value - the UI parameter value
   *
   */
  void setParamValue(const std::string &str, FAUSTFLOAT value) {
    const auto fPathZoneMapIter = fPathZoneMap.find(str);
    if (fPathZoneMapIter != fPathZoneMap.end()) {
      *fPathZoneMapIter->second = value;
      return;
    }

    const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
    if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
      *fShortnameZoneMapIter->second = value;
      return;
    }

    const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
    if (fLabelZoneMapIter != fLabelZoneMap.end()) {
      *fLabelZoneMapIter->second = value;
      return;
    }

    fprintf(stderr, "ERROR : setParamValue '%s' not found\n", str.c_str());
  }

  /**
   * Return the param value.
   *
   * @param str - the UI parameter label/shortname/path
   *
   * @return the param value.
   */
  FAUSTFLOAT getParamValue(const std::string &str) {
    const auto fPathZoneMapIter = fPathZoneMap.find(str);
    if (fPathZoneMapIter != fPathZoneMap.end()) {
      return *fPathZoneMapIter->second;
    }

    const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
    if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
      return *fShortnameZoneMapIter->second;
    }

    const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
    if (fLabelZoneMapIter != fLabelZoneMap.end()) {
      return *fLabelZoneMapIter->second;
    }

    fprintf(stderr, "ERROR : getParamValue '%s' not found\n", str.c_str());
    return 0;
  }

  // map access
  std::map<std::string, FAUSTFLOAT *> &getFullpathMap() { return fPathZoneMap; }
  std::map<std::string, FAUSTFLOAT *> &getShortnameMap() {
    return fShortnameZoneMap;
  }
  std::map<std::string, FAUSTFLOAT *> &getLabelMap() { return fLabelZoneMap; }

  /**
   * Return the number of parameters in the UI.
   *
   * @return the number of parameters
   */
  int getParamsCount() { return int(fPathZoneMap.size()); }

  /**
   * Return the param path.
   *
   * @param index - the UI parameter index
   *
   * @return the param path
   */
  std::string getParamAddress(int index) {
    if (index < 0 || index > int(fPathZoneMap.size())) {
      return "";
    } else {
      auto it = fPathZoneMap.begin();
      while (index-- > 0 && it++ != fPathZoneMap.end()) {
      }
      return it->first;
    }
  }

  const char *getParamAddress1(int index) {
    if (index < 0 || index > int(fPathZoneMap.size())) {
      return nullptr;
    } else {
      auto it = fPathZoneMap.begin();
      while (index-- > 0 && it++ != fPathZoneMap.end()) {
      }
      return it->first.c_str();
    }
  }

  /**
   * Return the param shortname.
   *
   * @param index - the UI parameter index
   *
   * @return the param shortname
   */
  std::string getParamShortname(int index) {
    if (index < 0 || index > int(fShortnameZoneMap.size())) {
      return "";
    } else {
      auto it = fShortnameZoneMap.begin();
      while (index-- > 0 && it++ != fShortnameZoneMap.end()) {
      }
      return it->first;
    }
  }

  const char *getParamShortname1(int index) {
    if (index < 0 || index > int(fShortnameZoneMap.size())) {
      return nullptr;
    } else {
      auto it = fShortnameZoneMap.begin();
      while (index-- > 0 && it++ != fShortnameZoneMap.end()) {
      }
      return it->first.c_str();
    }
  }

  /**
   * Return the param label.
   *
   * @param index - the UI parameter index
   *
   * @return the param label
   */
  std::string getParamLabel(int index) {
    if (index < 0 || index > int(fLabelZoneMap.size())) {
      return "";
    } else {
      auto it = fLabelZoneMap.begin();
      while (index-- > 0 && it++ != fLabelZoneMap.end()) {
      }
      return it->first;
    }
  }

  const char *getParamLabel1(int index) {
    if (index < 0 || index > int(fLabelZoneMap.size())) {
      return nullptr;
    } else {
      auto it = fLabelZoneMap.begin();
      while (index-- > 0 && it++ != fLabelZoneMap.end()) {
      }
      return it->first.c_str();
    }
  }

  /**
   * Return the param path.
   *
   * @param zone - the UI parameter memory zone
   *
   * @return the param path
   */
  std::string getParamAddress(FAUSTFLOAT *zone) {
    for (const auto &it : fPathZoneMap) {
      if (it.second == zone)
        return it.first;
    }
    return "";
  }

  /**
   * Return the param memory zone.
   *
   * @param zone - the UI parameter label/shortname/path
   *
   * @return the param path
   */
  FAUSTFLOAT *getParamZone(const std::string &str) {
    const auto fPathZoneMapIter = fPathZoneMap.find(str);
    if (fPathZoneMapIter != fPathZoneMap.end()) {
      return fPathZoneMapIter->second;
    }

    const auto fShortnameZoneMapIter = fShortnameZoneMap.find(str);
    if (fShortnameZoneMapIter != fShortnameZoneMap.end()) {
      return fShortnameZoneMapIter->second;
    }

    const auto fLabelZoneMapIter = fLabelZoneMap.find(str);
    if (fLabelZoneMapIter != fLabelZoneMap.end()) {
      return fLabelZoneMapIter->second;
    }

    return nullptr;
  }

  /**
   * Return the param memory zone.
   *
   * @param zone - the UI parameter index
   *
   * @return the param path
   */
  FAUSTFLOAT *getParamZone(int index) {
    if (index < 0 || index > int(fPathZoneMap.size())) {
      return nullptr;
    } else {
      auto it = fPathZoneMap.begin();
      while (index-- > 0 && it++ != fPathZoneMap.end()) {
      }
      return it->second;
    }
  }

  static bool endsWith(const std::string &str, const std::string &end) {
    size_t l1 = str.length();
    size_t l2 = end.length();
    return (l1 >= l2) && (0 == str.compare(l1 - l2, l2, end));
  }
};

#endif // FAUST_MAPUI_H
/**************************  END  MapUI.h **************************/
/************************** BEGIN LayoutUI.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef FAUST_LAYOUTUI_H
#define FAUST_LAYOUTUI_H

#include <iostream>
#include <map>
#include <ostream>
#include <stack>
#include <string>
#include <vector>

/************************** BEGIN MetaDataUI.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef MetaData_UI_H
#define MetaData_UI_H

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <assert.h>
#include <map>
#include <set>
#include <stdio.h> // We use the lighter fprintf code
#include <string.h>
#include <string>

/************************** BEGIN SimpleParser.h *********************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef SIMPLEPARSER_H
#define SIMPLEPARSER_H

// ---------------------------------------------------------------------
//                          Simple Parser
// A parser returns true if it was able to parse what it is
// supposed to parse and advance the pointer. Otherwise it returns false
// and the pointer is not advanced so that another parser can be tried.
// ---------------------------------------------------------------------

#include <assert.h>
#include <cmath>
#include <ctype.h>
#include <fstream>
#include <map>
#include <sstream>
#include <stdio.h> // We use the lighter fprintf code
#include <string>
#include <vector>

#ifndef _WIN32
#pragma GCC diagnostic ignored "-Wunused-function"
#endif

struct itemInfo {
  std::string type;
  std::string label;
  std::string shortname;
  std::string address;
  std::string url;
  int index;
  double init;
  double fmin;
  double fmax;
  double step;
  std::vector<std::pair<std::string, std::string>> meta;

  itemInfo() : index(0), init(0.), fmin(0.), fmax(0.), step(0.) {}
};

// ---------------------------------------------------------------------
//                          Elementary parsers
// ---------------------------------------------------------------------

// Report a parsing error
static bool parseError(const char *&p, const char *errmsg) {
  fprintf(stderr, "Parse error : %s here : %s\n", errmsg, p);
  return true;
}

/**
 * @brief skipBlank : advance pointer p to the first non blank character
 * @param p the string to parse, then the remaining string
 */
static void skipBlank(const char *&p) {
  while (isspace(*p)) {
    p++;
  }
}

// Parse character x, but don't report error if fails
static bool tryChar(const char *&p, char x) {
  skipBlank(p);
  if (x == *p) {
    p++;
    return true;
  } else {
    return false;
  }
}

/**
 * @brief parseChar : parse a specific character x
 * @param p the string to parse, then the remaining string
 * @param x the character to recognize
 * @return true if x was found at the begin of p
 */
static bool parseChar(const char *&p, char x) {
  skipBlank(p);
  if (x == *p) {
    p++;
    return true;
  } else {
    return false;
  }
}

/**
 * @brief parseWord : parse a specific string w
 * @param p the string to parse, then the remaining string
 * @param w the string to recognize
 * @return true if string w was found at the begin of p
 */
static bool parseWord(const char *&p, const char *w) {
  skipBlank(p);
  const char *saved = p; // to restore position if we fail
  while ((*w == *p) && (*w)) {
    ++w;
    ++p;
  }
  if (*w) {
    p = saved;
    return false;
  } else {
    return true;
  }
}

/**
 * @brief parseDouble : parse number [s]dddd[.dddd] or [s]d[.dddd][E|e][s][dddd]
 * and store the result in x
 * @param p the string to parse, then the remaining string
 * @param x the float number found if any
 * @return true if a float number was found at the begin of p
 */
static bool parseDouble(const char *&p, double &x) {
  double sign = 1.0;    // sign of the number
  double ipart = 0;     // integral part of the number
  double dpart = 0;     // decimal part of the number before division
  double dcoef = 1.0;   // division factor for the decimal part
  double expsign = 1.0; // sign of the E|e part
  double expcoef = 0.0; // multiplication factor of E|e part

  bool valid = false; // true if the number contains at least one digit

  skipBlank(p);
  const char *saved = p; // to restore position if we fail

  // Sign
  if (parseChar(p, '+')) {
    sign = 1.0;
  } else if (parseChar(p, '-')) {
    sign = -1.0;
  }

  // Integral part
  while (isdigit(*p)) {
    valid = true;
    ipart = ipart * 10 + (*p - '0');
    p++;
  }

  // Possible decimal part
  if (parseChar(p, '.')) {
    while (isdigit(*p)) {
      valid = true;
      dpart = dpart * 10 + (*p - '0');
      dcoef *= 10.0;
      p++;
    }
  }

  // Possible E|e part
  if (parseChar(p, 'E') || parseChar(p, 'e')) {
    if (parseChar(p, '+')) {
      expsign = 1.0;
    } else if (parseChar(p, '-')) {
      expsign = -1.0;
    }
    while (isdigit(*p)) {
      expcoef = expcoef * 10 + (*p - '0');
      p++;
    }
  }

  if (valid) {
    x = (sign * (ipart + dpart / dcoef)) * std::pow(10.0, expcoef * expsign);
  } else {
    p = saved;
  }
  return valid;
}

/**
 * @brief parseString, parse an arbitrary quoted string q...q and store the
 * result in s
 * @param p the string to parse, then the remaining string
 * @param quote the character used to quote the string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseString(const char *&p, char quote, std::string &s) {
  std::string str;
  skipBlank(p);

  const char *saved = p; // to restore position if we fail
  if (*p++ == quote) {
    while ((*p != 0) && (*p != quote)) {
      str += *p++;
    }
    if (*p++ == quote) {
      s = str;
      return true;
    }
  }
  p = saved;
  return false;
}

/**
 * @brief parseSQString, parse a single quoted string '...' and store the result
 * in s
 * @param p the string to parse, then the remaining string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseSQString(const char *&p, std::string &s) {
  return parseString(p, '\'', s);
}

/**
 * @brief parseDQString, parse a double quoted string "..." and store the result
 * in s
 * @param p the string to parse, then the remaining string
 * @param s the (unquoted) string found if any
 * @return true if a string was found at the begin of p
 */
static bool parseDQString(const char *&p, std::string &s) {
  return parseString(p, '"', s);
}

// ---------------------------------------------------------------------
//
//                          IMPLEMENTATION
//
// ---------------------------------------------------------------------

/**
 * @brief parseMenuItem, parse a menu item ...'low':440.0...
 * @param p the string to parse, then the remaining string
 * @param name the name found
 * @param value the value found
 * @return true if a nemu item was found
 */
static bool parseMenuItem(const char *&p, std::string &name, double &value) {
  const char *saved = p; // to restore position if we fail
  if (parseSQString(p, name) && parseChar(p, ':') && parseDouble(p, value)) {
    return true;
  } else {
    p = saved;
    return false;
  }
}

static bool parseMenuItem2(const char *&p, std::string &name) {
  const char *saved = p; // to restore position if we fail
  // single quoted
  if (parseSQString(p, name)) {
    return true;
  } else {
    p = saved;
    return false;
  }
}

/**
 * @brief parseMenuList, parse a menu list {'low' : 440.0; 'mid' : 880.0; 'hi' :
 * 1760.0}...
 * @param p the string to parse, then the remaining string
 * @param names the vector of names found
 * @param values the vector of values found
 * @return true if a menu list was found
 */
static bool parseMenuList(const char *&p, std::vector<std::string> &names,
                          std::vector<double> &values) {
  std::vector<std::string> tmpnames;
  std::vector<double> tmpvalues;
  const char *saved = p; // to restore position if we fail

  if (parseChar(p, '{')) {
    do {
      std::string n;
      double v;
      if (parseMenuItem(p, n, v)) {
        tmpnames.push_back(n);
        tmpvalues.push_back(v);
      } else {
        p = saved;
        return false;
      }
    } while (parseChar(p, ';'));
    if (parseChar(p, '}')) {
      // we suceeded
      names = tmpnames;
      values = tmpvalues;
      return true;
    }
  }
  p = saved;
  return false;
}

static bool parseMenuList2(const char *&p, std::vector<std::string> &names,
                           bool debug) {
  std::vector<std::string> tmpnames;
  const char *saved = p; // to restore position if we fail

  if (parseChar(p, '{')) {
    do {
      std::string n;
      if (parseMenuItem2(p, n)) {
        tmpnames.push_back(n);
      } else {
        goto error;
      }
    } while (parseChar(p, ';'));
    if (parseChar(p, '}')) {
      // we suceeded
      names = tmpnames;
      return true;
    }
  }

error:
  if (debug) {
    fprintf(stderr, "parseMenuList2 : (%s) is not a valid list !\n", p);
  }
  p = saved;
  return false;
}

/// ---------------------------------------------------------------------
// Parse list of strings
/// ---------------------------------------------------------------------
static bool parseList(const char *&p, std::vector<std::string> &items) {
  const char *saved = p; // to restore position if we fail
  if (parseChar(p, '[')) {
    do {
      std::string item;
      if (!parseDQString(p, item)) {
        p = saved;
        return false;
      }
      items.push_back(item);
    } while (tryChar(p, ','));
    return parseChar(p, ']');
  } else {
    p = saved;
    return false;
  }
}

static bool parseMetaData(const char *&p,
                          std::map<std::string, std::string> &metadatas) {
  const char *saved = p; // to restore position if we fail
  std::string metaKey, metaValue;
  if (parseChar(p, ':') && parseChar(p, '[')) {
    do {
      if (parseChar(p, '{') && parseDQString(p, metaKey) && parseChar(p, ':') &&
          parseDQString(p, metaValue) && parseChar(p, '}')) {
        metadatas[metaKey] = metaValue;
      }
    } while (tryChar(p, ','));
    return parseChar(p, ']');
  } else {
    p = saved;
    return false;
  }
}

static bool
parseItemMetaData(const char *&p,
                  std::vector<std::pair<std::string, std::string>> &metadatas) {
  const char *saved = p; // to restore position if we fail
  std::string metaKey, metaValue;
  if (parseChar(p, ':') && parseChar(p, '[')) {
    do {
      if (parseChar(p, '{') && parseDQString(p, metaKey) && parseChar(p, ':') &&
          parseDQString(p, metaValue) && parseChar(p, '}')) {
        metadatas.push_back(std::make_pair(metaKey, metaValue));
      }
    } while (tryChar(p, ','));
    return parseChar(p, ']');
  } else {
    p = saved;
    return false;
  }
}

// ---------------------------------------------------------------------
// Parse metadatas of the interface:
// "name" : "...", "inputs" : "...", "outputs" : "...", ...
// and store the result as key/value
/// ---------------------------------------------------------------------
static bool parseGlobalMetaData(const char *&p, std::string &key,
                                std::string &value, double &dbl,
                                std::map<std::string, std::string> &metadatas,
                                std::vector<std::string> &items) {
  const char *saved = p; // to restore position if we fail
  if (parseDQString(p, key)) {
    if (key == "meta") {
      return parseMetaData(p, metadatas);
    } else {
      return parseChar(p, ':') && (parseDQString(p, value) ||
                                   parseList(p, items) || parseDouble(p, dbl));
    }
  } else {
    p = saved;
    return false;
  }
}

// ---------------------------------------------------------------------
// Parse gui:
// "type" : "...", "label" : "...", "address" : "...", ...
// and store the result in uiItems Vector
/// ---------------------------------------------------------------------
static bool parseUI(const char *&p, std::vector<itemInfo> &uiItems,
                    int &numItems) {
  const char *saved = p; // to restore position if we fail
  if (parseChar(p, '{')) {

    std::string label;
    std::string value;
    double dbl = 0;

    do {
      if (parseDQString(p, label)) {
        if (label == "type") {
          if (uiItems.size() != 0) {
            numItems++;
          }
          if (parseChar(p, ':') && parseDQString(p, value)) {
            itemInfo item;
            item.type = value;
            uiItems.push_back(item);
          }
        }

        else if (label == "label") {
          if (parseChar(p, ':') && parseDQString(p, value)) {
            uiItems[numItems].label = value;
          }
        }

        else if (label == "shortname") {
          if (parseChar(p, ':') && parseDQString(p, value)) {
            uiItems[numItems].shortname = value;
          }
        }

        else if (label == "address") {
          if (parseChar(p, ':') && parseDQString(p, value)) {
            uiItems[numItems].address = value;
          }
        }

        else if (label == "url") {
          if (parseChar(p, ':') && parseDQString(p, value)) {
            uiItems[numItems].url = value;
          }
        }

        else if (label == "index") {
          if (parseChar(p, ':') && parseDouble(p, dbl)) {
            uiItems[numItems].index = int(dbl);
          }
        }

        else if (label == "meta") {
          if (!parseItemMetaData(p, uiItems[numItems].meta)) {
            return false;
          }
        }

        else if (label == "init") {
          if (parseChar(p, ':') && parseDouble(p, dbl)) {
            uiItems[numItems].init = dbl;
          }
        }

        else if (label == "min") {
          if (parseChar(p, ':') && parseDouble(p, dbl)) {
            uiItems[numItems].fmin = dbl;
          }
        }

        else if (label == "max") {
          if (parseChar(p, ':') && parseDouble(p, dbl)) {
            uiItems[numItems].fmax = dbl;
          }
        }

        else if (label == "step") {
          if (parseChar(p, ':') && parseDouble(p, dbl)) {
            uiItems[numItems].step = dbl;
          }
        }

        else if (label == "items") {
          if (parseChar(p, ':') && parseChar(p, '[')) {
            do {
              if (!parseUI(p, uiItems, numItems)) {
                p = saved;
                return false;
              }
            } while (tryChar(p, ','));
            if (parseChar(p, ']')) {
              itemInfo item;
              item.type = "close";
              uiItems.push_back(item);
              numItems++;
            }
          }
        } else {
          fprintf(stderr, "Parse error unknown : %s \n", label.c_str());
          assert(false);
        }
      } else {
        p = saved;
        return false;
      }

    } while (tryChar(p, ','));

    return parseChar(p, '}');
  } else {
    return true; // "items": [] is valid
  }
}

// ---------------------------------------------------------------------
// Parse full JSON record describing a JSON/Faust interface :
// {"metadatas": "...", "ui": [{ "type": "...", "label": "...", "items": [...],
// "address": "...","init": "...", "min": "...", "max": "...","step": "..."}]}
//
// and store the result in map Metadatas and vector containing the items of the
// interface. Returns true if parsing was successfull.
/// ---------------------------------------------------------------------
static bool
parseJson(const char *&p,
          std::map<std::string, std::pair<std::string, double>> &metaDatas0,
          std::map<std::string, std::string> &metaDatas1,
          std::map<std::string, std::vector<std::string>> &metaDatas2,
          std::vector<itemInfo> &uiItems) {
  parseChar(p, '{');

  do {
    std::string key;
    std::string value;
    double dbl = 0;
    std::vector<std::string> items;
    if (parseGlobalMetaData(p, key, value, dbl, metaDatas1, items)) {
      if (key != "meta") {
        // keep "name", "inputs", "outputs" key/value pairs
        if (items.size() > 0) {
          metaDatas2[key] = items;
          items.clear();
        } else if (value != "") {
          metaDatas0[key].first = value;
        } else {
          metaDatas0[key].second = dbl;
        }
      }
    } else if (key == "ui") {
      int numItems = 0;
      parseChar(p, '[') && parseUI(p, uiItems, numItems);
    }
  } while (tryChar(p, ','));

  return parseChar(p, '}');
}

#endif // SIMPLEPARSER_H
/**************************  END  SimpleParser.h **************************/

static bool startWith(const std::string &str, const std::string &prefix) {
  return (str.substr(0, prefix.size()) == prefix);
}

/**
 * Convert a dB value into a scale between 0 and 1 (following IEC standard ?)
 */
static FAUSTFLOAT dB2Scale(FAUSTFLOAT dB) {
  FAUSTFLOAT scale = FAUSTFLOAT(1.0);

  /*if (dB < -70.0f)
   scale = 0.0f;
   else*/
  if (dB < FAUSTFLOAT(-60.0))
    scale = (dB + FAUSTFLOAT(70.0)) * FAUSTFLOAT(0.0025);
  else if (dB < FAUSTFLOAT(-50.0))
    scale = (dB + FAUSTFLOAT(60.0)) * FAUSTFLOAT(0.005) + FAUSTFLOAT(0.025);
  else if (dB < FAUSTFLOAT(-40.0))
    scale = (dB + FAUSTFLOAT(50.0)) * FAUSTFLOAT(0.0075) + FAUSTFLOAT(0.075);
  else if (dB < FAUSTFLOAT(-30.0))
    scale = (dB + FAUSTFLOAT(40.0)) * FAUSTFLOAT(0.015) + FAUSTFLOAT(0.15);
  else if (dB < FAUSTFLOAT(-20.0))
    scale = (dB + FAUSTFLOAT(30.0)) * FAUSTFLOAT(0.02) + FAUSTFLOAT(0.3);
  else if (dB < FAUSTFLOAT(-0.001) ||
           dB > FAUSTFLOAT(0.001)) /* if (dB < 0.0) */
    scale = (dB + FAUSTFLOAT(20.0)) * FAUSTFLOAT(0.025) + FAUSTFLOAT(0.5);

  return scale;
}

/*******************************************************************************
 * MetaDataUI : Common class for MetaData handling
 ******************************************************************************/

//============================= BEGIN GROUP LABEL
// METADATA===========================
// Unlike widget's label, metadata inside group's label are not extracted
// directly by the Faust compiler. Therefore they must be extracted within the
// architecture file
//-----------------------------------------------------------------------------------

class MetaDataUI {

protected:
  std::string fGroupTooltip;
  std::map<FAUSTFLOAT *, FAUSTFLOAT>
      fGuiSize; // map widget zone with widget size coef
  std::map<FAUSTFLOAT *, std::string>
      fTooltip; // map widget zone with tooltip strings
  std::map<FAUSTFLOAT *, std::string>
      fUnit; // map widget zone to unit string (i.e. "dB")
  std::map<FAUSTFLOAT *, std::string>
      fRadioDescription; // map zone to {'low':440; ...; 'hi':1000.0}
  std::map<FAUSTFLOAT *, std::string>
      fMenuDescription;            // map zone to {'low':440; ...; 'hi':1000.0}
  std::set<FAUSTFLOAT *> fKnobSet; // set of widget zone to be knobs
  std::set<FAUSTFLOAT *> fLedSet;  // set of widget zone to be LEDs
  std::set<FAUSTFLOAT *>
      fNumSet; // set of widget zone to be numerical bargraphs
  std::set<FAUSTFLOAT *> fLogSet; // set of widget zone having a log UI scale
  std::set<FAUSTFLOAT *> fExpSet; // set of widget zone having an exp UI scale
  std::set<FAUSTFLOAT *> fHiddenSet; // set of hidden widget zone

  void clearMetadata() {
    fGuiSize.clear();
    fTooltip.clear();
    fUnit.clear();
    fRadioDescription.clear();
    fMenuDescription.clear();
    fKnobSet.clear();
    fLedSet.clear();
    fNumSet.clear();
    fLogSet.clear();
    fExpSet.clear();
    fHiddenSet.clear();
    fGroupTooltip = "";
  }

  /**
   * rmWhiteSpaces(): Remove the leading and trailing white spaces of a string
   * (but not those in the middle of the string)
   */
  static std::string rmWhiteSpaces(const std::string &s) {
    size_t i = s.find_first_not_of(" \t");
    size_t j = s.find_last_not_of(" \t");
    if ((i != std::string::npos) && (j != std::string::npos)) {
      return s.substr(i, 1 + j - i);
    } else {
      return "";
    }
  }

  /**
   * Format tooltip string by replacing some white spaces by
   * return characters so that line width doesn't exceed n.
   * Limitation : long words exceeding n are not cut.
   */
  std::string formatTooltip(int n, const std::string &tt) {
    std::string ss = tt; // ss string we are going to format
    int lws = 0;         // last white space encountered
    int lri = 0;         // last return inserted
    for (int i = 0; i < (int)tt.size(); i++) {
      if (tt[i] == ' ')
        lws = i;
      if (((i - lri) >= n) && (lws > lri)) {
        // insert return here
        ss[lws] = '\n';
        lri = lws;
      }
    }
    return ss;
  }

public:
  virtual ~MetaDataUI() {}

  enum Scale { kLin, kLog, kExp };

  Scale getScale(FAUSTFLOAT *zone) {
    if (fLogSet.count(zone) > 0)
      return kLog;
    if (fExpSet.count(zone) > 0)
      return kExp;
    return kLin;
  }

  bool isKnob(FAUSTFLOAT *zone) { return fKnobSet.count(zone) > 0; }

  bool isRadio(FAUSTFLOAT *zone) { return fRadioDescription.count(zone) > 0; }

  bool isMenu(FAUSTFLOAT *zone) { return fMenuDescription.count(zone) > 0; }

  bool isLed(FAUSTFLOAT *zone) { return fLedSet.count(zone) > 0; }

  bool isNumerical(FAUSTFLOAT *zone) { return fNumSet.count(zone) > 0; }

  bool isHidden(FAUSTFLOAT *zone) { return fHiddenSet.count(zone) > 0; }

  /**
   * Extracts metadata from a label : 'vol [unit: dB]' -> 'vol' +
   * metadata(unit=dB)
   */
  static void extractMetadata(const std::string &fulllabel, std::string &label,
                              std::map<std::string, std::string> &metadata) {
    enum { kLabel, kEscape1, kEscape2, kEscape3, kKey, kValue };
    int state = kLabel;
    int deep = 0;
    std::string key, value;

    for (unsigned int i = 0; i < fulllabel.size(); i++) {
      char c = fulllabel[i];
      switch (state) {
      case kLabel:
        assert(deep == 0);
        switch (c) {
        case '\\':
          state = kEscape1;
          break;
        case '[':
          state = kKey;
          deep++;
          break;
        default:
          label += c;
        }
        break;

      case kEscape1:
        label += c;
        state = kLabel;
        break;

      case kEscape2:
        key += c;
        state = kKey;
        break;

      case kEscape3:
        value += c;
        state = kValue;
        break;

      case kKey:
        assert(deep > 0);
        switch (c) {
        case '\\':
          state = kEscape2;
          break;

        case '[':
          deep++;
          key += c;
          break;

        case ':':
          if (deep == 1) {
            state = kValue;
          } else {
            key += c;
          }
          break;
        case ']':
          deep--;
          if (deep < 1) {
            metadata[rmWhiteSpaces(key)] = "";
            state = kLabel;
            key = "";
            value = "";
          } else {
            key += c;
          }
          break;
        default:
          key += c;
        }
        break;

      case kValue:
        assert(deep > 0);
        switch (c) {
        case '\\':
          state = kEscape3;
          break;

        case '[':
          deep++;
          value += c;
          break;

        case ']':
          deep--;
          if (deep < 1) {
            metadata[rmWhiteSpaces(key)] = rmWhiteSpaces(value);
            state = kLabel;
            key = "";
            value = "";
          } else {
            value += c;
          }
          break;
        default:
          value += c;
        }
        break;

      default:
        fprintf(stderr, "ERROR unrecognized state %d\n", state);
      }
    }
    label = rmWhiteSpaces(label);
  }

  /**
   * Analyses the widget zone metadata declarations and takes appropriate
   * actions.
   */
  void declare(FAUSTFLOAT *zone, const char *key, const char *value) {
    if (zone == 0) {
      // special zone 0 means group metadata
      if (strcmp(key, "tooltip") == 0) {
        // only group tooltip are currently implemented
        fGroupTooltip = formatTooltip(30, value);
      } else if ((strcmp(key, "hidden") == 0) && (strcmp(value, "1") == 0)) {
        fHiddenSet.insert(zone);
      }
    } else {
      if (strcmp(key, "size") == 0) {
        fGuiSize[zone] = atof(value);
      } else if (strcmp(key, "tooltip") == 0) {
        fTooltip[zone] = formatTooltip(30, value);
      } else if (strcmp(key, "unit") == 0) {
        fUnit[zone] = value;
      } else if ((strcmp(key, "hidden") == 0) && (strcmp(value, "1") == 0)) {
        fHiddenSet.insert(zone);
      } else if (strcmp(key, "scale") == 0) {
        if (strcmp(value, "log") == 0) {
          fLogSet.insert(zone);
        } else if (strcmp(value, "exp") == 0) {
          fExpSet.insert(zone);
        }
      } else if (strcmp(key, "style") == 0) {
        if (strcmp(value, "knob") == 0) {
          fKnobSet.insert(zone);
        } else if (strcmp(value, "led") == 0) {
          fLedSet.insert(zone);
        } else if (strcmp(value, "numerical") == 0) {
          fNumSet.insert(zone);
        } else {
          const char *p = value;
          if (parseWord(p, "radio")) {
            fRadioDescription[zone] = std::string(p);
          } else if (parseWord(p, "menu")) {
            fMenuDescription[zone] = std::string(p);
          }
        }
      }
    }
  }
};

#endif
/**************************  END  MetaDataUI.h **************************/

/*******************************************************************************
 * LayoutUI
 ******************************************************************************/

// Definition of the elements standard size

struct LayoutItemSize {

  float kKnobWidth = 10.f;
  float kKnobHeight = 10.f;

  float kVSliderWidth = 10.f;
  float kVSliderHeight = 25.f;

  float kHSliderWidth = 25.f;
  float kHSliderHeight = 10.f;

  float kButtonWidth = 10.f;
  float kButtonHeight = 10.f;

  float kCheckButtonWidth = 10.f;
  float kCheckButtonHeight = 10.f;

  float kNumEntryWidth = 10.f;
  float kNumEntryHeight = 10.f;

  float kVBargraphWidth = 10.f;
  float kVBargraphHeight = 20.f;

  float kHBargraphWidth = 20.f;
  float kHBargraphHeight = 10.f;
};

static LayoutItemSize gItemSize;

struct LayoutUI : public GenericUI, public PathBuilder, public MetaDataUI {

  // Base class
  struct UIItem {

    typedef std::shared_ptr<UIItem> shared_item;

    std::string fLabel;

    UIItem(const std::string &label) : fLabel(label) {}
    virtual ~UIItem() {}

    virtual float getTopX() { return 0.f; };
    virtual float getTopY() { return 0.f; };

    virtual float getBorderX() { return 0.f; };
    virtual float getBorderY() { return 0.f; };

    virtual float getWidth() { return 0.f; };
    virtual float getHeight() { return 0.f; };

    virtual void setSize(float width, float height) {}
    virtual void setPos(float top_x, float top_y) {}

    virtual std::ostream &print(std::ostream &file) { return file; }
  };

  // Leave
  struct UILeaveItem : UIItem {

    float fBorderX = 0.f;
    float fBorderY = 0.f;

    float fWidth = 0.f;
    float fHeight = 0.f;

    float fTopX = 0.f;
    float fTopY = 0.f;

    UILeaveItem(const std::string &label, float width = 0.0f,
                float height = 0.0f)
        : UIItem(label), fWidth(width), fHeight(height) {}

    float getTopX() override { return fTopX; };
    float getTopY() override { return fTopY; };

    float getBorderX() override { return fBorderX; };
    float getBorderY() override { return fBorderY; };

    float getWidth() override { return fWidth; }
    float getHeight() override { return fHeight; }

    // Adapt all leave sizes
    void setSize(float width, float height) override {
      fWidth = width;
      fHeight = height;
    }

    // Adapt all leave positions
    void setPos(float top_x, float top_y) override {
      fTopX = top_x;
      fTopY = top_y;
    }

    std::ostream &print(std::ostream &file) override {
      file << "fTopX = " << fTopX << " fTopY = " << fTopY << std::endl;
      file << "fWidth = " << fWidth << " fHeight = " << fHeight << std::endl;
      file << "--------------------------" << std::endl;
      return file;
    }
  };

  // Grouping classes
  struct Group : UIItem {

    typedef std::shared_ptr<Group> shared_group;

    std::vector<UIItem::shared_item> fItems;

    Group(const std::string &label) : UIItem(label) {}
    virtual ~Group() {}

    void add(UIItem::shared_item item) { fItems.push_back(item); }

    void setSize(float width, float height) override {
      float real_width = getWidth() - getBorderX();
      float real_height = getHeight() - getBorderY();

      float w_ratio = width / real_width;
      float h_ratio = height / real_height;

      for (const auto &it : fItems) {
        it->setSize(it->getWidth() * w_ratio, it->getHeight() * h_ratio);
      }
    }
  };

  struct HGroup : Group {

    HGroup(const std::string &label) : Group(label) {}

    float getWidth() override {
      float res = 0.f;
      for (const auto &it : fItems) {
        res += it->getWidth();
      }
      return res;
    }

    float getHeight() override {
      float res = 0.f;
      for (const auto &it : fItems) {
        res = std::max(res, it->getHeight());
      }
      return res;
    }

    void setPos(float top_x, float top_y) override {
      for (const auto &it : fItems) {
        it->setPos(top_x, top_y);
        top_x += it->getWidth();
      }
    }
  };

  struct VGroup : Group {

    VGroup(const std::string &label) : Group(label) {}

    float getWidth() override {
      float res = 0.f;
      for (const auto &it : fItems) {
        res = std::max(res, it->getWidth());
      }
      return res;
    }

    float getHeight() override {
      float res = 0.f;
      for (const auto &it : fItems) {
        res += it->getHeight();
      }
      return res;
    }

    void setPos(float top_x, float top_y) override {
      for (const auto &it : fItems) {
        it->setPos(top_x, top_y);
        top_y += it->getHeight();
      }
    }
  };

  // Terminal items
  struct Button : UILeaveItem {

    Button(const std::string &label)
        : UILeaveItem(label, gItemSize.kButtonWidth, gItemSize.kButtonHeight) {}

    std::ostream &print(std::ostream &file) override {
      file << "----------Button----------" << std::endl;
      return UILeaveItem::print(file);
    }
  };

  struct CheckButton : UILeaveItem {

    CheckButton(const std::string &label)
        : UILeaveItem(label, gItemSize.kCheckButtonWidth,
                      gItemSize.kCheckButtonHeight) {}

    std::ostream &print(std::ostream &file) override {
      file << "----------CheckButton----------" << std::endl;
      return UILeaveItem::print(file);
    }
  };

  struct HSlider : UILeaveItem {

    HSlider(const std::string &label)
        : UILeaveItem(label, gItemSize.kVSliderWidth,
                      gItemSize.kVSliderHeight) {}

    std::ostream &print(std::ostream &file) override {
      file << "----------HSlider----------" << std::endl;
      return UILeaveItem::print(file);
    }
  };

  struct VSlider : UILeaveItem {

    VSlider(const std::string &label)
        : UILeaveItem(label, gItemSize.kHSliderWidth,
                      gItemSize.kHSliderHeight) {}

    std::ostream &print(std::ostream &file) override {
      file << "----------VSlider----------" << std::endl;
      return UILeaveItem::print(file);
    }
  };

  struct NumEntry : UILeaveItem {

    NumEntry(const std::string &label)
        : UILeaveItem(label, gItemSize.kNumEntryWidth,
                      gItemSize.kNumEntryHeight) {}

    std::ostream &print(std::ostream &file) override {
      file << "----------NumEntry----------" << std::endl;
      return UILeaveItem::print(file);
    }
  };

  struct HBargraph : UILeaveItem {

    HBargraph(const std::string &label)
        : UILeaveItem(label, gItemSize.kHBargraphWidth,
                      gItemSize.kHBargraphHeight) {}

    std::ostream &print(std::ostream &file) override {
      file << "----------HBargraph----------" << std::endl;
      return UILeaveItem::print(file);
    }
  };

  struct VBargraph : UILeaveItem {

    VBargraph(const std::string &label)
        : UILeaveItem(label, gItemSize.kVBargraphWidth,
                      gItemSize.kVBargraphHeight) {}

    std::ostream &print(std::ostream &file) override {
      file << "----------VBargraph----------" << std::endl;
      return UILeaveItem::print(file);
    }
  };

  Group::shared_group fCurrentGroup = nullptr;
  std::stack<Group::shared_group> fGroupStack;
  std::map<std::string, UIItem::shared_item> fPathItemMap;

  void addItem(const char *label, UIItem::shared_item item) {
    fPathItemMap[buildPath(label)] = item;
    fCurrentGroup->add(item);
  }

  void addGroup(const char *label, Group::shared_group group) {
    pushLabel(label);
    if (fCurrentGroup) {
      fCurrentGroup->add(group);
      fGroupStack.push(fCurrentGroup);
    }
    fCurrentGroup = group;
  }

  LayoutUI() {}
  virtual ~LayoutUI() {}

  // -- widget's layouts

  virtual void openTabBox(const char *label) override { pushLabel(label); }

  virtual void openHorizontalBox(const char *label) override {
    addGroup(label, std::make_shared<HGroup>(label));
  }

  virtual void openVerticalBox(const char *label) override {
    addGroup(label, std::make_shared<VGroup>(label));
  }

  virtual void closeBox() override {
    popLabel();
    if (fGroupStack.empty()) {
      // Nothing for now
    } else {
      fCurrentGroup = fGroupStack.top();
      fGroupStack.pop();
    }
  }

  // -- active widgets

  virtual void addButton(const char *label, FAUSTFLOAT *zone) override {
    addItem(label, std::make_shared<Button>(label));
  }
  virtual void addCheckButton(const char *label, FAUSTFLOAT *zone) override {
    addItem(label, std::make_shared<CheckButton>(label));
  }
  virtual void addVerticalSlider(const char *label, FAUSTFLOAT *zone,
                                 FAUSTFLOAT init, FAUSTFLOAT min,
                                 FAUSTFLOAT max, FAUSTFLOAT step) override {
    addItem(label, std::make_shared<VSlider>(label));
  }
  virtual void addHorizontalSlider(const char *label, FAUSTFLOAT *zone,
                                   FAUSTFLOAT init, FAUSTFLOAT min,
                                   FAUSTFLOAT max, FAUSTFLOAT step) override {
    addItem(label, std::make_shared<HSlider>(label));
  }
  virtual void addNumEntry(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                           FAUSTFLOAT min, FAUSTFLOAT max,
                           FAUSTFLOAT step) override {
    addItem(label, std::make_shared<NumEntry>(label));
  }

  // -- passive widgets

  virtual void addHorizontalBargraph(const char *label, FAUSTFLOAT *zone,
                                     FAUSTFLOAT min, FAUSTFLOAT max) override {
    addItem(label, std::make_shared<HBargraph>(label));
  }
  virtual void addVerticalBargraph(const char *label, FAUSTFLOAT *zone,
                                   FAUSTFLOAT min, FAUSTFLOAT max) override {
    addItem(label, std::make_shared<VBargraph>(label));
  }

  // -- metadata declarations

  virtual void declare(FAUSTFLOAT *zone, const char *key,
                       const char *val) override {
    MetaDataUI::declare(zone, key, val);
  }
};

// Generic print
template <typename T>
inline std::ostream &operator<<(std::ostream &file, std::shared_ptr<T> item) {
  return item->print(file);
}

#endif // FAUST_LAYOUTUI_H
/**************************  END  LayoutUI.h **************************/
/************************** BEGIN ValueConverter.h ********************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __ValueConverter__
#define __ValueConverter__

/***************************************************************************************
 ValueConverter.h
 (GRAME, Copyright 2015-2019)

 Set of conversion objects used to map user interface values (for example a gui
slider delivering values between 0 and 1) to faust values (for example a vslider
between 20 and 20000) using a log scale.

 -- Utilities

 Range(lo,hi) : clip a value x between lo and hi
 Interpolator(lo,hi,v1,v2) : Maps a value x between lo and hi to a value y
between v1 and v2 Interpolator3pt(lo,mi,hi,v1,vm,v2) : Map values between lo mid
hi to values between v1 vm v2

 -- Value Converters

 ValueConverter::ui2faust(x)
 ValueConverter::faust2ui(x)

 -- ValueConverters used for sliders depending of the scale

 LinearValueConverter(umin, umax, fmin, fmax)
 LinearValueConverter2(lo, mi, hi, v1, vm, v2) using 2 segments
 LogValueConverter(umin, umax, fmin, fmax)
 ExpValueConverter(umin, umax, fmin, fmax)

 -- ValueConverters used for accelerometers based on 3 points

 AccUpConverter(amin, amid, amax, fmin, fmid, fmax)        -- curve 0
 AccDownConverter(amin, amid, amax, fmin, fmid, fmax)      -- curve 1
 AccUpDownConverter(amin, amid, amax, fmin, fmid, fmax)    -- curve 2
 AccDownUpConverter(amin, amid, amax, fmin, fmid, fmax)    -- curve 3

 -- lists of ZoneControl are used to implement accelerometers metadata for each
axes

 ZoneControl(zone, valueConverter) : a zone with an accelerometer data converter

 -- ZoneReader are used to implement screencolor metadata

 ZoneReader(zone, valueConverter) : a zone with a data converter

****************************************************************************************/

#include <algorithm> // std::max
#include <assert.h>
#include <cmath>
#include <float.h>
#include <vector>

//--------------------------------------------------------------------------------------
// Interpolator(lo,hi,v1,v2)
// Maps a value x between lo and hi to a value y between v1 and v2
// y = v1 + (x-lo)/(hi-lo)*(v2-v1)
// y = v1 + (x-lo) * coef           with coef = (v2-v1)/(hi-lo)
// y = v1 + x*coef - lo*coef
// y = v1 - lo*coef + x*coef
// y = offset + x*coef              with offset = v1 - lo*coef
//--------------------------------------------------------------------------------------
class FAUST_API Interpolator {

private:
  //--------------------------------------------------------------------------------------
  // Range(lo,hi) clip a value between lo and hi
  //--------------------------------------------------------------------------------------
  struct Range {
    double fLo;
    double fHi;

    Range(double x, double y)
        : fLo(std::min<double>(x, y)), fHi(std::max<double>(x, y)) {}
    double operator()(double x) {
      return (x < fLo) ? fLo : (x > fHi) ? fHi : x;
    }
  };

  Range fRange;
  double fCoef;
  double fOffset;

public:
  Interpolator(double lo, double hi, double v1, double v2) : fRange(lo, hi) {
    if (hi != lo) {
      // regular case
      fCoef = (v2 - v1) / (hi - lo);
      fOffset = v1 - lo * fCoef;
    } else {
      // degenerate case, avoids division by zero
      fCoef = 0;
      fOffset = (v1 + v2) / 2;
    }
  }
  double operator()(double v) {
    double x = fRange(v);
    return fOffset + x * fCoef;
  }

  void getLowHigh(double &amin, double &amax) {
    amin = fRange.fLo;
    amax = fRange.fHi;
  }
};

//--------------------------------------------------------------------------------------
// Interpolator3pt(lo,mi,hi,v1,vm,v2)
// Map values between lo mid hi to values between v1 vm v2
//--------------------------------------------------------------------------------------
class FAUST_API Interpolator3pt {

private:
  Interpolator fSegment1;
  Interpolator fSegment2;
  double fMid;

public:
  Interpolator3pt(double lo, double mi, double hi, double v1, double vm,
                  double v2)
      : fSegment1(lo, mi, v1, vm), fSegment2(mi, hi, vm, v2), fMid(mi) {}
  double operator()(double x) {
    return (x < fMid) ? fSegment1(x) : fSegment2(x);
  }

  void getMappingValues(double &amin, double &amid, double &amax) {
    fSegment1.getLowHigh(amin, amid);
    fSegment2.getLowHigh(amid, amax);
  }
};

//--------------------------------------------------------------------------------------
// Abstract ValueConverter class. Converts values between UI and Faust
// representations
//--------------------------------------------------------------------------------------
class FAUST_API ValueConverter {

public:
  virtual ~ValueConverter() {}
  virtual double ui2faust(double x) { return x; };
  virtual double faust2ui(double x) { return x; };
};

//--------------------------------------------------------------------------------------
// A converter than can be updated
//--------------------------------------------------------------------------------------

class FAUST_API UpdatableValueConverter : public ValueConverter {

protected:
  bool fActive;

public:
  UpdatableValueConverter() : fActive(true) {}
  virtual ~UpdatableValueConverter() {}

  virtual void setMappingValues(double amin, double amid, double amax,
                                double min, double init, double max) = 0;
  virtual void getMappingValues(double &amin, double &amid, double &amax) = 0;

  void setActive(bool on_off) { fActive = on_off; }
  bool getActive() { return fActive; }
};

//--------------------------------------------------------------------------------------
// Linear conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class FAUST_API LinearValueConverter : public ValueConverter {

private:
  Interpolator fUI2F;
  Interpolator fF2UI;

public:
  LinearValueConverter(double umin, double umax, double fmin, double fmax)
      : fUI2F(umin, umax, fmin, fmax), fF2UI(fmin, fmax, umin, umax) {}

  LinearValueConverter() : fUI2F(0., 0., 0., 0.), fF2UI(0., 0., 0., 0.) {}
  virtual double ui2faust(double x) { return fUI2F(x); }
  virtual double faust2ui(double x) { return fF2UI(x); }
};

//--------------------------------------------------------------------------------------
// Two segments linear conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class FAUST_API LinearValueConverter2 : public UpdatableValueConverter {

private:
  Interpolator3pt fUI2F;
  Interpolator3pt fF2UI;

public:
  LinearValueConverter2(double amin, double amid, double amax, double min,
                        double init, double max)
      : fUI2F(amin, amid, amax, min, init, max),
        fF2UI(min, init, max, amin, amid, amax) {}

  LinearValueConverter2()
      : fUI2F(0., 0., 0., 0., 0., 0.), fF2UI(0., 0., 0., 0., 0., 0.) {}

  virtual double ui2faust(double x) { return fUI2F(x); }
  virtual double faust2ui(double x) { return fF2UI(x); }

  virtual void setMappingValues(double amin, double amid, double amax,
                                double min, double init, double max) {
    fUI2F = Interpolator3pt(amin, amid, amax, min, init, max);
    fF2UI = Interpolator3pt(min, init, max, amin, amid, amax);
  }

  virtual void getMappingValues(double &amin, double &amid, double &amax) {
    fUI2F.getMappingValues(amin, amid, amax);
  }
};

//--------------------------------------------------------------------------------------
// Logarithmic conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class FAUST_API LogValueConverter : public LinearValueConverter {

public:
  // We use DBL_EPSILON which is bigger than DBL_MIN (safer)
  LogValueConverter(double umin, double umax, double fmin, double fmax)
      : LinearValueConverter(umin, umax,
                             std::log(std::max<double>(DBL_EPSILON, fmin)),
                             std::log(std::max<double>(DBL_EPSILON, fmax))) {}

  virtual double ui2faust(double x) {
    return std::exp(LinearValueConverter::ui2faust(x));
  }
  virtual double faust2ui(double x) {
    return LinearValueConverter::faust2ui(
        std::log(std::max<double>(DBL_EPSILON, x)));
  }
};

//--------------------------------------------------------------------------------------
// Exponential conversion between ui and Faust values
//--------------------------------------------------------------------------------------
class FAUST_API ExpValueConverter : public LinearValueConverter {

public:
  ExpValueConverter(double umin, double umax, double fmin, double fmax)
      : LinearValueConverter(umin, umax,
                             std::min<double>(DBL_MAX, std::exp(fmin)),
                             std::min<double>(DBL_MAX, std::exp(fmax))) {}

  virtual double ui2faust(double x) {
    return std::log(LinearValueConverter::ui2faust(x));
  }
  virtual double faust2ui(double x) {
    return LinearValueConverter::faust2ui(
        std::min<double>(DBL_MAX, std::exp(x)));
  }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using an Up curve (curve 0)
//--------------------------------------------------------------------------------------
class FAUST_API AccUpConverter : public UpdatableValueConverter {

private:
  Interpolator3pt fA2F;
  Interpolator3pt fF2A;

public:
  AccUpConverter(double amin, double amid, double amax, double fmin,
                 double fmid, double fmax)
      : fA2F(amin, amid, amax, fmin, fmid, fmax),
        fF2A(fmin, fmid, fmax, amin, amid, amax) {}

  virtual double ui2faust(double x) { return fA2F(x); }
  virtual double faust2ui(double x) { return fF2A(x); }

  virtual void setMappingValues(double amin, double amid, double amax,
                                double fmin, double fmid, double fmax) {
    //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccUpConverter update %f
    //%f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
    fA2F = Interpolator3pt(amin, amid, amax, fmin, fmid, fmax);
    fF2A = Interpolator3pt(fmin, fmid, fmax, amin, amid, amax);
  }

  virtual void getMappingValues(double &amin, double &amid, double &amax) {
    fA2F.getMappingValues(amin, amid, amax);
  }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using a Down curve (curve 1)
//--------------------------------------------------------------------------------------
class FAUST_API AccDownConverter : public UpdatableValueConverter {

private:
  Interpolator3pt fA2F;
  Interpolator3pt fF2A;

public:
  AccDownConverter(double amin, double amid, double amax, double fmin,
                   double fmid, double fmax)
      : fA2F(amin, amid, amax, fmax, fmid, fmin),
        fF2A(fmin, fmid, fmax, amax, amid, amin) {}

  virtual double ui2faust(double x) { return fA2F(x); }
  virtual double faust2ui(double x) { return fF2A(x); }

  virtual void setMappingValues(double amin, double amid, double amax,
                                double fmin, double fmid, double fmax) {
    //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccDownConverter update
    //%f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
    fA2F = Interpolator3pt(amin, amid, amax, fmax, fmid, fmin);
    fF2A = Interpolator3pt(fmin, fmid, fmax, amax, amid, amin);
  }

  virtual void getMappingValues(double &amin, double &amid, double &amax) {
    fA2F.getMappingValues(amin, amid, amax);
  }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using an Up-Down curve (curve 2)
//--------------------------------------------------------------------------------------
class FAUST_API AccUpDownConverter : public UpdatableValueConverter {

private:
  Interpolator3pt fA2F;
  Interpolator fF2A;

public:
  AccUpDownConverter(double amin, double amid, double amax, double fmin,
                     double fmid, double fmax)
      : fA2F(amin, amid, amax, fmin, fmax, fmin),
        fF2A(fmin, fmax, amin,
             amax) // Special, pseudo inverse of a non monotonic function
  {}

  virtual double ui2faust(double x) { return fA2F(x); }
  virtual double faust2ui(double x) { return fF2A(x); }

  virtual void setMappingValues(double amin, double amid, double amax,
                                double fmin, double fmid, double fmax) {
    //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccUpDownConverter update
    //%f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
    fA2F = Interpolator3pt(amin, amid, amax, fmin, fmax, fmin);
    fF2A = Interpolator(fmin, fmax, amin, amax);
  }

  virtual void getMappingValues(double &amin, double &amid, double &amax) {
    fA2F.getMappingValues(amin, amid, amax);
  }
};

//--------------------------------------------------------------------------------------
// Convert accelerometer or gyroscope values to Faust values
// Using a Down-Up curve (curve 3)
//--------------------------------------------------------------------------------------
class FAUST_API AccDownUpConverter : public UpdatableValueConverter {

private:
  Interpolator3pt fA2F;
  Interpolator fF2A;

public:
  AccDownUpConverter(double amin, double amid, double amax, double fmin,
                     double fmid, double fmax)
      : fA2F(amin, amid, amax, fmax, fmin, fmax),
        fF2A(fmin, fmax, amin,
             amax) // Special, pseudo inverse of a non monotonic function
  {}

  virtual double ui2faust(double x) { return fA2F(x); }
  virtual double faust2ui(double x) { return fF2A(x); }

  virtual void setMappingValues(double amin, double amid, double amax,
                                double fmin, double fmid, double fmax) {
    //__android_log_print(ANDROID_LOG_ERROR, "Faust", "AccDownUpConverter update
    //%f %f %f %f %f %f", amin,amid,amax,fmin,fmid,fmax);
    fA2F = Interpolator3pt(amin, amid, amax, fmax, fmin, fmax);
    fF2A = Interpolator(fmin, fmax, amin, amax);
  }

  virtual void getMappingValues(double &amin, double &amid, double &amax) {
    fA2F.getMappingValues(amin, amid, amax);
  }
};

//--------------------------------------------------------------------------------------
// Base class for ZoneControl
//--------------------------------------------------------------------------------------
class FAUST_API ZoneControl {

protected:
  FAUSTFLOAT *fZone;

public:
  ZoneControl(FAUSTFLOAT *zone) : fZone(zone) {}
  virtual ~ZoneControl() {}

  virtual void update(double v) const {}

  virtual void setMappingValues(int curve, double amin, double amid,
                                double amax, double min, double init,
                                double max) {}
  virtual void getMappingValues(double &amin, double &amid, double &amax) {}

  FAUSTFLOAT *getZone() { return fZone; }

  virtual void setActive(bool on_off) {}
  virtual bool getActive() { return false; }

  virtual int getCurve() { return -1; }
};

//--------------------------------------------------------------------------------------
//  Useful to implement accelerometers metadata as a list of ZoneControl for
//  each axes
//--------------------------------------------------------------------------------------
class FAUST_API ConverterZoneControl : public ZoneControl {

protected:
  ValueConverter *fValueConverter;

public:
  ConverterZoneControl(FAUSTFLOAT *zone, ValueConverter *converter)
      : ZoneControl(zone), fValueConverter(converter) {}
  virtual ~ConverterZoneControl() {
    delete fValueConverter;
  } // Assuming fValueConverter is not kept elsewhere...

  virtual void update(double v) const {
    *fZone = FAUSTFLOAT(fValueConverter->ui2faust(v));
  }

  ValueConverter *getConverter() { return fValueConverter; }
};

//--------------------------------------------------------------------------------------
// Association of a zone and a four value converter, each one for each possible
// curve. Useful to implement accelerometers metadata as a list of ZoneControl
// for each axes
//--------------------------------------------------------------------------------------
class FAUST_API CurveZoneControl : public ZoneControl {

private:
  std::vector<UpdatableValueConverter *> fValueConverters;
  int fCurve;

public:
  CurveZoneControl(FAUSTFLOAT *zone, int curve, double amin, double amid,
                   double amax, double min, double init, double max)
      : ZoneControl(zone), fCurve(0) {
    assert(curve >= 0 && curve <= 3);
    fValueConverters.push_back(
        new AccUpConverter(amin, amid, amax, min, init, max));
    fValueConverters.push_back(
        new AccDownConverter(amin, amid, amax, min, init, max));
    fValueConverters.push_back(
        new AccUpDownConverter(amin, amid, amax, min, init, max));
    fValueConverters.push_back(
        new AccDownUpConverter(amin, amid, amax, min, init, max));
    fCurve = curve;
  }
  virtual ~CurveZoneControl() {
    for (const auto &it : fValueConverters) {
      delete it;
    }
  }
  void update(double v) const {
    if (fValueConverters[fCurve]->getActive())
      *fZone = FAUSTFLOAT(fValueConverters[fCurve]->ui2faust(v));
  }

  void setMappingValues(int curve, double amin, double amid, double amax,
                        double min, double init, double max) {
    fValueConverters[curve]->setMappingValues(amin, amid, amax, min, init, max);
    fCurve = curve;
  }

  void getMappingValues(double &amin, double &amid, double &amax) {
    fValueConverters[fCurve]->getMappingValues(amin, amid, amax);
  }

  void setActive(bool on_off) {
    for (const auto &it : fValueConverters) {
      it->setActive(on_off);
    }
  }

  int getCurve() { return fCurve; }
};

class FAUST_API ZoneReader {

private:
  FAUSTFLOAT *fZone;
  Interpolator fInterpolator;

public:
  ZoneReader(FAUSTFLOAT *zone, double lo, double hi)
      : fZone(zone), fInterpolator(lo, hi, 0, 255) {}

  virtual ~ZoneReader() {}

  int getValue() {
    return (fZone != nullptr) ? int(fInterpolator(*fZone)) : 127;
  }
};

#endif
/**************************  END  ValueConverter.h **************************/
/************************** BEGIN misc.h *******************************
FAUST Architecture File
Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
---------------------------------------------------------------------
This program is free software; you can redistribute it and/or modify
it under the terms of the GNU Lesser General Public License as published by
the Free Software Foundation; either version 2.1 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License
along with this program; if not, write to the Free Software
Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

EXCEPTION : As a special exception, you may create a larger work
that contains this FAUST architecture section and distribute
that work under terms of your choice, so long as this FAUST
architecture section is not modified.
***************************************************************************/

#ifndef __misc__
#define __misc__

#include <algorithm>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string.h>
#include <string>

/************************** BEGIN meta.h *******************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __meta__
#define __meta__

/**
 The base class of Meta handler to be used in dsp::metadata(Meta* m) method to
 retrieve (key, value) metadata.
 */
struct FAUST_API Meta {
  virtual ~Meta() {}
  virtual void declare(const char *key, const char *value) = 0;
};

#endif
/**************************  END  meta.h **************************/

struct MY_Meta : Meta, std::map<const char *, const char *> {
  void declare(const char *key, const char *value) { (*this)[key] = value; }
};

static int lsr(int x, int n) { return int(((unsigned int)x) >> n); }

static int int2pow2(int x) {
  int r = 0;
  while ((1 << r) < x)
    r++;
  return r;
}

static long lopt(char *argv[], const char *name, long def) {
  for (int i = 0; argv[i]; i++)
    if (!strcmp(argv[i], name))
      return std::atoi(argv[i + 1]);
  return def;
}

static long lopt1(int argc, char *argv[], const char *longname,
                  const char *shortname, long def) {
  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i - 1], shortname) == 0 ||
        strcmp(argv[i - 1], longname) == 0) {
      return atoi(argv[i]);
    }
  }
  return def;
}

static const char *lopts(char *argv[], const char *name, const char *def) {
  for (int i = 0; argv[i]; i++)
    if (!strcmp(argv[i], name))
      return argv[i + 1];
  return def;
}

static const char *lopts1(int argc, char *argv[], const char *longname,
                          const char *shortname, const char *def) {
  for (int i = 2; i < argc; i++) {
    if (strcmp(argv[i - 1], shortname) == 0 ||
        strcmp(argv[i - 1], longname) == 0) {
      return argv[i];
    }
  }
  return def;
}

static bool isopt(char *argv[], const char *name) {
  for (int i = 0; argv[i]; i++)
    if (!strcmp(argv[i], name))
      return true;
  return false;
}

static std::string pathToContent(const std::string &path) {
  std::ifstream file(path.c_str(), std::ifstream::binary);

  file.seekg(0, file.end);
  int size = int(file.tellg());
  file.seekg(0, file.beg);

  // And allocate buffer to that a single line can be read...
  char *buffer = new char[size + 1];
  file.read(buffer, size);

  // Terminate the string
  buffer[size] = 0;
  std::string result = buffer;
  file.close();
  delete[] buffer;
  return result;
}

#endif

/**************************  END  misc.h **************************/
#include "plugin.hpp"

#ifdef SOUNDFILE
/************************** BEGIN SoundUI.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __SoundUI_H__
#define __SoundUI_H__

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

#if defined(__APPLE__) && !defined(__VCVRACK__) && !defined(JUCE_32BIT) &&     \
    !defined(JUCE_64BIT)
#include <CoreFoundation/CFBundle.h>
#endif

// Always included otherwise -i mode later on will not always include it (with
// the conditional includes)
/************************** BEGIN Soundfile.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __Soundfile__
#define __Soundfile__

#include <string.h>
#include <string>
#include <vector>

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#define BUFFER_SIZE 1024
#define SAMPLE_RATE 44100
#define MAX_CHAN 64
#define MAX_SOUNDFILE_PARTS 256

#ifdef _MSC_VER
#define PRE_PACKED_STRUCTURE __pragma(pack(push, 1))
#define POST_PACKED_STRUCTURE                                                  \
  ;                                                                            \
  __pragma(pack(pop))
#else
#define PRE_PACKED_STRUCTURE
#define POST_PACKED_STRUCTURE __attribute__((__packed__))
#endif

/*
 The soundfile structure to be used by the DSP code. Soundfile has a
 MAX_SOUNDFILE_PARTS parts (even a single soundfile or an empty soundfile). The
 fLength, fOffset and fSR fields are filled accordingly by repeating the actual
 parts if needed. The fBuffers contains MAX_CHAN non-interleaved arrays of
 samples.

 It has to be 'packed' to that the LLVM backend can correctly access it.

 Index computation:
        - p is the current part number [0..MAX_SOUNDFILE_PARTS-1] (must be
 proved by the type system)
        - i is the current position in the part. It will be constrained between
 [0..length]
        - idx(p,i) = fOffset[p] + max(0, min(i, fLength[p]));
*/

PRE_PACKED_STRUCTURE
struct Soundfile {
  void *fBuffers; // will correspond to a double** or float** pointer chosen at
                  // runtime
  int *fLength;   // length of each part (so fLength[P] contains the length in
                  // frames of part P)
  int *fSR; // sample rate of each part (so fSR[P] contains the SR of part P)
  int *fOffset;   // offset of each part in the global buffer (so fOffset[P]
                  // contains the offset in frames of part P)
  int fChannels;  // max number of channels of all concatenated files
  int fParts;     // the total number of loaded parts
  bool fIsDouble; // keep the sample format (float or double)

  Soundfile(int cur_chan, int length, int max_chan, int total_parts,
            bool is_double) {
    fLength = new int[MAX_SOUNDFILE_PARTS];
    fSR = new int[MAX_SOUNDFILE_PARTS];
    fOffset = new int[MAX_SOUNDFILE_PARTS];
    fIsDouble = is_double;
    fChannels = cur_chan;
    fParts = total_parts;
    if (fIsDouble) {
      fBuffers = allocBufferReal<double>(cur_chan, length, max_chan);
    } else {
      fBuffers = allocBufferReal<float>(cur_chan, length, max_chan);
    }
  }

  template <typename REAL>
  void *allocBufferReal(int cur_chan, int length, int max_chan) {
    REAL **buffers = new REAL *[max_chan];
    for (int chan = 0; chan < cur_chan; chan++) {
      buffers[chan] = new REAL[length];
      memset(buffers[chan], 0, sizeof(REAL) * length);
    }
    return buffers;
  }

  void copyToOut(int size, int channels, int max_channels, int offset,
                 void *buffer) {
    if (fIsDouble) {
      copyToOutReal<double>(size, channels, max_channels, offset, buffer);
    } else {
      copyToOutReal<float>(size, channels, max_channels, offset, buffer);
    }
  }

  void shareBuffers(int cur_chan, int max_chan) {
    // Share the same buffers for all other channels so that we have max_chan
    // channels available
    if (fIsDouble) {
      for (int chan = cur_chan; chan < max_chan; chan++) {
        static_cast<double **>(fBuffers)[chan] =
            static_cast<double **>(fBuffers)[chan % cur_chan];
      }
    } else {
      for (int chan = cur_chan; chan < max_chan; chan++) {
        static_cast<float **>(fBuffers)[chan] =
            static_cast<float **>(fBuffers)[chan % cur_chan];
      }
    }
  }

  template <typename REAL>
  void copyToOutReal(int size, int channels, int max_channels, int offset,
                     void *buffer) {
    for (int sample = 0; sample < size; sample++) {
      for (int chan = 0; chan < channels; chan++) {
        static_cast<REAL **>(fBuffers)[chan][offset + sample] =
            static_cast<REAL *>(buffer)[sample * max_channels + chan];
      }
    }
  }

  template <typename REAL>
  void getBuffersOffsetReal(void *buffers, int offset) {
    for (int chan = 0; chan < fChannels; chan++) {
      static_cast<REAL **>(buffers)[chan] =
          &(static_cast<REAL **>(fBuffers))[chan][offset];
    }
  }

  void emptyFile(int part, int &offset) {
    fLength[part] = BUFFER_SIZE;
    fSR[part] = SAMPLE_RATE;
    fOffset[part] = offset;
    // Update offset
    offset += fLength[part];
  }

  ~Soundfile() {
    // Free the real channels only
    if (fIsDouble) {
      for (int chan = 0; chan < fChannels; chan++) {
        delete[] static_cast<double **>(fBuffers)[chan];
      }
      delete[] static_cast<double **>(fBuffers);
    } else {
      for (int chan = 0; chan < fChannels; chan++) {
        delete[] static_cast<float **>(fBuffers)[chan];
      }
      delete[] static_cast<float **>(fBuffers);
    }
    delete[] fLength;
    delete[] fSR;
    delete[] fOffset;
  }

  typedef std::vector<std::string> Directories;

} POST_PACKED_STRUCTURE;

/*
 The generic soundfile reader.
 */

class SoundfileReader {

protected:
  int fDriverSR;

  // Check if a soundfile exists and return its real path_name
  std::string checkFile(const Soundfile::Directories &sound_directories,
                        const std::string &file_name) {
    if (checkFile(file_name)) {
      return file_name;
    } else {
      for (size_t i = 0; i < sound_directories.size(); i++) {
        std::string path_name = sound_directories[i] + "/" + file_name;
        if (checkFile(path_name)) {
          return path_name;
        }
      }
      return "";
    }
  }

  bool isResampling(int sample_rate) {
    return (fDriverSR > 0 && fDriverSR != sample_rate);
  }

  // To be implemented by subclasses

  /**
   * Check the availability of a sound resource.
   *
   * @param path_name - the name of the file, or sound resource identified this
   * way
   *
   * @return true if the sound resource is available, false otherwise.
   */
  virtual bool checkFile(const std::string &path_name) = 0;

  /**
   * Check the availability of a sound resource.
   *
   * @param buffer - the sound buffer
   * @param size - the sound buffer length
   *
   * @return true if the sound resource is available, false otherwise.
   */

  virtual bool checkFile(unsigned char *buffer, size_t size) { return true; }

  /**
   * Get the channels and length values of the given sound resource.
   *
   * @param path_name - the name of the file, or sound resource identified this
   * way
   * @param channels - the channels value to be filled with the sound resource
   * number of channels
   * @param length - the length value to be filled with the sound resource
   * length in frames
   *
   */
  virtual void getParamsFile(const std::string &path_name, int &channels,
                             int &length) = 0;

  /**
   * Get the channels and length values of the given sound resource.
   *
   * @param buffer - the sound buffer
   * @param size - the sound buffer length
   * @param channels - the channels value to be filled with the sound resource
   * number of channels
   * @param length - the length value to be filled with the sound resource
   * length in frames
   *
   */
  virtual void getParamsFile(unsigned char *buffer, size_t size, int &channels,
                             int &length) {}

  /**
   * Read one sound resource and fill the 'soundfile' structure accordingly
   *
   * @param soundfile - the soundfile to be filled
   * @param path_name - the name of the file, or sound resource identified this
   * way
   * @param part - the part number to be filled in the soundfile
   * @param offset - the offset value to be incremented with the actual sound
   * resource length in frames
   * @param max_chan - the maximum number of mono channels to fill
   *
   */
  virtual void readFile(Soundfile *soundfile, const std::string &path_name,
                        int part, int &offset, int max_chan) = 0;

  /**
   * Read one sound resource and fill the 'soundfile' structure accordingly
   *
   * @param soundfile - the soundfile to be filled
   * @param buffer - the sound buffer
   * @param size - the sound buffer length
   * @param part - the part number to be filled in the soundfile
   * @param offset - the offset value to be incremented with the actual sound
   * resource length in frames
   * @param max_chan - the maximum number of mono channels to fill
   *
   */
  virtual void readFile(Soundfile *soundfile, unsigned char *buffer,
                        size_t size, int part, int &offset, int max_chan) {}

public:
  SoundfileReader() {}
  virtual ~SoundfileReader() {}

  void setSampleRate(int sample_rate) { fDriverSR = sample_rate; }

  Soundfile *createSoundfile(const std::vector<std::string> &path_name_list,
                             int max_chan, bool is_double) {
    try {
      int cur_chan = 1; // At least one channel
      int total_length = 0;

      // Compute total length and channels max of all files
      for (size_t i = 0; i < path_name_list.size(); i++) {
        int chan, length;
        if (path_name_list[i] == "__empty_sound__") {
          length = BUFFER_SIZE;
          chan = 1;
        } else {
          getParamsFile(path_name_list[i], chan, length);
        }
        cur_chan = std::max<int>(cur_chan, chan);
        total_length += length;
      }

      // Complete with empty parts
      total_length +=
          (MAX_SOUNDFILE_PARTS - path_name_list.size()) * BUFFER_SIZE;

      // Create the soundfile
      Soundfile *soundfile = new Soundfile(cur_chan, total_length, max_chan,
                                           path_name_list.size(), is_double);

      // Init offset
      int offset = 0;

      // Read all files
      for (size_t i = 0; i < path_name_list.size(); i++) {
        if (path_name_list[i] == "__empty_sound__") {
          soundfile->emptyFile(i, offset);
        } else {
          readFile(soundfile, path_name_list[i], i, offset, max_chan);
        }
      }

      // Complete with empty parts
      for (size_t i = path_name_list.size(); i < MAX_SOUNDFILE_PARTS; i++) {
        soundfile->emptyFile(i, offset);
      }

      // Share the same buffers for all other channels so that we have max_chan
      // channels available
      soundfile->shareBuffers(cur_chan, max_chan);
      return soundfile;
    } catch (...) {
      return nullptr;
    }
  }

  // Check if all soundfiles exist and return their real path_name
  std::vector<std::string>
  checkFiles(const Soundfile::Directories &sound_directories,
             const std::vector<std::string> &file_name_list) {
    std::vector<std::string> path_name_list;
    for (size_t i = 0; i < file_name_list.size(); i++) {
      std::string path_name = checkFile(sound_directories, file_name_list[i]);
      // If 'path_name' is not found, it is replaced by an empty sound (=
      // silence)
      path_name_list.push_back((path_name == "") ? "__empty_sound__"
                                                 : path_name);
    }
    return path_name_list;
  }
};

#endif
/**************************  END  Soundfile.h **************************/

#if defined(JUCE_32BIT) || defined(JUCE_64BIT)
/************************** BEGIN JuceReader.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __JuceReader__
#define __JuceReader__

#include <assert.h>

#include "../JuceLibraryCode/JuceHeader.h"

struct JuceReader : public SoundfileReader {

  juce::AudioFormatManager fFormatManager;

  JuceReader() { fFormatManager.registerBasicFormats(); }
  virtual ~JuceReader() {}

  bool checkFile(const std::string &path_name) override {
    juce::File file =
        juce::File::getCurrentWorkingDirectory().getChildFile(path_name);
    if (file.existsAsFile()) {
      return true;
    } else {
      // std::cerr << "ERROR : cannot open '" << path_name << "'" << std::endl;
      return false;
    }
  }

  void getParamsFile(const std::string &path_name, int &channels,
                     int &length) override {
    std::unique_ptr<juce::AudioFormatReader> formatReader(
        fFormatManager.createReaderFor(
            juce::File::getCurrentWorkingDirectory().getChildFile(path_name)));
    channels = int(formatReader->numChannels);
    length = int(formatReader->lengthInSamples);
  }

  void readFile(Soundfile *soundfile, const std::string &path_name, int part,
                int &offset, int max_chan) override {
    std::unique_ptr<juce::AudioFormatReader> formatReader(
        fFormatManager.createReaderFor(
            juce::File::getCurrentWorkingDirectory().getChildFile(path_name)));

    soundfile->fLength[part] = int(formatReader->lengthInSamples);
    soundfile->fSR[part] = int(formatReader->sampleRate);
    soundfile->fOffset[part] = offset;

    void *buffers;
    if (soundfile->fIsDouble) {
      buffers = alloca(soundfile->fChannels * sizeof(double *));
      soundfile->getBuffersOffsetReal<double>(buffers, offset);
    } else {
      buffers = alloca(soundfile->fChannels * sizeof(float *));
      soundfile->getBuffersOffsetReal<float>(buffers, offset);
    }

    if (formatReader->read(reinterpret_cast<int *const *>(buffers),
                           int(formatReader->numChannels), 0,
                           int(formatReader->lengthInSamples), false)) {

      // Possibly convert samples
      if (!formatReader->usesFloatingPointData) {
        for (int chan = 0; chan < int(formatReader->numChannels); ++chan) {
          if (soundfile->fIsDouble) {
            // TODO
          } else {
            float *buffer = &(static_cast<float **>(
                soundfile->fBuffers))[chan][soundfile->fOffset[part]];
            juce::FloatVectorOperations::convertFixedToFloat(
                buffer, reinterpret_cast<const int *>(buffer),
                1.0f / 0x7fffffff, int(formatReader->lengthInSamples));
          }
        }
      }
    } else {
      std::cerr << "Error reading the file : " << path_name << std::endl;
    }

    // Update offset
    offset += soundfile->fLength[part];
  }
};

#endif
/**************************  END  JuceReader.h **************************/
static JuceReader gReader;
#elif defined(DAISY) || defined(SUPERCOLLIDER)
/************************** BEGIN WaveReader.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ********************************************************************/

#ifndef __WaveReader__
#define __WaveReader__

#include <assert.h>
#include <stdio.h>
#include <string.h>

// WAVE file description
typedef struct {

  // The canonical WAVE format starts with the RIFF header

  /**
   Variable: chunk_id
   Contains the letters "RIFF" in ASCII form (0x52494646 big-endian form).
   **/
  int chunk_id;

  /**
   Variable: chunk_size
   36 + SubChunk2Size, or more precisely: 4 + (8 + SubChunk1Size) + (8 +
   SubChunk2Size) This is the size of the rest of the chunk following this
   number. This is the size of the entire file in bytes minus 8 bytes for the
   two fields not included in this count: ChunkID and ChunkSize.
   **/
  int chunk_size;

  /**
   Variable: format
   Contains the letters "WAVE" (0x57415645 big-endian form).
   **/
  int format;

  // The "WAVE" format consists of two subchunks: "fmt " and "data":
  // The "fmt " subchunk describes the sound data's format:

  /**
   Variable: subchunk_1_id
   Contains the letters "fmt " (0x666d7420 big-endian form).
   **/
  int subchunk_1_id;

  /**
   Variable: subchunk_1_size
   16 for PCM. This is the size of the rest of the Subchunk which follows this
   number.
   **/
  int subchunk_1_size;

  /**
   Variable: audio_format
   PCM = 1 (i.e. Linear quantization) Values other than 1 indicate some form of
   compression.
   **/
  short audio_format;

  /**
   Variable: num_channels
   Mono = 1, Stereo = 2, etc.
   **/
  short num_channels;

  /**
   Variable: sample_rate
   8000, 44100, etc.
   **/
  int sample_rate;

  /**
   Variable: byte_rate
   == SampleRate * NumChannels * BitsPerSample/8
   **/
  int byte_rate;

  /**
   Variable: block_align
   == NumChannels * BitsPerSample/8
   The number of bytes for one sample including all channels. I wonder what
   happens when this number isn't an integer?
   **/
  short block_align;

  /**
   Variable: bits_per_sample
   8 bits = 8, 16 bits = 16, etc.
   **/
  short bits_per_sample;

  /**
   Here should come some extra parameters which i will avoid.
   **/

  // The "data" subchunk contains the size of the data and the actual sound:

  /**
   Variable: subchunk_2_id
   Contains the letters "data" (0x64617461 big-endian form).
   **/
  int subchunk_2_id;

  /**
   Variable: subchunk_2_size
   == NumSamples * NumChannels * BitsPerSample/8
   This is the number of bytes in the data. You can also think of this as the
   size of the read of the subchunk following this number.
   **/
  int subchunk_2_size;

  /**
   Variable: data
   The actual sound data.
   **/
  char *data;

} wave_t;

// Base reader
struct Reader {

  wave_t *fWave;

  inline int is_big_endian() {
    int a = 1;
    return !((char *)&a)[0];
  }

  inline int convert_to_int(char *buffer, int len) {
    int a = 0;
    if (!is_big_endian()) {
      for (int i = 0; i < len; i++) {
        ((char *)&a)[i] = buffer[i];
      }
    } else {
      for (int i = 0; i < len; i++) {
        ((char *)&a)[3 - i] = buffer[i];
      }
    }
    return a;
  }

  Reader() { fWave = (wave_t *)calloc(1, sizeof(wave_t)); }

  virtual ~Reader() {
    free(fWave->data);
    free(fWave);
  }

  bool load_wave_header() {
    char buffer[4];

    read(buffer, 4);
    if (strncmp(buffer, "RIFF", 4) != 0) {
      fprintf(stderr, "This is not valid WAV file!\n");
      return false;
    }
    fWave->chunk_id = convert_to_int(buffer, 4);

    read(buffer, 4);
    fWave->chunk_size = convert_to_int(buffer, 4);

    read(buffer, 4);
    fWave->format = convert_to_int(buffer, 4);

    read(buffer, 4);
    fWave->subchunk_1_id = convert_to_int(buffer, 4);

    read(buffer, 4);
    fWave->subchunk_1_size = convert_to_int(buffer, 4);

    read(buffer, 2);
    fWave->audio_format = convert_to_int(buffer, 2);

    read(buffer, 2);
    fWave->num_channels = convert_to_int(buffer, 2);

    read(buffer, 4);
    fWave->sample_rate = convert_to_int(buffer, 4);

    read(buffer, 4);
    fWave->byte_rate = convert_to_int(buffer, 4);

    read(buffer, 2);
    fWave->block_align = convert_to_int(buffer, 2);

    read(buffer, 2);
    fWave->bits_per_sample = convert_to_int(buffer, 2);

    read(buffer, 4);
    if (strncmp(buffer, "data", 4) != 0) {
      read(buffer, 4);
      int _extra_size = convert_to_int(buffer, 4);
      char _extra_data[_extra_size];
      read(_extra_data, _extra_size);
      read(buffer, 4);
      fWave->subchunk_2_id = convert_to_int(buffer, 4);
    } else {
      fWave->subchunk_2_id = convert_to_int(buffer, 4);
    }

    read(buffer, 4);
    fWave->subchunk_2_size = convert_to_int(buffer, 4);
    return true;
  }

  void load_wave() {
    // Read sound data
    fWave->data = (char *)malloc(fWave->subchunk_2_size);
    read(fWave->data, fWave->subchunk_2_size);
  }

  virtual void read(char *buffer, unsigned int size) = 0;
};

struct FileReader : public Reader {

  FILE *fFile;

  FileReader(const std::string &file_path) {
    fFile = fopen(file_path.c_str(), "rb");
    if (!fFile) {
      fprintf(stderr, "FileReader : cannot open file!\n");
      throw -1;
    }
    if (!load_wave_header()) {
      fprintf(stderr, "FileReader : not a WAV file!\n");
      throw -1;
    }
  }

  virtual ~FileReader() { fclose(fFile); }

  void read(char *buffer, unsigned int size) { fread(buffer, 1, size, fFile); }
};

extern const uint8_t file_start[] asm("_binary_FILE_start");
extern const uint8_t file_end[] asm("_binary_FILE_end");

struct MemoryReader : public Reader {

  int fPos;
  const uint8_t *fStart;
  const uint8_t *fEnd;

  MemoryReader(const uint8_t *start, const uint8_t *end) : fPos(0) {
    fStart = start;
    fEnd = end;
    if (!load_wave_header()) {
      fprintf(stderr, "MemoryReader : not a WAV file!\n");
      throw -1;
    }
  }

  virtual ~MemoryReader() {}

  void read(char *buffer, unsigned int size) {
    memcpy(buffer, fStart + fPos, size);
    fPos += size;
  }
};

// Using a FileReader to implement SoundfileReader

struct WaveReader : public SoundfileReader {

  WaveReader() {}
  virtual ~WaveReader() {}

  bool checkFile(const std::string &path_name) override {
    try {
      FileReader reader(path_name);
      return true;
    } catch (...) {
      return false;
    }
  }

  void getParamsFile(const std::string &path_name, int &channels,
                     int &length) override {
    FileReader reader(path_name);
    channels = reader.fWave->num_channels;
    length = (reader.fWave->subchunk_2_size * 8) /
             (reader.fWave->num_channels * reader.fWave->bits_per_sample);
  }

  void readFile(Soundfile *soundfile, const std::string &path_name, int part,
                int &offset, int max_chan) override {
    FileReader reader(path_name);
    reader.load_wave();

    soundfile->fLength[part] =
        (reader.fWave->subchunk_2_size * 8) /
        (reader.fWave->num_channels * reader.fWave->bits_per_sample);
    soundfile->fSR[part] = reader.fWave->sample_rate;
    soundfile->fOffset[part] = offset;

    // Audio frames have to be written for each chan
    if (reader.fWave->bits_per_sample == 16) {
      float factor = 1.f / 32767.f;
      for (int sample = 0; sample < soundfile->fLength[part]; sample++) {
        short *frame =
            (short *)&reader.fWave->data[reader.fWave->block_align * sample];
        if (soundfile->fIsDouble) {
          for (int chan = 0; chan < reader.fWave->num_channels; chan++) {
            static_cast<double **>(soundfile->fBuffers)[chan][offset + sample] =
                frame[chan] * factor;
          }
        } else {
          for (int chan = 0; chan < reader.fWave->num_channels; chan++) {
            static_cast<float **>(soundfile->fBuffers)[chan][offset + sample] =
                frame[chan] * factor;
          }
        }
      }
    } else if (reader.fWave->bits_per_sample == 32) {
      fprintf(stderr, "readFile : not implemented\n");
    }

    // Update offset
    offset += soundfile->fLength[part];
  }
};

#endif
/**************************  END  WaveReader.h **************************/
static WaveReader gReader;
#elif defined(ESP32)
/************************** BEGIN Esp32Reader.h **************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 *************************************************************************/

#ifndef FAUST_ESP32READER_H
#define FAUST_ESP32READER_H

#include "driver/sdspi_host.h"
#include "esp_err.h"
#include "esp_log.h"
#include "esp_spi_flash.h"
#include "esp_vfs_fat.h"
#include "sdmmc_cmd.h"
#include <stdio.h>

#define TAG "Esp32Reader"

#define SD_PIN_NUM_MISO GPIO_NUM_2
#define SD_PIN_NUM_MOSI GPIO_NUM_15
#define SD_PIN_NUM_CLK GPIO_NUM_14
#define SD_PIN_NUM_CS GPIO_NUM_13

struct Esp32Reader : public WaveReader {

  void sdcard_init() {
    ESP_LOGI(TAG, "Initializing SD card");
    ESP_LOGI(TAG, "Using SPI peripheral");

    sdmmc_host_t host = SDSPI_HOST_DEFAULT();
    sdspi_slot_config_t slot_config = SDSPI_SLOT_CONFIG_DEFAULT();
    slot_config.gpio_miso = SD_PIN_NUM_MISO;
    slot_config.gpio_mosi = SD_PIN_NUM_MOSI;
    slot_config.gpio_sck = SD_PIN_NUM_CLK;
    slot_config.gpio_cs = SD_PIN_NUM_CS;
    // This initializes the slot without card detect (CD) and write protect (WP)
    // signals. Modify slot_config.gpio_cd and slot_config.gpio_wp if your board
    // has these signals.

    // Options for mounting the filesystem.
    // If format_if_mount_failed is set to true, SD card will be partitioned and
    // formatted in case when mounting fails.
    esp_vfs_fat_sdmmc_mount_config_t mount_config = {
        .format_if_mount_failed = false,
        .max_files = 5,
        .allocation_unit_size = 16 * 1024};

    // Use settings defined above to initialize SD card and mount FAT
    // filesystem. Note: esp_vfs_fat_sdmmc_mount is an all-in-one convenience
    // function. Please check its source code and implement error recovery when
    // developing production applications.
    sdmmc_card_t *card;
    esp_err_t ret = esp_vfs_fat_sdmmc_mount("/sdcard", &host, &slot_config,
                                            &mount_config, &card);

    if (ret != ESP_OK) {
      if (ret == ESP_FAIL) {
        ESP_LOGE(TAG, "Failed to mount filesystem. "
                      "If you want the card to be formatted, set "
                      "format_if_mount_failed = true.");
      } else {
        ESP_LOGE(TAG,
                 "Failed to initialize the card (%s). "
                 "Make sure SD card lines have pull-up resistors in place.",
                 esp_err_to_name(ret));
      }
      return;
    }

    // Card has been initialized, print its properties
    sdmmc_card_print_info(stdout, card);
    ESP_LOGI(TAG, "SD card initialized");
  }

  Esp32Reader() { sdcard_init(); }

  // Access methods inherited from WaveReader
};

#endif // FAUST_ESP32READER_H
/**************************  END  Esp32Reader.h **************************/
static Esp32Reader gReader;
#elif defined(MEMORY_READER)
/************************** BEGIN MemoryReader.h ************************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __MemoryReader__
#define __MemoryReader__

/*
 A 'MemoryReader' object can be used to prepare a set of sound resources in
 memory, to be used by SoundUI::addSoundfile.

 A Soundfile* object will have to be filled with a list of sound resources: the
 fLength, fOffset, fSampleRate and fBuffers fields have to be completed with the
 appropriate values, and will be accessed in the DSP object while running.
 *
 */

// To adapt for a real case use

#define SOUND_CHAN 2
#define SOUND_LENGTH 4096
#define SOUND_SR 44100

struct MemoryReader : public SoundfileReader {

  MemoryReader() {}
  virtual ~MemoryReader() {}

  /**
   * Check the availability of a sound resource.
   *
   * @param path_name - the name of the file, or sound resource identified this
   * way
   *
   * @return true if the sound resource is available, false otherwise.
   */
  virtual bool checkFile(const std::string &path_name) override { return true; }

  /**
   * Get the channels and length values of the given sound resource.
   *
   * @param path_name - the name of the file, or sound resource identified this
   * way
   * @param channels - the channels value to be filled with the sound resource
   * number of channels
   * @param length - the length value to be filled with the sound resource
   * length in frames
   *
   */
  virtual void getParamsFile(const std::string &path_name, int &channels,
                             int &length) override {
    channels = SOUND_CHAN;
    length = SOUND_LENGTH;
  }

  /**
   * Read one sound resource and fill the 'soundfile' structure accordingly
   *
   * @param path_name - the name of the file, or sound resource identified this
   * way
   * @param part - the part number to be filled in the soundfile
   * @param offset - the offset value to be incremented with the actual sound
   * resource length in frames
   * @param max_chan - the maximum number of mono channels to fill
   *
   */
  virtual void readFile(Soundfile *soundfile, const std::string &path_name,
                        int part, int &offset, int max_chan) override {
    soundfile->fLength[part] = SOUND_LENGTH;
    soundfile->fSR[part] = SOUND_SR;
    soundfile->fOffset[part] = offset;

    // Audio frames have to be written for each chan
    if (soundfile->fIsDouble) {
      for (int sample = 0; sample < SOUND_LENGTH; sample++) {
        for (int chan = 0; chan < SOUND_CHAN; chan++) {
          static_cast<double **>(soundfile->fBuffers)[chan][offset + sample] =
              0.f;
        }
      }
    } else {
      for (int sample = 0; sample < SOUND_LENGTH; sample++) {
        for (int chan = 0; chan < SOUND_CHAN; chan++) {
          static_cast<float **>(soundfile->fBuffers)[chan][offset + sample] =
              0.f;
        }
      }
    }

    // Update offset
    offset += SOUND_LENGTH;
  }
};

#endif
/**************************  END  MemoryReader.h **************************/
static MemoryReader gReader;
#else
/************************** BEGIN LibsndfileReader.h *********************
 FAUST Architecture File
 Copyright (C) 2003-2022 GRAME, Centre National de Creation Musicale
 ---------------------------------------------------------------------
 This program is free software; you can redistribute it and/or modify
 it under the terms of the GNU Lesser General Public License as published by
 the Free Software Foundation; either version 2.1 of the License, or
 (at your option) any later version.

 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 GNU Lesser General Public License for more details.

 You should have received a copy of the GNU Lesser General Public License
 along with this program; if not, write to the Free Software
 Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.

 EXCEPTION : As a special exception, you may create a larger work
 that contains this FAUST architecture section and distribute
 that work under terms of your choice, so long as this FAUST
 architecture section is not modified.
 ************************************************************************/

#ifndef __LibsndfileReader__
#define __LibsndfileReader__

#ifdef _SAMPLERATE
#include <samplerate.h>
#endif
#include <assert.h>
#include <fstream>
#include <iostream>
#include <sndfile.h>
#include <string.h>

/*
// Deactivated for now, since the macOS remote cross-compiler fails with this
code. #if __has_include(<filesystem>) && __cplusplus >= 201703L #define
HAS_FILESYSTEM #include <filesystem> namespace fs = std::filesystem; #elif
__has_include(<experimental/filesystem>) && __cplusplus >= 201103L #define
HAS_FILESYSTEM #include <experimental/filesystem> namespace fs =
std::experimental::filesystem; #endif
*/

struct VFLibsndfile {

#define SIGNED_SIZEOF(x) ((int)sizeof(x))

  unsigned char *fBuffer;
  size_t fLength;
  size_t fOffset;
  SF_VIRTUAL_IO fVIO;

  VFLibsndfile(unsigned char *buffer, size_t length)
      : fBuffer(buffer), fLength(length), fOffset(0) {
    fVIO.get_filelen = vfget_filelen;
    fVIO.seek = vfseek;
    fVIO.read = vfread;
    fVIO.write = vfwrite;
    fVIO.tell = vftell;
  }

  static sf_count_t vfget_filelen(void *user_data) {
    VFLibsndfile *vf = static_cast<VFLibsndfile *>(user_data);
    return vf->fLength;
  }

  static sf_count_t vfseek(sf_count_t offset, int whence, void *user_data) {
    VFLibsndfile *vf = static_cast<VFLibsndfile *>(user_data);
    switch (whence) {
    case SEEK_SET:
      vf->fOffset = offset;
      break;

    case SEEK_CUR:
      vf->fOffset = vf->fOffset + offset;
      break;

    case SEEK_END:
      vf->fOffset = vf->fLength + offset;
      break;

    default:
      break;
    };

    return vf->fOffset;
  }

  static sf_count_t vfread(void *ptr, sf_count_t count, void *user_data) {
    VFLibsndfile *vf = static_cast<VFLibsndfile *>(user_data);

    /*
     **	This will break badly for files over 2Gig in length, but
     **	is sufficient for testing.
     */
    if (vf->fOffset + count > vf->fLength) {
      count = vf->fLength - vf->fOffset;
    }

    memcpy(ptr, vf->fBuffer + vf->fOffset, count);
    vf->fOffset += count;

    return count;
  }

  static sf_count_t vfwrite(const void *ptr, sf_count_t count,
                            void *user_data) {
    VFLibsndfile *vf = static_cast<VFLibsndfile *>(user_data);

    /*
     **	This will break badly for files over 2Gig in length, but
     **	is sufficient for testing.
     */
    if (vf->fOffset >= SIGNED_SIZEOF(vf->fBuffer)) {
      return 0;
    }

    if (vf->fOffset + count > SIGNED_SIZEOF(vf->fBuffer)) {
      count = sizeof(vf->fBuffer) - vf->fOffset;
    }

    memcpy(vf->fBuffer + vf->fOffset, ptr, (size_t)count);
    vf->fOffset += count;

    if (vf->fOffset > vf->fLength) {
      vf->fLength = vf->fOffset;
    }

    return count;
  }

  static sf_count_t vftell(void *user_data) {
    VFLibsndfile *vf = static_cast<VFLibsndfile *>(user_data);
    return vf->fOffset;
  }
};

struct LibsndfileReader : public SoundfileReader {

  LibsndfileReader() {}

  typedef sf_count_t (*sample_read)(SNDFILE *sndfile, void *buffer,
                                    sf_count_t frames);

  // Check file
  bool checkFile(const std::string &path_name) override {
    /*
     // Better since it supports Unicode characters.
     #ifdef HAS_FILESYSTEM
     if (!fs::exists(path_name)) {
            std::cerr << "FILE NOT FOUND\n";
            return false;
     }
     #endif
     */

    std::ofstream ofs;
    ofs.open(path_name, std::ios_base::in);
    if (!ofs.is_open()) {
      return false;
    }

    SF_INFO snd_info;
    snd_info.format = 0;
    SNDFILE *snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
    return checkFileAux(snd_file, path_name);
  }

  bool checkFile(unsigned char *buffer, size_t length) override {
    SF_INFO snd_info;
    snd_info.format = 0;
    VFLibsndfile vio(buffer, length);
    SNDFILE *snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
    return checkFileAux(snd_file, "virtual file");
  }

  bool checkFileAux(SNDFILE *snd_file, const std::string &path_name) {
    if (snd_file) {
      sf_close(snd_file);
      return true;
    } else {
      std::cerr << "ERROR : cannot open '" << path_name << "' ("
                << sf_strerror(NULL) << ")" << std::endl;
      return false;
    }
  }

  // Open the file and returns its length and channels
  void getParamsFile(const std::string &path_name, int &channels,
                     int &length) override {
    SF_INFO snd_info;
    snd_info.format = 0;
    SNDFILE *snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
    getParamsFileAux(snd_file, snd_info, channels, length);
  }

  void getParamsFile(unsigned char *buffer, size_t size, int &channels,
                     int &length) override {
    SF_INFO snd_info;
    snd_info.format = 0;
    VFLibsndfile vio(buffer, size);
    SNDFILE *snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
    getParamsFileAux(snd_file, snd_info, channels, length);
  }

  void getParamsFileAux(SNDFILE *snd_file, const SF_INFO &snd_info,
                        int &channels, int &length) {
    assert(snd_file);
    channels = int(snd_info.channels);
#ifdef _SAMPLERATE
    length = (isResampling(snd_info.samplerate))
                 ? ((double(snd_info.frames) * double(fDriverSR) /
                     double(snd_info.samplerate)) +
                    BUFFER_SIZE)
                 : int(snd_info.frames);
#else
    length = int(snd_info.frames);
#endif
    sf_close(snd_file);
  }

  // Read the file
  void readFile(Soundfile *soundfile, const std::string &path_name, int part,
                int &offset, int max_chan) override {
    SF_INFO snd_info;
    snd_info.format = 0;
    SNDFILE *snd_file = sf_open(path_name.c_str(), SFM_READ, &snd_info);
    readFileAux(soundfile, snd_file, snd_info, part, offset, max_chan);
  }

  void readFile(Soundfile *soundfile, unsigned char *buffer, size_t length,
                int part, int &offset, int max_chan) override {
    SF_INFO snd_info;
    snd_info.format = 0;
    VFLibsndfile vio(buffer, length);
    SNDFILE *snd_file = sf_open_virtual(&vio.fVIO, SFM_READ, &snd_info, &vio);
    readFileAux(soundfile, snd_file, snd_info, part, offset, max_chan);
  }

  // Will be called to fill all parts from 0 to MAX_SOUNDFILE_PARTS-1
  void readFileAux(Soundfile *soundfile, SNDFILE *snd_file,
                   const SF_INFO &snd_info, int part, int &offset,
                   int max_chan) {
    assert(snd_file);
    int channels = std::min<int>(max_chan, snd_info.channels);
#ifdef _SAMPLERATE
    if (isResampling(snd_info.samplerate)) {
      soundfile->fLength[part] =
          int(double(snd_info.frames) * double(fDriverSR) /
              double(snd_info.samplerate));
      soundfile->fSR[part] = fDriverSR;
    } else {
      soundfile->fLength[part] = int(snd_info.frames);
      soundfile->fSR[part] = snd_info.samplerate;
    }
#else
    soundfile->fLength[part] = int(snd_info.frames);
    soundfile->fSR[part] = snd_info.samplerate;
#endif
    soundfile->fOffset[part] = offset;

    // Read and fill snd_info.channels number of channels
    sf_count_t nbf;

    sample_read reader;
    void *buffer_in = nullptr;
    if (soundfile->fIsDouble) {
      buffer_in = static_cast<double *>(
          alloca(BUFFER_SIZE * sizeof(double) * snd_info.channels));
      reader = reinterpret_cast<sample_read>(sf_readf_double);
    } else {
      buffer_in = static_cast<float *>(
          alloca(BUFFER_SIZE * sizeof(float) * snd_info.channels));
      reader = reinterpret_cast<sample_read>(sf_readf_float);
    }

#ifdef _SAMPLERATE
    // Resampling
    SRC_STATE *resampler = nullptr;
    float *src_buffer_out = nullptr;
    float *src_buffer_in = nullptr;
    void *buffer_out = nullptr;
    if (isResampling(snd_info.samplerate)) {
      int error;
      resampler = src_new(SRC_SINC_FASTEST, channels, &error);
      if (error != 0) {
        std::cerr << "ERROR : src_new " << src_strerror(error) << std::endl;
        throw -1;
      }
      if (soundfile->fIsDouble) {
        // Additional buffers for SRC resampling
        src_buffer_in = static_cast<float *>(
            alloca(BUFFER_SIZE * sizeof(float) * snd_info.channels));
        src_buffer_out = static_cast<float *>(
            alloca(BUFFER_SIZE * sizeof(float) * snd_info.channels));
        buffer_out = static_cast<double *>(
            alloca(BUFFER_SIZE * sizeof(double) * snd_info.channels));
      } else {
        buffer_out = static_cast<float *>(
            alloca(BUFFER_SIZE * sizeof(float) * snd_info.channels));
      }
    }
#endif

    do {
      nbf = reader(snd_file, buffer_in, BUFFER_SIZE);
#ifdef _SAMPLERATE
      // Resampling
      if (isResampling(snd_info.samplerate)) {
        int in_offset = 0;
        SRC_DATA src_data;
        src_data.src_ratio = double(fDriverSR) / double(snd_info.samplerate);
        if (soundfile->fIsDouble) {
          for (int frame = 0; frame < (BUFFER_SIZE * snd_info.channels);
               frame++) {
            src_buffer_in[frame] =
                float(static_cast<float *>(buffer_in)[frame]);
          }
        }
        do {
          if (soundfile->fIsDouble) {
            src_data.data_in = src_buffer_in;
            src_data.data_out = src_buffer_out;
          } else {
            src_data.data_in = static_cast<const float *>(buffer_in);
            src_data.data_out = static_cast<float *>(buffer_out);
          }
          src_data.input_frames = nbf - in_offset;
          src_data.output_frames = BUFFER_SIZE;
          src_data.end_of_input = (nbf < BUFFER_SIZE);
          int res = src_process(resampler, &src_data);
          if (res != 0) {
            std::cerr << "ERROR : src_process " << src_strerror(res)
                      << std::endl;
            throw -1;
          }
          if (soundfile->fIsDouble) {
            for (int frame = 0; frame < (BUFFER_SIZE * snd_info.channels);
                 frame++) {
              static_cast<double *>(buffer_out)[frame] =
                  double(src_buffer_out[frame]);
            }
          }
          soundfile->copyToOut(src_data.output_frames_gen, channels,
                               snd_info.channels, offset, buffer_out);
          in_offset += src_data.input_frames_used;
          // Update offset
          offset += src_data.output_frames_gen;
        } while (in_offset < nbf);
      } else {
        soundfile->copyToOut(nbf, channels, snd_info.channels, offset,
                             buffer_in);
        // Update offset
        offset += nbf;
      }
#else
      soundfile->copyToOut(nbf, channels, snd_info.channels, offset, buffer_in);
      // Update offset
      offset += nbf;
#endif
    } while (nbf == BUFFER_SIZE);

    sf_close(snd_file);
#ifdef _SAMPLERATE
    if (resampler)
      src_delete(resampler);
#endif
  }
};

#endif
/**************************  END  LibsndfileReader.h **************************/
static LibsndfileReader gReader;
#endif

// To be used by DSP code if no SoundUI is used
static std::vector<std::string> gPathNameList;
static Soundfile *defaultsound = nullptr;

class SoundUI : public SoundUIInterface {

protected:
  // The soundfile directories
  Soundfile::Directories fSoundfileDir;
  // Map to share loaded soundfiles
  std::map<std::string, std::shared_ptr<Soundfile>> fSoundfileMap;
  // The soundfile reader
  std::shared_ptr<SoundfileReader> fSoundReader;
  bool fIsDouble;

public:
  /**
   * Create a soundfile loader which will typically use a concrete
   * SoundfileReader like LibsndfileReader or JuceReader to load soundfiles.
   *
   * @param sound_directory - the base directory to look for files, which paths
   * will be relative to this one
   * @param sample_rate - the audio driver SR which may be different from the
   * file SR, to possibly resample files
   * @param reader - an alternative soundfile reader
   * @param is_double - whether Faust code has been compiled in -double mode and
   * soundfile buffers have to be in double
   *
   * @return the soundfile loader.
   */
  SoundUI(const std::string &sound_directory = "", int sample_rate = -1,
          SoundfileReader *reader = nullptr, bool is_double = false) {
    fSoundfileDir.push_back(sound_directory);
    fSoundReader = (reader) ? std::shared_ptr<SoundfileReader>(reader)
                            // the static gReader should not be deleted, so use
                            // an empty destructor
                            : std::shared_ptr<SoundfileReader>(
                                  std::shared_ptr<SoundfileReader>{}, &gReader);
    fSoundReader->setSampleRate(sample_rate);
    fIsDouble = is_double;
    if (!defaultsound)
      defaultsound =
          gReader.createSoundfile(gPathNameList, MAX_CHAN, is_double);
  }

  /**
   * Create a soundfile loader which will typically use a concrete
   * SoundfileReader like LibsndfileReader or JuceReader to load soundfiles.
   *
   * @param sound_directories - a vector of base directories to look for files,
   * which paths will be relative to these ones
   * @param sample_rate - the audio driver SR which may be different from the
   * file SR, to possibly resample files
   * @param reader - an alternative soundfile reader
   * @param is_double - whether Faust code has been compiled in -double mode and
   * soundfile buffers have to be in double
   *
   * @return the soundfile loader.
   */
  SoundUI(const Soundfile::Directories &sound_directories, int sample_rate = -1,
          SoundfileReader *reader = nullptr, bool is_double = false)
      : fSoundfileDir(sound_directories) {
    fSoundReader = (reader) ? std::shared_ptr<SoundfileReader>(reader)
                            // the static gReader should not be deleted, so use
                            // an empty destructor
                            : std::shared_ptr<SoundfileReader>(
                                  std::shared_ptr<SoundfileReader>{}, &gReader);
    fSoundReader->setSampleRate(sample_rate);
    fIsDouble = is_double;
    if (!defaultsound)
      defaultsound =
          gReader.createSoundfile(gPathNameList, MAX_CHAN, is_double);
  }

  virtual ~SoundUI() {}

  // -- soundfiles
  virtual void addSoundfile(const char *label, const char *url,
                            Soundfile **sf_zone) {
    const char *saved_url = url; // 'url' is consumed by parseMenuList2
    std::vector<std::string> file_name_list;

    bool menu = parseMenuList2(url, file_name_list, true);
    // If not a list, we have as single file
    if (!menu) {
      file_name_list.push_back(saved_url);
    }

    // Parse the possible list
    std::string saved_url_real =
        std::string(saved_url) + "_" +
        std::to_string(fIsDouble); // fIsDouble is used in the key
    if (fSoundfileMap.find(saved_url_real) == fSoundfileMap.end()) {
      // Check all files and get their complete path
      std::vector<std::string> path_name_list =
          fSoundReader->checkFiles(fSoundfileDir, file_name_list);
      // Read them and create the Soundfile
      Soundfile *sound_file =
          fSoundReader->createSoundfile(path_name_list, MAX_CHAN, fIsDouble);
      if (sound_file) {
        fSoundfileMap[saved_url_real] = std::shared_ptr<Soundfile>(sound_file);
      } else {
        // If failure, use 'defaultsound'
        std::cerr << "addSoundfile : soundfile for " << saved_url
                  << " cannot be created !" << std::endl;
        *sf_zone = defaultsound;
        return;
      }
    }

    // Get the soundfile pointer
    *sf_zone = fSoundfileMap[saved_url_real].get();
  }

  /**
   * An OS dependant function to get the path of the running executable or
   * plugin. This will typically be used when creating a SoundUI soundfile
   * loader, like new SoundUI(SoundUI::getBinaryPath());
   *
   * @return the running executable or plugin path.
   */
  static std::string getBinaryPath() {
    std::string bundle_path_str;
#if defined(__APPLE__) && !defined(__VCVRACK__) && !defined(JUCE_32BIT) &&     \
    !defined(JUCE_64BIT)
    CFURLRef bundle_ref = CFBundleCopyBundleURL(CFBundleGetMainBundle());
    if (!bundle_ref) {
      std::cerr << "getBinaryPath CFBundleCopyBundleURL error\n";
      return "";
    }

    UInt8 bundle_path[1024];
    if (CFURLGetFileSystemRepresentation(bundle_ref, true, bundle_path, 1024)) {
      bundle_path_str = std::string((char *)bundle_path);
    } else {
      std::cerr << "getBinaryPath CFURLGetFileSystemRepresentation error\n";
    }
#endif
#ifdef ANDROID_DRIVER
    bundle_path_str = "/data/data/__CURRENT_ANDROID_PACKAGE__/files";
#endif
    return bundle_path_str;
  }

  /**
   * An OS dependant function to get the path of the running executable or
   * plugin. This will typically be used when creating a SoundUI soundfile
   * loader, like new SoundUI(SoundUI::getBinaryPathFrom());
   *
   * @param path - entry point to start getting the path of the running
   * executable or plugin.
   *
   * @return the running executable or plugin path.
   */
  static std::string getBinaryPathFrom(const std::string &path) {
    std::string bundle_path_str;
#if defined(__APPLE__) && !defined(__VCVRACK__) && !defined(JUCE_32BIT) &&     \
    !defined(JUCE_64BIT)
    CFBundleRef bundle =
        CFBundleGetBundleWithIdentifier(CFStringCreateWithCString(
            kCFAllocatorDefault, path.c_str(), CFStringGetSystemEncoding()));
    if (!bundle) {
      std::cerr << "getBinaryPathFrom CFBundleGetBundleWithIdentifier error '"
                << path << "'" << std::endl;
      return "";
    }

    CFURLRef bundle_ref = CFBundleCopyBundleURL(bundle);
    if (!bundle_ref) {
      std::cerr << "getBinaryPathFrom CFBundleCopyBundleURL error\n";
      return "";
    }

    UInt8 bundle_path[1024];
    if (CFURLGetFileSystemRepresentation(bundle_ref, true, bundle_path, 1024)) {
      bundle_path_str = std::string((char *)bundle_path);
    } else {
      std::cerr << "getBinaryPathFrom CFURLGetFileSystemRepresentation error\n";
    }
#endif
#ifdef ANDROID_DRIVER
    bundle_path_str = "/data/data/__CURRENT_ANDROID_PACKAGE__/files";
#endif
    return bundle_path_str;
  }
};

#endif
/**************************  END  SoundUI.h **************************/
#endif

// params = [buttons][entries][bargraph]

// A class to count items of each type.
// Parameters with the "CV:N" metadata are kept separately in fInputCV and
// fOutputCV and will be connected to audio CV inputs and outputs.

struct ManagerUI : public GenericUI {
  enum UIType {
    kButton,
    kCheckbox,
    kVSlider,
    kHSlider,
    kNumEntry,
    kVBargraph,
    kHBargraph,
    kNotype
  };

  struct UIItem {
    std::string fLabel;
    float fWidth = 0.f;
    float fHight = 0.f;
    UIType fType = kNotype;
    FAUSTFLOAT fInit;
    FAUSTFLOAT fMin;
    FAUSTFLOAT fMax;
    UIItem(const std::string &label, FAUSTFLOAT init, FAUSTFLOAT fmin,
           FAUSTFLOAT fmax, UIType type)
        : fLabel(label), fType(type), fInit(init), fMin(fmin), fMax(fmax) {}
  };

  std::vector<UIItem> fButtons;
  std::vector<UIItem> fRanges;
  std::vector<UIItem> fBargraph;
  std::vector<UIItem> fInputCV;
  std::vector<UIItem> fOutputCV;

  std::string fCV;

  void addRange(const char *label, FAUSTFLOAT init, FAUSTFLOAT min,
                FAUSTFLOAT max, UIType type) {
    if (fCV != "") {
      fInputCV.push_back(UIItem(label, init, min, max, type));
      fCV = "";
    } else {
      fRanges.push_back(UIItem(label, init, min, max, type));
    }
  }

  void addBargraph(const char *label, FAUSTFLOAT min, FAUSTFLOAT max,
                   UIType type) {
    if (fCV != "") {
      fOutputCV.push_back(UIItem(label, 0, min, max, type));
      fCV = "";
    } else {
      fBargraph.push_back(UIItem(label, 0, min, max, type));
    }
  }

  // External API
  void openTabBox(const char *label) {}
  void openHorizontalBox(const char *label) {}
  void openVerticalBox(const char *label) {}
  void closeBox() {}

  void addButton(const char *label, FAUSTFLOAT *zone) {
    if (fCV != "") {
      fInputCV.push_back(UIItem(label, 0, 1, 0, kButton));
      fCV = "";
    } else {
      fButtons.push_back(UIItem(label, 0, 1, 0, kButton));
    }
  }
  void addCheckButton(const char *label, FAUSTFLOAT *zone) {
    if (fCV != "") {
      fInputCV.push_back(UIItem(label, 0, 1, 0, kCheckbox));
      fCV = "";
    } else {
      fButtons.push_back(UIItem(label, 0, 1, 0, kCheckbox));
    }
  }

  void addVerticalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                         FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    addRange(label, init, min, max, kVSlider);
  }
  void addHorizontalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                           FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    addRange(label, init, min, max, kHSlider);
  }
  void addNumEntry(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                   FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    addRange(label, init, min, max, kNumEntry);
  }

  void addHorizontalBargraph(const char *label, FAUSTFLOAT *zone,
                             FAUSTFLOAT min, FAUSTFLOAT max) {
    addBargraph(label, min, max, kHBargraph);
  }
  void addVerticalBargraph(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT min,
                           FAUSTFLOAT max) {
    addBargraph(label, min, max, kVBargraph);
  }

  void declare(FAUSTFLOAT *zone, const char *key, const char *val) {
    if (std::string(key) == "CV" || std::string(key) == "cv") {
      fCV = val;
    }
  }
};

// UI handler for switches, knobs and lights
template <int VOICES> struct RackUI : public GenericUI {
  typedef function<void(Module *module)> updateFunction;

  std::vector<ConverterZoneControl *> fConverters;
  std::vector<updateFunction> fUpdateFunIn;
  std::vector<updateFunction> fUpdateFunOut;

  ManagerUI fParams;
  std::string fScale, fCV;

  int fButtonsCounter = 0;
  int fParamsCounter = 0;
  int fBargraphCounter = 0;
  int fCurVoice = -1;

  int parseIndex(const std::string &value) {
    try {
      return stoi(value);
    } catch (invalid_argument &e) {
      return -1;
    }
  }

  RackUI(const ManagerUI &counter) : fParams(counter), fScale("lin") {}

  virtual ~RackUI() {
    for (const auto &it : fConverters)
      delete it;
  }

  void reset() {
    fButtonsCounter = 0;
    fParamsCounter = 0;
    fBargraphCounter = 0;
    fScale = "lin";
    fCV = "";
  }

  void addButton(const char *label, FAUSTFLOAT *zone) {
    if (fCV != "") {
      int index = parseIndex(fCV) - 1;
      if (index < 0) {
        WARN("Incorrect index");
        return;
      }

      // Capture current voice
      int voice = fCurVoice;
      fUpdateFunIn.push_back([=](Module *module) {
        float cv = (VOICES == 1) ? module->inputs[index].getVoltage()
                                 : module->inputs[index].getVoltage(voice);
        *zone = cv / 10.f;
      });

      fCV = "";
    } else {
      // Takes the value at lambda contruction time
      int index = fButtonsCounter;
      fUpdateFunIn.push_back([=](Module *module) {
        // 'buttons' start at 0
        *zone = module->params[index].getValue() / 10.f;
      });
      fButtonsCounter++;
    }
  }

  void addCheckButton(const char *label, FAUSTFLOAT *zone) {
    addButton(label, zone);
  }

  void addVerticalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                         FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    addNumEntry(label, zone, init, min, max, step);
  }

  void addHorizontalSlider(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                           FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    addNumEntry(label, zone, init, min, max, step);
  }

  void addNumEntry(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT init,
                   FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) {
    ConverterZoneControl *converter;

    if (fCV != "") {

      int index = parseIndex(fCV) - 1;
      if (index < 0) {
        WARN("Incorrect index");
        return;
      }

      // Capture current voice
      int voice = fCurVoice;

      if (MapUI::endsWith(label, "freq")) {

        converter = new ConverterZoneControl(zone, new ValueConverter());
        fUpdateFunIn.push_back([=](Module *module) {
          float cv = (VOICES == 1) ? module->inputs[index].getVoltage()
                                   : module->inputs[index].getVoltage(voice);
          float freq = 440.f * std::pow(2.f, (cv - 0.75f));
          // Receive a 1V/oct pitch signal of the last held MIDI note:
          // https://vcvrack.com/manual/Core#midi-cv
          converter->update(freq);
        });
      } else if (MapUI::endsWith(label, "gate")) {

        converter = new ConverterZoneControl(zone, new ValueConverter());
        fUpdateFunIn.push_back([=](Module *module) {
          float cv = (VOICES == 1) ? module->inputs[index].getVoltage()
                                   : module->inputs[index].getVoltage(voice);
          // Receive a 10V signal when a key is held:
          // https://vcvrack.com/manual/Core#midi-cv
          converter->update(cv / 10.f);
        });
      } else if (MapUI::endsWith(label, "gain")) {

        converter = new ConverterZoneControl(zone, new ValueConverter());
        fUpdateFunIn.push_back([=](Module *module) {
          float cv = (VOICES == 1) ? module->inputs[index].getVoltage()
                                   : module->inputs[index].getVoltage(voice);
          // Receive a signal from 0V to 10V for the velocity:
          // https://vcvrack.com/manual/Core#midi-cv
          converter->update(cv / 10.f);
        });
      } else {
        if (fScale == "log") {
          converter = new ConverterZoneControl(
              zone, new LogValueConverter(-5, 5, min, max));
        } else if (fScale == "exp") {
          converter = new ConverterZoneControl(
              zone, new ExpValueConverter(-5, 5, min, max));
        } else {
          converter = new ConverterZoneControl(
              zone, new LinearValueConverter(-5, 5, min, max));
        }

        fUpdateFunIn.push_back([=](Module *module) {
          float cv = (VOICES == 1) ? module->inputs[index].getVoltage()
                                   : module->inputs[index].getVoltage(voice);
          converter->update(cv);
        });
      }

      fCV = "";
    } else {

      if (fScale == "log") {
        converter = new ConverterZoneControl(
            zone, new LogValueConverter(min, max, min, max));
      } else if (fScale == "exp") {
        converter = new ConverterZoneControl(
            zone, new ExpValueConverter(min, max, min, max));
      } else {
        converter = new ConverterZoneControl(zone, new ValueConverter());
      }

      // Takes the value at lambda contruction time
      int index = fParamsCounter;
      fUpdateFunIn.push_back([=](Module *module) {
        // 'nentries' start at fParams.fButtons.size()
        converter->update(
            module->params[index + fParams.fButtons.size()].getValue());
      });
      fScale = "lin";
      fParamsCounter++;
    }
    fConverters.push_back(converter);
  }

  void addBarGraph(FAUSTFLOAT *zone, FAUSTFLOAT min, FAUSTFLOAT max) {
    ConverterZoneControl *converter;

    if (fCV != "") {

      if (fScale == "log") {
        converter = new ConverterZoneControl(
            zone, new LogValueConverter(-5, 5, min, max));
      } else if (fScale == "exp") {
        converter = new ConverterZoneControl(
            zone, new ExpValueConverter(-5, 5, min, max));
      } else {
        converter = new ConverterZoneControl(
            zone, new LinearValueConverter(-5, 5, min, max));
      }

      int index = parseIndex(fCV) - 1;
      if (index < 0) {
        WARN("Incorrect index");
        return;
      }

      int voice = fCurVoice;
      fUpdateFunOut.push_back([=](Module *module) {
        if (VOICES == 1) {
          module->outputs[index].setVoltage(
              converter->getConverter()->faust2ui(*zone));
        } else {
          module->outputs[index].setVoltage(
              voice, converter->getConverter()->faust2ui(*zone));
        }
      });
      fCV = "";
    } else {

      if (fScale == "log") {
        converter = new ConverterZoneControl(
            zone, new LogValueConverter(0., 1., min, max));
      } else if (fScale == "exp") {
        converter = new ConverterZoneControl(
            zone, new ExpValueConverter(0., 1., min, max));
      } else {
        converter = new ConverterZoneControl(
            zone, new LinearValueConverter(0., 1., min, max));
      }

      // Takes the value at lambda contruction time
      int index = fBargraphCounter;

      fUpdateFunOut.push_back([=](Module *module) {
        module->lights[index].setBrightness(
            converter->getConverter()->faust2ui(*zone));
      });
      fBargraphCounter++;
    }

    fConverters.push_back(converter);
    fScale = "lin";
  }

  void addHorizontalBargraph(const char *label, FAUSTFLOAT *zone,
                             FAUSTFLOAT min, FAUSTFLOAT max) {
    addBarGraph(zone, min, max);
  }

  void addVerticalBargraph(const char *label, FAUSTFLOAT *zone, FAUSTFLOAT min,
                           FAUSTFLOAT max) {
    addBarGraph(zone, min, max);
  }

  void declare(FAUSTFLOAT *zone, const char *key, const char *val) {
    if (std::string(key) == "scale") {
      fScale = val;
    } else if (std::string(key) == "CV" || std::string(key) == "cv") {
      fCV = val;
    }
  }

  void updateInputs(Module *module) {
    for (const auto &it : fUpdateFunIn)
      it(module);
  }

  void updateOutputs(Module *module) {
    for (const auto &it : fUpdateFunOut)
      it(module);
  }
};

/******************************************************************************
 *******************************************************************************

 VECTOR INTRINSICS

 *******************************************************************************
 *******************************************************************************/

/********************END ARCHITECTURE SECTION (part 1/2)****************/

/**************************BEGIN USER SECTION **************************/

#ifndef FAUSTFLOAT
#define FAUSTFLOAT float
#endif

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <math.h>

static float tambura_faustpower2_f(float value) { return value * value; }

#ifndef FAUSTCLASS
#define FAUSTCLASS tambura
#endif

#ifdef __APPLE__
#define exp10f __exp10f
#define exp10 __exp10
#endif

#if defined(_WIN32)
#define RESTRICT __restrict
#else
#define RESTRICT __restrict__
#endif

#define FAUST_INT_CONTROLS 0
#define FAUST_REAL_CONTROLS 40

class tambura : public one_sample_dsp<float> {

private:
  int fSampleRate;
  float fConst0;
  float fConst1;
  FAUSTFLOAT fHslider0;
  float fConst2;
  float fRec0[2];
  FAUSTFLOAT fButton0;
  float fVec0[2];
  FAUSTFLOAT fHslider1;
  float fConst3;
  float fRec3[2];
  float fRec4[2];
  FAUSTFLOAT fHslider2;
  FAUSTFLOAT fHslider3;
  float fRec5[2];
  FAUSTFLOAT fHslider4;
  float fConst4;
  float fRec6[2];
  FAUSTFLOAT fHslider5;
  float fConst5;
  int iRec8[2];
  float fRec7[4];
  FAUSTFLOAT fHslider6;
  float fRec9[2];
  FAUSTFLOAT fHslider7;
  float fConst6;
  float fRec11[2];
  FAUSTFLOAT fCheckbox0;
  float fVec1[2];
  float fVec2[2];
  FAUSTFLOAT fHslider8;
  float fRec12[2];
  float fRec13[2];
  float fConst7;
  FAUSTFLOAT fHslider9;
  float fRec10[2];
  int IOTA0;
  float fVec3[8192];
  float fRec14[2];
  FAUSTFLOAT fHslider10;
  FAUSTFLOAT fHslider11;
  float fRec15[2];
  FAUSTFLOAT fHslider12;
  FAUSTFLOAT fHslider13;
  float fRec16[2];
  FAUSTFLOAT fHslider14;
  float fConst8;
  float fRec17[2];
  float fRec18[2];
  float fVec4[8192];
  float fVec5[2];
  float fVec6[2];
  float fRec2[4];
  float fRec20[2];
  float fVec7[8192];
  float fVec8[2];
  float fVec9[2];
  float fRec19[4];
  float fRec22[2];
  float fVec10[8192];
  float fVec11[2];
  float fVec12[2];
  float fRec21[4];
  float fRec24[2];
  float fVec13[8192];
  float fVec14[2];
  float fVec15[2];
  float fRec23[4];
  float fRec26[2];
  float fVec16[8192];
  float fVec17[2];
  float fVec18[2];
  float fRec25[4];
  float fRec28[2];
  float fVec19[8192];
  float fVec20[2];
  float fVec21[2];
  float fRec27[4];
  float fRec30[2];
  float fVec22[8192];
  float fVec23[2];
  float fVec24[2];
  float fRec29[4];
  float fRec32[2];
  float fVec25[8192];
  float fVec26[2];
  float fVec27[2];
  float fRec31[4];
  float fRec34[2];
  float fVec28[8192];
  float fVec29[2];
  float fVec30[2];
  float fRec33[4];
  float fVec31[2];
  float fRec1[2];
  FAUSTFLOAT fButton1;
  float fVec32[2];
  float fRec45[2];
  float fRec46[2];
  float fVec33[2];
  float fVec34[2];
  float fConst9;
  float fRec48[2];
  float fRec49[2];
  float fRec47[2];
  float fVec35[8192];
  float fRec50[2];
  float fConst11;
  float fConst15;
  float fConst16;
  float fConst18;
  float fConst23;
  float fConst24;
  FAUSTFLOAT fHslider15;
  float fRec54[2];
  float fVec36[2];
  float fConst25;
  float fConst26;
  float fConst27;
  float fRec53[2];
  float fRec55[2];
  float fRec52[3];
  float fConst29;
  float fConst30;
  float fRec51[3];
  float fConst32;
  float fConst33;
  float fVec37[2];
  float fRec58[2];
  float fRec59[2];
  float fRec57[3];
  float fRec56[3];
  float fVec38[2];
  float fRec62[2];
  float fRec63[2];
  float fRec61[3];
  float fRec60[3];
  float fRec64[2];
  float fVec39[8192];
  float fVec40[2];
  float fVec41[2];
  float fRec44[4];
  float fRec66[2];
  float fVec42[8192];
  float fVec43[2];
  float fVec44[2];
  float fRec65[4];
  float fRec68[2];
  float fVec45[8192];
  float fVec46[2];
  float fVec47[2];
  float fRec67[4];
  float fRec70[2];
  float fVec48[8192];
  float fVec49[2];
  float fVec50[2];
  float fRec69[4];
  float fRec72[2];
  float fVec51[8192];
  float fVec52[2];
  float fVec53[2];
  float fRec71[4];
  float fRec74[2];
  float fVec54[8192];
  float fVec55[2];
  float fVec56[2];
  float fRec73[4];
  float fRec76[2];
  float fVec57[8192];
  float fVec58[2];
  float fVec59[2];
  float fRec75[4];
  float fRec78[2];
  float fVec60[8192];
  float fVec61[2];
  float fVec62[2];
  float fRec77[4];
  float fRec80[2];
  float fVec63[8192];
  float fVec64[2];
  float fVec65[2];
  float fRec79[4];
  float fVec66[2];
  float fRec43[2];
  float fRec83[2];
  float fVec67[8192];
  float fVec68[2];
  float fVec69[2];
  float fRec82[4];
  float fRec85[2];
  float fVec70[8192];
  float fVec71[2];
  float fVec72[2];
  float fRec84[4];
  float fRec87[2];
  float fVec73[8192];
  float fVec74[2];
  float fVec75[2];
  float fRec86[4];
  float fRec89[2];
  float fVec76[8192];
  float fVec77[2];
  float fVec78[2];
  float fRec88[4];
  float fRec91[2];
  float fVec79[8192];
  float fVec80[2];
  float fVec81[2];
  float fRec90[4];
  float fRec93[2];
  float fVec82[8192];
  float fVec83[2];
  float fVec84[2];
  float fRec92[4];
  float fRec95[2];
  float fVec85[8192];
  float fVec86[2];
  float fVec87[2];
  float fRec94[4];
  float fRec97[2];
  float fVec88[8192];
  float fVec89[2];
  float fVec90[2];
  float fRec96[4];
  float fRec99[2];
  float fVec91[8192];
  float fVec92[2];
  float fVec93[2];
  float fRec98[4];
  float fVec94[2];
  float fRec81[2];
  float fRec36[2];
  FAUSTFLOAT fButton2;
  float fVec95[2];
  float fRec102[2];
  float fRec103[2];
  float fVec96[2];
  float fVec97[2];
  float fConst34;
  float fRec105[2];
  float fRec106[2];
  float fRec104[2];
  float fVec98[8192];
  float fRec107[2];
  float fVec99[2];
  float fRec110[2];
  float fRec111[2];
  float fRec109[3];
  float fRec108[3];
  float fRec112[2];
  float fVec100[8192];
  float fVec101[2];
  float fVec102[2];
  float fRec101[4];
  float fRec114[2];
  float fVec103[8192];
  float fVec104[2];
  float fVec105[2];
  float fRec113[4];
  float fRec116[2];
  float fVec106[8192];
  float fVec107[2];
  float fVec108[2];
  float fRec115[4];
  float fRec118[2];
  float fVec109[8192];
  float fVec110[2];
  float fVec111[2];
  float fRec117[4];
  float fRec120[2];
  float fVec112[8192];
  float fVec113[2];
  float fVec114[2];
  float fRec119[4];
  float fRec122[2];
  float fVec115[8192];
  float fVec116[2];
  float fVec117[2];
  float fRec121[4];
  float fRec124[2];
  float fVec118[8192];
  float fVec119[2];
  float fVec120[2];
  float fRec123[4];
  float fRec126[2];
  float fVec121[8192];
  float fVec122[2];
  float fVec123[2];
  float fRec125[4];
  float fRec128[2];
  float fVec124[8192];
  float fVec125[2];
  float fVec126[2];
  float fRec127[4];
  float fVec127[2];
  float fRec100[2];
  float fRec131[2];
  float fVec128[8192];
  float fVec129[2];
  float fVec130[2];
  float fRec130[4];
  float fRec133[2];
  float fVec131[8192];
  float fVec132[2];
  float fVec133[2];
  float fRec132[4];
  float fRec135[2];
  float fVec134[8192];
  float fVec135[2];
  float fVec136[2];
  float fRec134[4];
  float fRec137[2];
  float fVec137[8192];
  float fVec138[2];
  float fVec139[2];
  float fRec136[4];
  float fRec139[2];
  float fVec140[8192];
  float fVec141[2];
  float fVec142[2];
  float fRec138[4];
  float fRec141[2];
  float fVec143[8192];
  float fVec144[2];
  float fVec145[2];
  float fRec140[4];
  float fRec143[2];
  float fVec146[8192];
  float fVec147[2];
  float fVec148[2];
  float fRec142[4];
  float fRec145[2];
  float fVec149[8192];
  float fVec150[2];
  float fVec151[2];
  float fRec144[4];
  float fRec147[2];
  float fVec152[8192];
  float fVec153[2];
  float fVec154[2];
  float fRec146[4];
  float fVec155[2];
  float fRec129[2];
  float fRec38[2];
  FAUSTFLOAT fButton3;
  float fVec156[2];
  float fRec150[2];
  float fRec151[2];
  float fVec157[2];
  float fVec158[2];
  float fConst35;
  float fRec153[2];
  float fRec154[2];
  float fRec152[2];
  float fVec159[8192];
  float fRec155[2];
  float fRec156[2];
  float fVec160[8192];
  float fVec161[2];
  float fVec162[2];
  float fRec149[4];
  float fRec158[2];
  float fVec163[8192];
  float fVec164[2];
  float fVec165[2];
  float fRec157[4];
  float fRec160[2];
  float fVec166[8192];
  float fVec167[2];
  float fVec168[2];
  float fRec159[4];
  float fRec162[2];
  float fVec169[8192];
  float fVec170[2];
  float fVec171[2];
  float fRec161[4];
  float fRec164[2];
  float fVec172[8192];
  float fVec173[2];
  float fVec174[2];
  float fRec163[4];
  float fRec166[2];
  float fVec175[8192];
  float fVec176[2];
  float fVec177[2];
  float fRec165[4];
  float fRec168[2];
  float fVec178[8192];
  float fVec179[2];
  float fVec180[2];
  float fRec167[4];
  float fRec170[2];
  float fVec181[8192];
  float fVec182[2];
  float fVec183[2];
  float fRec169[4];
  float fRec172[2];
  float fVec184[8192];
  float fVec185[2];
  float fVec186[2];
  float fRec171[4];
  float fVec187[2];
  float fRec148[2];
  float fRec175[2];
  float fVec188[8192];
  float fVec189[2];
  float fVec190[2];
  float fRec174[4];
  float fRec177[2];
  float fVec191[8192];
  float fVec192[2];
  float fVec193[2];
  float fRec176[4];
  float fRec179[2];
  float fVec194[8192];
  float fVec195[2];
  float fVec196[2];
  float fRec178[4];
  float fRec181[2];
  float fVec197[8192];
  float fVec198[2];
  float fVec199[2];
  float fRec180[4];
  float fRec183[2];
  float fVec200[8192];
  float fVec201[2];
  float fVec202[2];
  float fRec182[4];
  float fRec185[2];
  float fVec203[8192];
  float fVec204[2];
  float fVec205[2];
  float fRec184[4];
  float fRec187[2];
  float fVec206[8192];
  float fVec207[2];
  float fVec208[2];
  float fRec186[4];
  float fRec189[2];
  float fVec209[8192];
  float fVec210[2];
  float fVec211[2];
  float fRec188[4];
  float fRec191[2];
  float fVec212[8192];
  float fVec213[2];
  float fVec214[2];
  float fRec190[4];
  float fVec215[2];
  float fRec173[2];
  float fRec40[2];
  float fRec194[2];
  float fVec216[8192];
  float fVec217[2];
  float fVec218[2];
  float fRec193[4];
  float fRec196[2];
  float fVec219[8192];
  float fVec220[2];
  float fVec221[2];
  float fRec195[4];
  float fRec198[2];
  float fVec222[8192];
  float fVec223[2];
  float fVec224[2];
  float fRec197[4];
  float fRec200[2];
  float fVec225[8192];
  float fVec226[2];
  float fVec227[2];
  float fRec199[4];
  float fRec202[2];
  float fVec228[8192];
  float fVec229[2];
  float fVec230[2];
  float fRec201[4];
  float fRec204[2];
  float fVec231[8192];
  float fVec232[2];
  float fVec233[2];
  float fRec203[4];
  float fRec206[2];
  float fVec234[8192];
  float fVec235[2];
  float fVec236[2];
  float fRec205[4];
  float fRec208[2];
  float fVec237[8192];
  float fVec238[2];
  float fVec239[2];
  float fRec207[4];
  float fRec210[2];
  float fVec240[8192];
  float fVec241[2];
  float fVec242[2];
  float fRec209[4];
  float fVec243[2];
  float fRec192[2];
  float fRec42[2];
  FAUSTFLOAT fHslider16;
  float fRec211[2];

public:
  tambura() {}

  tambura(int *icontrol, float *fcontrol)
      : one_sample_dsp(icontrol, fcontrol) {}

  void metadata(Meta *m) {
    m->declare("analyzers.lib/name", "Faust Analyzer Library");
    m->declare("analyzers.lib/version", "1.2.0");
    m->declare("author", "Oli Larkin (contact@olilarkin.co.uk)");
    m->declare("basics.lib/name", "Faust Basic Element Library");
    m->declare("basics.lib/tabulateNd",
               "Copyright (C) 2023 Bart Brouns <bart@magnetophon.nl>");
    m->declare("basics.lib/version", "1.11.1");
    m->declare("compile_options",
               "-a /usr/local/share/faust/vcvrack/template/src/FaustModule.cpp "
               "-lang cpp -i -os0 -fpga-mem 10000 -ct 1 -cn tambura -es 1 -mcd "
               "16 -single -ftz 0");
    m->declare("copyright", "Oliver Larkin");
    m->declare("delays.lib/fdelaylti:author", "Julius O. Smith III");
    m->declare("delays.lib/name", "Faust Delay Library");
    m->declare("delays.lib/version", "1.1.0");
    m->declare("description",
               "Pseudo physical model of an Indian Tambura/Tanpura");
    m->declare("filename", "tambura.dsp");
    m->declare("filters.lib/allpassnn:author", "Julius O. Smith III");
    m->declare("filters.lib/allpassnn:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/allpassnn:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/dcblocker:author", "Julius O. Smith III");
    m->declare("filters.lib/dcblocker:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/dcblocker:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/ff_comb:author", "Julius O. Smith III");
    m->declare("filters.lib/ff_comb:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/ff_comb:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/filterbank:author", "Julius O. Smith III");
    m->declare("filters.lib/filterbank:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/filterbank:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/fir:author", "Julius O. Smith III");
    m->declare("filters.lib/fir:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/fir:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/highpass:author", "Julius O. Smith III");
    m->declare("filters.lib/highpass:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/highshelf:author", "Julius O. Smith III");
    m->declare("filters.lib/highshelf:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/highshelf:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/iir:author", "Julius O. Smith III");
    m->declare("filters.lib/iir:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/iir:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/lowpass0_highpass1", "MIT-style STK-4.3 license");
    m->declare("filters.lib/lowpass0_highpass1:author", "Julius O. Smith III");
    m->declare("filters.lib/lowpass:author", "Julius O. Smith III");
    m->declare("filters.lib/lowpass:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/lowpass:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/name", "Faust Filters Library");
    m->declare("filters.lib/peak_eq:author", "Julius O. Smith III");
    m->declare("filters.lib/peak_eq:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/peak_eq:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/pole:author", "Julius O. Smith III");
    m->declare("filters.lib/pole:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/pole:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/tf1:author", "Julius O. Smith III");
    m->declare("filters.lib/tf1:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/tf1:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/tf1s:author", "Julius O. Smith III");
    m->declare("filters.lib/tf1s:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/tf1s:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/tf2:author", "Julius O. Smith III");
    m->declare("filters.lib/tf2:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/tf2:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/tf2s:author", "Julius O. Smith III");
    m->declare("filters.lib/tf2s:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/tf2s:license", "MIT-style STK-4.3 license");
    m->declare("filters.lib/version", "1.3.0");
    m->declare("filters.lib/zero:author", "Julius O. Smith III");
    m->declare("filters.lib/zero:copyright",
               "Copyright (C) 2003-2019 by Julius O. Smith III "
               "<jos@ccrma.stanford.edu>");
    m->declare("filters.lib/zero:license", "MIT-style STK-4.3 license");
    m->declare("licence", "GPL");
    m->declare("maths.lib/author", "GRAME");
    m->declare("maths.lib/copyright", "GRAME");
    m->declare("maths.lib/license", "LGPL with exception");
    m->declare("maths.lib/name", "Faust Math Library");
    m->declare("maths.lib/version", "2.7.0");
    m->declare("name", "tambura");
    m->declare("noises.lib/name", "Faust Noise Generator Library");
    m->declare("noises.lib/version", "1.4.0");
    m->declare("platform.lib/name", "Generic Platform Library");
    m->declare("platform.lib/version", "1.3.0");
    m->declare("routes.lib/name", "Faust Signal Routing Library");
    m->declare("routes.lib/version", "1.2.0");
    m->declare("signals.lib/name", "Faust Signal Routing Library");
    m->declare("signals.lib/onePoleSwitching:author",
               "Jonatan Liljedahl, revised by Dario Sanfilippo");
    m->declare("signals.lib/onePoleSwitching:licence", "STK-4.3");
    m->declare("signals.lib/version", "1.5.0");
    m->declare("version", "1.0");
  }

  virtual int getNumInputs() { return 0; }
  virtual int getNumOutputs() { return 2; }

  static void classInit(int sample_rate) {}

  void staticInit(int sample_rate) {}

  virtual void instanceConstants(int sample_rate) {
    fSampleRate = sample_rate;
    fConst0 =
        std::min<float>(1.92e+05f, std::max<float>(1.0f, float(fSampleRate)));
    fConst1 = std::exp(-(2e+01f / fConst0));
    fConst2 = 1.0f - fConst1;
    fConst3 = 0.001f * fConst0;
    fConst4 = 0.2f * fConst2;
    fConst5 = 0.0022727272f * fConst0;
    fConst6 = 1.0f / fConst0;
    fConst7 = std::exp(-(2e+02f / fConst0));
    fConst8 = 0.1f * fConst2;
    fConst9 = 0.6666667f * fConst0;
    float fConst10 = std::tan(23561.945f / fConst0);
    fConst11 = 2.0f * (1.0f - 1.0f / tambura_faustpower2_f(fConst10));
    float fConst12 = fConst0 * std::sin(47123.89f / fConst0);
    float fConst13 = 2042.0353f / fConst12;
    float fConst14 = 1.0f / fConst10;
    fConst15 = (fConst14 - fConst13) / fConst10 + 1.0f;
    fConst16 = 1.0f / ((fConst14 + fConst13) / fConst10 + 1.0f);
    float fConst17 = std::tan(7853.9814f / fConst0);
    fConst18 = 2.0f * (1.0f - 1.0f / tambura_faustpower2_f(fConst17));
    float fConst19 = 15707.963f / fConst0;
    float fConst20 = fConst0 * std::sin(fConst19);
    float fConst21 = 1256.6371f / fConst20;
    float fConst22 = 1.0f / fConst17;
    fConst23 = (fConst22 - fConst21) / fConst17 + 1.0f;
    fConst24 = 1.0f / ((fConst22 + fConst21) / fConst17 + 1.0f);
    fConst25 = 1.0f / std::tan(fConst19);
    fConst26 = 1.0f - fConst25;
    fConst27 = 1.0f / (fConst25 + 1.0f);
    float fConst28 = 6298.1045f / fConst20;
    fConst29 = (fConst22 - fConst28) / fConst17 + 1.0f;
    fConst30 = (fConst22 + fConst28) / fConst17 + 1.0f;
    float fConst31 = 20420.352f / fConst12;
    fConst32 = (fConst14 - fConst31) / fConst10 + 1.0f;
    fConst33 = (fConst14 + fConst31) / fConst10 + 1.0f;
    fConst34 = 0.5f * fConst0;
    fConst35 = 0.49751243f * fConst0;
  }

  virtual void instanceResetUserInterface() {
    fHslider0 = FAUSTFLOAT(1.0f);
    fButton0 = FAUSTFLOAT(0.0f);
    fHslider1 = FAUSTFLOAT(1e+01f);
    fHslider2 = FAUSTFLOAT(3.0f);
    fHslider3 = FAUSTFLOAT(36.0f);
    fHslider4 = FAUSTFLOAT(0.001f);
    fHslider5 = FAUSTFLOAT(1.0f);
    fHslider6 = FAUSTFLOAT(0.13f);
    fHslider7 = FAUSTFLOAT(0.1f);
    fCheckbox0 = FAUSTFLOAT(0.0f);
    fHslider8 = FAUSTFLOAT(1.0f);
    fHslider9 = FAUSTFLOAT(0.07f);
    fHslider10 = FAUSTFLOAT(0.25f);
    fHslider11 = FAUSTFLOAT(1e+01f);
    fHslider12 = FAUSTFLOAT(1.0f);
    fHslider13 = FAUSTFLOAT(0.0f);
    fHslider14 = FAUSTFLOAT(0.0f);
    fButton1 = FAUSTFLOAT(0.0f);
    fHslider15 = FAUSTFLOAT(0.1f);
    fButton2 = FAUSTFLOAT(0.0f);
    fButton3 = FAUSTFLOAT(0.0f);
    fHslider16 = FAUSTFLOAT(0.0f);
  }

  virtual void instanceClear() {
    for (int l0 = 0; l0 < 2; l0 = l0 + 1) {
      fRec0[l0] = 0.0f;
    }
    for (int l1 = 0; l1 < 2; l1 = l1 + 1) {
      fVec0[l1] = 0.0f;
    }
    for (int l2 = 0; l2 < 2; l2 = l2 + 1) {
      fRec3[l2] = 0.0f;
    }
    for (int l3 = 0; l3 < 2; l3 = l3 + 1) {
      fRec4[l3] = 0.0f;
    }
    for (int l4 = 0; l4 < 2; l4 = l4 + 1) {
      fRec5[l4] = 0.0f;
    }
    for (int l5 = 0; l5 < 2; l5 = l5 + 1) {
      fRec6[l5] = 0.0f;
    }
    for (int l6 = 0; l6 < 2; l6 = l6 + 1) {
      iRec8[l6] = 0;
    }
    for (int l7 = 0; l7 < 4; l7 = l7 + 1) {
      fRec7[l7] = 0.0f;
    }
    for (int l8 = 0; l8 < 2; l8 = l8 + 1) {
      fRec9[l8] = 0.0f;
    }
    for (int l9 = 0; l9 < 2; l9 = l9 + 1) {
      fRec11[l9] = 0.0f;
    }
    for (int l10 = 0; l10 < 2; l10 = l10 + 1) {
      fVec1[l10] = 0.0f;
    }
    for (int l11 = 0; l11 < 2; l11 = l11 + 1) {
      fVec2[l11] = 0.0f;
    }
    for (int l12 = 0; l12 < 2; l12 = l12 + 1) {
      fRec12[l12] = 0.0f;
    }
    for (int l13 = 0; l13 < 2; l13 = l13 + 1) {
      fRec13[l13] = 0.0f;
    }
    for (int l14 = 0; l14 < 2; l14 = l14 + 1) {
      fRec10[l14] = 0.0f;
    }
    IOTA0 = 0;
    for (int l15 = 0; l15 < 8192; l15 = l15 + 1) {
      fVec3[l15] = 0.0f;
    }
    for (int l16 = 0; l16 < 2; l16 = l16 + 1) {
      fRec14[l16] = 0.0f;
    }
    for (int l17 = 0; l17 < 2; l17 = l17 + 1) {
      fRec15[l17] = 0.0f;
    }
    for (int l18 = 0; l18 < 2; l18 = l18 + 1) {
      fRec16[l18] = 0.0f;
    }
    for (int l19 = 0; l19 < 2; l19 = l19 + 1) {
      fRec17[l19] = 0.0f;
    }
    for (int l20 = 0; l20 < 2; l20 = l20 + 1) {
      fRec18[l20] = 0.0f;
    }
    for (int l21 = 0; l21 < 8192; l21 = l21 + 1) {
      fVec4[l21] = 0.0f;
    }
    for (int l22 = 0; l22 < 2; l22 = l22 + 1) {
      fVec5[l22] = 0.0f;
    }
    for (int l23 = 0; l23 < 2; l23 = l23 + 1) {
      fVec6[l23] = 0.0f;
    }
    for (int l24 = 0; l24 < 4; l24 = l24 + 1) {
      fRec2[l24] = 0.0f;
    }
    for (int l25 = 0; l25 < 2; l25 = l25 + 1) {
      fRec20[l25] = 0.0f;
    }
    for (int l26 = 0; l26 < 8192; l26 = l26 + 1) {
      fVec7[l26] = 0.0f;
    }
    for (int l27 = 0; l27 < 2; l27 = l27 + 1) {
      fVec8[l27] = 0.0f;
    }
    for (int l28 = 0; l28 < 2; l28 = l28 + 1) {
      fVec9[l28] = 0.0f;
    }
    for (int l29 = 0; l29 < 4; l29 = l29 + 1) {
      fRec19[l29] = 0.0f;
    }
    for (int l30 = 0; l30 < 2; l30 = l30 + 1) {
      fRec22[l30] = 0.0f;
    }
    for (int l31 = 0; l31 < 8192; l31 = l31 + 1) {
      fVec10[l31] = 0.0f;
    }
    for (int l32 = 0; l32 < 2; l32 = l32 + 1) {
      fVec11[l32] = 0.0f;
    }
    for (int l33 = 0; l33 < 2; l33 = l33 + 1) {
      fVec12[l33] = 0.0f;
    }
    for (int l34 = 0; l34 < 4; l34 = l34 + 1) {
      fRec21[l34] = 0.0f;
    }
    for (int l35 = 0; l35 < 2; l35 = l35 + 1) {
      fRec24[l35] = 0.0f;
    }
    for (int l36 = 0; l36 < 8192; l36 = l36 + 1) {
      fVec13[l36] = 0.0f;
    }
    for (int l37 = 0; l37 < 2; l37 = l37 + 1) {
      fVec14[l37] = 0.0f;
    }
    for (int l38 = 0; l38 < 2; l38 = l38 + 1) {
      fVec15[l38] = 0.0f;
    }
    for (int l39 = 0; l39 < 4; l39 = l39 + 1) {
      fRec23[l39] = 0.0f;
    }
    for (int l40 = 0; l40 < 2; l40 = l40 + 1) {
      fRec26[l40] = 0.0f;
    }
    for (int l41 = 0; l41 < 8192; l41 = l41 + 1) {
      fVec16[l41] = 0.0f;
    }
    for (int l42 = 0; l42 < 2; l42 = l42 + 1) {
      fVec17[l42] = 0.0f;
    }
    for (int l43 = 0; l43 < 2; l43 = l43 + 1) {
      fVec18[l43] = 0.0f;
    }
    for (int l44 = 0; l44 < 4; l44 = l44 + 1) {
      fRec25[l44] = 0.0f;
    }
    for (int l45 = 0; l45 < 2; l45 = l45 + 1) {
      fRec28[l45] = 0.0f;
    }
    for (int l46 = 0; l46 < 8192; l46 = l46 + 1) {
      fVec19[l46] = 0.0f;
    }
    for (int l47 = 0; l47 < 2; l47 = l47 + 1) {
      fVec20[l47] = 0.0f;
    }
    for (int l48 = 0; l48 < 2; l48 = l48 + 1) {
      fVec21[l48] = 0.0f;
    }
    for (int l49 = 0; l49 < 4; l49 = l49 + 1) {
      fRec27[l49] = 0.0f;
    }
    for (int l50 = 0; l50 < 2; l50 = l50 + 1) {
      fRec30[l50] = 0.0f;
    }
    for (int l51 = 0; l51 < 8192; l51 = l51 + 1) {
      fVec22[l51] = 0.0f;
    }
    for (int l52 = 0; l52 < 2; l52 = l52 + 1) {
      fVec23[l52] = 0.0f;
    }
    for (int l53 = 0; l53 < 2; l53 = l53 + 1) {
      fVec24[l53] = 0.0f;
    }
    for (int l54 = 0; l54 < 4; l54 = l54 + 1) {
      fRec29[l54] = 0.0f;
    }
    for (int l55 = 0; l55 < 2; l55 = l55 + 1) {
      fRec32[l55] = 0.0f;
    }
    for (int l56 = 0; l56 < 8192; l56 = l56 + 1) {
      fVec25[l56] = 0.0f;
    }
    for (int l57 = 0; l57 < 2; l57 = l57 + 1) {
      fVec26[l57] = 0.0f;
    }
    for (int l58 = 0; l58 < 2; l58 = l58 + 1) {
      fVec27[l58] = 0.0f;
    }
    for (int l59 = 0; l59 < 4; l59 = l59 + 1) {
      fRec31[l59] = 0.0f;
    }
    for (int l60 = 0; l60 < 2; l60 = l60 + 1) {
      fRec34[l60] = 0.0f;
    }
    for (int l61 = 0; l61 < 8192; l61 = l61 + 1) {
      fVec28[l61] = 0.0f;
    }
    for (int l62 = 0; l62 < 2; l62 = l62 + 1) {
      fVec29[l62] = 0.0f;
    }
    for (int l63 = 0; l63 < 2; l63 = l63 + 1) {
      fVec30[l63] = 0.0f;
    }
    for (int l64 = 0; l64 < 4; l64 = l64 + 1) {
      fRec33[l64] = 0.0f;
    }
    for (int l65 = 0; l65 < 2; l65 = l65 + 1) {
      fVec31[l65] = 0.0f;
    }
    for (int l66 = 0; l66 < 2; l66 = l66 + 1) {
      fRec1[l66] = 0.0f;
    }
    for (int l67 = 0; l67 < 2; l67 = l67 + 1) {
      fVec32[l67] = 0.0f;
    }
    for (int l68 = 0; l68 < 2; l68 = l68 + 1) {
      fRec45[l68] = 0.0f;
    }
    for (int l69 = 0; l69 < 2; l69 = l69 + 1) {
      fRec46[l69] = 0.0f;
    }
    for (int l70 = 0; l70 < 2; l70 = l70 + 1) {
      fVec33[l70] = 0.0f;
    }
    for (int l71 = 0; l71 < 2; l71 = l71 + 1) {
      fVec34[l71] = 0.0f;
    }
    for (int l72 = 0; l72 < 2; l72 = l72 + 1) {
      fRec48[l72] = 0.0f;
    }
    for (int l73 = 0; l73 < 2; l73 = l73 + 1) {
      fRec49[l73] = 0.0f;
    }
    for (int l74 = 0; l74 < 2; l74 = l74 + 1) {
      fRec47[l74] = 0.0f;
    }
    for (int l75 = 0; l75 < 8192; l75 = l75 + 1) {
      fVec35[l75] = 0.0f;
    }
    for (int l76 = 0; l76 < 2; l76 = l76 + 1) {
      fRec50[l76] = 0.0f;
    }
    for (int l77 = 0; l77 < 2; l77 = l77 + 1) {
      fRec54[l77] = 0.0f;
    }
    for (int l78 = 0; l78 < 2; l78 = l78 + 1) {
      fVec36[l78] = 0.0f;
    }
    for (int l79 = 0; l79 < 2; l79 = l79 + 1) {
      fRec53[l79] = 0.0f;
    }
    for (int l80 = 0; l80 < 2; l80 = l80 + 1) {
      fRec55[l80] = 0.0f;
    }
    for (int l81 = 0; l81 < 3; l81 = l81 + 1) {
      fRec52[l81] = 0.0f;
    }
    for (int l82 = 0; l82 < 3; l82 = l82 + 1) {
      fRec51[l82] = 0.0f;
    }
    for (int l83 = 0; l83 < 2; l83 = l83 + 1) {
      fVec37[l83] = 0.0f;
    }
    for (int l84 = 0; l84 < 2; l84 = l84 + 1) {
      fRec58[l84] = 0.0f;
    }
    for (int l85 = 0; l85 < 2; l85 = l85 + 1) {
      fRec59[l85] = 0.0f;
    }
    for (int l86 = 0; l86 < 3; l86 = l86 + 1) {
      fRec57[l86] = 0.0f;
    }
    for (int l87 = 0; l87 < 3; l87 = l87 + 1) {
      fRec56[l87] = 0.0f;
    }
    for (int l88 = 0; l88 < 2; l88 = l88 + 1) {
      fVec38[l88] = 0.0f;
    }
    for (int l89 = 0; l89 < 2; l89 = l89 + 1) {
      fRec62[l89] = 0.0f;
    }
    for (int l90 = 0; l90 < 2; l90 = l90 + 1) {
      fRec63[l90] = 0.0f;
    }
    for (int l91 = 0; l91 < 3; l91 = l91 + 1) {
      fRec61[l91] = 0.0f;
    }
    for (int l92 = 0; l92 < 3; l92 = l92 + 1) {
      fRec60[l92] = 0.0f;
    }
    for (int l93 = 0; l93 < 2; l93 = l93 + 1) {
      fRec64[l93] = 0.0f;
    }
    for (int l94 = 0; l94 < 8192; l94 = l94 + 1) {
      fVec39[l94] = 0.0f;
    }
    for (int l95 = 0; l95 < 2; l95 = l95 + 1) {
      fVec40[l95] = 0.0f;
    }
    for (int l96 = 0; l96 < 2; l96 = l96 + 1) {
      fVec41[l96] = 0.0f;
    }
    for (int l97 = 0; l97 < 4; l97 = l97 + 1) {
      fRec44[l97] = 0.0f;
    }
    for (int l98 = 0; l98 < 2; l98 = l98 + 1) {
      fRec66[l98] = 0.0f;
    }
    for (int l99 = 0; l99 < 8192; l99 = l99 + 1) {
      fVec42[l99] = 0.0f;
    }
    for (int l100 = 0; l100 < 2; l100 = l100 + 1) {
      fVec43[l100] = 0.0f;
    }
    for (int l101 = 0; l101 < 2; l101 = l101 + 1) {
      fVec44[l101] = 0.0f;
    }
    for (int l102 = 0; l102 < 4; l102 = l102 + 1) {
      fRec65[l102] = 0.0f;
    }
    for (int l103 = 0; l103 < 2; l103 = l103 + 1) {
      fRec68[l103] = 0.0f;
    }
    for (int l104 = 0; l104 < 8192; l104 = l104 + 1) {
      fVec45[l104] = 0.0f;
    }
    for (int l105 = 0; l105 < 2; l105 = l105 + 1) {
      fVec46[l105] = 0.0f;
    }
    for (int l106 = 0; l106 < 2; l106 = l106 + 1) {
      fVec47[l106] = 0.0f;
    }
    for (int l107 = 0; l107 < 4; l107 = l107 + 1) {
      fRec67[l107] = 0.0f;
    }
    for (int l108 = 0; l108 < 2; l108 = l108 + 1) {
      fRec70[l108] = 0.0f;
    }
    for (int l109 = 0; l109 < 8192; l109 = l109 + 1) {
      fVec48[l109] = 0.0f;
    }
    for (int l110 = 0; l110 < 2; l110 = l110 + 1) {
      fVec49[l110] = 0.0f;
    }
    for (int l111 = 0; l111 < 2; l111 = l111 + 1) {
      fVec50[l111] = 0.0f;
    }
    for (int l112 = 0; l112 < 4; l112 = l112 + 1) {
      fRec69[l112] = 0.0f;
    }
    for (int l113 = 0; l113 < 2; l113 = l113 + 1) {
      fRec72[l113] = 0.0f;
    }
    for (int l114 = 0; l114 < 8192; l114 = l114 + 1) {
      fVec51[l114] = 0.0f;
    }
    for (int l115 = 0; l115 < 2; l115 = l115 + 1) {
      fVec52[l115] = 0.0f;
    }
    for (int l116 = 0; l116 < 2; l116 = l116 + 1) {
      fVec53[l116] = 0.0f;
    }
    for (int l117 = 0; l117 < 4; l117 = l117 + 1) {
      fRec71[l117] = 0.0f;
    }
    for (int l118 = 0; l118 < 2; l118 = l118 + 1) {
      fRec74[l118] = 0.0f;
    }
    for (int l119 = 0; l119 < 8192; l119 = l119 + 1) {
      fVec54[l119] = 0.0f;
    }
    for (int l120 = 0; l120 < 2; l120 = l120 + 1) {
      fVec55[l120] = 0.0f;
    }
    for (int l121 = 0; l121 < 2; l121 = l121 + 1) {
      fVec56[l121] = 0.0f;
    }
    for (int l122 = 0; l122 < 4; l122 = l122 + 1) {
      fRec73[l122] = 0.0f;
    }
    for (int l123 = 0; l123 < 2; l123 = l123 + 1) {
      fRec76[l123] = 0.0f;
    }
    for (int l124 = 0; l124 < 8192; l124 = l124 + 1) {
      fVec57[l124] = 0.0f;
    }
    for (int l125 = 0; l125 < 2; l125 = l125 + 1) {
      fVec58[l125] = 0.0f;
    }
    for (int l126 = 0; l126 < 2; l126 = l126 + 1) {
      fVec59[l126] = 0.0f;
    }
    for (int l127 = 0; l127 < 4; l127 = l127 + 1) {
      fRec75[l127] = 0.0f;
    }
    for (int l128 = 0; l128 < 2; l128 = l128 + 1) {
      fRec78[l128] = 0.0f;
    }
    for (int l129 = 0; l129 < 8192; l129 = l129 + 1) {
      fVec60[l129] = 0.0f;
    }
    for (int l130 = 0; l130 < 2; l130 = l130 + 1) {
      fVec61[l130] = 0.0f;
    }
    for (int l131 = 0; l131 < 2; l131 = l131 + 1) {
      fVec62[l131] = 0.0f;
    }
    for (int l132 = 0; l132 < 4; l132 = l132 + 1) {
      fRec77[l132] = 0.0f;
    }
    for (int l133 = 0; l133 < 2; l133 = l133 + 1) {
      fRec80[l133] = 0.0f;
    }
    for (int l134 = 0; l134 < 8192; l134 = l134 + 1) {
      fVec63[l134] = 0.0f;
    }
    for (int l135 = 0; l135 < 2; l135 = l135 + 1) {
      fVec64[l135] = 0.0f;
    }
    for (int l136 = 0; l136 < 2; l136 = l136 + 1) {
      fVec65[l136] = 0.0f;
    }
    for (int l137 = 0; l137 < 4; l137 = l137 + 1) {
      fRec79[l137] = 0.0f;
    }
    for (int l138 = 0; l138 < 2; l138 = l138 + 1) {
      fVec66[l138] = 0.0f;
    }
    for (int l139 = 0; l139 < 2; l139 = l139 + 1) {
      fRec43[l139] = 0.0f;
    }
    for (int l140 = 0; l140 < 2; l140 = l140 + 1) {
      fRec83[l140] = 0.0f;
    }
    for (int l141 = 0; l141 < 8192; l141 = l141 + 1) {
      fVec67[l141] = 0.0f;
    }
    for (int l142 = 0; l142 < 2; l142 = l142 + 1) {
      fVec68[l142] = 0.0f;
    }
    for (int l143 = 0; l143 < 2; l143 = l143 + 1) {
      fVec69[l143] = 0.0f;
    }
    for (int l144 = 0; l144 < 4; l144 = l144 + 1) {
      fRec82[l144] = 0.0f;
    }
    for (int l145 = 0; l145 < 2; l145 = l145 + 1) {
      fRec85[l145] = 0.0f;
    }
    for (int l146 = 0; l146 < 8192; l146 = l146 + 1) {
      fVec70[l146] = 0.0f;
    }
    for (int l147 = 0; l147 < 2; l147 = l147 + 1) {
      fVec71[l147] = 0.0f;
    }
    for (int l148 = 0; l148 < 2; l148 = l148 + 1) {
      fVec72[l148] = 0.0f;
    }
    for (int l149 = 0; l149 < 4; l149 = l149 + 1) {
      fRec84[l149] = 0.0f;
    }
    for (int l150 = 0; l150 < 2; l150 = l150 + 1) {
      fRec87[l150] = 0.0f;
    }
    for (int l151 = 0; l151 < 8192; l151 = l151 + 1) {
      fVec73[l151] = 0.0f;
    }
    for (int l152 = 0; l152 < 2; l152 = l152 + 1) {
      fVec74[l152] = 0.0f;
    }
    for (int l153 = 0; l153 < 2; l153 = l153 + 1) {
      fVec75[l153] = 0.0f;
    }
    for (int l154 = 0; l154 < 4; l154 = l154 + 1) {
      fRec86[l154] = 0.0f;
    }
    for (int l155 = 0; l155 < 2; l155 = l155 + 1) {
      fRec89[l155] = 0.0f;
    }
    for (int l156 = 0; l156 < 8192; l156 = l156 + 1) {
      fVec76[l156] = 0.0f;
    }
    for (int l157 = 0; l157 < 2; l157 = l157 + 1) {
      fVec77[l157] = 0.0f;
    }
    for (int l158 = 0; l158 < 2; l158 = l158 + 1) {
      fVec78[l158] = 0.0f;
    }
    for (int l159 = 0; l159 < 4; l159 = l159 + 1) {
      fRec88[l159] = 0.0f;
    }
    for (int l160 = 0; l160 < 2; l160 = l160 + 1) {
      fRec91[l160] = 0.0f;
    }
    for (int l161 = 0; l161 < 8192; l161 = l161 + 1) {
      fVec79[l161] = 0.0f;
    }
    for (int l162 = 0; l162 < 2; l162 = l162 + 1) {
      fVec80[l162] = 0.0f;
    }
    for (int l163 = 0; l163 < 2; l163 = l163 + 1) {
      fVec81[l163] = 0.0f;
    }
    for (int l164 = 0; l164 < 4; l164 = l164 + 1) {
      fRec90[l164] = 0.0f;
    }
    for (int l165 = 0; l165 < 2; l165 = l165 + 1) {
      fRec93[l165] = 0.0f;
    }
    for (int l166 = 0; l166 < 8192; l166 = l166 + 1) {
      fVec82[l166] = 0.0f;
    }
    for (int l167 = 0; l167 < 2; l167 = l167 + 1) {
      fVec83[l167] = 0.0f;
    }
    for (int l168 = 0; l168 < 2; l168 = l168 + 1) {
      fVec84[l168] = 0.0f;
    }
    for (int l169 = 0; l169 < 4; l169 = l169 + 1) {
      fRec92[l169] = 0.0f;
    }
    for (int l170 = 0; l170 < 2; l170 = l170 + 1) {
      fRec95[l170] = 0.0f;
    }
    for (int l171 = 0; l171 < 8192; l171 = l171 + 1) {
      fVec85[l171] = 0.0f;
    }
    for (int l172 = 0; l172 < 2; l172 = l172 + 1) {
      fVec86[l172] = 0.0f;
    }
    for (int l173 = 0; l173 < 2; l173 = l173 + 1) {
      fVec87[l173] = 0.0f;
    }
    for (int l174 = 0; l174 < 4; l174 = l174 + 1) {
      fRec94[l174] = 0.0f;
    }
    for (int l175 = 0; l175 < 2; l175 = l175 + 1) {
      fRec97[l175] = 0.0f;
    }
    for (int l176 = 0; l176 < 8192; l176 = l176 + 1) {
      fVec88[l176] = 0.0f;
    }
    for (int l177 = 0; l177 < 2; l177 = l177 + 1) {
      fVec89[l177] = 0.0f;
    }
    for (int l178 = 0; l178 < 2; l178 = l178 + 1) {
      fVec90[l178] = 0.0f;
    }
    for (int l179 = 0; l179 < 4; l179 = l179 + 1) {
      fRec96[l179] = 0.0f;
    }
    for (int l180 = 0; l180 < 2; l180 = l180 + 1) {
      fRec99[l180] = 0.0f;
    }
    for (int l181 = 0; l181 < 8192; l181 = l181 + 1) {
      fVec91[l181] = 0.0f;
    }
    for (int l182 = 0; l182 < 2; l182 = l182 + 1) {
      fVec92[l182] = 0.0f;
    }
    for (int l183 = 0; l183 < 2; l183 = l183 + 1) {
      fVec93[l183] = 0.0f;
    }
    for (int l184 = 0; l184 < 4; l184 = l184 + 1) {
      fRec98[l184] = 0.0f;
    }
    for (int l185 = 0; l185 < 2; l185 = l185 + 1) {
      fVec94[l185] = 0.0f;
    }
    for (int l186 = 0; l186 < 2; l186 = l186 + 1) {
      fRec81[l186] = 0.0f;
    }
    for (int l187 = 0; l187 < 2; l187 = l187 + 1) {
      fRec36[l187] = 0.0f;
    }
    for (int l188 = 0; l188 < 2; l188 = l188 + 1) {
      fVec95[l188] = 0.0f;
    }
    for (int l189 = 0; l189 < 2; l189 = l189 + 1) {
      fRec102[l189] = 0.0f;
    }
    for (int l190 = 0; l190 < 2; l190 = l190 + 1) {
      fRec103[l190] = 0.0f;
    }
    for (int l191 = 0; l191 < 2; l191 = l191 + 1) {
      fVec96[l191] = 0.0f;
    }
    for (int l192 = 0; l192 < 2; l192 = l192 + 1) {
      fVec97[l192] = 0.0f;
    }
    for (int l193 = 0; l193 < 2; l193 = l193 + 1) {
      fRec105[l193] = 0.0f;
    }
    for (int l194 = 0; l194 < 2; l194 = l194 + 1) {
      fRec106[l194] = 0.0f;
    }
    for (int l195 = 0; l195 < 2; l195 = l195 + 1) {
      fRec104[l195] = 0.0f;
    }
    for (int l196 = 0; l196 < 8192; l196 = l196 + 1) {
      fVec98[l196] = 0.0f;
    }
    for (int l197 = 0; l197 < 2; l197 = l197 + 1) {
      fRec107[l197] = 0.0f;
    }
    for (int l198 = 0; l198 < 2; l198 = l198 + 1) {
      fVec99[l198] = 0.0f;
    }
    for (int l199 = 0; l199 < 2; l199 = l199 + 1) {
      fRec110[l199] = 0.0f;
    }
    for (int l200 = 0; l200 < 2; l200 = l200 + 1) {
      fRec111[l200] = 0.0f;
    }
    for (int l201 = 0; l201 < 3; l201 = l201 + 1) {
      fRec109[l201] = 0.0f;
    }
    for (int l202 = 0; l202 < 3; l202 = l202 + 1) {
      fRec108[l202] = 0.0f;
    }
    for (int l203 = 0; l203 < 2; l203 = l203 + 1) {
      fRec112[l203] = 0.0f;
    }
    for (int l204 = 0; l204 < 8192; l204 = l204 + 1) {
      fVec100[l204] = 0.0f;
    }
    for (int l205 = 0; l205 < 2; l205 = l205 + 1) {
      fVec101[l205] = 0.0f;
    }
    for (int l206 = 0; l206 < 2; l206 = l206 + 1) {
      fVec102[l206] = 0.0f;
    }
    for (int l207 = 0; l207 < 4; l207 = l207 + 1) {
      fRec101[l207] = 0.0f;
    }
    for (int l208 = 0; l208 < 2; l208 = l208 + 1) {
      fRec114[l208] = 0.0f;
    }
    for (int l209 = 0; l209 < 8192; l209 = l209 + 1) {
      fVec103[l209] = 0.0f;
    }
    for (int l210 = 0; l210 < 2; l210 = l210 + 1) {
      fVec104[l210] = 0.0f;
    }
    for (int l211 = 0; l211 < 2; l211 = l211 + 1) {
      fVec105[l211] = 0.0f;
    }
    for (int l212 = 0; l212 < 4; l212 = l212 + 1) {
      fRec113[l212] = 0.0f;
    }
    for (int l213 = 0; l213 < 2; l213 = l213 + 1) {
      fRec116[l213] = 0.0f;
    }
    for (int l214 = 0; l214 < 8192; l214 = l214 + 1) {
      fVec106[l214] = 0.0f;
    }
    for (int l215 = 0; l215 < 2; l215 = l215 + 1) {
      fVec107[l215] = 0.0f;
    }
    for (int l216 = 0; l216 < 2; l216 = l216 + 1) {
      fVec108[l216] = 0.0f;
    }
    for (int l217 = 0; l217 < 4; l217 = l217 + 1) {
      fRec115[l217] = 0.0f;
    }
    for (int l218 = 0; l218 < 2; l218 = l218 + 1) {
      fRec118[l218] = 0.0f;
    }
    for (int l219 = 0; l219 < 8192; l219 = l219 + 1) {
      fVec109[l219] = 0.0f;
    }
    for (int l220 = 0; l220 < 2; l220 = l220 + 1) {
      fVec110[l220] = 0.0f;
    }
    for (int l221 = 0; l221 < 2; l221 = l221 + 1) {
      fVec111[l221] = 0.0f;
    }
    for (int l222 = 0; l222 < 4; l222 = l222 + 1) {
      fRec117[l222] = 0.0f;
    }
    for (int l223 = 0; l223 < 2; l223 = l223 + 1) {
      fRec120[l223] = 0.0f;
    }
    for (int l224 = 0; l224 < 8192; l224 = l224 + 1) {
      fVec112[l224] = 0.0f;
    }
    for (int l225 = 0; l225 < 2; l225 = l225 + 1) {
      fVec113[l225] = 0.0f;
    }
    for (int l226 = 0; l226 < 2; l226 = l226 + 1) {
      fVec114[l226] = 0.0f;
    }
    for (int l227 = 0; l227 < 4; l227 = l227 + 1) {
      fRec119[l227] = 0.0f;
    }
    for (int l228 = 0; l228 < 2; l228 = l228 + 1) {
      fRec122[l228] = 0.0f;
    }
    for (int l229 = 0; l229 < 8192; l229 = l229 + 1) {
      fVec115[l229] = 0.0f;
    }
    for (int l230 = 0; l230 < 2; l230 = l230 + 1) {
      fVec116[l230] = 0.0f;
    }
    for (int l231 = 0; l231 < 2; l231 = l231 + 1) {
      fVec117[l231] = 0.0f;
    }
    for (int l232 = 0; l232 < 4; l232 = l232 + 1) {
      fRec121[l232] = 0.0f;
    }
    for (int l233 = 0; l233 < 2; l233 = l233 + 1) {
      fRec124[l233] = 0.0f;
    }
    for (int l234 = 0; l234 < 8192; l234 = l234 + 1) {
      fVec118[l234] = 0.0f;
    }
    for (int l235 = 0; l235 < 2; l235 = l235 + 1) {
      fVec119[l235] = 0.0f;
    }
    for (int l236 = 0; l236 < 2; l236 = l236 + 1) {
      fVec120[l236] = 0.0f;
    }
    for (int l237 = 0; l237 < 4; l237 = l237 + 1) {
      fRec123[l237] = 0.0f;
    }
    for (int l238 = 0; l238 < 2; l238 = l238 + 1) {
      fRec126[l238] = 0.0f;
    }
    for (int l239 = 0; l239 < 8192; l239 = l239 + 1) {
      fVec121[l239] = 0.0f;
    }
    for (int l240 = 0; l240 < 2; l240 = l240 + 1) {
      fVec122[l240] = 0.0f;
    }
    for (int l241 = 0; l241 < 2; l241 = l241 + 1) {
      fVec123[l241] = 0.0f;
    }
    for (int l242 = 0; l242 < 4; l242 = l242 + 1) {
      fRec125[l242] = 0.0f;
    }
    for (int l243 = 0; l243 < 2; l243 = l243 + 1) {
      fRec128[l243] = 0.0f;
    }
    for (int l244 = 0; l244 < 8192; l244 = l244 + 1) {
      fVec124[l244] = 0.0f;
    }
    for (int l245 = 0; l245 < 2; l245 = l245 + 1) {
      fVec125[l245] = 0.0f;
    }
    for (int l246 = 0; l246 < 2; l246 = l246 + 1) {
      fVec126[l246] = 0.0f;
    }
    for (int l247 = 0; l247 < 4; l247 = l247 + 1) {
      fRec127[l247] = 0.0f;
    }
    for (int l248 = 0; l248 < 2; l248 = l248 + 1) {
      fVec127[l248] = 0.0f;
    }
    for (int l249 = 0; l249 < 2; l249 = l249 + 1) {
      fRec100[l249] = 0.0f;
    }
    for (int l250 = 0; l250 < 2; l250 = l250 + 1) {
      fRec131[l250] = 0.0f;
    }
    for (int l251 = 0; l251 < 8192; l251 = l251 + 1) {
      fVec128[l251] = 0.0f;
    }
    for (int l252 = 0; l252 < 2; l252 = l252 + 1) {
      fVec129[l252] = 0.0f;
    }
    for (int l253 = 0; l253 < 2; l253 = l253 + 1) {
      fVec130[l253] = 0.0f;
    }
    for (int l254 = 0; l254 < 4; l254 = l254 + 1) {
      fRec130[l254] = 0.0f;
    }
    for (int l255 = 0; l255 < 2; l255 = l255 + 1) {
      fRec133[l255] = 0.0f;
    }
    for (int l256 = 0; l256 < 8192; l256 = l256 + 1) {
      fVec131[l256] = 0.0f;
    }
    for (int l257 = 0; l257 < 2; l257 = l257 + 1) {
      fVec132[l257] = 0.0f;
    }
    for (int l258 = 0; l258 < 2; l258 = l258 + 1) {
      fVec133[l258] = 0.0f;
    }
    for (int l259 = 0; l259 < 4; l259 = l259 + 1) {
      fRec132[l259] = 0.0f;
    }
    for (int l260 = 0; l260 < 2; l260 = l260 + 1) {
      fRec135[l260] = 0.0f;
    }
    for (int l261 = 0; l261 < 8192; l261 = l261 + 1) {
      fVec134[l261] = 0.0f;
    }
    for (int l262 = 0; l262 < 2; l262 = l262 + 1) {
      fVec135[l262] = 0.0f;
    }
    for (int l263 = 0; l263 < 2; l263 = l263 + 1) {
      fVec136[l263] = 0.0f;
    }
    for (int l264 = 0; l264 < 4; l264 = l264 + 1) {
      fRec134[l264] = 0.0f;
    }
    for (int l265 = 0; l265 < 2; l265 = l265 + 1) {
      fRec137[l265] = 0.0f;
    }
    for (int l266 = 0; l266 < 8192; l266 = l266 + 1) {
      fVec137[l266] = 0.0f;
    }
    for (int l267 = 0; l267 < 2; l267 = l267 + 1) {
      fVec138[l267] = 0.0f;
    }
    for (int l268 = 0; l268 < 2; l268 = l268 + 1) {
      fVec139[l268] = 0.0f;
    }
    for (int l269 = 0; l269 < 4; l269 = l269 + 1) {
      fRec136[l269] = 0.0f;
    }
    for (int l270 = 0; l270 < 2; l270 = l270 + 1) {
      fRec139[l270] = 0.0f;
    }
    for (int l271 = 0; l271 < 8192; l271 = l271 + 1) {
      fVec140[l271] = 0.0f;
    }
    for (int l272 = 0; l272 < 2; l272 = l272 + 1) {
      fVec141[l272] = 0.0f;
    }
    for (int l273 = 0; l273 < 2; l273 = l273 + 1) {
      fVec142[l273] = 0.0f;
    }
    for (int l274 = 0; l274 < 4; l274 = l274 + 1) {
      fRec138[l274] = 0.0f;
    }
    for (int l275 = 0; l275 < 2; l275 = l275 + 1) {
      fRec141[l275] = 0.0f;
    }
    for (int l276 = 0; l276 < 8192; l276 = l276 + 1) {
      fVec143[l276] = 0.0f;
    }
    for (int l277 = 0; l277 < 2; l277 = l277 + 1) {
      fVec144[l277] = 0.0f;
    }
    for (int l278 = 0; l278 < 2; l278 = l278 + 1) {
      fVec145[l278] = 0.0f;
    }
    for (int l279 = 0; l279 < 4; l279 = l279 + 1) {
      fRec140[l279] = 0.0f;
    }
    for (int l280 = 0; l280 < 2; l280 = l280 + 1) {
      fRec143[l280] = 0.0f;
    }
    for (int l281 = 0; l281 < 8192; l281 = l281 + 1) {
      fVec146[l281] = 0.0f;
    }
    for (int l282 = 0; l282 < 2; l282 = l282 + 1) {
      fVec147[l282] = 0.0f;
    }
    for (int l283 = 0; l283 < 2; l283 = l283 + 1) {
      fVec148[l283] = 0.0f;
    }
    for (int l284 = 0; l284 < 4; l284 = l284 + 1) {
      fRec142[l284] = 0.0f;
    }
    for (int l285 = 0; l285 < 2; l285 = l285 + 1) {
      fRec145[l285] = 0.0f;
    }
    for (int l286 = 0; l286 < 8192; l286 = l286 + 1) {
      fVec149[l286] = 0.0f;
    }
    for (int l287 = 0; l287 < 2; l287 = l287 + 1) {
      fVec150[l287] = 0.0f;
    }
    for (int l288 = 0; l288 < 2; l288 = l288 + 1) {
      fVec151[l288] = 0.0f;
    }
    for (int l289 = 0; l289 < 4; l289 = l289 + 1) {
      fRec144[l289] = 0.0f;
    }
    for (int l290 = 0; l290 < 2; l290 = l290 + 1) {
      fRec147[l290] = 0.0f;
    }
    for (int l291 = 0; l291 < 8192; l291 = l291 + 1) {
      fVec152[l291] = 0.0f;
    }
    for (int l292 = 0; l292 < 2; l292 = l292 + 1) {
      fVec153[l292] = 0.0f;
    }
    for (int l293 = 0; l293 < 2; l293 = l293 + 1) {
      fVec154[l293] = 0.0f;
    }
    for (int l294 = 0; l294 < 4; l294 = l294 + 1) {
      fRec146[l294] = 0.0f;
    }
    for (int l295 = 0; l295 < 2; l295 = l295 + 1) {
      fVec155[l295] = 0.0f;
    }
    for (int l296 = 0; l296 < 2; l296 = l296 + 1) {
      fRec129[l296] = 0.0f;
    }
    for (int l297 = 0; l297 < 2; l297 = l297 + 1) {
      fRec38[l297] = 0.0f;
    }
    for (int l298 = 0; l298 < 2; l298 = l298 + 1) {
      fVec156[l298] = 0.0f;
    }
    for (int l299 = 0; l299 < 2; l299 = l299 + 1) {
      fRec150[l299] = 0.0f;
    }
    for (int l300 = 0; l300 < 2; l300 = l300 + 1) {
      fRec151[l300] = 0.0f;
    }
    for (int l301 = 0; l301 < 2; l301 = l301 + 1) {
      fVec157[l301] = 0.0f;
    }
    for (int l302 = 0; l302 < 2; l302 = l302 + 1) {
      fVec158[l302] = 0.0f;
    }
    for (int l303 = 0; l303 < 2; l303 = l303 + 1) {
      fRec153[l303] = 0.0f;
    }
    for (int l304 = 0; l304 < 2; l304 = l304 + 1) {
      fRec154[l304] = 0.0f;
    }
    for (int l305 = 0; l305 < 2; l305 = l305 + 1) {
      fRec152[l305] = 0.0f;
    }
    for (int l306 = 0; l306 < 8192; l306 = l306 + 1) {
      fVec159[l306] = 0.0f;
    }
    for (int l307 = 0; l307 < 2; l307 = l307 + 1) {
      fRec155[l307] = 0.0f;
    }
    for (int l308 = 0; l308 < 2; l308 = l308 + 1) {
      fRec156[l308] = 0.0f;
    }
    for (int l309 = 0; l309 < 8192; l309 = l309 + 1) {
      fVec160[l309] = 0.0f;
    }
    for (int l310 = 0; l310 < 2; l310 = l310 + 1) {
      fVec161[l310] = 0.0f;
    }
    for (int l311 = 0; l311 < 2; l311 = l311 + 1) {
      fVec162[l311] = 0.0f;
    }
    for (int l312 = 0; l312 < 4; l312 = l312 + 1) {
      fRec149[l312] = 0.0f;
    }
    for (int l313 = 0; l313 < 2; l313 = l313 + 1) {
      fRec158[l313] = 0.0f;
    }
    for (int l314 = 0; l314 < 8192; l314 = l314 + 1) {
      fVec163[l314] = 0.0f;
    }
    for (int l315 = 0; l315 < 2; l315 = l315 + 1) {
      fVec164[l315] = 0.0f;
    }
    for (int l316 = 0; l316 < 2; l316 = l316 + 1) {
      fVec165[l316] = 0.0f;
    }
    for (int l317 = 0; l317 < 4; l317 = l317 + 1) {
      fRec157[l317] = 0.0f;
    }
    for (int l318 = 0; l318 < 2; l318 = l318 + 1) {
      fRec160[l318] = 0.0f;
    }
    for (int l319 = 0; l319 < 8192; l319 = l319 + 1) {
      fVec166[l319] = 0.0f;
    }
    for (int l320 = 0; l320 < 2; l320 = l320 + 1) {
      fVec167[l320] = 0.0f;
    }
    for (int l321 = 0; l321 < 2; l321 = l321 + 1) {
      fVec168[l321] = 0.0f;
    }
    for (int l322 = 0; l322 < 4; l322 = l322 + 1) {
      fRec159[l322] = 0.0f;
    }
    for (int l323 = 0; l323 < 2; l323 = l323 + 1) {
      fRec162[l323] = 0.0f;
    }
    for (int l324 = 0; l324 < 8192; l324 = l324 + 1) {
      fVec169[l324] = 0.0f;
    }
    for (int l325 = 0; l325 < 2; l325 = l325 + 1) {
      fVec170[l325] = 0.0f;
    }
    for (int l326 = 0; l326 < 2; l326 = l326 + 1) {
      fVec171[l326] = 0.0f;
    }
    for (int l327 = 0; l327 < 4; l327 = l327 + 1) {
      fRec161[l327] = 0.0f;
    }
    for (int l328 = 0; l328 < 2; l328 = l328 + 1) {
      fRec164[l328] = 0.0f;
    }
    for (int l329 = 0; l329 < 8192; l329 = l329 + 1) {
      fVec172[l329] = 0.0f;
    }
    for (int l330 = 0; l330 < 2; l330 = l330 + 1) {
      fVec173[l330] = 0.0f;
    }
    for (int l331 = 0; l331 < 2; l331 = l331 + 1) {
      fVec174[l331] = 0.0f;
    }
    for (int l332 = 0; l332 < 4; l332 = l332 + 1) {
      fRec163[l332] = 0.0f;
    }
    for (int l333 = 0; l333 < 2; l333 = l333 + 1) {
      fRec166[l333] = 0.0f;
    }
    for (int l334 = 0; l334 < 8192; l334 = l334 + 1) {
      fVec175[l334] = 0.0f;
    }
    for (int l335 = 0; l335 < 2; l335 = l335 + 1) {
      fVec176[l335] = 0.0f;
    }
    for (int l336 = 0; l336 < 2; l336 = l336 + 1) {
      fVec177[l336] = 0.0f;
    }
    for (int l337 = 0; l337 < 4; l337 = l337 + 1) {
      fRec165[l337] = 0.0f;
    }
    for (int l338 = 0; l338 < 2; l338 = l338 + 1) {
      fRec168[l338] = 0.0f;
    }
    for (int l339 = 0; l339 < 8192; l339 = l339 + 1) {
      fVec178[l339] = 0.0f;
    }
    for (int l340 = 0; l340 < 2; l340 = l340 + 1) {
      fVec179[l340] = 0.0f;
    }
    for (int l341 = 0; l341 < 2; l341 = l341 + 1) {
      fVec180[l341] = 0.0f;
    }
    for (int l342 = 0; l342 < 4; l342 = l342 + 1) {
      fRec167[l342] = 0.0f;
    }
    for (int l343 = 0; l343 < 2; l343 = l343 + 1) {
      fRec170[l343] = 0.0f;
    }
    for (int l344 = 0; l344 < 8192; l344 = l344 + 1) {
      fVec181[l344] = 0.0f;
    }
    for (int l345 = 0; l345 < 2; l345 = l345 + 1) {
      fVec182[l345] = 0.0f;
    }
    for (int l346 = 0; l346 < 2; l346 = l346 + 1) {
      fVec183[l346] = 0.0f;
    }
    for (int l347 = 0; l347 < 4; l347 = l347 + 1) {
      fRec169[l347] = 0.0f;
    }
    for (int l348 = 0; l348 < 2; l348 = l348 + 1) {
      fRec172[l348] = 0.0f;
    }
    for (int l349 = 0; l349 < 8192; l349 = l349 + 1) {
      fVec184[l349] = 0.0f;
    }
    for (int l350 = 0; l350 < 2; l350 = l350 + 1) {
      fVec185[l350] = 0.0f;
    }
    for (int l351 = 0; l351 < 2; l351 = l351 + 1) {
      fVec186[l351] = 0.0f;
    }
    for (int l352 = 0; l352 < 4; l352 = l352 + 1) {
      fRec171[l352] = 0.0f;
    }
    for (int l353 = 0; l353 < 2; l353 = l353 + 1) {
      fVec187[l353] = 0.0f;
    }
    for (int l354 = 0; l354 < 2; l354 = l354 + 1) {
      fRec148[l354] = 0.0f;
    }
    for (int l355 = 0; l355 < 2; l355 = l355 + 1) {
      fRec175[l355] = 0.0f;
    }
    for (int l356 = 0; l356 < 8192; l356 = l356 + 1) {
      fVec188[l356] = 0.0f;
    }
    for (int l357 = 0; l357 < 2; l357 = l357 + 1) {
      fVec189[l357] = 0.0f;
    }
    for (int l358 = 0; l358 < 2; l358 = l358 + 1) {
      fVec190[l358] = 0.0f;
    }
    for (int l359 = 0; l359 < 4; l359 = l359 + 1) {
      fRec174[l359] = 0.0f;
    }
    for (int l360 = 0; l360 < 2; l360 = l360 + 1) {
      fRec177[l360] = 0.0f;
    }
    for (int l361 = 0; l361 < 8192; l361 = l361 + 1) {
      fVec191[l361] = 0.0f;
    }
    for (int l362 = 0; l362 < 2; l362 = l362 + 1) {
      fVec192[l362] = 0.0f;
    }
    for (int l363 = 0; l363 < 2; l363 = l363 + 1) {
      fVec193[l363] = 0.0f;
    }
    for (int l364 = 0; l364 < 4; l364 = l364 + 1) {
      fRec176[l364] = 0.0f;
    }
    for (int l365 = 0; l365 < 2; l365 = l365 + 1) {
      fRec179[l365] = 0.0f;
    }
    for (int l366 = 0; l366 < 8192; l366 = l366 + 1) {
      fVec194[l366] = 0.0f;
    }
    for (int l367 = 0; l367 < 2; l367 = l367 + 1) {
      fVec195[l367] = 0.0f;
    }
    for (int l368 = 0; l368 < 2; l368 = l368 + 1) {
      fVec196[l368] = 0.0f;
    }
    for (int l369 = 0; l369 < 4; l369 = l369 + 1) {
      fRec178[l369] = 0.0f;
    }
    for (int l370 = 0; l370 < 2; l370 = l370 + 1) {
      fRec181[l370] = 0.0f;
    }
    for (int l371 = 0; l371 < 8192; l371 = l371 + 1) {
      fVec197[l371] = 0.0f;
    }
    for (int l372 = 0; l372 < 2; l372 = l372 + 1) {
      fVec198[l372] = 0.0f;
    }
    for (int l373 = 0; l373 < 2; l373 = l373 + 1) {
      fVec199[l373] = 0.0f;
    }
    for (int l374 = 0; l374 < 4; l374 = l374 + 1) {
      fRec180[l374] = 0.0f;
    }
    for (int l375 = 0; l375 < 2; l375 = l375 + 1) {
      fRec183[l375] = 0.0f;
    }
    for (int l376 = 0; l376 < 8192; l376 = l376 + 1) {
      fVec200[l376] = 0.0f;
    }
    for (int l377 = 0; l377 < 2; l377 = l377 + 1) {
      fVec201[l377] = 0.0f;
    }
    for (int l378 = 0; l378 < 2; l378 = l378 + 1) {
      fVec202[l378] = 0.0f;
    }
    for (int l379 = 0; l379 < 4; l379 = l379 + 1) {
      fRec182[l379] = 0.0f;
    }
    for (int l380 = 0; l380 < 2; l380 = l380 + 1) {
      fRec185[l380] = 0.0f;
    }
    for (int l381 = 0; l381 < 8192; l381 = l381 + 1) {
      fVec203[l381] = 0.0f;
    }
    for (int l382 = 0; l382 < 2; l382 = l382 + 1) {
      fVec204[l382] = 0.0f;
    }
    for (int l383 = 0; l383 < 2; l383 = l383 + 1) {
      fVec205[l383] = 0.0f;
    }
    for (int l384 = 0; l384 < 4; l384 = l384 + 1) {
      fRec184[l384] = 0.0f;
    }
    for (int l385 = 0; l385 < 2; l385 = l385 + 1) {
      fRec187[l385] = 0.0f;
    }
    for (int l386 = 0; l386 < 8192; l386 = l386 + 1) {
      fVec206[l386] = 0.0f;
    }
    for (int l387 = 0; l387 < 2; l387 = l387 + 1) {
      fVec207[l387] = 0.0f;
    }
    for (int l388 = 0; l388 < 2; l388 = l388 + 1) {
      fVec208[l388] = 0.0f;
    }
    for (int l389 = 0; l389 < 4; l389 = l389 + 1) {
      fRec186[l389] = 0.0f;
    }
    for (int l390 = 0; l390 < 2; l390 = l390 + 1) {
      fRec189[l390] = 0.0f;
    }
    for (int l391 = 0; l391 < 8192; l391 = l391 + 1) {
      fVec209[l391] = 0.0f;
    }
    for (int l392 = 0; l392 < 2; l392 = l392 + 1) {
      fVec210[l392] = 0.0f;
    }
    for (int l393 = 0; l393 < 2; l393 = l393 + 1) {
      fVec211[l393] = 0.0f;
    }
    for (int l394 = 0; l394 < 4; l394 = l394 + 1) {
      fRec188[l394] = 0.0f;
    }
    for (int l395 = 0; l395 < 2; l395 = l395 + 1) {
      fRec191[l395] = 0.0f;
    }
    for (int l396 = 0; l396 < 8192; l396 = l396 + 1) {
      fVec212[l396] = 0.0f;
    }
    for (int l397 = 0; l397 < 2; l397 = l397 + 1) {
      fVec213[l397] = 0.0f;
    }
    for (int l398 = 0; l398 < 2; l398 = l398 + 1) {
      fVec214[l398] = 0.0f;
    }
    for (int l399 = 0; l399 < 4; l399 = l399 + 1) {
      fRec190[l399] = 0.0f;
    }
    for (int l400 = 0; l400 < 2; l400 = l400 + 1) {
      fVec215[l400] = 0.0f;
    }
    for (int l401 = 0; l401 < 2; l401 = l401 + 1) {
      fRec173[l401] = 0.0f;
    }
    for (int l402 = 0; l402 < 2; l402 = l402 + 1) {
      fRec40[l402] = 0.0f;
    }
    for (int l403 = 0; l403 < 2; l403 = l403 + 1) {
      fRec194[l403] = 0.0f;
    }
    for (int l404 = 0; l404 < 8192; l404 = l404 + 1) {
      fVec216[l404] = 0.0f;
    }
    for (int l405 = 0; l405 < 2; l405 = l405 + 1) {
      fVec217[l405] = 0.0f;
    }
    for (int l406 = 0; l406 < 2; l406 = l406 + 1) {
      fVec218[l406] = 0.0f;
    }
    for (int l407 = 0; l407 < 4; l407 = l407 + 1) {
      fRec193[l407] = 0.0f;
    }
    for (int l408 = 0; l408 < 2; l408 = l408 + 1) {
      fRec196[l408] = 0.0f;
    }
    for (int l409 = 0; l409 < 8192; l409 = l409 + 1) {
      fVec219[l409] = 0.0f;
    }
    for (int l410 = 0; l410 < 2; l410 = l410 + 1) {
      fVec220[l410] = 0.0f;
    }
    for (int l411 = 0; l411 < 2; l411 = l411 + 1) {
      fVec221[l411] = 0.0f;
    }
    for (int l412 = 0; l412 < 4; l412 = l412 + 1) {
      fRec195[l412] = 0.0f;
    }
    for (int l413 = 0; l413 < 2; l413 = l413 + 1) {
      fRec198[l413] = 0.0f;
    }
    for (int l414 = 0; l414 < 8192; l414 = l414 + 1) {
      fVec222[l414] = 0.0f;
    }
    for (int l415 = 0; l415 < 2; l415 = l415 + 1) {
      fVec223[l415] = 0.0f;
    }
    for (int l416 = 0; l416 < 2; l416 = l416 + 1) {
      fVec224[l416] = 0.0f;
    }
    for (int l417 = 0; l417 < 4; l417 = l417 + 1) {
      fRec197[l417] = 0.0f;
    }
    for (int l418 = 0; l418 < 2; l418 = l418 + 1) {
      fRec200[l418] = 0.0f;
    }
    for (int l419 = 0; l419 < 8192; l419 = l419 + 1) {
      fVec225[l419] = 0.0f;
    }
    for (int l420 = 0; l420 < 2; l420 = l420 + 1) {
      fVec226[l420] = 0.0f;
    }
    for (int l421 = 0; l421 < 2; l421 = l421 + 1) {
      fVec227[l421] = 0.0f;
    }
    for (int l422 = 0; l422 < 4; l422 = l422 + 1) {
      fRec199[l422] = 0.0f;
    }
    for (int l423 = 0; l423 < 2; l423 = l423 + 1) {
      fRec202[l423] = 0.0f;
    }
    for (int l424 = 0; l424 < 8192; l424 = l424 + 1) {
      fVec228[l424] = 0.0f;
    }
    for (int l425 = 0; l425 < 2; l425 = l425 + 1) {
      fVec229[l425] = 0.0f;
    }
    for (int l426 = 0; l426 < 2; l426 = l426 + 1) {
      fVec230[l426] = 0.0f;
    }
    for (int l427 = 0; l427 < 4; l427 = l427 + 1) {
      fRec201[l427] = 0.0f;
    }
    for (int l428 = 0; l428 < 2; l428 = l428 + 1) {
      fRec204[l428] = 0.0f;
    }
    for (int l429 = 0; l429 < 8192; l429 = l429 + 1) {
      fVec231[l429] = 0.0f;
    }
    for (int l430 = 0; l430 < 2; l430 = l430 + 1) {
      fVec232[l430] = 0.0f;
    }
    for (int l431 = 0; l431 < 2; l431 = l431 + 1) {
      fVec233[l431] = 0.0f;
    }
    for (int l432 = 0; l432 < 4; l432 = l432 + 1) {
      fRec203[l432] = 0.0f;
    }
    for (int l433 = 0; l433 < 2; l433 = l433 + 1) {
      fRec206[l433] = 0.0f;
    }
    for (int l434 = 0; l434 < 8192; l434 = l434 + 1) {
      fVec234[l434] = 0.0f;
    }
    for (int l435 = 0; l435 < 2; l435 = l435 + 1) {
      fVec235[l435] = 0.0f;
    }
    for (int l436 = 0; l436 < 2; l436 = l436 + 1) {
      fVec236[l436] = 0.0f;
    }
    for (int l437 = 0; l437 < 4; l437 = l437 + 1) {
      fRec205[l437] = 0.0f;
    }
    for (int l438 = 0; l438 < 2; l438 = l438 + 1) {
      fRec208[l438] = 0.0f;
    }
    for (int l439 = 0; l439 < 8192; l439 = l439 + 1) {
      fVec237[l439] = 0.0f;
    }
    for (int l440 = 0; l440 < 2; l440 = l440 + 1) {
      fVec238[l440] = 0.0f;
    }
    for (int l441 = 0; l441 < 2; l441 = l441 + 1) {
      fVec239[l441] = 0.0f;
    }
    for (int l442 = 0; l442 < 4; l442 = l442 + 1) {
      fRec207[l442] = 0.0f;
    }
    for (int l443 = 0; l443 < 2; l443 = l443 + 1) {
      fRec210[l443] = 0.0f;
    }
    for (int l444 = 0; l444 < 8192; l444 = l444 + 1) {
      fVec240[l444] = 0.0f;
    }
    for (int l445 = 0; l445 < 2; l445 = l445 + 1) {
      fVec241[l445] = 0.0f;
    }
    for (int l446 = 0; l446 < 2; l446 = l446 + 1) {
      fVec242[l446] = 0.0f;
    }
    for (int l447 = 0; l447 < 4; l447 = l447 + 1) {
      fRec209[l447] = 0.0f;
    }
    for (int l448 = 0; l448 < 2; l448 = l448 + 1) {
      fVec243[l448] = 0.0f;
    }
    for (int l449 = 0; l449 < 2; l449 = l449 + 1) {
      fRec192[l449] = 0.0f;
    }
    for (int l450 = 0; l450 < 2; l450 = l450 + 1) {
      fRec42[l450] = 0.0f;
    }
    for (int l451 = 0; l451 < 2; l451 = l451 + 1) {
      fRec211[l451] = 0.0f;
    }
  }

  virtual void init(int sample_rate) { instanceInit(sample_rate); }

  virtual void instanceInit(int sample_rate) {
    staticInit(sample_rate);
    instanceConstants(sample_rate);
    instanceResetUserInterface();
    instanceClear();
  }

  virtual tambura *clone() { return new tambura(); }

  virtual int getSampleRate() { return fSampleRate; }

  virtual void buildUserInterface(UI *ui_interface) {
    ui_interface->openVerticalBox("tambura");
    ui_interface->openHorizontalBox("main");
    ui_interface->declare(&fHslider3, "1", "");
    ui_interface->declare(&fHslider3, "CV", "5");
    ui_interface->declare(&fHslider3, "style", "knob");
    ui_interface->addHorizontalSlider("sa", &fHslider3, FAUSTFLOAT(36.0f),
                                      FAUSTFLOAT(24.0f), FAUSTFLOAT(72.0f),
                                      FAUSTFLOAT(1.0f));
    ui_interface->declare(&fHslider11, "2", "");
    ui_interface->declare(&fHslider11, "CV", "6");
    ui_interface->declare(&fHslider11, "style", "knob");
    ui_interface->declare(&fHslider11, "unit", "s");
    ui_interface->addHorizontalSlider("decay_time", &fHslider11,
                                      FAUSTFLOAT(1e+01f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.1f));
    ui_interface->declare(&fHslider13, "3", "");
    ui_interface->declare(&fHslider13, "CV", "7");
    ui_interface->declare(&fHslider13, "style", "knob");
    ui_interface->addHorizontalSlider("high_freq_loss", &fHslider13,
                                      FAUSTFLOAT(0.0f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider4, "4", "");
    ui_interface->declare(&fHslider4, "CV", "8");
    ui_interface->declare(&fHslider4, "scale", "exp");
    ui_interface->declare(&fHslider4, "style", "knob");
    ui_interface->addHorizontalSlider("harmonic_motion", &fHslider4,
                                      FAUSTFLOAT(0.001f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
    ui_interface->declare(&fHslider15, "5", "");
    ui_interface->declare(&fHslider15, "CV", "9");
    ui_interface->declare(&fHslider15, "style", "knob");
    ui_interface->addHorizontalSlider("sympathetic_coupling", &fHslider15,
                                      FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0001f));
    ui_interface->declare(&fHslider14, "6", "");
    ui_interface->declare(&fHslider14, "CV", "10");
    ui_interface->declare(&fHslider14, "style", "knob");
    ui_interface->addHorizontalSlider("jawari", &fHslider14, FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f),
                                      FAUSTFLOAT(0.001f));
    ui_interface->declare(&fHslider0, "7", "");
    ui_interface->declare(&fHslider0, "CV", "11");
    ui_interface->declare(&fHslider0, "style", "knob");
    ui_interface->addHorizontalSlider("string_spread", &fHslider0,
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider5, "8", "");
    ui_interface->declare(&fHslider5, "CV", "12");
    ui_interface->declare(&fHslider5, "style", "knob");
    ui_interface->addHorizontalSlider("tune_scale", &fHslider5,
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.9f),
                                      FAUSTFLOAT(1.1f), FAUSTFLOAT(0.001f));
    ui_interface->declare(&fHslider12, "9", "");
    ui_interface->declare(&fHslider12, "CV", "13");
    ui_interface->declare(&fHslider12, "style", "knob");
    ui_interface->addHorizontalSlider("decay_scale", &fHslider12,
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.1f),
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(0.001f));
    ui_interface->closeBox();
    ui_interface->openHorizontalBox("pick");
    ui_interface->declare(&fHslider6, "1", "");
    ui_interface->declare(&fHslider6, "CV", "14");
    ui_interface->declare(&fHslider6, "style", "knob");
    ui_interface->addHorizontalSlider("material", &fHslider6, FAUSTFLOAT(0.13f),
                                      FAUSTFLOAT(0.0f), FAUSTFLOAT(1.0f),
                                      FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider9, "2", "");
    ui_interface->declare(&fHslider9, "CV", "15");
    ui_interface->declare(&fHslider9, "scale", "exp");
    ui_interface->declare(&fHslider9, "style", "knob");
    ui_interface->addHorizontalSlider("attack_time", &fHslider9,
                                      FAUSTFLOAT(0.07f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(0.5f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider8, "3", "");
    ui_interface->declare(&fHslider8, "CV", "16");
    ui_interface->declare(&fHslider8, "style", "knob");
    ui_interface->addHorizontalSlider("decay_time", &fHslider8,
                                      FAUSTFLOAT(1.0f), FAUSTFLOAT(1.0f),
                                      FAUSTFLOAT(1e+02f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider10, "4", "");
    ui_interface->declare(&fHslider10, "CV", "17");
    ui_interface->declare(&fHslider10, "style", "knob");
    ui_interface->addHorizontalSlider("position", &fHslider10,
                                      FAUSTFLOAT(0.25f), FAUSTFLOAT(0.01f),
                                      FAUSTFLOAT(0.5f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider2, "5", "");
    ui_interface->declare(&fHslider2, "CV", "18");
    ui_interface->declare(&fHslider2, "style", "knob");
    ui_interface->declare(&fHslider2, "unit", "st");
    ui_interface->addHorizontalSlider("bend_depth", &fHslider2,
                                      FAUSTFLOAT(3.0f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(12.0f), FAUSTFLOAT(0.01f));
    ui_interface->declare(&fHslider1, "6", "");
    ui_interface->declare(&fHslider1, "CV", "19");
    ui_interface->declare(&fHslider1, "style", "knob");
    ui_interface->declare(&fHslider1, "unit", "ms");
    ui_interface->addHorizontalSlider("bend_time", &fHslider1,
                                      FAUSTFLOAT(1e+01f), FAUSTFLOAT(1.0f),
                                      FAUSTFLOAT(2e+02f), FAUSTFLOAT(1.0f));
    ui_interface->closeBox();
    ui_interface->openHorizontalBox("trigger");
    ui_interface->declare(&fHslider7, "style", "knob");
    ui_interface->declare(&fHslider7, "unit", "hz");
    ui_interface->addHorizontalSlider("auto pluck rate", &fHslider7,
                                      FAUSTFLOAT(0.1f), FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(0.5f), FAUSTFLOAT(0.001f));
    ui_interface->addCheckButton("enable auto pluck", &fCheckbox0);
    ui_interface->declare(&fButton1, "CV", "0");
    ui_interface->addButton("gate0", &fButton1);
    ui_interface->declare(&fButton2, "CV", "1");
    ui_interface->addButton("gate1", &fButton2);
    ui_interface->declare(&fButton3, "CV", "2");
    ui_interface->addButton("gate2", &fButton3);
    ui_interface->declare(&fButton0, "CV", "3");
    ui_interface->addButton("gate3", &fButton0);
    ui_interface->closeBox();
    ui_interface->declare(&fHslider16, "unit", "dB");
    ui_interface->addHorizontalSlider("volume", &fHslider16, FAUSTFLOAT(0.0f),
                                      FAUSTFLOAT(-36.0f), FAUSTFLOAT(4.0f),
                                      FAUSTFLOAT(0.1f));
    ui_interface->closeBox();
  }

  virtual void control(int *RESTRICT iControl, float *RESTRICT fControl) {
    fControl[0] = fConst2 * float(fHslider0);
    fControl[1] = float(fButton0);
    fControl[2] = 1.0f - fControl[1];
    fControl[3] = fConst3 * float(fHslider1);
    fControl[4] = float(fHslider2);
    fControl[5] = tambura_faustpower2_f(fControl[1]) * fControl[4];
    fControl[6] = fConst2 * float(fHslider3);
    fControl[7] = fConst4 * float(fHslider4);
    fControl[8] = float(fHslider5);
    fControl[9] = fConst5 / fControl[8];
    fControl[10] = fConst2 * float(fHslider6);
    fControl[11] = fConst6 * float(fHslider7);
    fControl[12] = float(fCheckbox0);
    fControl[13] = float(fHslider8);
    fControl[14] = fConst0 * fControl[13];
    fControl[15] = float(fHslider9);
    fControl[16] = float(fHslider10);
    fControl[17] = fConst2 * float(fHslider11);
    fControl[18] = float(fHslider12);
    fControl[19] = 0.0022727272f / (fControl[8] * fControl[18]);
    fControl[20] = fConst2 * (1.0f - float(fHslider13));
    fControl[21] = fConst8 * float(fHslider14);
    fControl[22] = float(fButton1);
    fControl[23] = 1.0f - fControl[22];
    fControl[24] = tambura_faustpower2_f(fControl[22]) * fControl[4];
    fControl[25] = fConst9 * fControl[13];
    fControl[26] = 0.6666667f * fControl[15];
    fControl[27] = 0.0022727272f / fControl[18];
    fControl[28] = fConst2 * float(fHslider15);
    fControl[29] = float(fButton2);
    fControl[30] = 1.0f - fControl[29];
    fControl[31] = tambura_faustpower2_f(fControl[29]) * fControl[4];
    fControl[32] = fConst34 * fControl[13];
    fControl[33] = 0.5f * fControl[15];
    fControl[34] = float(fButton3);
    fControl[35] = 1.0f - fControl[34];
    fControl[36] = tambura_faustpower2_f(fControl[34]) * fControl[4];
    fControl[37] = fConst35 * fControl[13];
    fControl[38] = 0.49751243f * fControl[15];
    fControl[39] = fConst2 * std::pow(1e+01f, 0.05f * float(fHslider16));
  }

  virtual int getNumIntControls() { return 0; }
  virtual int getNumRealControls() { return 40; }

  virtual void compute(FAUSTFLOAT *RESTRICT inputs,
                       FAUSTFLOAT *RESTRICT outputs, int *RESTRICT iControl,
                       float *RESTRICT fControl) {
    fRec0[0] = fControl[0] + fConst1 * fRec0[1];
    float fTemp0 = 0.5f * (fRec0[0] + 1.0f);
    fVec0[0] = fControl[2];
    float fTemp1 = ((fControl[2] != fVec0[1]) ? fControl[3] : fRec3[1] + -1.0f);
    fRec3[0] = fTemp1;
    fRec4[0] = ((fTemp1 <= 0.0f)
                    ? fControl[2]
                    : fRec4[1] + (1.0f - fRec4[1] - fControl[1]) / fTemp1);
    fRec5[0] = fControl[6] + fConst1 * fRec5[1];
    float fTemp2 = fRec5[0] + fControl[5] * tambura_faustpower2_f(fRec4[0]);
    fRec6[0] = fControl[7] + fConst1 * fRec6[1];
    float fTemp3 = 4.0f * fRec6[0];
    float fTemp4 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f * (fTemp3 + fTemp2)));
    float fTemp5 = fControl[9] / fTemp4;
    float fTemp6 = fTemp5 + -2.499995f;
    float fTemp7 = std::floor(fTemp6);
    iRec8[0] = 1103515245 * iRec8[1] + 12345;
    float fTemp8 = 4.656613e-10f * float(iRec8[0]);
    fRec7[0] = 0.5221894f * fRec7[3] + fTemp8 + 2.494956f * fRec7[1] -
               2.0172658f * fRec7[2];
    fRec9[0] = fControl[10] + fConst1 * fRec9[1];
    float fTemp9 =
        fRec9[0] *
            (0.049922034f * fRec7[0] + 0.0506127f * fRec7[2] +
             (-1.0f - (0.095993534f * fRec7[1] + 0.004408786f * fRec7[3]))) +
        1.0f;
    fRec11[0] =
        fControl[11] + (fRec11[1] - std::floor(fControl[11] + fRec11[1]));
    float fTemp10 =
        fControl[12] * float((fRec11[0] > 0.75f) & (fRec11[0] < 1.0f));
    float fTemp11 = fControl[1] + fTemp10;
    fVec1[0] = fTemp11;
    float fTemp12 = 1.0f - fTemp10 - fControl[1];
    fVec2[0] = fTemp12;
    float fTemp13 =
        ((fTemp12 != fVec2[1]) ? fControl[14] / fRec5[0] : fRec12[1] + -1.0f);
    fRec12[0] = fTemp13;
    fRec13[0] =
        ((fTemp13 <= 0.0f)
             ? fTemp12
             : fRec13[1] +
                   (1.0f - (fTemp10 + fRec13[1]) - fControl[1]) / fTemp13);
    float fTemp14 = fRec13[0] * fTemp11;
    float fTemp15 = fControl[15] / fRec5[0];
    int iTemp16 = std::fabs(fTemp15) < 1.1920929e-07f;
    float fTemp17 =
        ((fTemp14 > fRec10[1])
             ? ((iTemp16) ? 0.0f
                          : std::exp(-(fConst6 / ((iTemp16) ? 1.0f : fTemp15))))
             : fConst7);
    fRec10[0] = fTemp14 * (1.0f - fTemp17) + fRec10[1] * fTemp17;
    float fTemp18 = fRec10[0] * fTemp9;
    fVec3[IOTA0 & 8191] = fTemp18;
    int iTemp19 = (fVec1[1] <= 0.0f) & (fTemp11 > 0.0f);
    float fTemp20 = std::fabs(fTemp8);
    fRec14[0] = fRec14[1] * float(1 - iTemp19) + fTemp20 * float(iTemp19);
    float fTemp21 =
        fVec3[(IOTA0 - int(std::min<float>(
                           4096.0f,
                           std::max<float>(0.0f, fConst0 * ((fControl[16] +
                                                             0.2f * fRec14[0]) /
                                                            fRec5[0]))))) &
              8191];
    fRec15[0] = fControl[17] + fConst1 * fRec15[1];
    float fTemp22 = fRec15[0] * fTemp4;
    fRec16[0] = fControl[20] + fConst1 * fRec16[1];
    float fTemp23 = fRec16[0] * (fTemp18 - fTemp21);
    float fTemp24 = 1.0f - fTemp23;
    float fTemp25 = fTemp23 + 1.0f;
    float fTemp26 =
        0.5f * fTemp25 * fRec2[2] + 0.25f * fTemp24 * (fRec2[1] + fRec2[3]);
    fRec17[0] = fControl[21] + fConst1 * fRec17[1];
    float fTemp27 = 3.1415927f * fRec17[0] * fTemp26;
    float fTemp28 = std::cos(fTemp27);
    float fTemp29 = std::sin(fTemp27);
    fRec18[0] = fTemp26 * fTemp28 - fTemp29 * fRec18[1];
    fVec4[IOTA0 & 8191] = fTemp18 +
                          (fTemp26 * fTemp29 + fRec18[1] * fTemp28) *
                              std::pow(0.001f, fControl[19] / fTemp22) -
                          fTemp21;
    float fTemp30 =
        fVec4[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp6)))) &
              8191];
    fVec5[0] = fTemp30;
    float fTemp31 = (fTemp30 - fVec5[1]) * (fTemp5 + (-2.0f - fTemp7));
    fVec6[0] = fTemp31;
    fRec2[0] = fTemp30 +
               0.5f * (fVec6[1] - fTemp31) * (fTemp7 + (3.0f - fTemp5)) -
               fTemp31;
    float fTemp32 = 3.0f * fRec6[0];
    float fTemp33 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp32 + fTemp2)));
    float fTemp34 = fControl[9] / fTemp33;
    float fTemp35 = fTemp34 + -2.499995f;
    float fTemp36 = std::floor(fTemp35);
    float fTemp37 = fRec15[0] * fTemp33;
    float fTemp38 =
        0.5f * fTemp25 * fRec19[2] + 0.25f * fTemp24 * (fRec19[1] + fRec19[3]);
    float fTemp39 = 3.1415927f * fRec17[0] * fTemp38;
    float fTemp40 = std::cos(fTemp39);
    float fTemp41 = std::sin(fTemp39);
    fRec20[0] = fTemp38 * fTemp40 - fTemp41 * fRec20[1];
    fVec7[IOTA0 & 8191] = fTemp18 +
                          (fTemp38 * fTemp41 + fRec20[1] * fTemp40) *
                              std::pow(0.001f, fControl[19] / fTemp37) -
                          fTemp21;
    float fTemp42 =
        fVec7[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp35)))) &
              8191];
    fVec8[0] = fTemp42;
    float fTemp43 = (fTemp42 - fVec8[1]) * (fTemp34 + (-2.0f - fTemp36));
    fVec9[0] = fTemp43;
    fRec19[0] = fTemp42 +
                0.5f * (fVec9[1] - fTemp43) * (fTemp36 + (3.0f - fTemp34)) -
                fTemp43;
    float fTemp44 = 2.0f * fRec6[0];
    float fTemp45 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp44 + fTemp2)));
    float fTemp46 = fControl[9] / fTemp45;
    float fTemp47 = fTemp46 + -2.499995f;
    float fTemp48 = std::floor(fTemp47);
    float fTemp49 = fRec15[0] * fTemp45;
    float fTemp50 =
        0.5f * fTemp25 * fRec21[2] + 0.25f * fTemp24 * (fRec21[1] + fRec21[3]);
    float fTemp51 = 3.1415927f * fRec17[0] * fTemp50;
    float fTemp52 = std::cos(fTemp51);
    float fTemp53 = std::sin(fTemp51);
    fRec22[0] = fTemp50 * fTemp52 - fTemp53 * fRec22[1];
    fVec10[IOTA0 & 8191] = fTemp18 +
                           (fTemp50 * fTemp53 + fRec22[1] * fTemp52) *
                               std::pow(0.001f, fControl[19] / fTemp49) -
                           fTemp21;
    float fTemp54 =
        fVec10[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp47)))) &
               8191];
    fVec11[0] = fTemp54;
    float fTemp55 = (fTemp54 - fVec11[1]) * (fTemp46 + (-2.0f - fTemp48));
    fVec12[0] = fTemp55;
    fRec21[0] = fTemp54 +
                0.5f * (fVec12[1] - fTemp55) * (fTemp48 + (3.0f - fTemp46)) -
                fTemp55;
    float fTemp56 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fRec6[0] + fTemp2)));
    float fTemp57 = fControl[9] / fTemp56;
    float fTemp58 = fTemp57 + -2.499995f;
    float fTemp59 = std::floor(fTemp58);
    float fTemp60 = fRec15[0] * fTemp56;
    float fTemp61 =
        0.5f * fTemp25 * fRec23[2] + 0.25f * fTemp24 * (fRec23[1] + fRec23[3]);
    float fTemp62 = 3.1415927f * fRec17[0] * fTemp61;
    float fTemp63 = std::cos(fTemp62);
    float fTemp64 = std::sin(fTemp62);
    fRec24[0] = fTemp61 * fTemp63 - fTemp64 * fRec24[1];
    fVec13[IOTA0 & 8191] = fTemp18 +
                           (fTemp61 * fTemp64 + fRec24[1] * fTemp63) *
                               std::pow(0.001f, fControl[19] / fTemp60) -
                           fTemp21;
    float fTemp65 =
        fVec13[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp58)))) &
               8191];
    fVec14[0] = fTemp65;
    float fTemp66 = (fTemp65 - fVec14[1]) * (fTemp57 + (-2.0f - fTemp59));
    fVec15[0] = fTemp66;
    fRec23[0] = fTemp65 +
                0.5f * (fVec15[1] - fTemp66) * (fTemp59 + (3.0f - fTemp57)) -
                fTemp66;
    float fTemp67 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f * fTemp2));
    float fTemp68 = fControl[9] / fTemp67;
    float fTemp69 = fTemp68 + -2.499995f;
    float fTemp70 = std::floor(fTemp69);
    float fTemp71 = fRec15[0] * fTemp67;
    float fTemp72 =
        0.5f * fTemp25 * fRec25[2] + 0.25f * fTemp24 * (fRec25[1] + fRec25[3]);
    float fTemp73 = 3.1415927f * fRec17[0] * fTemp72;
    float fTemp74 = std::cos(fTemp73);
    float fTemp75 = std::sin(fTemp73);
    fRec26[0] = fTemp72 * fTemp74 - fTemp75 * fRec26[1];
    fVec16[IOTA0 & 8191] = fTemp18 +
                           (fTemp72 * fTemp75 + fRec26[1] * fTemp74) *
                               std::pow(0.001f, fControl[19] / fTemp71) -
                           fTemp21;
    float fTemp76 =
        fVec16[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp69)))) &
               8191];
    fVec17[0] = fTemp76;
    float fTemp77 = (fTemp76 - fVec17[1]) * (fTemp68 + (-2.0f - fTemp70));
    fVec18[0] = fTemp77;
    fRec25[0] = fTemp76 +
                0.5f * (fVec18[1] - fTemp77) * (fTemp70 + (3.0f - fTemp68)) -
                fTemp77;
    float fTemp78 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp2 - fRec6[0])));
    float fTemp79 = fControl[9] / fTemp78;
    float fTemp80 = fTemp79 + -2.499995f;
    float fTemp81 = std::floor(fTemp80);
    float fTemp82 = fRec15[0] * fTemp78;
    float fTemp83 =
        0.5f * fTemp25 * fRec27[2] + 0.25f * fTemp24 * (fRec27[1] + fRec27[3]);
    float fTemp84 = 3.1415927f * fRec17[0] * fTemp83;
    float fTemp85 = std::cos(fTemp84);
    float fTemp86 = std::sin(fTemp84);
    fRec28[0] = fTemp83 * fTemp85 - fTemp86 * fRec28[1];
    fVec19[IOTA0 & 8191] = fTemp18 +
                           (fTemp83 * fTemp86 + fRec28[1] * fTemp85) *
                               std::pow(0.001f, fControl[19] / fTemp82) -
                           fTemp21;
    float fTemp87 =
        fVec19[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp80)))) &
               8191];
    fVec20[0] = fTemp87;
    float fTemp88 = (fTemp87 - fVec20[1]) * (fTemp79 + (-2.0f - fTemp81));
    fVec21[0] = fTemp88;
    fRec27[0] = fTemp87 +
                0.5f * (fVec21[1] - fTemp88) * (fTemp81 + (3.0f - fTemp79)) -
                fTemp88;
    float fTemp89 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp2 - fTemp44)));
    float fTemp90 = fControl[9] / fTemp89;
    float fTemp91 = fTemp90 + -2.499995f;
    float fTemp92 = std::floor(fTemp91);
    float fTemp93 = fRec15[0] * fTemp89;
    float fTemp94 =
        0.5f * fTemp25 * fRec29[2] + 0.25f * fTemp24 * (fRec29[1] + fRec29[3]);
    float fTemp95 = 3.1415927f * fRec17[0] * fTemp94;
    float fTemp96 = std::cos(fTemp95);
    float fTemp97 = std::sin(fTemp95);
    fRec30[0] = fTemp94 * fTemp96 - fTemp97 * fRec30[1];
    fVec22[IOTA0 & 8191] = fTemp18 +
                           (fTemp94 * fTemp97 + fRec30[1] * fTemp96) *
                               std::pow(0.001f, fControl[19] / fTemp93) -
                           fTemp21;
    float fTemp98 =
        fVec22[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp91)))) &
               8191];
    fVec23[0] = fTemp98;
    float fTemp99 = (fTemp98 - fVec23[1]) * (fTemp90 + (-2.0f - fTemp92));
    fVec24[0] = fTemp99;
    fRec29[0] = fTemp98 +
                0.5f * (fVec24[1] - fTemp99) * (fTemp92 + (3.0f - fTemp90)) -
                fTemp99;
    float fTemp100 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp2 - fTemp32)));
    float fTemp101 = fControl[9] / fTemp100;
    float fTemp102 = fTemp101 + -2.499995f;
    float fTemp103 = std::floor(fTemp102);
    float fTemp104 = fRec15[0] * fTemp100;
    float fTemp105 =
        0.5f * fTemp25 * fRec31[2] + 0.25f * fTemp24 * (fRec31[1] + fRec31[3]);
    float fTemp106 = 3.1415927f * fRec17[0] * fTemp105;
    float fTemp107 = std::cos(fTemp106);
    float fTemp108 = std::sin(fTemp106);
    fRec32[0] = fTemp105 * fTemp107 - fTemp108 * fRec32[1];
    fVec25[IOTA0 & 8191] = fTemp18 +
                           (fTemp105 * fTemp108 + fRec32[1] * fTemp107) *
                               std::pow(0.001f, fControl[19] / fTemp104) -
                           fTemp21;
    float fTemp109 =
        fVec25[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp102)))) &
               8191];
    fVec26[0] = fTemp109;
    float fTemp110 = (fTemp109 - fVec26[1]) * (fTemp101 + (-2.0f - fTemp103));
    fVec27[0] = fTemp110;
    fRec31[0] = fTemp109 +
                0.5f * (fVec27[1] - fTemp110) * (fTemp103 + (3.0f - fTemp101)) -
                fTemp110;
    float fTemp111 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f * (fTemp2 - fTemp3)));
    float fTemp112 = fControl[9] / fTemp111;
    float fTemp113 = fTemp112 + -2.499995f;
    float fTemp114 = std::floor(fTemp113);
    float fTemp115 = fRec15[0] * fTemp111;
    float fTemp116 =
        0.5f * fTemp25 * fRec33[2] + 0.25f * fTemp24 * (fRec33[1] + fRec33[3]);
    float fTemp117 = 3.1415927f * fRec17[0] * fTemp116;
    float fTemp118 = std::cos(fTemp117);
    float fTemp119 = std::sin(fTemp117);
    fRec34[0] = fTemp116 * fTemp118 - fTemp119 * fRec34[1];
    fVec28[IOTA0 & 8191] = fTemp18 +
                           (fTemp116 * fTemp119 + fRec34[1] * fTemp118) *
                               std::pow(0.001f, fControl[19] / fTemp115) -
                           fTemp21;
    float fTemp120 =
        fVec28[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp113)))) &
               8191];
    fVec29[0] = fTemp120;
    float fTemp121 = (fTemp120 - fVec29[1]) * (fTemp112 + (-2.0f - fTemp114));
    fVec30[0] = fTemp121;
    fRec33[0] = fTemp120 +
                0.5f * (fVec30[1] - fTemp121) * (fTemp114 + (3.0f - fTemp112)) -
                fTemp121;
    fVec31[0] = fRec33[0] + fRec31[0] + fRec29[0] + fRec27[0] + fRec25[0] +
                fRec23[0] + fRec21[0] + fRec19[0] + fRec2[0];
    fRec1[0] = fRec2[0] + fRec19[0] + fRec21[0] + fRec23[0] + fRec25[0] +
               fRec27[0] + fRec29[0] + fRec31[0] + fRec33[0] +
               0.995f * fRec1[1] - fVec31[1];
    float fTemp122 = 0.01f * fRec1[0];
    float fTemp123 = 1.5f * fRec5[0];
    fVec32[0] = fControl[23];
    float fTemp124 =
        ((fControl[23] != fVec32[1]) ? fControl[3] : fRec45[1] + -1.0f);
    fRec45[0] = fTemp124;
    fRec46[0] =
        ((fTemp124 <= 0.0f)
             ? fControl[23]
             : fRec46[1] + (1.0f - fRec46[1] - fControl[22]) / fTemp124);
    float fTemp125 = fControl[24] * tambura_faustpower2_f(fRec46[0]);
    float fTemp126 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f *
                                            (fTemp125 + fTemp3 + fTemp123)));
    float fTemp127 = fConst5 / fTemp126;
    float fTemp128 = fTemp127 + -2.499995f;
    float fTemp129 = std::floor(fTemp128);
    float fTemp130 = fControl[12] * float(fRec11[0] < 0.25f);
    float fTemp131 = fControl[22] + fTemp130;
    fVec33[0] = fTemp131;
    float fTemp132 = 1.0f - fTemp130 - fControl[22];
    fVec34[0] = fTemp132;
    float fTemp133 =
        ((fTemp132 != fVec34[1]) ? fControl[25] / fRec5[0] : fRec48[1] + -1.0f);
    fRec48[0] = fTemp133;
    fRec49[0] =
        ((fTemp133 <= 0.0f)
             ? fTemp132
             : fRec49[1] +
                   (1.0f - (fTemp130 + fRec49[1]) - fControl[22]) / fTemp133);
    float fTemp134 = fRec49[0] * fTemp131;
    float fTemp135 = fControl[26] / fRec5[0];
    int iTemp136 = std::fabs(fTemp135) < 1.1920929e-07f;
    float fTemp137 =
        ((fTemp134 > fRec47[1])
             ? ((iTemp136)
                    ? 0.0f
                    : std::exp(-(fConst6 / ((iTemp136) ? 1.0f : fTemp135))))
             : fConst7);
    fRec47[0] = fTemp134 * (1.0f - fTemp137) + fRec47[1] * fTemp137;
    float fTemp138 = fRec47[0] * fTemp9;
    fVec35[IOTA0 & 8191] = fTemp138;
    int iTemp139 = (fVec33[1] <= 0.0f) & (fTemp131 > 0.0f);
    fRec50[0] = fRec50[1] * float(1 - iTemp139) + fTemp20 * float(iTemp139);
    float fTemp140 =
        fVec35[(IOTA0 - int(std::min<float>(
                            4096.0f, std::max<float>(
                                         0.0f, fConst9 * ((fControl[16] +
                                                           0.2f * fRec50[0]) /
                                                          fRec5[0]))))) &
               8191];
    float fTemp141 = fRec15[0] * fTemp126;
    float fTemp142 = fConst11 * fRec51[1];
    float fTemp143 = fConst18 * fRec52[1];
    fRec54[0] = fControl[28] + fConst1 * fRec54[1];
    float fTemp144 = fRec54[0] * fRec42[1];
    fVec36[0] = fTemp144;
    fRec53[0] = -(fConst27 *
                  (fConst26 * fRec53[1] - fConst25 * (fTemp144 - fVec36[1])));
    fRec55[0] = -(fConst27 * (fConst26 * fRec55[1] - (fTemp144 + fVec36[1])));
    fRec52[0] = fRec55[0] + 1e-05f * fRec53[0] -
                fConst24 * (fConst23 * fRec52[2] + fTemp143);
    fRec51[0] =
        fConst24 * (fTemp143 + fConst30 * fRec52[0] + fConst29 * fRec52[2]) -
        fConst16 * (fConst15 * fRec51[2] + fTemp142);
    float fTemp145 = fTemp142 + fConst33 * fRec51[0] + fConst32 * fRec51[2];
    float fTemp146 = fConst11 * fRec56[1];
    float fTemp147 = fConst18 * fRec57[1];
    float fTemp148 = fRec54[0] * fRec40[1];
    fVec37[0] = fTemp148;
    fRec58[0] = -(fConst27 *
                  (fConst26 * fRec58[1] - fConst25 * (fTemp148 - fVec37[1])));
    fRec59[0] = -(fConst27 * (fConst26 * fRec59[1] - (fTemp148 + fVec37[1])));
    fRec57[0] = fRec59[0] + 1e-05f * fRec58[0] -
                fConst24 * (fConst23 * fRec57[2] + fTemp147);
    fRec56[0] =
        fConst24 * (fTemp147 + fConst30 * fRec57[0] + fConst29 * fRec57[2]) -
        fConst16 * (fConst15 * fRec56[2] + fTemp146);
    float fTemp149 = fTemp146 + fConst33 * fRec56[0] + fConst32 * fRec56[2];
    float fTemp150 = fConst11 * fRec60[1];
    float fTemp151 = fConst18 * fRec61[1];
    float fTemp152 = fRec54[0] * fRec38[1];
    fVec38[0] = fTemp152;
    fRec62[0] = -(fConst27 *
                  (fConst26 * fRec62[1] - fConst25 * (fTemp152 - fVec38[1])));
    fRec63[0] = -(fConst27 * (fConst26 * fRec63[1] - (fTemp152 + fVec38[1])));
    fRec61[0] = fRec63[0] + 1e-05f * fRec62[0] -
                fConst24 * (fConst23 * fRec61[2] + fTemp151);
    fRec60[0] =
        fConst24 * (fTemp151 + fConst30 * fRec61[0] + fConst29 * fRec61[2]) -
        fConst16 * (fConst15 * fRec60[2] + fTemp150);
    float fTemp153 = fTemp150 + fConst33 * fRec60[0] + fConst32 * fRec60[2];
    float fTemp154 = fConst16 * (fTemp153 + fTemp149 + fTemp145);
    float fTemp155 = fRec16[0] * (fTemp154 + fTemp138 - fTemp140);
    float fTemp156 = 1.0f - fTemp155;
    float fTemp157 = fTemp155 + 1.0f;
    float fTemp158 = 0.5f * fTemp157 * fRec44[2] +
                     0.25f * fTemp156 * (fRec44[1] + fRec44[3]);
    float fTemp159 = 3.1415927f * fRec17[0] * fTemp158;
    float fTemp160 = std::cos(fTemp159);
    float fTemp161 = std::sin(fTemp159);
    fRec64[0] = fTemp158 * fTemp160 - fTemp161 * fRec64[1];
    fVec39[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp158 * fTemp161 + fRec64[1] * fTemp160) *
                               std::pow(0.001f, fControl[27] / fTemp141) -
                           fTemp140;
    float fTemp162 =
        fVec39[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp128)))) &
               8191];
    fVec40[0] = fTemp162;
    float fTemp163 = (fTemp162 - fVec40[1]) * (fTemp127 + (-2.0f - fTemp129));
    fVec41[0] = fTemp163;
    fRec44[0] = fTemp162 +
                0.5f * (fVec41[1] - fTemp163) * (fTemp129 + (3.0f - fTemp127)) -
                fTemp163;
    float fTemp164 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f *
                                            (fTemp125 + fTemp32 + fTemp123)));
    float fTemp165 = fConst5 / fTemp164;
    float fTemp166 = fTemp165 + -2.499995f;
    float fTemp167 = std::floor(fTemp166);
    float fTemp168 = fRec15[0] * fTemp164;
    float fTemp169 = 0.5f * fTemp157 * fRec65[2] +
                     0.25f * fTemp156 * (fRec65[1] + fRec65[3]);
    float fTemp170 = 3.1415927f * fRec17[0] * fTemp169;
    float fTemp171 = std::cos(fTemp170);
    float fTemp172 = std::sin(fTemp170);
    fRec66[0] = fTemp169 * fTemp171 - fTemp172 * fRec66[1];
    fVec42[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp169 * fTemp172 + fRec66[1] * fTemp171) *
                               std::pow(0.001f, fControl[27] / fTemp168) -
                           fTemp140;
    float fTemp173 =
        fVec42[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp166)))) &
               8191];
    fVec43[0] = fTemp173;
    float fTemp174 = (fTemp173 - fVec43[1]) * (fTemp165 + (-2.0f - fTemp167));
    fVec44[0] = fTemp174;
    fRec65[0] = fTemp173 +
                0.5f * (fVec44[1] - fTemp174) * (fTemp167 + (3.0f - fTemp165)) -
                fTemp174;
    float fTemp175 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f *
                                            (fTemp125 + fTemp44 + fTemp123)));
    float fTemp176 = fConst5 / fTemp175;
    float fTemp177 = fTemp176 + -2.499995f;
    float fTemp178 = std::floor(fTemp177);
    float fTemp179 = fRec15[0] * fTemp175;
    float fTemp180 = 0.5f * fTemp157 * fRec67[2] +
                     0.25f * fTemp156 * (fRec67[1] + fRec67[3]);
    float fTemp181 = 3.1415927f * fRec17[0] * fTemp180;
    float fTemp182 = std::cos(fTemp181);
    float fTemp183 = std::sin(fTemp181);
    fRec68[0] = fTemp180 * fTemp182 - fTemp183 * fRec68[1];
    fVec45[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp180 * fTemp183 + fRec68[1] * fTemp182) *
                               std::pow(0.001f, fControl[27] / fTemp179) -
                           fTemp140;
    float fTemp184 =
        fVec45[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp177)))) &
               8191];
    fVec46[0] = fTemp184;
    float fTemp185 = (fTemp184 - fVec46[1]) * (fTemp176 + (-2.0f - fTemp178));
    fVec47[0] = fTemp185;
    fRec67[0] = fTemp184 +
                0.5f * (fVec47[1] - fTemp185) * (fTemp178 + (3.0f - fTemp176)) -
                fTemp185;
    float fTemp186 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f *
                                            (fTemp125 + fRec6[0] + fTemp123)));
    float fTemp187 = fConst5 / fTemp186;
    float fTemp188 = fTemp187 + -2.499995f;
    float fTemp189 = std::floor(fTemp188);
    float fTemp190 = fRec15[0] * fTemp186;
    float fTemp191 = 0.5f * fTemp157 * fRec69[2] +
                     0.25f * fTemp156 * (fRec69[1] + fRec69[3]);
    float fTemp192 = 3.1415927f * fRec17[0] * fTemp191;
    float fTemp193 = std::cos(fTemp192);
    float fTemp194 = std::sin(fTemp192);
    fRec70[0] = fTemp191 * fTemp193 - fTemp194 * fRec70[1];
    fVec48[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp191 * fTemp194 + fRec70[1] * fTemp193) *
                               std::pow(0.001f, fControl[27] / fTemp190) -
                           fTemp140;
    float fTemp195 =
        fVec48[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp188)))) &
               8191];
    fVec49[0] = fTemp195;
    float fTemp196 = (fTemp195 - fVec49[1]) * (fTemp187 + (-2.0f - fTemp189));
    fVec50[0] = fTemp196;
    fRec69[0] = fTemp195 +
                0.5f * (fVec50[1] - fTemp196) * (fTemp189 + (3.0f - fTemp187)) -
                fTemp196;
    float fTemp197 = fTemp123 + fTemp125;
    float fTemp198 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f * fTemp197));
    float fTemp199 = fConst5 / fTemp198;
    float fTemp200 = fTemp199 + -2.499995f;
    float fTemp201 = std::floor(fTemp200);
    float fTemp202 = fRec15[0] * fTemp198;
    float fTemp203 = 0.5f * fTemp157 * fRec71[2] +
                     0.25f * fTemp156 * (fRec71[1] + fRec71[3]);
    float fTemp204 = 3.1415927f * fRec17[0] * fTemp203;
    float fTemp205 = std::cos(fTemp204);
    float fTemp206 = std::sin(fTemp204);
    fRec72[0] = fTemp203 * fTemp205 - fTemp206 * fRec72[1];
    fVec51[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp203 * fTemp206 + fRec72[1] * fTemp205) *
                               std::pow(0.001f, fControl[27] / fTemp202) -
                           fTemp140;
    float fTemp207 =
        fVec51[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp200)))) &
               8191];
    fVec52[0] = fTemp207;
    float fTemp208 = (fTemp207 - fVec52[1]) * (fTemp199 + (-2.0f - fTemp201));
    fVec53[0] = fTemp208;
    fRec71[0] = fTemp207 +
                0.5f * (fVec53[1] - fTemp208) * (fTemp201 + (3.0f - fTemp199)) -
                fTemp208;
    float fTemp209 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp197 - fRec6[0])));
    float fTemp210 = fConst5 / fTemp209;
    float fTemp211 = fTemp210 + -2.499995f;
    float fTemp212 = std::floor(fTemp211);
    float fTemp213 = fRec15[0] * fTemp209;
    float fTemp214 = 0.5f * fTemp157 * fRec73[2] +
                     0.25f * fTemp156 * (fRec73[1] + fRec73[3]);
    float fTemp215 = 3.1415927f * fRec17[0] * fTemp214;
    float fTemp216 = std::cos(fTemp215);
    float fTemp217 = std::sin(fTemp215);
    fRec74[0] = fTemp214 * fTemp216 - fTemp217 * fRec74[1];
    fVec54[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp214 * fTemp217 + fRec74[1] * fTemp216) *
                               std::pow(0.001f, fControl[27] / fTemp213) -
                           fTemp140;
    float fTemp218 =
        fVec54[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp211)))) &
               8191];
    fVec55[0] = fTemp218;
    float fTemp219 = (fTemp218 - fVec55[1]) * (fTemp210 + (-2.0f - fTemp212));
    fVec56[0] = fTemp219;
    fRec73[0] = fTemp218 +
                0.5f * (fVec56[1] - fTemp219) * (fTemp212 + (3.0f - fTemp210)) -
                fTemp219;
    float fTemp220 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp197 - fTemp44)));
    float fTemp221 = fConst5 / fTemp220;
    float fTemp222 = fTemp221 + -2.499995f;
    float fTemp223 = std::floor(fTemp222);
    float fTemp224 = fRec15[0] * fTemp220;
    float fTemp225 = 0.5f * fTemp157 * fRec75[2] +
                     0.25f * fTemp156 * (fRec75[1] + fRec75[3]);
    float fTemp226 = 3.1415927f * fRec17[0] * fTemp225;
    float fTemp227 = std::cos(fTemp226);
    float fTemp228 = std::sin(fTemp226);
    fRec76[0] = fTemp225 * fTemp227 - fTemp228 * fRec76[1];
    fVec57[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp225 * fTemp228 + fRec76[1] * fTemp227) *
                               std::pow(0.001f, fControl[27] / fTemp224) -
                           fTemp140;
    float fTemp229 =
        fVec57[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp222)))) &
               8191];
    fVec58[0] = fTemp229;
    float fTemp230 = (fTemp229 - fVec58[1]) * (fTemp221 + (-2.0f - fTemp223));
    fVec59[0] = fTemp230;
    fRec75[0] = fTemp229 +
                0.5f * (fVec59[1] - fTemp230) * (fTemp223 + (3.0f - fTemp221)) -
                fTemp230;
    float fTemp231 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp197 - fTemp32)));
    float fTemp232 = fConst5 / fTemp231;
    float fTemp233 = fTemp232 + -2.499995f;
    float fTemp234 = std::floor(fTemp233);
    float fTemp235 = fRec15[0] * fTemp231;
    float fTemp236 = 0.5f * fTemp157 * fRec77[2] +
                     0.25f * fTemp156 * (fRec77[1] + fRec77[3]);
    float fTemp237 = 3.1415927f * fRec17[0] * fTemp236;
    float fTemp238 = std::cos(fTemp237);
    float fTemp239 = std::sin(fTemp237);
    fRec78[0] = fTemp236 * fTemp238 - fTemp239 * fRec78[1];
    fVec60[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp236 * fTemp239 + fRec78[1] * fTemp238) *
                               std::pow(0.001f, fControl[27] / fTemp235) -
                           fTemp140;
    float fTemp240 =
        fVec60[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp233)))) &
               8191];
    fVec61[0] = fTemp240;
    float fTemp241 = (fTemp240 - fVec61[1]) * (fTemp232 + (-2.0f - fTemp234));
    fVec62[0] = fTemp241;
    fRec77[0] = fTemp240 +
                0.5f * (fVec62[1] - fTemp241) * (fTemp234 + (3.0f - fTemp232)) -
                fTemp241;
    float fTemp242 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp197 - fTemp3)));
    float fTemp243 = fConst5 / fTemp242;
    float fTemp244 = fTemp243 + -2.499995f;
    float fTemp245 = std::floor(fTemp244);
    float fTemp246 = fRec15[0] * fTemp242;
    float fTemp247 = 0.5f * fTemp157 * fRec79[2] +
                     0.25f * fTemp156 * (fRec79[1] + fRec79[3]);
    float fTemp248 = 3.1415927f * fRec17[0] * fTemp247;
    float fTemp249 = std::cos(fTemp248);
    float fTemp250 = std::sin(fTemp248);
    fRec80[0] = fTemp247 * fTemp249 - fTemp250 * fRec80[1];
    fVec63[IOTA0 & 8191] = fTemp138 + fTemp154 +
                           (fTemp247 * fTemp250 + fRec80[1] * fTemp249) *
                               std::pow(0.001f, fControl[27] / fTemp246) -
                           fTemp140;
    float fTemp251 =
        fVec63[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp244)))) &
               8191];
    fVec64[0] = fTemp251;
    float fTemp252 = (fTemp251 - fVec64[1]) * (fTemp243 + (-2.0f - fTemp245));
    fVec65[0] = fTemp252;
    fRec79[0] = fTemp251 +
                0.5f * (fVec65[1] - fTemp252) * (fTemp245 + (3.0f - fTemp243)) -
                fTemp252;
    float fTemp253 = fRec79[0] + fRec77[0] + fRec75[0] + fRec73[0] + fRec71[0] +
                     fRec69[0] + fRec67[0] + fRec65[0] + fRec44[0];
    fVec66[0] = fTemp253;
    fRec43[0] = fTemp253 + 0.995f * fRec43[1] - fVec66[1];
    float fRec35 = 0.01f * fRec43[0];
    float fTemp254 = fControl[9] / fTemp126;
    float fTemp255 = fTemp254 + -2.499995f;
    float fTemp256 = std::floor(fTemp255);
    float fTemp257 = fRec16[0] * (fTemp138 - fTemp140);
    float fTemp258 = 1.0f - fTemp257;
    float fTemp259 = fTemp257 + 1.0f;
    float fTemp260 = 0.5f * fTemp259 * fRec82[2] +
                     0.25f * fTemp258 * (fRec82[1] + fRec82[3]);
    float fTemp261 = 3.1415927f * fRec17[0] * fTemp260;
    float fTemp262 = std::cos(fTemp261);
    float fTemp263 = std::sin(fTemp261);
    fRec83[0] = fTemp260 * fTemp262 - fTemp263 * fRec83[1];
    fVec67[IOTA0 & 8191] = fTemp138 +
                           (fTemp260 * fTemp263 + fRec83[1] * fTemp262) *
                               std::pow(0.001f, fControl[19] / fTemp141) -
                           fTemp140;
    float fTemp264 =
        fVec67[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp255)))) &
               8191];
    fVec68[0] = fTemp264;
    float fTemp265 = (fTemp264 - fVec68[1]) * (fTemp254 + (-2.0f - fTemp256));
    fVec69[0] = fTemp265;
    fRec82[0] = fTemp264 +
                0.5f * (fVec69[1] - fTemp265) * (fTemp256 + (3.0f - fTemp254)) -
                fTemp265;
    float fTemp266 = fControl[9] / fTemp164;
    float fTemp267 = fTemp266 + -2.499995f;
    float fTemp268 = std::floor(fTemp267);
    float fTemp269 = 0.5f * fTemp259 * fRec84[2] +
                     0.25f * fTemp258 * (fRec84[1] + fRec84[3]);
    float fTemp270 = 3.1415927f * fRec17[0] * fTemp269;
    float fTemp271 = std::cos(fTemp270);
    float fTemp272 = std::sin(fTemp270);
    fRec85[0] = fTemp269 * fTemp271 - fTemp272 * fRec85[1];
    fVec70[IOTA0 & 8191] = fTemp138 +
                           (fTemp269 * fTemp272 + fRec85[1] * fTemp271) *
                               std::pow(0.001f, fControl[19] / fTemp168) -
                           fTemp140;
    float fTemp273 =
        fVec70[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp267)))) &
               8191];
    fVec71[0] = fTemp273;
    float fTemp274 = (fTemp273 - fVec71[1]) * (fTemp266 + (-2.0f - fTemp268));
    fVec72[0] = fTemp274;
    fRec84[0] = fTemp273 +
                0.5f * (fVec72[1] - fTemp274) * (fTemp268 + (3.0f - fTemp266)) -
                fTemp274;
    float fTemp275 = fControl[9] / fTemp175;
    float fTemp276 = fTemp275 + -2.499995f;
    float fTemp277 = std::floor(fTemp276);
    float fTemp278 = 0.5f * fTemp259 * fRec86[2] +
                     0.25f * fTemp258 * (fRec86[1] + fRec86[3]);
    float fTemp279 = 3.1415927f * fRec17[0] * fTemp278;
    float fTemp280 = std::cos(fTemp279);
    float fTemp281 = std::sin(fTemp279);
    fRec87[0] = fTemp278 * fTemp280 - fTemp281 * fRec87[1];
    fVec73[IOTA0 & 8191] = fTemp138 +
                           (fTemp278 * fTemp281 + fRec87[1] * fTemp280) *
                               std::pow(0.001f, fControl[19] / fTemp179) -
                           fTemp140;
    float fTemp282 =
        fVec73[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp276)))) &
               8191];
    fVec74[0] = fTemp282;
    float fTemp283 = (fTemp282 - fVec74[1]) * (fTemp275 + (-2.0f - fTemp277));
    fVec75[0] = fTemp283;
    fRec86[0] = fTemp282 +
                0.5f * (fVec75[1] - fTemp283) * (fTemp277 + (3.0f - fTemp275)) -
                fTemp283;
    float fTemp284 = fControl[9] / fTemp186;
    float fTemp285 = fTemp284 + -2.499995f;
    float fTemp286 = std::floor(fTemp285);
    float fTemp287 = 0.5f * fTemp259 * fRec88[2] +
                     0.25f * fTemp258 * (fRec88[1] + fRec88[3]);
    float fTemp288 = 3.1415927f * fRec17[0] * fTemp287;
    float fTemp289 = std::cos(fTemp288);
    float fTemp290 = std::sin(fTemp288);
    fRec89[0] = fTemp287 * fTemp289 - fTemp290 * fRec89[1];
    fVec76[IOTA0 & 8191] = fTemp138 +
                           (fTemp287 * fTemp290 + fRec89[1] * fTemp289) *
                               std::pow(0.001f, fControl[19] / fTemp190) -
                           fTemp140;
    float fTemp291 =
        fVec76[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp285)))) &
               8191];
    fVec77[0] = fTemp291;
    float fTemp292 = (fTemp291 - fVec77[1]) * (fTemp284 + (-2.0f - fTemp286));
    fVec78[0] = fTemp292;
    fRec88[0] = fTemp291 +
                0.5f * (fVec78[1] - fTemp292) * (fTemp286 + (3.0f - fTemp284)) -
                fTemp292;
    float fTemp293 = fControl[9] / fTemp198;
    float fTemp294 = fTemp293 + -2.499995f;
    float fTemp295 = std::floor(fTemp294);
    float fTemp296 = 0.5f * fTemp259 * fRec90[2] +
                     0.25f * fTemp258 * (fRec90[1] + fRec90[3]);
    float fTemp297 = 3.1415927f * fRec17[0] * fTemp296;
    float fTemp298 = std::cos(fTemp297);
    float fTemp299 = std::sin(fTemp297);
    fRec91[0] = fTemp296 * fTemp298 - fTemp299 * fRec91[1];
    fVec79[IOTA0 & 8191] = fTemp138 +
                           (fTemp296 * fTemp299 + fRec91[1] * fTemp298) *
                               std::pow(0.001f, fControl[19] / fTemp202) -
                           fTemp140;
    float fTemp300 =
        fVec79[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp294)))) &
               8191];
    fVec80[0] = fTemp300;
    float fTemp301 = (fTemp300 - fVec80[1]) * (fTemp293 + (-2.0f - fTemp295));
    fVec81[0] = fTemp301;
    fRec90[0] = fTemp300 +
                0.5f * (fVec81[1] - fTemp301) * (fTemp295 + (3.0f - fTemp293)) -
                fTemp301;
    float fTemp302 = fControl[9] / fTemp209;
    float fTemp303 = fTemp302 + -2.499995f;
    float fTemp304 = std::floor(fTemp303);
    float fTemp305 = 0.5f * fTemp259 * fRec92[2] +
                     0.25f * fTemp258 * (fRec92[1] + fRec92[3]);
    float fTemp306 = 3.1415927f * fRec17[0] * fTemp305;
    float fTemp307 = std::cos(fTemp306);
    float fTemp308 = std::sin(fTemp306);
    fRec93[0] = fTemp305 * fTemp307 - fTemp308 * fRec93[1];
    fVec82[IOTA0 & 8191] = fTemp138 +
                           (fTemp305 * fTemp308 + fRec93[1] * fTemp307) *
                               std::pow(0.001f, fControl[19] / fTemp213) -
                           fTemp140;
    float fTemp309 =
        fVec82[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp303)))) &
               8191];
    fVec83[0] = fTemp309;
    float fTemp310 = (fTemp309 - fVec83[1]) * (fTemp302 + (-2.0f - fTemp304));
    fVec84[0] = fTemp310;
    fRec92[0] = fTemp309 +
                0.5f * (fVec84[1] - fTemp310) * (fTemp304 + (3.0f - fTemp302)) -
                fTemp310;
    float fTemp311 = fControl[9] / fTemp220;
    float fTemp312 = fTemp311 + -2.499995f;
    float fTemp313 = std::floor(fTemp312);
    float fTemp314 = 0.5f * fTemp259 * fRec94[2] +
                     0.25f * fTemp258 * (fRec94[1] + fRec94[3]);
    float fTemp315 = 3.1415927f * fRec17[0] * fTemp314;
    float fTemp316 = std::cos(fTemp315);
    float fTemp317 = std::sin(fTemp315);
    fRec95[0] = fTemp314 * fTemp316 - fTemp317 * fRec95[1];
    fVec85[IOTA0 & 8191] = fTemp138 +
                           (fTemp314 * fTemp317 + fRec95[1] * fTemp316) *
                               std::pow(0.001f, fControl[19] / fTemp224) -
                           fTemp140;
    float fTemp318 =
        fVec85[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp312)))) &
               8191];
    fVec86[0] = fTemp318;
    float fTemp319 = (fTemp318 - fVec86[1]) * (fTemp311 + (-2.0f - fTemp313));
    fVec87[0] = fTemp319;
    fRec94[0] = fTemp318 +
                0.5f * (fVec87[1] - fTemp319) * (fTemp313 + (3.0f - fTemp311)) -
                fTemp319;
    float fTemp320 = fControl[9] / fTemp231;
    float fTemp321 = fTemp320 + -2.499995f;
    float fTemp322 = std::floor(fTemp321);
    float fTemp323 = 0.5f * fTemp259 * fRec96[2] +
                     0.25f * fTemp258 * (fRec96[1] + fRec96[3]);
    float fTemp324 = 3.1415927f * fRec17[0] * fTemp323;
    float fTemp325 = std::cos(fTemp324);
    float fTemp326 = std::sin(fTemp324);
    fRec97[0] = fTemp323 * fTemp325 - fTemp326 * fRec97[1];
    fVec88[IOTA0 & 8191] = fTemp138 +
                           (fTemp323 * fTemp326 + fRec97[1] * fTemp325) *
                               std::pow(0.001f, fControl[19] / fTemp235) -
                           fTemp140;
    float fTemp327 =
        fVec88[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp321)))) &
               8191];
    fVec89[0] = fTemp327;
    float fTemp328 = (fTemp327 - fVec89[1]) * (fTemp320 + (-2.0f - fTemp322));
    fVec90[0] = fTemp328;
    fRec96[0] = fTemp327 +
                0.5f * (fVec90[1] - fTemp328) * (fTemp322 + (3.0f - fTemp320)) -
                fTemp328;
    float fTemp329 = fControl[9] / fTemp242;
    float fTemp330 = fTemp329 + -2.499995f;
    float fTemp331 = std::floor(fTemp330);
    float fTemp332 = 0.5f * fTemp259 * fRec98[2] +
                     0.25f * fTemp258 * (fRec98[1] + fRec98[3]);
    float fTemp333 = 3.1415927f * fRec17[0] * fTemp332;
    float fTemp334 = std::cos(fTemp333);
    float fTemp335 = std::sin(fTemp333);
    fRec99[0] = fTemp332 * fTemp334 - fTemp335 * fRec99[1];
    fVec91[IOTA0 & 8191] = fTemp138 +
                           (fTemp332 * fTemp335 + fRec99[1] * fTemp334) *
                               std::pow(0.001f, fControl[19] / fTemp246) -
                           fTemp140;
    float fTemp336 =
        fVec91[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp330)))) &
               8191];
    fVec92[0] = fTemp336;
    float fTemp337 = (fTemp336 - fVec92[1]) * (fTemp329 + (-2.0f - fTemp331));
    fVec93[0] = fTemp337;
    fRec98[0] = fTemp336 +
                0.5f * (fVec93[1] - fTemp337) * (fTemp331 + (3.0f - fTemp329)) -
                fTemp337;
    fVec94[0] = fRec98[0] + fRec96[0] + fRec94[0] + fRec92[0] + fRec90[0] +
                fRec88[0] + fRec86[0] + fRec84[0] + fRec82[0];
    fRec81[0] = fRec82[0] + fRec84[0] + fRec86[0] + fRec88[0] + fRec90[0] +
                fRec92[0] + fRec94[0] + fRec96[0] + fRec98[0] +
                0.995f * fRec81[1] - fVec94[1];
    float fTemp338 = 0.01f * fRec81[0];
    fRec36[0] = fTemp338;
    fVec95[0] = fControl[30];
    float fTemp339 =
        ((fControl[30] != fVec95[1]) ? fControl[3] : fRec102[1] + -1.0f);
    fRec102[0] = fTemp339;
    fRec103[0] =
        ((fTemp339 <= 0.0f)
             ? fControl[30]
             : fRec103[1] + (1.0f - fRec103[1] - fControl[29]) / fTemp339);
    float fTemp340 = fControl[31] * tambura_faustpower2_f(fRec103[0]);
    float fTemp341 = 2.0f * fRec5[0] + fTemp340;
    float fTemp342 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp3 + fTemp341)));
    float fTemp343 = fConst5 / fTemp342;
    float fTemp344 = fTemp343 + -2.499995f;
    float fTemp345 = std::floor(fTemp344);
    float fTemp346 =
        fControl[12] * float((fRec11[0] > 0.25f) & (fRec11[0] < 0.5f));
    float fTemp347 = fControl[29] + fTemp346;
    fVec96[0] = fTemp347;
    float fTemp348 = 1.0f - fTemp346 - fControl[29];
    fVec97[0] = fTemp348;
    float fTemp349 = ((fTemp348 != fVec97[1]) ? fControl[32] / fRec5[0]
                                              : fRec105[1] + -1.0f);
    fRec105[0] = fTemp349;
    fRec106[0] =
        ((fTemp349 <= 0.0f)
             ? fTemp348
             : fRec106[1] +
                   (1.0f - (fTemp346 + fRec106[1]) - fControl[29]) / fTemp349);
    float fTemp350 = fRec106[0] * fTemp347;
    float fTemp351 = fControl[33] / fRec5[0];
    int iTemp352 = std::fabs(fTemp351) < 1.1920929e-07f;
    float fTemp353 =
        ((fTemp350 > fRec104[1])
             ? ((iTemp352)
                    ? 0.0f
                    : std::exp(-(fConst6 / ((iTemp352) ? 1.0f : fTemp351))))
             : fConst7);
    fRec104[0] = fTemp350 * (1.0f - fTemp353) + fRec104[1] * fTemp353;
    float fTemp354 = fRec104[0] * fTemp9;
    fVec98[IOTA0 & 8191] = fTemp354;
    int iTemp355 = (fVec96[1] <= 0.0f) & (fTemp347 > 0.0f);
    fRec107[0] = fRec107[1] * float(1 - iTemp355) + fTemp20 * float(iTemp355);
    float fTemp356 =
        fVec98[(IOTA0 - int(std::min<float>(
                            4096.0f, std::max<float>(
                                         0.0f, fConst34 * ((fControl[16] +
                                                            0.2f * fRec107[0]) /
                                                           fRec5[0]))))) &
               8191];
    float fTemp357 = fRec15[0] * fTemp342;
    float fTemp358 = fConst11 * fRec108[1];
    float fTemp359 = fConst18 * fRec109[1];
    float fTemp360 = fRec54[0] * fRec36[1];
    fVec99[0] = fTemp360;
    fRec110[0] = -(fConst27 *
                   (fConst26 * fRec110[1] - fConst25 * (fTemp360 - fVec99[1])));
    fRec111[0] = -(fConst27 * (fConst26 * fRec111[1] - (fTemp360 + fVec99[1])));
    fRec109[0] = fRec111[0] + 1e-05f * fRec110[0] -
                 fConst24 * (fConst23 * fRec109[2] + fTemp359);
    fRec108[0] =
        fConst24 * (fTemp359 + fConst30 * fRec109[0] + fConst29 * fRec109[2]) -
        fConst16 * (fConst15 * fRec108[2] + fTemp358);
    float fTemp361 = fTemp358 + fConst33 * fRec108[0] + fConst32 * fRec108[2];
    float fTemp362 = fConst16 * (fTemp145 + fTemp149 + fTemp361) + fTemp354;
    float fTemp363 = fRec16[0] * (fTemp362 - fTemp356);
    float fTemp364 = 1.0f - fTemp363;
    float fTemp365 = fTemp363 + 1.0f;
    float fTemp366 = 0.5f * fTemp365 * fRec101[2] +
                     0.25f * fTemp364 * (fRec101[1] + fRec101[3]);
    float fTemp367 = 3.1415927f * fRec17[0] * fTemp366;
    float fTemp368 = std::cos(fTemp367);
    float fTemp369 = std::sin(fTemp367);
    fRec112[0] = fTemp366 * fTemp368 - fTemp369 * fRec112[1];
    fVec100[IOTA0 & 8191] = fTemp362 +
                            (fTemp366 * fTemp369 + fRec112[1] * fTemp368) *
                                std::pow(0.001f, fControl[27] / fTemp357) -
                            fTemp356;
    float fTemp370 =
        fVec100[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp344)))) &
                8191];
    fVec101[0] = fTemp370;
    float fTemp371 = (fTemp370 - fVec101[1]) * (fTemp343 + (-2.0f - fTemp345));
    fVec102[0] = fTemp371;
    fRec101[0] =
        fTemp370 +
        0.5f * (fVec102[1] - fTemp371) * (fTemp345 + (3.0f - fTemp343)) -
        fTemp371;
    float fTemp372 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp32 + fTemp341)));
    float fTemp373 = fConst5 / fTemp372;
    float fTemp374 = fTemp373 + -2.499995f;
    float fTemp375 = std::floor(fTemp374);
    float fTemp376 = fRec15[0] * fTemp372;
    float fTemp377 = 0.5f * fTemp365 * fRec113[2] +
                     0.25f * fTemp364 * (fRec113[1] + fRec113[3]);
    float fTemp378 = 3.1415927f * fRec17[0] * fTemp377;
    float fTemp379 = std::cos(fTemp378);
    float fTemp380 = std::sin(fTemp378);
    fRec114[0] = fTemp377 * fTemp379 - fTemp380 * fRec114[1];
    fVec103[IOTA0 & 8191] = fTemp362 +
                            (fTemp377 * fTemp380 + fRec114[1] * fTemp379) *
                                std::pow(0.001f, fControl[27] / fTemp376) -
                            fTemp356;
    float fTemp381 =
        fVec103[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp374)))) &
                8191];
    fVec104[0] = fTemp381;
    float fTemp382 = (fTemp381 - fVec104[1]) * (fTemp373 + (-2.0f - fTemp375));
    fVec105[0] = fTemp382;
    fRec113[0] =
        fTemp381 +
        0.5f * (fVec105[1] - fTemp382) * (fTemp375 + (3.0f - fTemp373)) -
        fTemp382;
    float fTemp383 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f *
                                   (fTemp340 + 2.0f * (fRec5[0] + fRec6[0]))));
    float fTemp384 = fConst5 / fTemp383;
    float fTemp385 = fTemp384 + -2.499995f;
    float fTemp386 = std::floor(fTemp385);
    float fTemp387 = fRec15[0] * fTemp383;
    float fTemp388 = 0.5f * fTemp365 * fRec115[2] +
                     0.25f * fTemp364 * (fRec115[1] + fRec115[3]);
    float fTemp389 = 3.1415927f * fRec17[0] * fTemp388;
    float fTemp390 = std::cos(fTemp389);
    float fTemp391 = std::sin(fTemp389);
    fRec116[0] = fTemp388 * fTemp390 - fTemp391 * fRec116[1];
    fVec106[IOTA0 & 8191] = fTemp362 +
                            (fTemp388 * fTemp391 + fRec116[1] * fTemp390) *
                                std::pow(0.001f, fControl[27] / fTemp387) -
                            fTemp356;
    float fTemp392 =
        fVec106[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp385)))) &
                8191];
    fVec107[0] = fTemp392;
    float fTemp393 = (fTemp392 - fVec107[1]) * (fTemp384 + (-2.0f - fTemp386));
    fVec108[0] = fTemp393;
    fRec115[0] =
        fTemp392 +
        0.5f * (fVec108[1] - fTemp393) * (fTemp386 + (3.0f - fTemp384)) -
        fTemp393;
    float fTemp394 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fRec6[0] + fTemp341)));
    float fTemp395 = fConst5 / fTemp394;
    float fTemp396 = fTemp395 + -2.499995f;
    float fTemp397 = std::floor(fTemp396);
    float fTemp398 = fRec15[0] * fTemp394;
    float fTemp399 = 0.5f * fTemp365 * fRec117[2] +
                     0.25f * fTemp364 * (fRec117[1] + fRec117[3]);
    float fTemp400 = 3.1415927f * fRec17[0] * fTemp399;
    float fTemp401 = std::cos(fTemp400);
    float fTemp402 = std::sin(fTemp400);
    fRec118[0] = fTemp399 * fTemp401 - fTemp402 * fRec118[1];
    fVec109[IOTA0 & 8191] = fTemp362 +
                            (fTemp399 * fTemp402 + fRec118[1] * fTemp401) *
                                std::pow(0.001f, fControl[27] / fTemp398) -
                            fTemp356;
    float fTemp403 =
        fVec109[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp396)))) &
                8191];
    fVec110[0] = fTemp403;
    float fTemp404 = (fTemp403 - fVec110[1]) * (fTemp395 + (-2.0f - fTemp397));
    fVec111[0] = fTemp404;
    fRec117[0] =
        fTemp403 +
        0.5f * (fVec111[1] - fTemp404) * (fTemp397 + (3.0f - fTemp395)) -
        fTemp404;
    float fTemp405 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f * fTemp341));
    float fTemp406 = fConst5 / fTemp405;
    float fTemp407 = fTemp406 + -2.499995f;
    float fTemp408 = std::floor(fTemp407);
    float fTemp409 = fRec15[0] * fTemp405;
    float fTemp410 = 0.5f * fTemp365 * fRec119[2] +
                     0.25f * fTemp364 * (fRec119[1] + fRec119[3]);
    float fTemp411 = 3.1415927f * fRec17[0] * fTemp410;
    float fTemp412 = std::cos(fTemp411);
    float fTemp413 = std::sin(fTemp411);
    fRec120[0] = fTemp410 * fTemp412 - fTemp413 * fRec120[1];
    fVec112[IOTA0 & 8191] = fTemp362 +
                            (fTemp410 * fTemp413 + fRec120[1] * fTemp412) *
                                std::pow(0.001f, fControl[27] / fTemp409) -
                            fTemp356;
    float fTemp414 =
        fVec112[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp407)))) &
                8191];
    fVec113[0] = fTemp414;
    float fTemp415 = (fTemp414 - fVec113[1]) * (fTemp406 + (-2.0f - fTemp408));
    fVec114[0] = fTemp415;
    fRec119[0] =
        fTemp414 +
        0.5f * (fVec114[1] - fTemp415) * (fTemp408 + (3.0f - fTemp406)) -
        fTemp415;
    float fTemp416 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp341 - fRec6[0])));
    float fTemp417 = fConst5 / fTemp416;
    float fTemp418 = fTemp417 + -2.499995f;
    float fTemp419 = std::floor(fTemp418);
    float fTemp420 = fRec15[0] * fTemp416;
    float fTemp421 = 0.5f * fTemp365 * fRec121[2] +
                     0.25f * fTemp364 * (fRec121[1] + fRec121[3]);
    float fTemp422 = 3.1415927f * fRec17[0] * fTemp421;
    float fTemp423 = std::cos(fTemp422);
    float fTemp424 = std::sin(fTemp422);
    fRec122[0] = fTemp421 * fTemp423 - fTemp424 * fRec122[1];
    fVec115[IOTA0 & 8191] = fTemp362 +
                            (fTemp421 * fTemp424 + fRec122[1] * fTemp423) *
                                std::pow(0.001f, fControl[27] / fTemp420) -
                            fTemp356;
    float fTemp425 =
        fVec115[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp418)))) &
                8191];
    fVec116[0] = fTemp425;
    float fTemp426 = (fTemp425 - fVec116[1]) * (fTemp417 + (-2.0f - fTemp419));
    fVec117[0] = fTemp426;
    fRec121[0] =
        fTemp425 +
        0.5f * (fVec117[1] - fTemp426) * (fTemp419 + (3.0f - fTemp417)) -
        fTemp426;
    float fTemp427 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f *
                                   (fTemp340 + 2.0f * (fRec5[0] - fRec6[0]))));
    float fTemp428 = fConst5 / fTemp427;
    float fTemp429 = fTemp428 + -2.499995f;
    float fTemp430 = std::floor(fTemp429);
    float fTemp431 = fRec15[0] * fTemp427;
    float fTemp432 = 0.5f * fTemp365 * fRec123[2] +
                     0.25f * fTemp364 * (fRec123[1] + fRec123[3]);
    float fTemp433 = 3.1415927f * fRec17[0] * fTemp432;
    float fTemp434 = std::cos(fTemp433);
    float fTemp435 = std::sin(fTemp433);
    fRec124[0] = fTemp432 * fTemp434 - fTemp435 * fRec124[1];
    fVec118[IOTA0 & 8191] = fTemp362 +
                            (fTemp432 * fTemp435 + fRec124[1] * fTemp434) *
                                std::pow(0.001f, fControl[27] / fTemp431) -
                            fTemp356;
    float fTemp436 =
        fVec118[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp429)))) &
                8191];
    fVec119[0] = fTemp436;
    float fTemp437 = (fTemp436 - fVec119[1]) * (fTemp428 + (-2.0f - fTemp430));
    fVec120[0] = fTemp437;
    fRec123[0] =
        fTemp436 +
        0.5f * (fVec120[1] - fTemp437) * (fTemp430 + (3.0f - fTemp428)) -
        fTemp437;
    float fTemp438 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp341 - fTemp32)));
    float fTemp439 = fConst5 / fTemp438;
    float fTemp440 = fTemp439 + -2.499995f;
    float fTemp441 = std::floor(fTemp440);
    float fTemp442 = fRec15[0] * fTemp438;
    float fTemp443 = 0.5f * fTemp365 * fRec125[2] +
                     0.25f * fTemp364 * (fRec125[1] + fRec125[3]);
    float fTemp444 = 3.1415927f * fRec17[0] * fTemp443;
    float fTemp445 = std::cos(fTemp444);
    float fTemp446 = std::sin(fTemp444);
    fRec126[0] = fTemp443 * fTemp445 - fTemp446 * fRec126[1];
    fVec121[IOTA0 & 8191] = fTemp362 +
                            (fTemp443 * fTemp446 + fRec126[1] * fTemp445) *
                                std::pow(0.001f, fControl[27] / fTemp442) -
                            fTemp356;
    float fTemp447 =
        fVec121[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp440)))) &
                8191];
    fVec122[0] = fTemp447;
    float fTemp448 = (fTemp447 - fVec122[1]) * (fTemp439 + (-2.0f - fTemp441));
    fVec123[0] = fTemp448;
    fRec125[0] =
        fTemp447 +
        0.5f * (fVec123[1] - fTemp448) * (fTemp441 + (3.0f - fTemp439)) -
        fTemp448;
    float fTemp449 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp341 - fTemp3)));
    float fTemp450 = fConst5 / fTemp449;
    float fTemp451 = fTemp450 + -2.499995f;
    float fTemp452 = std::floor(fTemp451);
    float fTemp453 = fRec15[0] * fTemp449;
    float fTemp454 = 0.5f * fTemp365 * fRec127[2] +
                     0.25f * fTemp364 * (fRec127[1] + fRec127[3]);
    float fTemp455 = 3.1415927f * fRec17[0] * fTemp454;
    float fTemp456 = std::cos(fTemp455);
    float fTemp457 = std::sin(fTemp455);
    fRec128[0] = fTemp454 * fTemp456 - fTemp457 * fRec128[1];
    fVec124[IOTA0 & 8191] = fTemp362 +
                            (fTemp454 * fTemp457 + fRec128[1] * fTemp456) *
                                std::pow(0.001f, fControl[27] / fTemp453) -
                            fTemp356;
    float fTemp458 =
        fVec124[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp451)))) &
                8191];
    fVec125[0] = fTemp458;
    float fTemp459 = (fTemp458 - fVec125[1]) * (fTemp450 + (-2.0f - fTemp452));
    fVec126[0] = fTemp459;
    fRec127[0] =
        fTemp458 +
        0.5f * (fVec126[1] - fTemp459) * (fTemp452 + (3.0f - fTemp450)) -
        fTemp459;
    fVec127[0] = fRec127[0] + fRec125[0] + fRec123[0] + fRec121[0] +
                 fRec119[0] + fRec117[0] + fRec115[0] + fRec113[0] + fRec101[0];
    fRec100[0] = fRec101[0] + fRec113[0] + fRec115[0] + fRec117[0] +
                 fRec119[0] + fRec121[0] + fRec123[0] + fRec125[0] +
                 fRec127[0] + 0.995f * fRec100[1] - fVec127[1];
    float fRec37 = 0.01f * fRec100[0];
    float fTemp460 = fControl[9] / fTemp342;
    float fTemp461 = fTemp460 + -2.499995f;
    float fTemp462 = std::floor(fTemp461);
    float fTemp463 = fRec16[0] * (fTemp354 - fTemp356);
    float fTemp464 = 1.0f - fTemp463;
    float fTemp465 = fTemp463 + 1.0f;
    float fTemp466 = 0.5f * fTemp465 * fRec130[2] +
                     0.25f * fTemp464 * (fRec130[1] + fRec130[3]);
    float fTemp467 = 3.1415927f * fRec17[0] * fTemp466;
    float fTemp468 = std::cos(fTemp467);
    float fTemp469 = std::sin(fTemp467);
    fRec131[0] = fTemp466 * fTemp468 - fTemp469 * fRec131[1];
    fVec128[IOTA0 & 8191] = fTemp354 +
                            (fTemp466 * fTemp469 + fRec131[1] * fTemp468) *
                                std::pow(0.001f, fControl[19] / fTemp357) -
                            fTemp356;
    float fTemp470 =
        fVec128[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp461)))) &
                8191];
    fVec129[0] = fTemp470;
    float fTemp471 = (fTemp470 - fVec129[1]) * (fTemp460 + (-2.0f - fTemp462));
    fVec130[0] = fTemp471;
    fRec130[0] =
        fTemp470 +
        0.5f * (fVec130[1] - fTemp471) * (fTemp462 + (3.0f - fTemp460)) -
        fTemp471;
    float fTemp472 = fControl[9] / fTemp372;
    float fTemp473 = fTemp472 + -2.499995f;
    float fTemp474 = std::floor(fTemp473);
    float fTemp475 = 0.5f * fTemp465 * fRec132[2] +
                     0.25f * fTemp464 * (fRec132[1] + fRec132[3]);
    float fTemp476 = 3.1415927f * fRec17[0] * fTemp475;
    float fTemp477 = std::cos(fTemp476);
    float fTemp478 = std::sin(fTemp476);
    fRec133[0] = fTemp475 * fTemp477 - fTemp478 * fRec133[1];
    fVec131[IOTA0 & 8191] = fTemp354 +
                            (fTemp475 * fTemp478 + fRec133[1] * fTemp477) *
                                std::pow(0.001f, fControl[19] / fTemp376) -
                            fTemp356;
    float fTemp479 =
        fVec131[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp473)))) &
                8191];
    fVec132[0] = fTemp479;
    float fTemp480 = (fTemp479 - fVec132[1]) * (fTemp472 + (-2.0f - fTemp474));
    fVec133[0] = fTemp480;
    fRec132[0] =
        fTemp479 +
        0.5f * (fVec133[1] - fTemp480) * (fTemp474 + (3.0f - fTemp472)) -
        fTemp480;
    float fTemp481 = fControl[9] / fTemp383;
    float fTemp482 = fTemp481 + -2.499995f;
    float fTemp483 = std::floor(fTemp482);
    float fTemp484 = 0.5f * fTemp465 * fRec134[2] +
                     0.25f * fTemp464 * (fRec134[1] + fRec134[3]);
    float fTemp485 = 3.1415927f * fRec17[0] * fTemp484;
    float fTemp486 = std::cos(fTemp485);
    float fTemp487 = std::sin(fTemp485);
    fRec135[0] = fTemp484 * fTemp486 - fTemp487 * fRec135[1];
    fVec134[IOTA0 & 8191] = fTemp354 +
                            (fTemp484 * fTemp487 + fRec135[1] * fTemp486) *
                                std::pow(0.001f, fControl[19] / fTemp387) -
                            fTemp356;
    float fTemp488 =
        fVec134[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp482)))) &
                8191];
    fVec135[0] = fTemp488;
    float fTemp489 = (fTemp488 - fVec135[1]) * (fTemp481 + (-2.0f - fTemp483));
    fVec136[0] = fTemp489;
    fRec134[0] =
        fTemp488 +
        0.5f * (fVec136[1] - fTemp489) * (fTemp483 + (3.0f - fTemp481)) -
        fTemp489;
    float fTemp490 = fControl[9] / fTemp394;
    float fTemp491 = fTemp490 + -2.499995f;
    float fTemp492 = std::floor(fTemp491);
    float fTemp493 = 0.5f * fTemp465 * fRec136[2] +
                     0.25f * fTemp464 * (fRec136[1] + fRec136[3]);
    float fTemp494 = 3.1415927f * fRec17[0] * fTemp493;
    float fTemp495 = std::cos(fTemp494);
    float fTemp496 = std::sin(fTemp494);
    fRec137[0] = fTemp493 * fTemp495 - fTemp496 * fRec137[1];
    fVec137[IOTA0 & 8191] = fTemp354 +
                            (fTemp493 * fTemp496 + fRec137[1] * fTemp495) *
                                std::pow(0.001f, fControl[19] / fTemp398) -
                            fTemp356;
    float fTemp497 =
        fVec137[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp491)))) &
                8191];
    fVec138[0] = fTemp497;
    float fTemp498 = (fTemp497 - fVec138[1]) * (fTemp490 + (-2.0f - fTemp492));
    fVec139[0] = fTemp498;
    fRec136[0] =
        fTemp497 +
        0.5f * (fVec139[1] - fTemp498) * (fTemp492 + (3.0f - fTemp490)) -
        fTemp498;
    float fTemp499 = fControl[9] / fTemp405;
    float fTemp500 = fTemp499 + -2.499995f;
    float fTemp501 = std::floor(fTemp500);
    float fTemp502 = 0.5f * fTemp465 * fRec138[2] +
                     0.25f * fTemp464 * (fRec138[1] + fRec138[3]);
    float fTemp503 = 3.1415927f * fRec17[0] * fTemp502;
    float fTemp504 = std::cos(fTemp503);
    float fTemp505 = std::sin(fTemp503);
    fRec139[0] = fTemp502 * fTemp504 - fTemp505 * fRec139[1];
    fVec140[IOTA0 & 8191] = fTemp354 +
                            (fTemp502 * fTemp505 + fRec139[1] * fTemp504) *
                                std::pow(0.001f, fControl[19] / fTemp409) -
                            fTemp356;
    float fTemp506 =
        fVec140[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp500)))) &
                8191];
    fVec141[0] = fTemp506;
    float fTemp507 = (fTemp506 - fVec141[1]) * (fTemp499 + (-2.0f - fTemp501));
    fVec142[0] = fTemp507;
    fRec138[0] =
        fTemp506 +
        0.5f * (fVec142[1] - fTemp507) * (fTemp501 + (3.0f - fTemp499)) -
        fTemp507;
    float fTemp508 = fControl[9] / fTemp416;
    float fTemp509 = fTemp508 + -2.499995f;
    float fTemp510 = std::floor(fTemp509);
    float fTemp511 = 0.5f * fTemp465 * fRec140[2] +
                     0.25f * fTemp464 * (fRec140[1] + fRec140[3]);
    float fTemp512 = 3.1415927f * fRec17[0] * fTemp511;
    float fTemp513 = std::cos(fTemp512);
    float fTemp514 = std::sin(fTemp512);
    fRec141[0] = fTemp511 * fTemp513 - fTemp514 * fRec141[1];
    fVec143[IOTA0 & 8191] = fTemp354 +
                            (fTemp511 * fTemp514 + fRec141[1] * fTemp513) *
                                std::pow(0.001f, fControl[19] / fTemp420) -
                            fTemp356;
    float fTemp515 =
        fVec143[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp509)))) &
                8191];
    fVec144[0] = fTemp515;
    float fTemp516 = (fTemp515 - fVec144[1]) * (fTemp508 + (-2.0f - fTemp510));
    fVec145[0] = fTemp516;
    fRec140[0] =
        fTemp515 +
        0.5f * (fVec145[1] - fTemp516) * (fTemp510 + (3.0f - fTemp508)) -
        fTemp516;
    float fTemp517 = fControl[9] / fTemp427;
    float fTemp518 = fTemp517 + -2.499995f;
    float fTemp519 = std::floor(fTemp518);
    float fTemp520 = 0.5f * fTemp465 * fRec142[2] +
                     0.25f * fTemp464 * (fRec142[1] + fRec142[3]);
    float fTemp521 = 3.1415927f * fRec17[0] * fTemp520;
    float fTemp522 = std::cos(fTemp521);
    float fTemp523 = std::sin(fTemp521);
    fRec143[0] = fTemp520 * fTemp522 - fTemp523 * fRec143[1];
    fVec146[IOTA0 & 8191] = fTemp354 +
                            (fTemp520 * fTemp523 + fRec143[1] * fTemp522) *
                                std::pow(0.001f, fControl[19] / fTemp431) -
                            fTemp356;
    float fTemp524 =
        fVec146[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp518)))) &
                8191];
    fVec147[0] = fTemp524;
    float fTemp525 = (fTemp524 - fVec147[1]) * (fTemp517 + (-2.0f - fTemp519));
    fVec148[0] = fTemp525;
    fRec142[0] =
        fTemp524 +
        0.5f * (fVec148[1] - fTemp525) * (fTemp519 + (3.0f - fTemp517)) -
        fTemp525;
    float fTemp526 = fControl[9] / fTemp438;
    float fTemp527 = fTemp526 + -2.499995f;
    float fTemp528 = std::floor(fTemp527);
    float fTemp529 = 0.5f * fTemp465 * fRec144[2] +
                     0.25f * fTemp464 * (fRec144[1] + fRec144[3]);
    float fTemp530 = 3.1415927f * fRec17[0] * fTemp529;
    float fTemp531 = std::cos(fTemp530);
    float fTemp532 = std::sin(fTemp530);
    fRec145[0] = fTemp529 * fTemp531 - fTemp532 * fRec145[1];
    fVec149[IOTA0 & 8191] = fTemp354 +
                            (fTemp529 * fTemp532 + fRec145[1] * fTemp531) *
                                std::pow(0.001f, fControl[19] / fTemp442) -
                            fTemp356;
    float fTemp533 =
        fVec149[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp527)))) &
                8191];
    fVec150[0] = fTemp533;
    float fTemp534 = (fTemp533 - fVec150[1]) * (fTemp526 + (-2.0f - fTemp528));
    fVec151[0] = fTemp534;
    fRec144[0] =
        fTemp533 +
        0.5f * (fVec151[1] - fTemp534) * (fTemp528 + (3.0f - fTemp526)) -
        fTemp534;
    float fTemp535 = fControl[9] / fTemp449;
    float fTemp536 = fTemp535 + -2.499995f;
    float fTemp537 = std::floor(fTemp536);
    float fTemp538 = 0.5f * fTemp465 * fRec146[2] +
                     0.25f * fTemp464 * (fRec146[1] + fRec146[3]);
    float fTemp539 = 3.1415927f * fRec17[0] * fTemp538;
    float fTemp540 = std::cos(fTemp539);
    float fTemp541 = std::sin(fTemp539);
    fRec147[0] = fTemp538 * fTemp540 - fTemp541 * fRec147[1];
    fVec152[IOTA0 & 8191] = fTemp354 +
                            (fTemp538 * fTemp541 + fRec147[1] * fTemp540) *
                                std::pow(0.001f, fControl[19] / fTemp453) -
                            fTemp356;
    float fTemp542 =
        fVec152[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp536)))) &
                8191];
    fVec153[0] = fTemp542;
    float fTemp543 = (fTemp542 - fVec153[1]) * (fTemp535 + (-2.0f - fTemp537));
    fVec154[0] = fTemp543;
    fRec146[0] =
        fTemp542 +
        0.5f * (fVec154[1] - fTemp543) * (fTemp537 + (3.0f - fTemp535)) -
        fTemp543;
    fVec155[0] = fRec146[0] + fRec144[0] + fRec142[0] + fRec140[0] +
                 fRec138[0] + fRec136[0] + fRec134[0] + fRec132[0] + fRec130[0];
    fRec129[0] = fRec130[0] + fRec132[0] + fRec134[0] + fRec136[0] +
                 fRec138[0] + fRec140[0] + fRec142[0] + fRec144[0] +
                 fRec146[0] + 0.995f * fRec129[1] - fVec155[1];
    float fTemp544 = 0.01f * fRec129[0];
    fRec38[0] = fTemp544;
    fVec156[0] = fControl[35];
    float fTemp545 =
        ((fControl[35] != fVec156[1]) ? fControl[3] : fRec150[1] + -1.0f);
    fRec150[0] = fTemp545;
    fRec151[0] =
        ((fTemp545 <= 0.0f)
             ? fControl[35]
             : fRec151[1] + (1.0f - fRec151[1] - fControl[34]) / fTemp545);
    float fTemp546 =
        2.01f * fRec5[0] + fControl[36] * tambura_faustpower2_f(fRec151[0]);
    float fTemp547 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp3 + fTemp546)));
    float fTemp548 = fConst5 / fTemp547;
    float fTemp549 = fTemp548 + -2.499995f;
    float fTemp550 = std::floor(fTemp549);
    float fTemp551 =
        fControl[12] * float((fRec11[0] > 0.5f) & (fRec11[0] < 0.75f));
    float fTemp552 = fControl[34] + fTemp551;
    fVec157[0] = fTemp552;
    float fTemp553 = 1.0f - fTemp551 - fControl[34];
    fVec158[0] = fTemp553;
    float fTemp554 = ((fTemp553 != fVec158[1]) ? fControl[37] / fRec5[0]
                                               : fRec153[1] + -1.0f);
    fRec153[0] = fTemp554;
    fRec154[0] =
        ((fTemp554 <= 0.0f)
             ? fTemp553
             : fRec154[1] +
                   (1.0f - (fTemp551 + fRec154[1]) - fControl[34]) / fTemp554);
    float fTemp555 = fRec154[0] * fTemp552;
    float fTemp556 = fControl[38] / fRec5[0];
    int iTemp557 = std::fabs(fTemp556) < 1.1920929e-07f;
    float fTemp558 =
        ((fTemp555 > fRec152[1])
             ? ((iTemp557)
                    ? 0.0f
                    : std::exp(-(fConst6 / ((iTemp557) ? 1.0f : fTemp556))))
             : fConst7);
    fRec152[0] = fTemp555 * (1.0f - fTemp558) + fRec152[1] * fTemp558;
    float fTemp559 = fRec152[0] * fTemp9;
    fVec159[IOTA0 & 8191] = fTemp559;
    int iTemp560 = (fVec157[1] <= 0.0f) & (fTemp552 > 0.0f);
    fRec155[0] = fRec155[1] * float(1 - iTemp560) + fTemp20 * float(iTemp560);
    float fTemp561 =
        fVec159[(IOTA0 -
                 int(std::min<float>(
                     4096.0f,
                     std::max<float>(
                         0.0f, fConst35 * ((fControl[16] + 0.2f * fRec155[0]) /
                                           fRec5[0]))))) &
                8191];
    float fTemp562 = fRec15[0] * fTemp547;
    float fTemp563 = fTemp153 + fTemp361;
    float fTemp564 = fConst16 * (fTemp145 + fTemp563) + fTemp559;
    float fTemp565 = fRec16[0] * (fTemp564 - fTemp561);
    float fTemp566 = 1.0f - fTemp565;
    float fTemp567 = fTemp565 + 1.0f;
    float fTemp568 = 0.5f * fTemp567 * fRec149[2] +
                     0.25f * fTemp566 * (fRec149[1] + fRec149[3]);
    float fTemp569 = 3.1415927f * fRec17[0] * fTemp568;
    float fTemp570 = std::cos(fTemp569);
    float fTemp571 = std::sin(fTemp569);
    fRec156[0] = fTemp568 * fTemp570 - fTemp571 * fRec156[1];
    fVec160[IOTA0 & 8191] = fTemp564 +
                            (fTemp568 * fTemp571 + fRec156[1] * fTemp570) *
                                std::pow(0.001f, fControl[27] / fTemp562) -
                            fTemp561;
    float fTemp572 =
        fVec160[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp549)))) &
                8191];
    fVec161[0] = fTemp572;
    float fTemp573 = (fTemp572 - fVec161[1]) * (fTemp548 + (-2.0f - fTemp550));
    fVec162[0] = fTemp573;
    fRec149[0] =
        fTemp572 +
        0.5f * (fVec162[1] - fTemp573) * (fTemp550 + (3.0f - fTemp548)) -
        fTemp573;
    float fTemp574 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp32 + fTemp546)));
    float fTemp575 = fConst5 / fTemp574;
    float fTemp576 = fTemp575 + -2.499995f;
    float fTemp577 = std::floor(fTemp576);
    float fTemp578 = fRec15[0] * fTemp574;
    float fTemp579 = 0.5f * fTemp567 * fRec157[2] +
                     0.25f * fTemp566 * (fRec157[1] + fRec157[3]);
    float fTemp580 = 3.1415927f * fRec17[0] * fTemp579;
    float fTemp581 = std::cos(fTemp580);
    float fTemp582 = std::sin(fTemp580);
    fRec158[0] = fTemp579 * fTemp581 - fTemp582 * fRec158[1];
    fVec163[IOTA0 & 8191] = fTemp564 +
                            (fTemp579 * fTemp582 + fRec158[1] * fTemp581) *
                                std::pow(0.001f, fControl[27] / fTemp578) -
                            fTemp561;
    float fTemp583 =
        fVec163[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp576)))) &
                8191];
    fVec164[0] = fTemp583;
    float fTemp584 = (fTemp583 - fVec164[1]) * (fTemp575 + (-2.0f - fTemp577));
    fVec165[0] = fTemp584;
    fRec157[0] =
        fTemp583 +
        0.5f * (fVec165[1] - fTemp584) * (fTemp577 + (3.0f - fTemp575)) -
        fTemp584;
    float fTemp585 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp44 + fTemp546)));
    float fTemp586 = fConst5 / fTemp585;
    float fTemp587 = fTemp586 + -2.499995f;
    float fTemp588 = std::floor(fTemp587);
    float fTemp589 = fRec15[0] * fTemp585;
    float fTemp590 = 0.5f * fTemp567 * fRec159[2] +
                     0.25f * fTemp566 * (fRec159[1] + fRec159[3]);
    float fTemp591 = 3.1415927f * fRec17[0] * fTemp590;
    float fTemp592 = std::cos(fTemp591);
    float fTemp593 = std::sin(fTemp591);
    fRec160[0] = fTemp590 * fTemp592 - fTemp593 * fRec160[1];
    fVec166[IOTA0 & 8191] = fTemp564 +
                            (fTemp590 * fTemp593 + fRec160[1] * fTemp592) *
                                std::pow(0.001f, fControl[27] / fTemp589) -
                            fTemp561;
    float fTemp594 =
        fVec166[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp587)))) &
                8191];
    fVec167[0] = fTemp594;
    float fTemp595 = (fTemp594 - fVec167[1]) * (fTemp586 + (-2.0f - fTemp588));
    fVec168[0] = fTemp595;
    fRec159[0] =
        fTemp594 +
        0.5f * (fVec168[1] - fTemp595) * (fTemp588 + (3.0f - fTemp586)) -
        fTemp595;
    float fTemp596 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fRec6[0] + fTemp546)));
    float fTemp597 = fConst5 / fTemp596;
    float fTemp598 = fTemp597 + -2.499995f;
    float fTemp599 = std::floor(fTemp598);
    float fTemp600 = fRec15[0] * fTemp596;
    float fTemp601 = 0.5f * fTemp567 * fRec161[2] +
                     0.25f * fTemp566 * (fRec161[1] + fRec161[3]);
    float fTemp602 = 3.1415927f * fRec17[0] * fTemp601;
    float fTemp603 = std::cos(fTemp602);
    float fTemp604 = std::sin(fTemp602);
    fRec162[0] = fTemp601 * fTemp603 - fTemp604 * fRec162[1];
    fVec169[IOTA0 & 8191] = fTemp564 +
                            (fTemp601 * fTemp604 + fRec162[1] * fTemp603) *
                                std::pow(0.001f, fControl[27] / fTemp600) -
                            fTemp561;
    float fTemp605 =
        fVec169[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp598)))) &
                8191];
    fVec170[0] = fTemp605;
    float fTemp606 = (fTemp605 - fVec170[1]) * (fTemp597 + (-2.0f - fTemp599));
    fVec171[0] = fTemp606;
    fRec161[0] =
        fTemp605 +
        0.5f * (fVec171[1] - fTemp606) * (fTemp599 + (3.0f - fTemp597)) -
        fTemp606;
    float fTemp607 =
        std::pow(2.0f, 1.442695f * std::log(0.0022727272f * fTemp546));
    float fTemp608 = fConst5 / fTemp607;
    float fTemp609 = fTemp608 + -2.499995f;
    float fTemp610 = std::floor(fTemp609);
    float fTemp611 = fRec15[0] * fTemp607;
    float fTemp612 = 0.5f * fTemp567 * fRec163[2] +
                     0.25f * fTemp566 * (fRec163[1] + fRec163[3]);
    float fTemp613 = 3.1415927f * fRec17[0] * fTemp612;
    float fTemp614 = std::cos(fTemp613);
    float fTemp615 = std::sin(fTemp613);
    fRec164[0] = fTemp612 * fTemp614 - fTemp615 * fRec164[1];
    fVec172[IOTA0 & 8191] = fTemp564 +
                            (fTemp612 * fTemp615 + fRec164[1] * fTemp614) *
                                std::pow(0.001f, fControl[27] / fTemp611) -
                            fTemp561;
    float fTemp616 =
        fVec172[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp609)))) &
                8191];
    fVec173[0] = fTemp616;
    float fTemp617 = (fTemp616 - fVec173[1]) * (fTemp608 + (-2.0f - fTemp610));
    fVec174[0] = fTemp617;
    fRec163[0] =
        fTemp616 +
        0.5f * (fVec174[1] - fTemp617) * (fTemp610 + (3.0f - fTemp608)) -
        fTemp617;
    float fTemp618 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp546 - fRec6[0])));
    float fTemp619 = fConst5 / fTemp618;
    float fTemp620 = fTemp619 + -2.499995f;
    float fTemp621 = std::floor(fTemp620);
    float fTemp622 = fRec15[0] * fTemp618;
    float fTemp623 = 0.5f * fTemp567 * fRec165[2] +
                     0.25f * fTemp566 * (fRec165[1] + fRec165[3]);
    float fTemp624 = 3.1415927f * fRec17[0] * fTemp623;
    float fTemp625 = std::cos(fTemp624);
    float fTemp626 = std::sin(fTemp624);
    fRec166[0] = fTemp623 * fTemp625 - fTemp626 * fRec166[1];
    fVec175[IOTA0 & 8191] = fTemp564 +
                            (fTemp623 * fTemp626 + fRec166[1] * fTemp625) *
                                std::pow(0.001f, fControl[27] / fTemp622) -
                            fTemp561;
    float fTemp627 =
        fVec175[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp620)))) &
                8191];
    fVec176[0] = fTemp627;
    float fTemp628 = (fTemp627 - fVec176[1]) * (fTemp619 + (-2.0f - fTemp621));
    fVec177[0] = fTemp628;
    fRec165[0] =
        fTemp627 +
        0.5f * (fVec177[1] - fTemp628) * (fTemp621 + (3.0f - fTemp619)) -
        fTemp628;
    float fTemp629 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp546 - fTemp44)));
    float fTemp630 = fConst5 / fTemp629;
    float fTemp631 = fTemp630 + -2.499995f;
    float fTemp632 = std::floor(fTemp631);
    float fTemp633 = fRec15[0] * fTemp629;
    float fTemp634 = 0.5f * fTemp567 * fRec167[2] +
                     0.25f * fTemp566 * (fRec167[1] + fRec167[3]);
    float fTemp635 = 3.1415927f * fRec17[0] * fTemp634;
    float fTemp636 = std::cos(fTemp635);
    float fTemp637 = std::sin(fTemp635);
    fRec168[0] = fTemp634 * fTemp636 - fTemp637 * fRec168[1];
    fVec178[IOTA0 & 8191] = fTemp564 +
                            (fTemp634 * fTemp637 + fRec168[1] * fTemp636) *
                                std::pow(0.001f, fControl[27] / fTemp633) -
                            fTemp561;
    float fTemp638 =
        fVec178[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp631)))) &
                8191];
    fVec179[0] = fTemp638;
    float fTemp639 = (fTemp638 - fVec179[1]) * (fTemp630 + (-2.0f - fTemp632));
    fVec180[0] = fTemp639;
    fRec167[0] =
        fTemp638 +
        0.5f * (fVec180[1] - fTemp639) * (fTemp632 + (3.0f - fTemp630)) -
        fTemp639;
    float fTemp640 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp546 - fTemp32)));
    float fTemp641 = fConst5 / fTemp640;
    float fTemp642 = fTemp641 + -2.499995f;
    float fTemp643 = std::floor(fTemp642);
    float fTemp644 = fRec15[0] * fTemp640;
    float fTemp645 = 0.5f * fTemp567 * fRec169[2] +
                     0.25f * fTemp566 * (fRec169[1] + fRec169[3]);
    float fTemp646 = 3.1415927f * fRec17[0] * fTemp645;
    float fTemp647 = std::cos(fTemp646);
    float fTemp648 = std::sin(fTemp646);
    fRec170[0] = fTemp645 * fTemp647 - fTemp648 * fRec170[1];
    fVec181[IOTA0 & 8191] = fTemp564 +
                            (fTemp645 * fTemp648 + fRec170[1] * fTemp647) *
                                std::pow(0.001f, fControl[27] / fTemp644) -
                            fTemp561;
    float fTemp649 =
        fVec181[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp642)))) &
                8191];
    fVec182[0] = fTemp649;
    float fTemp650 = (fTemp649 - fVec182[1]) * (fTemp641 + (-2.0f - fTemp643));
    fVec183[0] = fTemp650;
    fRec169[0] =
        fTemp649 +
        0.5f * (fVec183[1] - fTemp650) * (fTemp643 + (3.0f - fTemp641)) -
        fTemp650;
    float fTemp651 = std::pow(
        2.0f, 1.442695f * std::log(0.0022727272f * (fTemp546 - fTemp3)));
    float fTemp652 = fConst5 / fTemp651;
    float fTemp653 = fTemp652 + -2.499995f;
    float fTemp654 = std::floor(fTemp653);
    float fTemp655 = fRec15[0] * fTemp651;
    float fTemp656 = 0.5f * fTemp567 * fRec171[2] +
                     0.25f * fTemp566 * (fRec171[1] + fRec171[3]);
    float fTemp657 = 3.1415927f * fRec17[0] * fTemp656;
    float fTemp658 = std::cos(fTemp657);
    float fTemp659 = std::sin(fTemp657);
    fRec172[0] = fTemp656 * fTemp658 - fTemp659 * fRec172[1];
    fVec184[IOTA0 & 8191] = fTemp564 +
                            (fTemp656 * fTemp659 + fRec172[1] * fTemp658) *
                                std::pow(0.001f, fControl[27] / fTemp655) -
                            fTemp561;
    float fTemp660 =
        fVec184[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp653)))) &
                8191];
    fVec185[0] = fTemp660;
    float fTemp661 = (fTemp660 - fVec185[1]) * (fTemp652 + (-2.0f - fTemp654));
    fVec186[0] = fTemp661;
    fRec171[0] =
        fTemp660 +
        0.5f * (fVec186[1] - fTemp661) * (fTemp654 + (3.0f - fTemp652)) -
        fTemp661;
    fVec187[0] = fRec171[0] + fRec169[0] + fRec167[0] + fRec165[0] +
                 fRec163[0] + fRec161[0] + fRec159[0] + fRec157[0] + fRec149[0];
    fRec148[0] = fRec149[0] + fRec157[0] + fRec159[0] + fRec161[0] +
                 fRec163[0] + fRec165[0] + fRec167[0] + fRec169[0] +
                 fRec171[0] + 0.995f * fRec148[1] - fVec187[1];
    float fRec39 = 0.01f * fRec148[0];
    float fTemp662 = fControl[9] / fTemp547;
    float fTemp663 = fTemp662 + -2.499995f;
    float fTemp664 = std::floor(fTemp663);
    float fTemp665 = fRec16[0] * (fTemp559 - fTemp561);
    float fTemp666 = 1.0f - fTemp665;
    float fTemp667 = fTemp665 + 1.0f;
    float fTemp668 = 0.5f * fTemp667 * fRec174[2] +
                     0.25f * fTemp666 * (fRec174[1] + fRec174[3]);
    float fTemp669 = 3.1415927f * fRec17[0] * fTemp668;
    float fTemp670 = std::cos(fTemp669);
    float fTemp671 = std::sin(fTemp669);
    fRec175[0] = fTemp668 * fTemp670 - fTemp671 * fRec175[1];
    fVec188[IOTA0 & 8191] = fTemp559 +
                            (fTemp668 * fTemp671 + fRec175[1] * fTemp670) *
                                std::pow(0.001f, fControl[19] / fTemp562) -
                            fTemp561;
    float fTemp672 =
        fVec188[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp663)))) &
                8191];
    fVec189[0] = fTemp672;
    float fTemp673 = (fTemp672 - fVec189[1]) * (fTemp662 + (-2.0f - fTemp664));
    fVec190[0] = fTemp673;
    fRec174[0] =
        fTemp672 +
        0.5f * (fVec190[1] - fTemp673) * (fTemp664 + (3.0f - fTemp662)) -
        fTemp673;
    float fTemp674 = fControl[9] / fTemp574;
    float fTemp675 = fTemp674 + -2.499995f;
    float fTemp676 = std::floor(fTemp675);
    float fTemp677 = 0.5f * fTemp667 * fRec176[2] +
                     0.25f * fTemp666 * (fRec176[1] + fRec176[3]);
    float fTemp678 = 3.1415927f * fRec17[0] * fTemp677;
    float fTemp679 = std::cos(fTemp678);
    float fTemp680 = std::sin(fTemp678);
    fRec177[0] = fTemp677 * fTemp679 - fTemp680 * fRec177[1];
    fVec191[IOTA0 & 8191] = fTemp559 +
                            (fTemp677 * fTemp680 + fRec177[1] * fTemp679) *
                                std::pow(0.001f, fControl[19] / fTemp578) -
                            fTemp561;
    float fTemp681 =
        fVec191[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp675)))) &
                8191];
    fVec192[0] = fTemp681;
    float fTemp682 = (fTemp681 - fVec192[1]) * (fTemp674 + (-2.0f - fTemp676));
    fVec193[0] = fTemp682;
    fRec176[0] =
        fTemp681 +
        0.5f * (fVec193[1] - fTemp682) * (fTemp676 + (3.0f - fTemp674)) -
        fTemp682;
    float fTemp683 = fControl[9] / fTemp585;
    float fTemp684 = fTemp683 + -2.499995f;
    float fTemp685 = std::floor(fTemp684);
    float fTemp686 = 0.5f * fTemp667 * fRec178[2] +
                     0.25f * fTemp666 * (fRec178[1] + fRec178[3]);
    float fTemp687 = 3.1415927f * fRec17[0] * fTemp686;
    float fTemp688 = std::cos(fTemp687);
    float fTemp689 = std::sin(fTemp687);
    fRec179[0] = fTemp686 * fTemp688 - fTemp689 * fRec179[1];
    fVec194[IOTA0 & 8191] = fTemp559 +
                            (fTemp686 * fTemp689 + fRec179[1] * fTemp688) *
                                std::pow(0.001f, fControl[19] / fTemp589) -
                            fTemp561;
    float fTemp690 =
        fVec194[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp684)))) &
                8191];
    fVec195[0] = fTemp690;
    float fTemp691 = (fTemp690 - fVec195[1]) * (fTemp683 + (-2.0f - fTemp685));
    fVec196[0] = fTemp691;
    fRec178[0] =
        fTemp690 +
        0.5f * (fVec196[1] - fTemp691) * (fTemp685 + (3.0f - fTemp683)) -
        fTemp691;
    float fTemp692 = fControl[9] / fTemp596;
    float fTemp693 = fTemp692 + -2.499995f;
    float fTemp694 = std::floor(fTemp693);
    float fTemp695 = 0.5f * fTemp667 * fRec180[2] +
                     0.25f * fTemp666 * (fRec180[1] + fRec180[3]);
    float fTemp696 = 3.1415927f * fRec17[0] * fTemp695;
    float fTemp697 = std::cos(fTemp696);
    float fTemp698 = std::sin(fTemp696);
    fRec181[0] = fTemp695 * fTemp697 - fTemp698 * fRec181[1];
    fVec197[IOTA0 & 8191] = fTemp559 +
                            (fTemp695 * fTemp698 + fRec181[1] * fTemp697) *
                                std::pow(0.001f, fControl[19] / fTemp600) -
                            fTemp561;
    float fTemp699 =
        fVec197[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp693)))) &
                8191];
    fVec198[0] = fTemp699;
    float fTemp700 = (fTemp699 - fVec198[1]) * (fTemp692 + (-2.0f - fTemp694));
    fVec199[0] = fTemp700;
    fRec180[0] =
        fTemp699 +
        0.5f * (fVec199[1] - fTemp700) * (fTemp694 + (3.0f - fTemp692)) -
        fTemp700;
    float fTemp701 = fControl[9] / fTemp607;
    float fTemp702 = fTemp701 + -2.499995f;
    float fTemp703 = std::floor(fTemp702);
    float fTemp704 = 0.5f * fTemp667 * fRec182[2] +
                     0.25f * fTemp666 * (fRec182[1] + fRec182[3]);
    float fTemp705 = 3.1415927f * fRec17[0] * fTemp704;
    float fTemp706 = std::cos(fTemp705);
    float fTemp707 = std::sin(fTemp705);
    fRec183[0] = fTemp704 * fTemp706 - fTemp707 * fRec183[1];
    fVec200[IOTA0 & 8191] = fTemp559 +
                            (fTemp704 * fTemp707 + fRec183[1] * fTemp706) *
                                std::pow(0.001f, fControl[19] / fTemp611) -
                            fTemp561;
    float fTemp708 =
        fVec200[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp702)))) &
                8191];
    fVec201[0] = fTemp708;
    float fTemp709 = (fTemp708 - fVec201[1]) * (fTemp701 + (-2.0f - fTemp703));
    fVec202[0] = fTemp709;
    fRec182[0] =
        fTemp708 +
        0.5f * (fVec202[1] - fTemp709) * (fTemp703 + (3.0f - fTemp701)) -
        fTemp709;
    float fTemp710 = fControl[9] / fTemp618;
    float fTemp711 = fTemp710 + -2.499995f;
    float fTemp712 = std::floor(fTemp711);
    float fTemp713 = 0.5f * fTemp667 * fRec184[2] +
                     0.25f * fTemp666 * (fRec184[1] + fRec184[3]);
    float fTemp714 = 3.1415927f * fRec17[0] * fTemp713;
    float fTemp715 = std::cos(fTemp714);
    float fTemp716 = std::sin(fTemp714);
    fRec185[0] = fTemp713 * fTemp715 - fTemp716 * fRec185[1];
    fVec203[IOTA0 & 8191] = fTemp559 +
                            (fTemp713 * fTemp716 + fRec185[1] * fTemp715) *
                                std::pow(0.001f, fControl[19] / fTemp622) -
                            fTemp561;
    float fTemp717 =
        fVec203[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp711)))) &
                8191];
    fVec204[0] = fTemp717;
    float fTemp718 = (fTemp717 - fVec204[1]) * (fTemp710 + (-2.0f - fTemp712));
    fVec205[0] = fTemp718;
    fRec184[0] =
        fTemp717 +
        0.5f * (fVec205[1] - fTemp718) * (fTemp712 + (3.0f - fTemp710)) -
        fTemp718;
    float fTemp719 = fControl[9] / fTemp629;
    float fTemp720 = fTemp719 + -2.499995f;
    float fTemp721 = std::floor(fTemp720);
    float fTemp722 = 0.5f * fTemp667 * fRec186[2] +
                     0.25f * fTemp666 * (fRec186[1] + fRec186[3]);
    float fTemp723 = 3.1415927f * fRec17[0] * fTemp722;
    float fTemp724 = std::cos(fTemp723);
    float fTemp725 = std::sin(fTemp723);
    fRec187[0] = fTemp722 * fTemp724 - fTemp725 * fRec187[1];
    fVec206[IOTA0 & 8191] = fTemp559 +
                            (fTemp722 * fTemp725 + fRec187[1] * fTemp724) *
                                std::pow(0.001f, fControl[19] / fTemp633) -
                            fTemp561;
    float fTemp726 =
        fVec206[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp720)))) &
                8191];
    fVec207[0] = fTemp726;
    float fTemp727 = (fTemp726 - fVec207[1]) * (fTemp719 + (-2.0f - fTemp721));
    fVec208[0] = fTemp727;
    fRec186[0] =
        fTemp726 +
        0.5f * (fVec208[1] - fTemp727) * (fTemp721 + (3.0f - fTemp719)) -
        fTemp727;
    float fTemp728 = fControl[9] / fTemp640;
    float fTemp729 = fTemp728 + -2.499995f;
    float fTemp730 = std::floor(fTemp729);
    float fTemp731 = 0.5f * fTemp667 * fRec188[2] +
                     0.25f * fTemp666 * (fRec188[1] + fRec188[3]);
    float fTemp732 = 3.1415927f * fRec17[0] * fTemp731;
    float fTemp733 = std::cos(fTemp732);
    float fTemp734 = std::sin(fTemp732);
    fRec189[0] = fTemp731 * fTemp733 - fTemp734 * fRec189[1];
    fVec209[IOTA0 & 8191] = fTemp559 +
                            (fTemp731 * fTemp734 + fRec189[1] * fTemp733) *
                                std::pow(0.001f, fControl[19] / fTemp644) -
                            fTemp561;
    float fTemp735 =
        fVec209[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp729)))) &
                8191];
    fVec210[0] = fTemp735;
    float fTemp736 = (fTemp735 - fVec210[1]) * (fTemp728 + (-2.0f - fTemp730));
    fVec211[0] = fTemp736;
    fRec188[0] =
        fTemp735 +
        0.5f * (fVec211[1] - fTemp736) * (fTemp730 + (3.0f - fTemp728)) -
        fTemp736;
    float fTemp737 = fControl[9] / fTemp651;
    float fTemp738 = fTemp737 + -2.499995f;
    float fTemp739 = std::floor(fTemp738);
    float fTemp740 = 0.5f * fTemp667 * fRec190[2] +
                     0.25f * fTemp666 * (fRec190[1] + fRec190[3]);
    float fTemp741 = 3.1415927f * fRec17[0] * fTemp740;
    float fTemp742 = std::cos(fTemp741);
    float fTemp743 = std::sin(fTemp741);
    fRec191[0] = fTemp740 * fTemp742 - fTemp743 * fRec191[1];
    fVec212[IOTA0 & 8191] = fTemp559 +
                            (fTemp740 * fTemp743 + fRec191[1] * fTemp742) *
                                std::pow(0.001f, fControl[19] / fTemp655) -
                            fTemp561;
    float fTemp744 =
        fVec212[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp738)))) &
                8191];
    fVec213[0] = fTemp744;
    float fTemp745 = (fTemp744 - fVec213[1]) * (fTemp737 + (-2.0f - fTemp739));
    fVec214[0] = fTemp745;
    fRec190[0] =
        fTemp744 +
        0.5f * (fVec214[1] - fTemp745) * (fTemp739 + (3.0f - fTemp737)) -
        fTemp745;
    fVec215[0] = fRec190[0] + fRec188[0] + fRec186[0] + fRec184[0] +
                 fRec182[0] + fRec180[0] + fRec178[0] + fRec176[0] + fRec174[0];
    fRec173[0] = fRec174[0] + fRec176[0] + fRec178[0] + fRec180[0] +
                 fRec182[0] + fRec184[0] + fRec186[0] + fRec188[0] +
                 fRec190[0] + 0.995f * fRec173[1] - fVec215[1];
    float fTemp746 = 0.01f * fRec173[0];
    fRec40[0] = fTemp746;
    float fTemp747 = fConst5 / fTemp4;
    float fTemp748 = fTemp747 + -2.499995f;
    float fTemp749 = std::floor(fTemp748);
    float fTemp750 = fConst16 * (fTemp149 + fTemp563) + fTemp18;
    float fTemp751 = fRec16[0] * (fTemp750 - fTemp21);
    float fTemp752 = 1.0f - fTemp751;
    float fTemp753 = fTemp751 + 1.0f;
    float fTemp754 = 0.5f * fTemp753 * fRec193[2] +
                     0.25f * fTemp752 * (fRec193[1] + fRec193[3]);
    float fTemp755 = 3.1415927f * fRec17[0] * fTemp754;
    float fTemp756 = std::cos(fTemp755);
    float fTemp757 = std::sin(fTemp755);
    fRec194[0] = fTemp754 * fTemp756 - fTemp757 * fRec194[1];
    fVec216[IOTA0 & 8191] = fTemp750 +
                            (fTemp754 * fTemp757 + fRec194[1] * fTemp756) *
                                std::pow(0.001f, fControl[27] / fTemp22) -
                            fTemp21;
    float fTemp758 =
        fVec216[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp748)))) &
                8191];
    fVec217[0] = fTemp758;
    float fTemp759 = (fTemp758 - fVec217[1]) * (fTemp747 + (-2.0f - fTemp749));
    fVec218[0] = fTemp759;
    fRec193[0] =
        fTemp758 +
        0.5f * (fVec218[1] - fTemp759) * (fTemp749 + (3.0f - fTemp747)) -
        fTemp759;
    float fTemp760 = fConst5 / fTemp33;
    float fTemp761 = fTemp760 + -2.499995f;
    float fTemp762 = std::floor(fTemp761);
    float fTemp763 = 0.5f * fTemp753 * fRec195[2] +
                     0.25f * fTemp752 * (fRec195[1] + fRec195[3]);
    float fTemp764 = 3.1415927f * fRec17[0] * fTemp763;
    float fTemp765 = std::cos(fTemp764);
    float fTemp766 = std::sin(fTemp764);
    fRec196[0] = fTemp763 * fTemp765 - fTemp766 * fRec196[1];
    fVec219[IOTA0 & 8191] = fTemp750 +
                            (fTemp763 * fTemp766 + fRec196[1] * fTemp765) *
                                std::pow(0.001f, fControl[27] / fTemp37) -
                            fTemp21;
    float fTemp767 =
        fVec219[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp761)))) &
                8191];
    fVec220[0] = fTemp767;
    float fTemp768 = (fTemp767 - fVec220[1]) * (fTemp760 + (-2.0f - fTemp762));
    fVec221[0] = fTemp768;
    fRec195[0] =
        fTemp767 +
        0.5f * (fVec221[1] - fTemp768) * (fTemp762 + (3.0f - fTemp760)) -
        fTemp768;
    float fTemp769 = fConst5 / fTemp45;
    float fTemp770 = fTemp769 + -2.499995f;
    float fTemp771 = std::floor(fTemp770);
    float fTemp772 = 0.5f * fTemp753 * fRec197[2] +
                     0.25f * fTemp752 * (fRec197[1] + fRec197[3]);
    float fTemp773 = 3.1415927f * fRec17[0] * fTemp772;
    float fTemp774 = std::cos(fTemp773);
    float fTemp775 = std::sin(fTemp773);
    fRec198[0] = fTemp772 * fTemp774 - fTemp775 * fRec198[1];
    fVec222[IOTA0 & 8191] = fTemp750 +
                            (fTemp772 * fTemp775 + fRec198[1] * fTemp774) *
                                std::pow(0.001f, fControl[27] / fTemp49) -
                            fTemp21;
    float fTemp776 =
        fVec222[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp770)))) &
                8191];
    fVec223[0] = fTemp776;
    float fTemp777 = (fTemp776 - fVec223[1]) * (fTemp769 + (-2.0f - fTemp771));
    fVec224[0] = fTemp777;
    fRec197[0] =
        fTemp776 +
        0.5f * (fVec224[1] - fTemp777) * (fTemp771 + (3.0f - fTemp769)) -
        fTemp777;
    float fTemp778 = fConst5 / fTemp56;
    float fTemp779 = fTemp778 + -2.499995f;
    float fTemp780 = std::floor(fTemp779);
    float fTemp781 = 0.5f * fTemp753 * fRec199[2] +
                     0.25f * fTemp752 * (fRec199[1] + fRec199[3]);
    float fTemp782 = 3.1415927f * fRec17[0] * fTemp781;
    float fTemp783 = std::cos(fTemp782);
    float fTemp784 = std::sin(fTemp782);
    fRec200[0] = fTemp781 * fTemp783 - fTemp784 * fRec200[1];
    fVec225[IOTA0 & 8191] = fTemp750 +
                            (fTemp781 * fTemp784 + fRec200[1] * fTemp783) *
                                std::pow(0.001f, fControl[27] / fTemp60) -
                            fTemp21;
    float fTemp785 =
        fVec225[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp779)))) &
                8191];
    fVec226[0] = fTemp785;
    float fTemp786 = (fTemp785 - fVec226[1]) * (fTemp778 + (-2.0f - fTemp780));
    fVec227[0] = fTemp786;
    fRec199[0] =
        fTemp785 +
        0.5f * (fVec227[1] - fTemp786) * (fTemp780 + (3.0f - fTemp778)) -
        fTemp786;
    float fTemp787 = fConst5 / fTemp67;
    float fTemp788 = fTemp787 + -2.499995f;
    float fTemp789 = std::floor(fTemp788);
    float fTemp790 = 0.5f * fTemp753 * fRec201[2] +
                     0.25f * fTemp752 * (fRec201[1] + fRec201[3]);
    float fTemp791 = 3.1415927f * fRec17[0] * fTemp790;
    float fTemp792 = std::cos(fTemp791);
    float fTemp793 = std::sin(fTemp791);
    fRec202[0] = fTemp790 * fTemp792 - fTemp793 * fRec202[1];
    fVec228[IOTA0 & 8191] = fTemp750 +
                            (fTemp790 * fTemp793 + fRec202[1] * fTemp792) *
                                std::pow(0.001f, fControl[27] / fTemp71) -
                            fTemp21;
    float fTemp794 =
        fVec228[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp788)))) &
                8191];
    fVec229[0] = fTemp794;
    float fTemp795 = (fTemp794 - fVec229[1]) * (fTemp787 + (-2.0f - fTemp789));
    fVec230[0] = fTemp795;
    fRec201[0] =
        fTemp794 +
        0.5f * (fVec230[1] - fTemp795) * (fTemp789 + (3.0f - fTemp787)) -
        fTemp795;
    float fTemp796 = fConst5 / fTemp78;
    float fTemp797 = fTemp796 + -2.499995f;
    float fTemp798 = std::floor(fTemp797);
    float fTemp799 = 0.5f * fTemp753 * fRec203[2] +
                     0.25f * fTemp752 * (fRec203[1] + fRec203[3]);
    float fTemp800 = 3.1415927f * fRec17[0] * fTemp799;
    float fTemp801 = std::cos(fTemp800);
    float fTemp802 = std::sin(fTemp800);
    fRec204[0] = fTemp799 * fTemp801 - fTemp802 * fRec204[1];
    fVec231[IOTA0 & 8191] = fTemp750 +
                            (fTemp799 * fTemp802 + fRec204[1] * fTemp801) *
                                std::pow(0.001f, fControl[27] / fTemp82) -
                            fTemp21;
    float fTemp803 =
        fVec231[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp797)))) &
                8191];
    fVec232[0] = fTemp803;
    float fTemp804 = (fTemp803 - fVec232[1]) * (fTemp796 + (-2.0f - fTemp798));
    fVec233[0] = fTemp804;
    fRec203[0] =
        fTemp803 +
        0.5f * (fVec233[1] - fTemp804) * (fTemp798 + (3.0f - fTemp796)) -
        fTemp804;
    float fTemp805 = fConst5 / fTemp89;
    float fTemp806 = fTemp805 + -2.499995f;
    float fTemp807 = std::floor(fTemp806);
    float fTemp808 = 0.5f * fTemp753 * fRec205[2] +
                     0.25f * fTemp752 * (fRec205[1] + fRec205[3]);
    float fTemp809 = 3.1415927f * fRec17[0] * fTemp808;
    float fTemp810 = std::cos(fTemp809);
    float fTemp811 = std::sin(fTemp809);
    fRec206[0] = fTemp808 * fTemp810 - fTemp811 * fRec206[1];
    fVec234[IOTA0 & 8191] = fTemp750 +
                            (fTemp808 * fTemp811 + fRec206[1] * fTemp810) *
                                std::pow(0.001f, fControl[27] / fTemp93) -
                            fTemp21;
    float fTemp812 =
        fVec234[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp806)))) &
                8191];
    fVec235[0] = fTemp812;
    float fTemp813 = (fTemp812 - fVec235[1]) * (fTemp805 + (-2.0f - fTemp807));
    fVec236[0] = fTemp813;
    fRec205[0] =
        fTemp812 +
        0.5f * (fVec236[1] - fTemp813) * (fTemp807 + (3.0f - fTemp805)) -
        fTemp813;
    float fTemp814 = fConst5 / fTemp100;
    float fTemp815 = fTemp814 + -2.499995f;
    float fTemp816 = std::floor(fTemp815);
    float fTemp817 = 0.5f * fTemp753 * fRec207[2] +
                     0.25f * fTemp752 * (fRec207[1] + fRec207[3]);
    float fTemp818 = 3.1415927f * fRec17[0] * fTemp817;
    float fTemp819 = std::cos(fTemp818);
    float fTemp820 = std::sin(fTemp818);
    fRec208[0] = fTemp817 * fTemp819 - fTemp820 * fRec208[1];
    fVec237[IOTA0 & 8191] = fTemp750 +
                            (fTemp817 * fTemp820 + fRec208[1] * fTemp819) *
                                std::pow(0.001f, fControl[27] / fTemp104) -
                            fTemp21;
    float fTemp821 =
        fVec237[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp815)))) &
                8191];
    fVec238[0] = fTemp821;
    float fTemp822 = (fTemp821 - fVec238[1]) * (fTemp814 + (-2.0f - fTemp816));
    fVec239[0] = fTemp822;
    fRec207[0] =
        fTemp821 +
        0.5f * (fVec239[1] - fTemp822) * (fTemp816 + (3.0f - fTemp814)) -
        fTemp822;
    float fTemp823 = fConst5 / fTemp111;
    float fTemp824 = fTemp823 + -2.499995f;
    float fTemp825 = std::floor(fTemp824);
    float fTemp826 = 0.5f * fTemp753 * fRec209[2] +
                     0.25f * fTemp752 * (fRec209[1] + fRec209[3]);
    float fTemp827 = 3.1415927f * fRec17[0] * fTemp826;
    float fTemp828 = std::cos(fTemp827);
    float fTemp829 = std::sin(fTemp827);
    fRec210[0] = fTemp826 * fTemp828 - fTemp829 * fRec210[1];
    fVec240[IOTA0 & 8191] = fTemp750 +
                            (fTemp826 * fTemp829 + fRec210[1] * fTemp828) *
                                std::pow(0.001f, fControl[27] / fTemp115) -
                            fTemp21;
    float fTemp830 =
        fVec240[(IOTA0 - std::min<int>(4096, std::max<int>(0, int(fTemp824)))) &
                8191];
    fVec241[0] = fTemp830;
    float fTemp831 = (fTemp830 - fVec241[1]) * (fTemp823 + (-2.0f - fTemp825));
    fVec242[0] = fTemp831;
    fRec209[0] =
        fTemp830 +
        0.5f * (fVec242[1] - fTemp831) * (fTemp825 + (3.0f - fTemp823)) -
        fTemp831;
    fVec243[0] = fRec209[0] + fRec207[0] + fRec205[0] + fRec203[0] +
                 fRec201[0] + fRec199[0] + fRec197[0] + fRec195[0] + fRec193[0];
    fRec192[0] = fRec193[0] + fRec195[0] + fRec197[0] + fRec199[0] +
                 fRec201[0] + fRec203[0] + fRec205[0] + fRec207[0] +
                 fRec209[0] + 0.995f * fRec192[1] - fVec243[1];
    float fRec41 = 0.01f * fRec192[0];
    fRec42[0] = fTemp122;
    float fTemp832 = fRec41 + fTemp122;
    float fTemp833 = 0.16666667f * fRec0[0];
    float fTemp834 = fRec39 + fTemp746;
    float fTemp835 = 0.16666667f * fRec0[0];
    float fTemp836 = fRec37 + fTemp544;
    float fTemp837 = 0.5f * (1.0f - fRec0[0]);
    float fTemp838 = fRec35 + fTemp338;
    fRec211[0] = fControl[39] + fConst1 * fRec211[1];
    outputs[0] =
        FAUSTFLOAT(fRec211[0] * (fTemp838 * std::sqrt(1.0f - fTemp837) +
                                 fTemp836 * std::sqrt(fTemp835 + 0.5f) +
                                 fTemp834 * std::sqrt(0.5f - fTemp833) +
                                 fTemp832 * std::sqrt(1.0f - fTemp0)));
    outputs[1] =
        FAUSTFLOAT(fRec211[0] * (fTemp838 * std::sqrt(fTemp837) +
                                 fTemp836 * std::sqrt(0.5f - fTemp835) +
                                 fTemp834 * std::sqrt(fTemp833 + 0.5f) +
                                 fTemp832 * std::sqrt(fTemp0)));
    fRec0[1] = fRec0[0];
    fVec0[1] = fVec0[0];
    fRec3[1] = fRec3[0];
    fRec4[1] = fRec4[0];
    fRec5[1] = fRec5[0];
    fRec6[1] = fRec6[0];
    iRec8[1] = iRec8[0];
    for (int j0 = 3; j0 > 0; j0 = j0 - 1) {
      fRec7[j0] = fRec7[j0 - 1];
    }
    fRec9[1] = fRec9[0];
    fRec11[1] = fRec11[0];
    fVec1[1] = fVec1[0];
    fVec2[1] = fVec2[0];
    fRec12[1] = fRec12[0];
    fRec13[1] = fRec13[0];
    fRec10[1] = fRec10[0];
    IOTA0 = IOTA0 + 1;
    fRec14[1] = fRec14[0];
    fRec15[1] = fRec15[0];
    fRec16[1] = fRec16[0];
    fRec17[1] = fRec17[0];
    fRec18[1] = fRec18[0];
    fVec5[1] = fVec5[0];
    fVec6[1] = fVec6[0];
    for (int j1 = 3; j1 > 0; j1 = j1 - 1) {
      fRec2[j1] = fRec2[j1 - 1];
    }
    fRec20[1] = fRec20[0];
    fVec8[1] = fVec8[0];
    fVec9[1] = fVec9[0];
    for (int j2 = 3; j2 > 0; j2 = j2 - 1) {
      fRec19[j2] = fRec19[j2 - 1];
    }
    fRec22[1] = fRec22[0];
    fVec11[1] = fVec11[0];
    fVec12[1] = fVec12[0];
    for (int j3 = 3; j3 > 0; j3 = j3 - 1) {
      fRec21[j3] = fRec21[j3 - 1];
    }
    fRec24[1] = fRec24[0];
    fVec14[1] = fVec14[0];
    fVec15[1] = fVec15[0];
    for (int j4 = 3; j4 > 0; j4 = j4 - 1) {
      fRec23[j4] = fRec23[j4 - 1];
    }
    fRec26[1] = fRec26[0];
    fVec17[1] = fVec17[0];
    fVec18[1] = fVec18[0];
    for (int j5 = 3; j5 > 0; j5 = j5 - 1) {
      fRec25[j5] = fRec25[j5 - 1];
    }
    fRec28[1] = fRec28[0];
    fVec20[1] = fVec20[0];
    fVec21[1] = fVec21[0];
    for (int j6 = 3; j6 > 0; j6 = j6 - 1) {
      fRec27[j6] = fRec27[j6 - 1];
    }
    fRec30[1] = fRec30[0];
    fVec23[1] = fVec23[0];
    fVec24[1] = fVec24[0];
    for (int j7 = 3; j7 > 0; j7 = j7 - 1) {
      fRec29[j7] = fRec29[j7 - 1];
    }
    fRec32[1] = fRec32[0];
    fVec26[1] = fVec26[0];
    fVec27[1] = fVec27[0];
    for (int j8 = 3; j8 > 0; j8 = j8 - 1) {
      fRec31[j8] = fRec31[j8 - 1];
    }
    fRec34[1] = fRec34[0];
    fVec29[1] = fVec29[0];
    fVec30[1] = fVec30[0];
    for (int j9 = 3; j9 > 0; j9 = j9 - 1) {
      fRec33[j9] = fRec33[j9 - 1];
    }
    fVec31[1] = fVec31[0];
    fRec1[1] = fRec1[0];
    fVec32[1] = fVec32[0];
    fRec45[1] = fRec45[0];
    fRec46[1] = fRec46[0];
    fVec33[1] = fVec33[0];
    fVec34[1] = fVec34[0];
    fRec48[1] = fRec48[0];
    fRec49[1] = fRec49[0];
    fRec47[1] = fRec47[0];
    fRec50[1] = fRec50[0];
    fRec54[1] = fRec54[0];
    fVec36[1] = fVec36[0];
    fRec53[1] = fRec53[0];
    fRec55[1] = fRec55[0];
    fRec52[2] = fRec52[1];
    fRec52[1] = fRec52[0];
    fRec51[2] = fRec51[1];
    fRec51[1] = fRec51[0];
    fVec37[1] = fVec37[0];
    fRec58[1] = fRec58[0];
    fRec59[1] = fRec59[0];
    fRec57[2] = fRec57[1];
    fRec57[1] = fRec57[0];
    fRec56[2] = fRec56[1];
    fRec56[1] = fRec56[0];
    fVec38[1] = fVec38[0];
    fRec62[1] = fRec62[0];
    fRec63[1] = fRec63[0];
    fRec61[2] = fRec61[1];
    fRec61[1] = fRec61[0];
    fRec60[2] = fRec60[1];
    fRec60[1] = fRec60[0];
    fRec64[1] = fRec64[0];
    fVec40[1] = fVec40[0];
    fVec41[1] = fVec41[0];
    for (int j10 = 3; j10 > 0; j10 = j10 - 1) {
      fRec44[j10] = fRec44[j10 - 1];
    }
    fRec66[1] = fRec66[0];
    fVec43[1] = fVec43[0];
    fVec44[1] = fVec44[0];
    for (int j11 = 3; j11 > 0; j11 = j11 - 1) {
      fRec65[j11] = fRec65[j11 - 1];
    }
    fRec68[1] = fRec68[0];
    fVec46[1] = fVec46[0];
    fVec47[1] = fVec47[0];
    for (int j12 = 3; j12 > 0; j12 = j12 - 1) {
      fRec67[j12] = fRec67[j12 - 1];
    }
    fRec70[1] = fRec70[0];
    fVec49[1] = fVec49[0];
    fVec50[1] = fVec50[0];
    for (int j13 = 3; j13 > 0; j13 = j13 - 1) {
      fRec69[j13] = fRec69[j13 - 1];
    }
    fRec72[1] = fRec72[0];
    fVec52[1] = fVec52[0];
    fVec53[1] = fVec53[0];
    for (int j14 = 3; j14 > 0; j14 = j14 - 1) {
      fRec71[j14] = fRec71[j14 - 1];
    }
    fRec74[1] = fRec74[0];
    fVec55[1] = fVec55[0];
    fVec56[1] = fVec56[0];
    for (int j15 = 3; j15 > 0; j15 = j15 - 1) {
      fRec73[j15] = fRec73[j15 - 1];
    }
    fRec76[1] = fRec76[0];
    fVec58[1] = fVec58[0];
    fVec59[1] = fVec59[0];
    for (int j16 = 3; j16 > 0; j16 = j16 - 1) {
      fRec75[j16] = fRec75[j16 - 1];
    }
    fRec78[1] = fRec78[0];
    fVec61[1] = fVec61[0];
    fVec62[1] = fVec62[0];
    for (int j17 = 3; j17 > 0; j17 = j17 - 1) {
      fRec77[j17] = fRec77[j17 - 1];
    }
    fRec80[1] = fRec80[0];
    fVec64[1] = fVec64[0];
    fVec65[1] = fVec65[0];
    for (int j18 = 3; j18 > 0; j18 = j18 - 1) {
      fRec79[j18] = fRec79[j18 - 1];
    }
    fVec66[1] = fVec66[0];
    fRec43[1] = fRec43[0];
    fRec83[1] = fRec83[0];
    fVec68[1] = fVec68[0];
    fVec69[1] = fVec69[0];
    for (int j19 = 3; j19 > 0; j19 = j19 - 1) {
      fRec82[j19] = fRec82[j19 - 1];
    }
    fRec85[1] = fRec85[0];
    fVec71[1] = fVec71[0];
    fVec72[1] = fVec72[0];
    for (int j20 = 3; j20 > 0; j20 = j20 - 1) {
      fRec84[j20] = fRec84[j20 - 1];
    }
    fRec87[1] = fRec87[0];
    fVec74[1] = fVec74[0];
    fVec75[1] = fVec75[0];
    for (int j21 = 3; j21 > 0; j21 = j21 - 1) {
      fRec86[j21] = fRec86[j21 - 1];
    }
    fRec89[1] = fRec89[0];
    fVec77[1] = fVec77[0];
    fVec78[1] = fVec78[0];
    for (int j22 = 3; j22 > 0; j22 = j22 - 1) {
      fRec88[j22] = fRec88[j22 - 1];
    }
    fRec91[1] = fRec91[0];
    fVec80[1] = fVec80[0];
    fVec81[1] = fVec81[0];
    for (int j23 = 3; j23 > 0; j23 = j23 - 1) {
      fRec90[j23] = fRec90[j23 - 1];
    }
    fRec93[1] = fRec93[0];
    fVec83[1] = fVec83[0];
    fVec84[1] = fVec84[0];
    for (int j24 = 3; j24 > 0; j24 = j24 - 1) {
      fRec92[j24] = fRec92[j24 - 1];
    }
    fRec95[1] = fRec95[0];
    fVec86[1] = fVec86[0];
    fVec87[1] = fVec87[0];
    for (int j25 = 3; j25 > 0; j25 = j25 - 1) {
      fRec94[j25] = fRec94[j25 - 1];
    }
    fRec97[1] = fRec97[0];
    fVec89[1] = fVec89[0];
    fVec90[1] = fVec90[0];
    for (int j26 = 3; j26 > 0; j26 = j26 - 1) {
      fRec96[j26] = fRec96[j26 - 1];
    }
    fRec99[1] = fRec99[0];
    fVec92[1] = fVec92[0];
    fVec93[1] = fVec93[0];
    for (int j27 = 3; j27 > 0; j27 = j27 - 1) {
      fRec98[j27] = fRec98[j27 - 1];
    }
    fVec94[1] = fVec94[0];
    fRec81[1] = fRec81[0];
    fRec36[1] = fRec36[0];
    fVec95[1] = fVec95[0];
    fRec102[1] = fRec102[0];
    fRec103[1] = fRec103[0];
    fVec96[1] = fVec96[0];
    fVec97[1] = fVec97[0];
    fRec105[1] = fRec105[0];
    fRec106[1] = fRec106[0];
    fRec104[1] = fRec104[0];
    fRec107[1] = fRec107[0];
    fVec99[1] = fVec99[0];
    fRec110[1] = fRec110[0];
    fRec111[1] = fRec111[0];
    fRec109[2] = fRec109[1];
    fRec109[1] = fRec109[0];
    fRec108[2] = fRec108[1];
    fRec108[1] = fRec108[0];
    fRec112[1] = fRec112[0];
    fVec101[1] = fVec101[0];
    fVec102[1] = fVec102[0];
    for (int j28 = 3; j28 > 0; j28 = j28 - 1) {
      fRec101[j28] = fRec101[j28 - 1];
    }
    fRec114[1] = fRec114[0];
    fVec104[1] = fVec104[0];
    fVec105[1] = fVec105[0];
    for (int j29 = 3; j29 > 0; j29 = j29 - 1) {
      fRec113[j29] = fRec113[j29 - 1];
    }
    fRec116[1] = fRec116[0];
    fVec107[1] = fVec107[0];
    fVec108[1] = fVec108[0];
    for (int j30 = 3; j30 > 0; j30 = j30 - 1) {
      fRec115[j30] = fRec115[j30 - 1];
    }
    fRec118[1] = fRec118[0];
    fVec110[1] = fVec110[0];
    fVec111[1] = fVec111[0];
    for (int j31 = 3; j31 > 0; j31 = j31 - 1) {
      fRec117[j31] = fRec117[j31 - 1];
    }
    fRec120[1] = fRec120[0];
    fVec113[1] = fVec113[0];
    fVec114[1] = fVec114[0];
    for (int j32 = 3; j32 > 0; j32 = j32 - 1) {
      fRec119[j32] = fRec119[j32 - 1];
    }
    fRec122[1] = fRec122[0];
    fVec116[1] = fVec116[0];
    fVec117[1] = fVec117[0];
    for (int j33 = 3; j33 > 0; j33 = j33 - 1) {
      fRec121[j33] = fRec121[j33 - 1];
    }
    fRec124[1] = fRec124[0];
    fVec119[1] = fVec119[0];
    fVec120[1] = fVec120[0];
    for (int j34 = 3; j34 > 0; j34 = j34 - 1) {
      fRec123[j34] = fRec123[j34 - 1];
    }
    fRec126[1] = fRec126[0];
    fVec122[1] = fVec122[0];
    fVec123[1] = fVec123[0];
    for (int j35 = 3; j35 > 0; j35 = j35 - 1) {
      fRec125[j35] = fRec125[j35 - 1];
    }
    fRec128[1] = fRec128[0];
    fVec125[1] = fVec125[0];
    fVec126[1] = fVec126[0];
    for (int j36 = 3; j36 > 0; j36 = j36 - 1) {
      fRec127[j36] = fRec127[j36 - 1];
    }
    fVec127[1] = fVec127[0];
    fRec100[1] = fRec100[0];
    fRec131[1] = fRec131[0];
    fVec129[1] = fVec129[0];
    fVec130[1] = fVec130[0];
    for (int j37 = 3; j37 > 0; j37 = j37 - 1) {
      fRec130[j37] = fRec130[j37 - 1];
    }
    fRec133[1] = fRec133[0];
    fVec132[1] = fVec132[0];
    fVec133[1] = fVec133[0];
    for (int j38 = 3; j38 > 0; j38 = j38 - 1) {
      fRec132[j38] = fRec132[j38 - 1];
    }
    fRec135[1] = fRec135[0];
    fVec135[1] = fVec135[0];
    fVec136[1] = fVec136[0];
    for (int j39 = 3; j39 > 0; j39 = j39 - 1) {
      fRec134[j39] = fRec134[j39 - 1];
    }
    fRec137[1] = fRec137[0];
    fVec138[1] = fVec138[0];
    fVec139[1] = fVec139[0];
    for (int j40 = 3; j40 > 0; j40 = j40 - 1) {
      fRec136[j40] = fRec136[j40 - 1];
    }
    fRec139[1] = fRec139[0];
    fVec141[1] = fVec141[0];
    fVec142[1] = fVec142[0];
    for (int j41 = 3; j41 > 0; j41 = j41 - 1) {
      fRec138[j41] = fRec138[j41 - 1];
    }
    fRec141[1] = fRec141[0];
    fVec144[1] = fVec144[0];
    fVec145[1] = fVec145[0];
    for (int j42 = 3; j42 > 0; j42 = j42 - 1) {
      fRec140[j42] = fRec140[j42 - 1];
    }
    fRec143[1] = fRec143[0];
    fVec147[1] = fVec147[0];
    fVec148[1] = fVec148[0];
    for (int j43 = 3; j43 > 0; j43 = j43 - 1) {
      fRec142[j43] = fRec142[j43 - 1];
    }
    fRec145[1] = fRec145[0];
    fVec150[1] = fVec150[0];
    fVec151[1] = fVec151[0];
    for (int j44 = 3; j44 > 0; j44 = j44 - 1) {
      fRec144[j44] = fRec144[j44 - 1];
    }
    fRec147[1] = fRec147[0];
    fVec153[1] = fVec153[0];
    fVec154[1] = fVec154[0];
    for (int j45 = 3; j45 > 0; j45 = j45 - 1) {
      fRec146[j45] = fRec146[j45 - 1];
    }
    fVec155[1] = fVec155[0];
    fRec129[1] = fRec129[0];
    fRec38[1] = fRec38[0];
    fVec156[1] = fVec156[0];
    fRec150[1] = fRec150[0];
    fRec151[1] = fRec151[0];
    fVec157[1] = fVec157[0];
    fVec158[1] = fVec158[0];
    fRec153[1] = fRec153[0];
    fRec154[1] = fRec154[0];
    fRec152[1] = fRec152[0];
    fRec155[1] = fRec155[0];
    fRec156[1] = fRec156[0];
    fVec161[1] = fVec161[0];
    fVec162[1] = fVec162[0];
    for (int j46 = 3; j46 > 0; j46 = j46 - 1) {
      fRec149[j46] = fRec149[j46 - 1];
    }
    fRec158[1] = fRec158[0];
    fVec164[1] = fVec164[0];
    fVec165[1] = fVec165[0];
    for (int j47 = 3; j47 > 0; j47 = j47 - 1) {
      fRec157[j47] = fRec157[j47 - 1];
    }
    fRec160[1] = fRec160[0];
    fVec167[1] = fVec167[0];
    fVec168[1] = fVec168[0];
    for (int j48 = 3; j48 > 0; j48 = j48 - 1) {
      fRec159[j48] = fRec159[j48 - 1];
    }
    fRec162[1] = fRec162[0];
    fVec170[1] = fVec170[0];
    fVec171[1] = fVec171[0];
    for (int j49 = 3; j49 > 0; j49 = j49 - 1) {
      fRec161[j49] = fRec161[j49 - 1];
    }
    fRec164[1] = fRec164[0];
    fVec173[1] = fVec173[0];
    fVec174[1] = fVec174[0];
    for (int j50 = 3; j50 > 0; j50 = j50 - 1) {
      fRec163[j50] = fRec163[j50 - 1];
    }
    fRec166[1] = fRec166[0];
    fVec176[1] = fVec176[0];
    fVec177[1] = fVec177[0];
    for (int j51 = 3; j51 > 0; j51 = j51 - 1) {
      fRec165[j51] = fRec165[j51 - 1];
    }
    fRec168[1] = fRec168[0];
    fVec179[1] = fVec179[0];
    fVec180[1] = fVec180[0];
    for (int j52 = 3; j52 > 0; j52 = j52 - 1) {
      fRec167[j52] = fRec167[j52 - 1];
    }
    fRec170[1] = fRec170[0];
    fVec182[1] = fVec182[0];
    fVec183[1] = fVec183[0];
    for (int j53 = 3; j53 > 0; j53 = j53 - 1) {
      fRec169[j53] = fRec169[j53 - 1];
    }
    fRec172[1] = fRec172[0];
    fVec185[1] = fVec185[0];
    fVec186[1] = fVec186[0];
    for (int j54 = 3; j54 > 0; j54 = j54 - 1) {
      fRec171[j54] = fRec171[j54 - 1];
    }
    fVec187[1] = fVec187[0];
    fRec148[1] = fRec148[0];
    fRec175[1] = fRec175[0];
    fVec189[1] = fVec189[0];
    fVec190[1] = fVec190[0];
    for (int j55 = 3; j55 > 0; j55 = j55 - 1) {
      fRec174[j55] = fRec174[j55 - 1];
    }
    fRec177[1] = fRec177[0];
    fVec192[1] = fVec192[0];
    fVec193[1] = fVec193[0];
    for (int j56 = 3; j56 > 0; j56 = j56 - 1) {
      fRec176[j56] = fRec176[j56 - 1];
    }
    fRec179[1] = fRec179[0];
    fVec195[1] = fVec195[0];
    fVec196[1] = fVec196[0];
    for (int j57 = 3; j57 > 0; j57 = j57 - 1) {
      fRec178[j57] = fRec178[j57 - 1];
    }
    fRec181[1] = fRec181[0];
    fVec198[1] = fVec198[0];
    fVec199[1] = fVec199[0];
    for (int j58 = 3; j58 > 0; j58 = j58 - 1) {
      fRec180[j58] = fRec180[j58 - 1];
    }
    fRec183[1] = fRec183[0];
    fVec201[1] = fVec201[0];
    fVec202[1] = fVec202[0];
    for (int j59 = 3; j59 > 0; j59 = j59 - 1) {
      fRec182[j59] = fRec182[j59 - 1];
    }
    fRec185[1] = fRec185[0];
    fVec204[1] = fVec204[0];
    fVec205[1] = fVec205[0];
    for (int j60 = 3; j60 > 0; j60 = j60 - 1) {
      fRec184[j60] = fRec184[j60 - 1];
    }
    fRec187[1] = fRec187[0];
    fVec207[1] = fVec207[0];
    fVec208[1] = fVec208[0];
    for (int j61 = 3; j61 > 0; j61 = j61 - 1) {
      fRec186[j61] = fRec186[j61 - 1];
    }
    fRec189[1] = fRec189[0];
    fVec210[1] = fVec210[0];
    fVec211[1] = fVec211[0];
    for (int j62 = 3; j62 > 0; j62 = j62 - 1) {
      fRec188[j62] = fRec188[j62 - 1];
    }
    fRec191[1] = fRec191[0];
    fVec213[1] = fVec213[0];
    fVec214[1] = fVec214[0];
    for (int j63 = 3; j63 > 0; j63 = j63 - 1) {
      fRec190[j63] = fRec190[j63 - 1];
    }
    fVec215[1] = fVec215[0];
    fRec173[1] = fRec173[0];
    fRec40[1] = fRec40[0];
    fRec194[1] = fRec194[0];
    fVec217[1] = fVec217[0];
    fVec218[1] = fVec218[0];
    for (int j64 = 3; j64 > 0; j64 = j64 - 1) {
      fRec193[j64] = fRec193[j64 - 1];
    }
    fRec196[1] = fRec196[0];
    fVec220[1] = fVec220[0];
    fVec221[1] = fVec221[0];
    for (int j65 = 3; j65 > 0; j65 = j65 - 1) {
      fRec195[j65] = fRec195[j65 - 1];
    }
    fRec198[1] = fRec198[0];
    fVec223[1] = fVec223[0];
    fVec224[1] = fVec224[0];
    for (int j66 = 3; j66 > 0; j66 = j66 - 1) {
      fRec197[j66] = fRec197[j66 - 1];
    }
    fRec200[1] = fRec200[0];
    fVec226[1] = fVec226[0];
    fVec227[1] = fVec227[0];
    for (int j67 = 3; j67 > 0; j67 = j67 - 1) {
      fRec199[j67] = fRec199[j67 - 1];
    }
    fRec202[1] = fRec202[0];
    fVec229[1] = fVec229[0];
    fVec230[1] = fVec230[0];
    for (int j68 = 3; j68 > 0; j68 = j68 - 1) {
      fRec201[j68] = fRec201[j68 - 1];
    }
    fRec204[1] = fRec204[0];
    fVec232[1] = fVec232[0];
    fVec233[1] = fVec233[0];
    for (int j69 = 3; j69 > 0; j69 = j69 - 1) {
      fRec203[j69] = fRec203[j69 - 1];
    }
    fRec206[1] = fRec206[0];
    fVec235[1] = fVec235[0];
    fVec236[1] = fVec236[0];
    for (int j70 = 3; j70 > 0; j70 = j70 - 1) {
      fRec205[j70] = fRec205[j70 - 1];
    }
    fRec208[1] = fRec208[0];
    fVec238[1] = fVec238[0];
    fVec239[1] = fVec239[0];
    for (int j71 = 3; j71 > 0; j71 = j71 - 1) {
      fRec207[j71] = fRec207[j71 - 1];
    }
    fRec210[1] = fRec210[0];
    fVec241[1] = fVec241[0];
    fVec242[1] = fVec242[0];
    for (int j72 = 3; j72 > 0; j72 = j72 - 1) {
      fRec209[j72] = fRec209[j72 - 1];
    }
    fVec243[1] = fVec243[0];
    fRec192[1] = fRec192[0];
    fRec42[1] = fRec42[0];
    fRec211[1] = fRec211[0];
  }
};

/***************************END USER SECTION ***************************/

/*******************BEGIN ARCHITECTURE SECTION (part 2/2)***************/

#define CONTROL_RATE_HZ 100
#define DEFAULT_SR 44100

template <int VOICES> struct tamburaModule : Module {

  LayoutUI fLayoutUI;
  RackUI<VOICES> *fRackUI; // One single version for all VOICES
  tambura fDSP[VOICES];
  int fControlCounter;
#ifdef SOUNDFILE
  SoundUI *fSoundUI;
#endif
  tamburaModule() {
    // Count items of button, nentry, bargraph categories
    ManagerUI params;
    fDSP[0].buildUserInterface(&params);
#ifdef SOUNDFILE
    fSoundUI = new SoundUI(rack::asset::plugin(pluginInstance, "res/"));
#endif
    // Controllers are connected to all VOICES
    fRackUI = new RackUI<VOICES>(params);
    for (int v = 0; v < VOICES; v++) {
      fRackUI->fCurVoice = v;
      fDSP[v].buildUserInterface(fRackUI);
      fRackUI->reset();
#ifdef SOUNDFILE
      fDSP[v].buildUserInterface(fSoundUI);
#endif
    }

    uint buttons = params.fButtons.size();
    uint entries = params.fRanges.size();
    uint bargraphs = params.fBargraph.size();
    uint inputCV = params.fInputCV.size();
    uint outputCV = params.fOutputCV.size();

    // Config: by default we allocate complete set of parameters, even if all of
    // them are not 'connected' using metadata
    config(buttons + entries, inputCV + fDSP[0].getNumInputs(),
           outputCV + fDSP[0].getNumOutputs(), bargraphs);

    // Setup buttons
    for (uint pa = 0; pa < buttons; pa++) {
      configParam(pa, 0.f, 1.f, 0.f, "");
    }
    // Setup range params min/max/init values
    for (uint pa = 0; pa < entries; pa++) {
      configParam(pa + buttons, params.fRanges[pa].fMin,
                  params.fRanges[pa].fMax, params.fRanges[pa].fInit, "");
    }

    for (int v = 0; v < VOICES; v++) {
      // Init control zones
      fDSP[v].initControl();
      // Init DSP with default SR
      fDSP[v].init(DEFAULT_SR);
    }

    // So that control update will be done at first cycle
    fControlCounter = 1;

    // Set items minimal size
    gItemSize.kVSliderWidth = 20.0;
    gItemSize.kVSliderHeight = 20.0;

    gItemSize.kHSliderWidth = 20.0;
    gItemSize.kHSliderHeight = 20.0;

    gItemSize.kButtonWidth = 20.0;
    gItemSize.kButtonHeight = 20.0;

    gItemSize.kCheckButtonWidth = 20.0;
    gItemSize.kCheckButtonWidth = 20.0;
  }

  ~tamburaModule() {
    delete fRackUI;
#ifdef SOUNDFILE
    delete fSoundUI;
#endif
  }

  void process(const ProcessArgs &args) override {
    // Possibly update SR
    if (args.sampleRate != fDSP[0].getSampleRate()) {
      for (int v = 0; v < VOICES; v++) {
        fDSP[v].init(args.sampleRate);
      }
    }

    // Update control and inputs controllers at CONTROL_RATE_HZ
    if (--fControlCounter == 0) {
      // DSP Control
      for (int v = 0; v < VOICES; v++) {
        fDSP[v].control(fDSP[v].iZone, fDSP[v].fZone);
      }
      // Controller update
      fRackUI->updateInputs(this);
    }

    // Setup polyphony for CV inputs
    uint inputsCV = fRackUI->fParams.fInputCV.size();
    for (uint chan = 0; chan < inputsCV; chan++) {
      inputs[chan].setChannels(VOICES);
    }

    // Setup polyphony for audio inputs (shifted by inputsCV in 'inputs' struct)
    for (int chan = 0; chan < fDSP[0].getNumInputs(); chan++) {
      inputs[chan + inputsCV].setChannels(VOICES);
    }

    // Setup polyphony for CV outputs
    uint outputsCV = fRackUI->fParams.fOutputCV.size();
    for (uint chan = 0; chan < outputsCV; chan++) {
      outputs[chan].setChannels(VOICES);
    }

    // Setup polyphony for audio outputs (shifted by outputsCV in 'outputs'
    // struct)
    for (int chan = 0; chan < fDSP[0].getNumOutputs(); chan++) {
      outputs[chan + outputsCV].setChannels(VOICES);
    }

    FAUSTFLOAT *inputs_aux = static_cast<FAUSTFLOAT *>(
        alloca(fDSP[0].getNumInputs() * sizeof(FAUSTFLOAT)));
    FAUSTFLOAT *outputs_aux = static_cast<FAUSTFLOAT *>(
        alloca(fDSP[0].getNumOutputs() * sizeof(FAUSTFLOAT)));

    // Compute all VOICES
    for (int v = 0; v < VOICES; v++) {

      // Copy inputs
      for (int chan = 0; chan < fDSP[0].getNumInputs(); chan++) {
        inputs_aux[chan] =
            ((VOICES == 1) ? inputs[chan + inputsCV].getVoltageSum()
                           : inputs[chan + inputsCV].getVoltage(v)) /
            5.0f;
      }

      // One sample compute
      fDSP[v].compute(inputs_aux, outputs_aux, fDSP[v].iZone, fDSP[v].fZone);

      // Copy outputs
      for (int chan = 0; chan < fDSP[0].getNumOutputs(); chan++) {
        outputs[chan + outputsCV].setVoltage(outputs_aux[chan] * 5.0f, v);
      }
    }

    // Update output controllers at CONTROL_RATE_HZ
    if (fControlCounter == 0) {
      fRackUI->updateOutputs(this);
      fControlCounter = args.sampleRate / CONTROL_RATE_HZ;
    }
  }

  void getMinimumSize(float &width, float &height) {
    // Set items minimal size
    gItemSize.kHSliderWidth = 50.0;
    gItemSize.kVSliderWidth = 50.0;

    gItemSize.kButtonWidth = 50.0;
    gItemSize.kButtonHeight = 50.0;

    // Prepare layout
    fDSP[0].buildUserInterface(&fLayoutUI);

    cout << "==========================" << endl;
    for (const auto &it : fLayoutUI.fPathItemMap) {
      cout << it.first << endl;
      cout << it.second << endl;
    }

    cout << "Width " << fLayoutUI.fCurrentGroup->getWidth() << endl;
    cout << "Height " << fLayoutUI.fCurrentGroup->getHeight() << endl;

    width = fLayoutUI.fCurrentGroup->getWidth();
    height = fLayoutUI.fCurrentGroup->getHeight();
  }

  void setSize(float x_pos, float y_pos, float width, float height) {
    fLayoutUI.fCurrentGroup->setSize(width, height);
    fLayoutUI.fCurrentGroup->setPos(x_pos, y_pos);

    cout << "==========================" << endl;
    for (const auto &it : fLayoutUI.fPathItemMap) {
      cout << it.first << endl;
      cout << it.second << endl;
    }

    cout << "Width " << fLayoutUI.fCurrentGroup->getWidth() << endl;
    cout << "Height " << fLayoutUI.fCurrentGroup->getHeight() << endl;
  }
};

// Draw a opaque background color
struct FaustBackgroundWidget : widget::Widget {

  std::shared_ptr<Font> fFont;

  FaustBackgroundWidget(std::shared_ptr<Font> font) { fFont = font; }

  void draw(const DrawArgs &args) override {
    nvgBeginPath(args.vg);
    nvgRect(args.vg, 0, 0, box.size.x, box.size.y);
    // nvgRGB(234,88,15)); // Faust orange color
    nvgFillColor(args.vg,
                 nvgRGB(254, 108, 35)); // Faust orange color + 20 white
    nvgFill(args.vg);
  }
};

template <int VOICES> struct tamburaModuleWidget : ModuleWidget {

  std::shared_ptr<Font> fFont;

  void addBackground(int width, int height) {
    // Default size
    box.size.x = mm2px(300);
    box.size.y = height;
    FaustBackgroundWidget *wd = new FaustBackgroundWidget(fFont);
    wd->setSize(Vec(box.size.x, box.size.y));
    addChild(wd);

    // General title
    addLabel(Vec(width / 2.0 - 32, 10.0), "Faust", 20);

    addChild(createWidget<ScrewSilver>(Vec(RACK_GRID_WIDTH, 0)));
    addChild(
        createWidget<ScrewSilver>(Vec(box.size.x - 2 * RACK_GRID_WIDTH, 0)));
    addChild(createWidget<ScrewSilver>(
        Vec(RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
    addChild(createWidget<ScrewSilver>(Vec(
        box.size.x - 2 * RACK_GRID_WIDTH, RACK_GRID_HEIGHT - RACK_GRID_WIDTH)));
  }

  tamburaModuleWidget(tamburaModule<VOICES> *module) {

    // fFont = APP->window->loadFont(asset::plugin(pluginInstance,
    // "res/Lato-HairlineItalic.ttf"));
    bndSetFont(0);
    setModule(module);

    // Set a large SVG
    // setPanel(APP->window->loadSvg(asset::plugin(pluginInstance,
    // "res/FaustModule.svg")));

    // Module is null at plugins selection step, so we cannot create the final
    // GUI at that time
    if (module) {

      // Compute available size by removing space for CV inputs/outputs and
      // audio inputs/outputs
      uint buttons = module->fRackUI->fParams.fButtons.size();
      uint nentries = module->fRackUI->fParams.fRanges.size();
      uint bargraphs = module->fRackUI->fParams.fBargraph.size();
      uint inputsCV = module->fRackUI->fParams.fInputCV.size();
      uint outputsCV = module->fRackUI->fParams.fOutputCV.size();

      float item_width = 18;
      float item_height = 20;

      /*
      uint reserved_height = ((inputsCV > 0) ? 10 : 0)
              + ((outputsCV > 0) ? 10 : 0)
              + ((module->fDSP[0].getNumInputs() > 0) ? 10 : 0)
              + ((module->fDSP[0].getNumOutputs() > 0) ? 10 : 0);
      */

      vector<int> items = {2,
                           int(bargraphs),
                           int(buttons),
                           int(nentries),
                           module->fDSP[0].getNumInputs(),
                           module->fDSP[0].getNumOutputs()};
      sort(items.begin(), items.end(), greater<int>());
      int needed_width = mm2px(items[0] * item_width);
      addBackground(std::max(int(RACK_GRID_WIDTH), int(needed_width)),
                    RACK_GRID_HEIGHT);

      /*
      // TODO

      // Get UI minimum size
      float minimal_width, minimal_height;
      module->getMinimumSize(minimal_width, minimal_height);

      // Then prepare the size of UI params
      module->setSize(5.f, 5.f, minimal_width, 100 - reserved_height);

      // Add params
      //addLabel(mm2px(Vec(6, 10.0)), "Params");
      */

      // Add buttons
      for (uint pa = 0; pa < buttons; pa++) {
        std::string label = module->fRackUI->fParams.fButtons[pa].fLabel;
        if (module->fRackUI->fParams.fButtons[pa].fType ==
            ManagerUI::UIType::kButton) {
          addParam(createParamCentered<BefacoPush>(
              mm2px(Vec(8.0 + pa * item_width, item_height)), module, pa));
        } else {
          addParam(createParamCentered<CKSS>(
              mm2px(Vec(8.0 + pa * item_width, item_height)), module, pa));
        }
        addLabel(mm2px(Vec(0.0 + pa * item_width, item_height + 5.0)), label);
      }

      // Add ranges (sliders, nentry)
      for (uint pa = 0; pa < nentries; pa++) {
        std::string label = module->fRackUI->fParams.fRanges[pa].fLabel;
        addParam(createParamCentered<RoundBlackKnob>(
            mm2px(Vec(8.0 + pa * item_width, item_height * 2)), module,
            pa + buttons));
        addLabel(mm2px(Vec(0.0 + pa * item_width, item_height * 2 + 5.0)),
                 label);
      }

      // Add bargraph
      for (uint pa = 0; pa < bargraphs; pa++) {
        std::string label = module->fRackUI->fParams.fBargraph[pa].fLabel;
        addChild(createLightCentered<LargeLight<RedLight>>(
            mm2px(Vec(8.0 + pa * item_width, item_height * 3)), module, pa));
        addLabel(mm2px(Vec(0.0 + pa * item_width, item_height * 3 + 5.0)),
                 label);
      }

      // Add CV inputs
      if (inputsCV > 0) {
        addLabel(mm2px(Vec(0.0, 55.0)), "Inputs CV");
        for (uint chan = 0; chan < inputsCV; chan++) {
          addInput(createInputCentered<PJ301MPort>(
              mm2px(Vec(8.0 + chan * 15, 66.0)), module, chan));
        }
      }

      // Add audio inputs (shifted by inputsCV in 'inputs' struct)
      if (module->fDSP[0].getNumInputs() > 0) {
        addLabel(mm2px(Vec(0.0, 72.0)), "Inputs");
        for (int chan = 0; chan < module->fDSP[0].getNumInputs(); chan++) {
          addInput(createInputCentered<PJ301MPort>(
              mm2px(Vec(8.0 + chan * 15, 83.0)), module, inputsCV + chan));
        }
      }

      // Add CV outputs
      if (outputsCV > 0) {
        addLabel(mm2px(Vec(0.0, 89.0)), "Outputs CV");
        for (uint chan = 0; chan < outputsCV; chan++) {
          addOutput(createOutputCentered<PJ301MPort>(
              mm2px(Vec(8.0 + chan * 15, 100.0)), module, chan));
        }
      }

      // Add outputs (shifted by outputsCV in 'outputs' struct)
      if (module->fDSP[0].getNumOutputs() > 0) {
        addLabel(mm2px(Vec(0.0, 106.0)), "Outputs");
        for (int chan = 0; chan < module->fDSP[0].getNumOutputs(); chan++) {
          addOutput(createOutputCentered<PJ301MPort>(
              mm2px(Vec(8.0 + chan * 15, 117.0)), module, outputsCV + chan));
        }
      }
    } else {
      addBackground(RACK_GRID_WIDTH * 5, RACK_GRID_HEIGHT);
    }
  }

  // TODO: use nvgText
  // https://community.vcvrack.com/t/advanced-nanovg-custom-label/6769
  // https://www.1001fonts.com
  Label *addLabel(const Vec &v, const std::string &str, float fontSize = 13) {
    NVGcolor black = nvgRGB(0, 0, 0);
    Label *label = new Label();
    label->box.pos = v;
    label->text = str;
    label->color = black;
    label->fontSize = fontSize;
    // label->alignment = rack::ui::Label::RIGHT_ALIGNMENT;
    addChild(label);
    return label;
  }
};

// Create and init plugin
Model *modelFaustModule =
    createModel<tamburaModule<NVOICES>, tamburaModuleWidget<NVOICES>>(
        "tambura");

void init(Plugin *p) {
  pluginInstance = p;
  // Add modules here
  p->addModel(modelFaustModule);
}

/********************END ARCHITECTURE SECTION (part 2/2)****************/

#endif
