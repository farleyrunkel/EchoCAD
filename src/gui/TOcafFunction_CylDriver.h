
#ifndef _TOcafFunction_CylDriver_HeaderFile
#define _TOcafFunction_CylDriver_HeaderFile

#include <TFunction_Driver.hxx>
#include <TFunction_Logbook.hxx>

//! Creation of an instance of the cylinder driver. It's possible (and recommended)
//! to have only one instance of a driver for the whole session.
class TOcafFunction_CylDriver : public TFunction_Driver
{
  DEFINE_STANDARD_RTTI_INLINE(TOcafFunction_CylDriver, TFunction_Driver)
public:
   static const Standard_GUID& GetID();
public:

  //! Creation of an instance of the driver. It's possible (and recommended)
  //! to have only one instance of a driver for the whole session.
   TOcafFunction_CylDriver();

  //! Validation of the object label, its arguments and its results.
   virtual void Validate (Handle(TFunction_Logbook)& theLog) const override;

  //! We call this method to check if the object was modified to be invoked.
  //! If the object label or an argument is modified, we must recompute the object - to call the method Execute().
   virtual Standard_Boolean MustExecute (const Handle(TFunction_Logbook)& theLog) const override;

  //! We compute the object and topologically name it.
  //! If during the execution we found something wrong,
  //! we return the number of the failure. For example:
  //!  1 - an attribute hasn't been found,
  //!  2 - algorithm failed,
  //! if there are no any mistakes occurred we return 0:
  //!  0 - no mistakes were found.
   virtual Standard_Integer Execute (Handle(TFunction_Logbook)& theLog) const override;

};

#endif
