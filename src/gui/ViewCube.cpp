
#include "ViewCube.h"


namespace echocad{

ViewCube::ViewCube() : AIS_ViewCube()
{
	SetFixedAnimationLoop(false);
	SetAutoStartAnimation(true);

	Handle(Graphic3d_TransformPers) aTrsfPers = new Graphic3d_TransformPers(
		Graphic3d_TMF_TriedronPers,
		Aspect_TOTP_RIGHT_UPPER,
		Graphic3d_Vec2i(70, 110)
	);
	SetTransformPersistence(aTrsfPers);

	// set view cube size smaller
	SetSize(40);


}; 

IMPLEMENT_STANDARD_RTTIEXT(ViewSphere, AIS_InteractiveObject)

ViewSphere::ViewSphere()
	: myAxisAspect(new Prs3d_ShadingAspect()),
	myToDisplayAxes(true),
	myAxesRadius(1.0),
	myAxesConeRadius(1.0),
	mySphereSize(12.0)
{
	myLocation = gp_Pnt(0, 0, 0);

	myInfiniteState = true;
	myIsMutable = true;
	myDrawer->SetZLayer(Graphic3d_ZLayerId_Topmost);

	myDrawer->SetTextAspect(new Prs3d_TextAspect());
	myDrawer->SetShadingAspect(new Prs3d_ShadingAspect());

	myDynHilightDrawer = new Prs3d_Drawer();
	myDynHilightDrawer->SetLink(myDrawer);
	myDynHilightDrawer->SetShadingAspect(new Prs3d_ShadingAspect());

	myTransformPersistence = new Graphic3d_TransformPers(
		Graphic3d_TMF_TriedronPers,
		Aspect_TOTP_RIGHT_UPPER,
		Graphic3d_Vec2i(370, 310)
	);

	myAxesLabels.Bind(Prs3d_DatumParts_XAxis, "X");
	myAxesLabels.Bind(Prs3d_DatumParts_YAxis, "Y");
	myAxesLabels.Bind(Prs3d_DatumParts_ZAxis, "Z");

	myAxisMaterials.Bind(Prs3d_DatumParts_XAxis,  Graphic3d_MaterialAspect());
	myAxisMaterials.Bind(Prs3d_DatumParts_YAxis,  Graphic3d_MaterialAspect());
	myAxisMaterials.Bind(Prs3d_DatumParts_ZAxis,  Graphic3d_MaterialAspect());

	setDefaultAttributes();

	// define default size
	SetSize(50.0);
}


} // end namespace echocad