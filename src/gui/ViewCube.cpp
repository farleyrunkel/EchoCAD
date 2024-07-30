
#include "ViewCube.h"

#include <Prs3d_ToolSector.hxx>
#include <Prs3d_PointAspect.hxx>

#include <Prs3d_ToolDisk.hxx>
#include <Graphic3d_Text.hxx>
#include <Graphic3d_ArrayOfPoints.hxx>

namespace echocad{

ViewCube::ViewCube() : AIS_ViewCube()
{
	SetFixedAnimationLoop(false);
	SetAutoStartAnimation(true);

	Handle(Graphic3d_TransformPers) aTrsfPers = new Graphic3d_TransformPers(
		Graphic3d_TMF_TriedronPers,
		Aspect_TOTP_RIGHT_UPPER,
		Graphic3d_Vec2i(0, 0)
	);
	SetTransformPersistence(aTrsfPers);

	// set view cube size smaller
	SetSize(40);
}; 

IMPLEMENT_STANDARD_RTTIEXT(ViewSphere, AIS_InteractiveObject)

ViewSphere::ViewSphere()
	: // myAxisAspect(new Prs3d_ShadingAspect()),
	myToDisplayAxes(true),
	myAxesRadius(1.0),
	myAxesConeRadius(1.0),
	mySphereSize(10.0)
{
	myLocation = gp_Pnt(0, 0, 0);

	myInfiniteState = true;
	myIsMutable = true;
	myDrawer->SetZLayer(Graphic3d_ZLayerId_Topmost);

	myDrawer->SetTextAspect(new Prs3d_TextAspect());
	myDrawer->SetShadingAspect(new Prs3d_ShadingAspect());
	myDrawer->SetDatumAspect(new Prs3d_DatumAspect());
	 
	myDynHilightDrawer = new Prs3d_Drawer();
	myDynHilightDrawer->SetLink(myDrawer);
	myDynHilightDrawer->SetShadingAspect(new Prs3d_ShadingAspect());

	myTransformPersistence = new Graphic3d_TransformPers(
		Graphic3d_TMF_TriedronPers,
		Aspect_TOTP_RIGHT_UPPER,
		Graphic3d_Vec2i(80, 110)
	);

	myAxesLabels.Bind(Prs3d_DatumParts_XAxis, "X");
	myAxesLabels.Bind(Prs3d_DatumParts_YAxis, "Y");
	myAxesLabels.Bind(Prs3d_DatumParts_ZAxis, "Z");

	setDefaultAttributes();

	// define default size
	SetSize(40.0);
}

void ViewSphere::setDefaultAttributes()
{
	myDrawer->TextAspect()->SetHorizontalJustification(Graphic3d_HTA_CENTER);
	myDrawer->TextAspect()->SetVerticalJustification(Graphic3d_VTA_CENTER);
	myDrawer->TextAspect()->SetColor(Quantity_NOC_BLACK);
	myDrawer->TextAspect()->SetFont(Font_NOF_KOREAN);
	myDrawer->TextAspect()->SetHeight(12.0);
	myDrawer->TextAspect()->Aspect()->SetTextZoomable(false); // the whole object is drawn within transformation-persistence
	// this should be forced back-face culling regardless Closed flag
	myDrawer->TextAspect()->Aspect()->SetFaceCulling(Graphic3d_TypeOfBackfacingModel_BackCulled);
	myDrawer->TextAspect()->Aspect()->SetTextStyle(Aspect_TOST_ANNOTATION);
	myDrawer->TextAspect()->Aspect()->SetTextFontAspect(Font_FontAspect_Bold);

	myDrawer->SetFaceBoundaryDraw(false);

	myDrawer->DatumAspect()->SetDrawArrows(false);

	// 创建一个 Graphic3d_MaterialAspect 对象
	Graphic3d_MaterialAspect materialAspect =  Graphic3d_MaterialAspect();

	// 设置材质的环境光（Ambient）和漫反射（Diffuse）为相同的颜色
	// 这样做是为了确保光照对材质的影响最小
	materialAspect.SetAmbientColor(Quantity_Color(0.7, 0.7, 0.7, Quantity_TOC_RGB)); // 可以调整为所需的颜色
	materialAspect.SetDiffuseColor(Quantity_Color(0.7, 0.7, 0.7, Quantity_TOC_RGB)); // 可以调整为所需的颜色

	// 禁用镜面反射和光泽度，确保没有高光效果
	materialAspect.SetSpecularColor(Quantity_Color(0.0, 0.0, 0.0, Quantity_TOC_RGB));
	materialAspect.SetShininess(0.0);

	myDrawer->DatumAspect()->ShadingAspect(Prs3d_DatumParts_XAxis)->SetMaterial(materialAspect);
	myDrawer->DatumAspect()->ShadingAspect(Prs3d_DatumParts_XAxis)->SetColor(Quantity_Color(246 / 255., 54 / 255., 82 / 255., Quantity_TOC_sRGB));

	myDrawer->DatumAspect()->ShadingAspect(Prs3d_DatumParts_YAxis)->SetMaterial(materialAspect);
	myDrawer->DatumAspect()->ShadingAspect(Prs3d_DatumParts_YAxis)->SetColor(Quantity_Color(112 / 255., 164 / 255., 28 / 255., Quantity_TOC_sRGB));

	myDrawer->DatumAspect()->ShadingAspect(Prs3d_DatumParts_ZAxis)->SetMaterial(materialAspect);
	myDrawer->DatumAspect()->ShadingAspect(Prs3d_DatumParts_ZAxis)->SetColor(Quantity_Color(47 / 255.,  132 / 255.,227 / 255., Quantity_TOC_sRGB));

	myDrawer->SetFaceBoundaryDraw(false);
}

void ViewSphere::Compute(const Handle(PrsMgr_PresentationManager)& theManager, const Handle(Prs3d_Presentation)& thePrs, const Standard_Integer theMode)
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

			gp_Ax1 anAx1;
			switch (aPart)
			{
			case Prs3d_DatumParts_XAxis:
				anAx1 = gp_Ax1(aLocation, gp::DX());
				break;
			case Prs3d_DatumParts_YAxis:
				anAx1 = gp_Ax1(aLocation, gp::DY());
				break;
			case Prs3d_DatumParts_ZAxis:
				anAx1 = gp_Ax1(aLocation, gp::DZ());
				break;
			default: break;
			}

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

				gp_Pnt aTextOrigin = anAx1.Location().Translated(gp_Vec(anAx1.Direction().X() * (anAxisSize ),
					anAx1.Direction().Y() * (anAxisSize ),
					anAx1.Direction().Z() * (anAxisSize )));

				Prs3d_Text::Draw(anAxisLabelGroup, myDrawer->TextAspect(), TCollection_ExtendedString(anAxisLabel), aTextOrigin);
			}
		}
	}
}

void ViewSphere::ComputeSelection(const Handle(SelectMgr_Selection)& theSelection, const Standard_Integer theMode)
{

}


} // end namespace echocad