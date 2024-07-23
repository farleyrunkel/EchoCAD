
#ifndef VIEWCUBE_H
#define VIEWCUBE_H

#include <AIS_ViewCube.hxx>
#include <Prs3d_DatumAspect.hxx>
#include <Prs3d_Arrow.hxx>
#include <Prs3d_Text.hxx>
#include <Prs3d_ToolSphere.hxx>


namespace
{
	static const Standard_Integer THE_NB_ROUND_SPLITS = 8;
	static const Standard_Integer THE_NB_DISK_SLICES = 20;
	static const Standard_Integer THE_NB_ARROW_FACETTES = 20;

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

	void setDefaultAttributes()
	{
		myDrawer->TextAspect()->SetHorizontalJustification(Graphic3d_HTA_CENTER);
		myDrawer->TextAspect()->SetVerticalJustification(Graphic3d_VTA_BOTTOM);
		myDrawer->TextAspect()->SetColor(Quantity_NOC_BLACK);
		myDrawer->TextAspect()->SetFont(Font_NOF_SANS_SERIF);
		myDrawer->TextAspect()->SetHeight(12.0);
		myDrawer->TextAspect()->Aspect()->SetTextZoomable(true); // the whole object is drawn within transformation-persistence
		// this should be forced back-face culling regardless Closed flag
		myDrawer->TextAspect()->Aspect()->SetFaceCulling(Graphic3d_TypeOfBackfacingModel_BackCulled);

		myDrawer->SetFaceBoundaryDraw(false);
	}

	void Compute(const Handle(PrsMgr_PresentationManager)&,
		const Handle(Prs3d_Presentation)& thePrs,
		const Standard_Integer theMode)
	{
		// create a view sphere like blender which have 3 asix and 6 sphere
		thePrs->SetInfiniteState(true);
		if (theMode != 0)
		{
			return;
		}

		const gp_Pnt aLocation = myLocation;

		// Display axes

		if (myToDisplayAxes)
		{
			const Standard_Real anAxisSize = mySize;
			const Handle(Prs3d_DatumAspect)& aDatumAspect = myDrawer->DatumAspect();
			for (Standard_Integer anAxisIter = Prs3d_DatumParts_XAxis; anAxisIter <= Prs3d_DatumParts_ZAxis; ++anAxisIter)
			{
				const Prs3d_DatumParts aPart = (Prs3d_DatumParts)anAxisIter;
				if (!aDatumAspect->DrawDatumPart(aPart))
				{
					continue;
				}

				Graphic3d_MaterialAspect aMat(Graphic3d_NameOfMaterial_UserDefined);
				aMat.SetTransparency(0.5);
				gp_Ax1 anAx1;
				switch (aPart)
				{
				case Prs3d_DatumParts_XAxis:
					anAx1 = gp_Ax1(aLocation, gp::DX()); 
					aMat.SetColor(Quantity_NOC_RED);
					break;
				case Prs3d_DatumParts_YAxis: 
					anAx1 = gp_Ax1(aLocation, gp::DY()); 
					aMat.SetColor(Quantity_NOC_GREEN);
					break;
				case Prs3d_DatumParts_ZAxis: 
					anAx1 = gp_Ax1(aLocation, gp::DZ()); 
					aMat.SetColor(Quantity_NOC_BLUE1);
					break;
				default: break;
				}

				aDatumAspect->ShadingAspect(aPart)->Aspect()->SetFrontMaterial(aMat);

				Handle(Graphic3d_Group) anAxisGroup = thePrs->NewGroup();
				anAxisGroup->SetClosed(true);
				anAxisGroup->SetGroupPrimitivesAspect(aDatumAspect->ShadingAspect(aPart)->Aspect());

				const Standard_Real anArrowLength = 0;
				Handle(Graphic3d_ArrayOfTriangles) aTriangleArray = Prs3d_Arrow::DrawShaded(anAx1, myAxesRadius, anAxisSize, myAxesConeRadius, anArrowLength, THE_NB_ARROW_FACETTES);
				anAxisGroup->AddPrimitiveArray(aTriangleArray);

				Prs3d_ToolSphere aTool(mySphereSize, THE_NB_DISK_SLICES, THE_NB_DISK_SLICES);
				gp_Trsf aTrsf;
				gp_Pnt aTextOrigin = anAx1.Location().Translated(gp_Vec(anAx1.Direction().X() * (anAxisSize + anArrowLength),
					anAx1.Direction().Y() * (anAxisSize + anArrowLength),
					anAx1.Direction().Z() * (anAxisSize + anArrowLength)));
				aTrsf.SetTranslation(gp_Vec(gp::Origin(), aTextOrigin));
				// Display center
				{
					Handle(Graphic3d_Group) aGroup = thePrs->NewGroup();
					aGroup->SetClosed(true);
					aGroup->SetGroupPrimitivesAspect(aDatumAspect->ShadingAspect(aPart)->Aspect());
					Handle(Graphic3d_ArrayOfTriangles) aCenterArray;
					aTool.FillArray(aCenterArray, aTrsf);
					aGroup->AddPrimitiveArray(aCenterArray);
				}
				{
					Handle(Graphic3d_Group) aGroup = thePrs->NewGroup();
					aGroup->SetClosed(true);
					aGroup->SetGroupPrimitivesAspect(aDatumAspect->ShadingAspect(aPart)->Aspect());
					Handle(Graphic3d_ArrayOfTriangles) aCenterArray;
					aTool.FillArray(aCenterArray, aTrsf.Inverted());
					aGroup->AddPrimitiveArray(aCenterArray);
				}

				TCollection_AsciiString anAxisLabel;
				if (aDatumAspect->ToDrawLabels()
					&& myAxesLabels.Find(aPart, anAxisLabel)
					&& !anAxisLabel.IsEmpty())
				{
					Handle(Graphic3d_Group) anAxisLabelGroup = thePrs->NewGroup();
					gp_Pnt aTextOrigin = anAx1.Location().Translated(gp_Vec(anAx1.Direction().X() * (anAxisSize - mySphereSize / 2.0),
						anAx1.Direction().Y() * (anAxisSize - mySphereSize / 2.0),
						anAx1.Direction().Z() * (anAxisSize - mySphereSize / 2.0)));
					Prs3d_Text::Draw(anAxisLabelGroup, aDatumAspect->TextAspect(aPart), TCollection_ExtendedString(anAxisLabel), aTextOrigin);
				}
			}
		}
	}


	void ComputeSelection(const Handle(SelectMgr_Selection)& theSelection,
		const Standard_Integer theMode)
	{

	}

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
	 
	Handle(Prs3d_ShadingAspect)  myAxisAspect;
};

} // namespace echocad


#endif // !VIEWCUBE_H
