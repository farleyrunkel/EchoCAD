/*
##Copyright 2016 Thomas Paviot (tpaviot@gmail.com)
##
##This file is part of pythonOCC.
##
##pythonOCC is free software: you can redistribute it and/or modify
##it under the terms of the GNU Lesser General Public License as published by
##the Free Software Foundation, either version 3 of the License, or
##(at your option) any later version.
##
##pythonOCC is distributed in the hope that it will be useful,
##but WITHOUT ANY WARRANTY; without even the implied warranty of
##MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##GNU General Public License for more details.
##
##You should have received a copy of the GNU Lesser General Public License
##along with pythonOCC.  If not, see <http://www.gnu.org/licenses/>.
*/

%module(package="OCC") Temp

%include ../SWIG_files/common/ExceptionCatcher.i

%{
#include <Temp.h>
%}

//! Specifies aspect of system font.
enum tmp_Font_FontAspect
{
  Font_FontAspect_UNDEFINED = -1,
  Font_FontAspect_Regular   =  0,
  Font_FontAspect_Bold,
  Font_FontAspect_Italic,
  Font_FontAspect_BoldItalic,

  // old aliases
  Font_FA_Undefined  = Font_FontAspect_UNDEFINED,
  Font_FA_Regular    = Font_FontAspect_Regular,
  Font_FA_Bold       = Font_FontAspect_Bold,
  Font_FA_Italic     = Font_FontAspect_Italic,
  Font_FA_BoldItalic = Font_FontAspect_BoldItalic
};

%feature("compactdefaultargs") tmp_text_to_brep;
%feature("autodoc", "	* Creates a shape of the given text. 
	:param text_to_render: Text to render
	:type text_to_render: char *
	:param aFontName: Font name
	:type aFontName: char *
	:param aFontAspect:  Fontaspect, FontAspect_UNDEFINED, Font_FontAspect_Regular, Font_FontAspect_Bold, Font_FA_Italic or Font_FA_BoldItalic
	:type aFontAspect: Font_FontAspect
	:param aSize: Size of the font
	:type aSize: float
	:param isCompositeCurve:
	:type isCompositeCurve: bool
	:rtype: TopoDS_Shape
") tmp_text_to_brep;
TopoDS_Shape tmp_text_to_brep(const char* aText, const char* aName, Font_FontAspect aFontAspect, float aSize, bool anIsCompositeCurve);

%feature("compactdefaultargs") tmp_register_font;
%feature("autodoc", "	* Add a font to the available font list. 
	:param aFontPath: Path to the font.
	:type aFontPath: char *
	:param aFontAspect
	:type Font_FontAspect
	:rtype: void
") tmp_register_font;
void tmp_register_font(char* aFontPath, Font_FontAspect aFontAspect=Font_FontAspect_UNDEFINED);
