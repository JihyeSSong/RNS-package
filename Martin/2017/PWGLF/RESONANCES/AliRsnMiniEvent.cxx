//
// Mini-Event
// Contains only the useful quantities computed on the event
// which can be used for event mixing, or for direct output
// when doing analysis w.r. to multiplicity or event plane, for example.
//
// Author: A. Pulvirenti
// Developers: F. Bellini (fbellini@cern.ch)
//

#include "AliRsnMiniParticle.h"
#include "AliRsnMiniEvent.h"

ClassImp(AliRsnMiniEvent)

//__________________________________________________________________________________________________
AliRsnMiniEvent::AliRsnMiniEvent(const AliRsnMiniEvent &copy) :
   TObject(copy),
   fID(copy.fID),
   fVz(copy.fVz),
   fMult(copy.fMult),
   fRefMult(copy.fRefMult),
   fTracklets(copy.fTracklets),
   fAngle(copy.fAngle),
   fQnVector(copy.fQnVector),
   fLeading(copy.fLeading),
   fParticles(copy.fParticles)
{
//
// Copy constructor.
// Implemented as requested by C++ standards.
// Can be used in PROOF and by plugins.
//
}

//__________________________________________________________________________________________________
AliRsnMiniEvent &AliRsnMiniEvent::operator=(const AliRsnMiniEvent &copy)
{
//
// Assignment operator.
// Implemented as requested by C++ standards.
// Can be used in PROOF and by plugins.
//

   TObject::operator=(copy);
   if (this == &copy)
      return *this;
   fID = copy.fID;
   fVz = copy.fVz;
   fMult = copy.fMult;
   fRefMult = copy.fRefMult;
   fTracklets = copy.fTracklets;
   fAngle = copy.fAngle;
   fQnVector = copy.fQnVector;
   fLeading = copy.fLeading;
   fParticles = copy.fParticles;
   return (*this);
}

//__________________________________________________________________________________________________
void AliRsnMiniEvent::AddParticle(AliRsnMiniParticle copy)
{
//
// Add a new particle to the list and returns a pointer to it,
// in order to allow to se its parameters.
//

   Int_t n = fParticles.GetEntries();
   new (fParticles[n]) AliRsnMiniParticle(copy);
}

//__________________________________________________________________________________________________
AliRsnMiniParticle *AliRsnMiniEvent::GetParticle(Int_t i)
{
//
// Return the particle
//

   if (i < 0 || i > fParticles.GetEntriesFast()) return 0x0;

   return (AliRsnMiniParticle *)fParticles[i];
}

//__________________________________________________________________________________________________
AliRsnMiniParticle *AliRsnMiniEvent::LeadingParticle()
{
//
// Return the leading particle
//

   if (fLeading < 0) return 0x0;
   if (fLeading >= fParticles.GetEntriesFast()) return 0x0;

   return (AliRsnMiniParticle *)fParticles[fLeading];
}

//__________________________________________________________________________________________________
Int_t AliRsnMiniEvent::CountParticles(TArrayI &found, Char_t charge, Int_t cutID)
{
//
// Counts how many particles have the specified charge and cut bit
// if charge is not '+', '-' or '0', all charges are considered
// if cut bit is < 0, it is not checked
//

   Int_t i, npart = fParticles.GetEntriesFast();
   Int_t    count = 0;
   AliRsnMiniParticle *part = 0x0;

   found.Set(npart);

   for (i = 0; i < npart; i++) {
      part = (AliRsnMiniParticle *)fParticles[i];
      if (charge == '+' || charge == '-' || charge == '0') {
         if (part->Charge() != charge) continue;
      }
      if (cutID >= 0) {
         if (!part->HasCutBit(cutID)) continue;
      }
      found[count] = i;
      count++;
   }

   found.Set(count);
   return count;
}
