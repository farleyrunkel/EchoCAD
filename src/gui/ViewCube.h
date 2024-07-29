
#ifndef VIEWCUBE_H
#define VIEWCUBE_H

#include <AIS_ViewCube.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Prs3d_Arrow.hxx>
#include <Prs3d_Text.hxx>
#include <Prs3d_ToolSphere.hxx>


namespace
{
	static const Standard_Integer THE_NB_ROUND_SPLITS = 18;
	static const Standard_Integer THE_NB_DISK_SLICES = 120;
	static const Standard_Integer THE_NB_ARROW_FACETTES = 120;

	//! Return the number of non-zero components.
	static Standard_Integer nbDirectionComponents(const gp_Dir& theDir)
	{
		Standard_Integer aNbComps = 0;
		for (Standard_Integer aCompIter = 1; aCompIter <= 3; ++aCompIter)
		{
			if (Abs(theDir.Coord(aCompIter)) > gp::Resolution())
			{
				++aNbComps;
			}
		}
		return aNbComps;
	}
}

namespace echocad
{

class ViewCube : public AIS_ViewCube
{
public:
	ViewCube();

};


class ViewSphere : public AIS_InteractiveObject
{
	DEFINE_STANDARD_RTTIEXT(ViewSphere, AIS_InteractiveObject)

public:

	ViewSphere();

	void SetSize(Standard_Real theSize) { mySize = theSize; }

	void setDefaultAttributes();

	void Compute(const Handle(PrsMgr_PresentationManager)&,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode);


	void ComputeSelection(const Handle(SelectMgr_Selection)& theSelection,
		const Standard_Integer theMode);

private:

	bool myToDisplayAxes;
	Standard_Real                 myAxesRadius;        //!< radius of axes of the trihedron; 1.0 by default
	Standard_Real                 myAxesConeRadius;	//!< radius of cones of the trihedron; 1.5 by default
	Standard_Real                 mySize;			
	Standard_Real                 mySphereSize;		//!< size of the sphere; 12.0 by default

	NCollection_DataMap<Prs3d_DatumParts, TCollection_AsciiString>
		myAxesLabels;           //!< map with axes labels
	NCollection_DataMap<Prs3d_DatumParts, Graphic3d_MaterialAspect>
		myAxisMaterials;           //!< map with axes labels
	gp_Pnt myLocation;		  //!< location of the trihedron; (0, 0, 0) by default
	 
	Handle(Prs3d_ShadingAspect)  myXAxisAspect;
	Handle(Prs3d_ShadingAspect)  myYAxisAspect;
	Handle(Prs3d_ShadingAspect)  myZAxisAspect;
};

} // namespace echocad


#endif // !VIEWCUBE_H
