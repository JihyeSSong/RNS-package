#ifndef ALIRSNMINIEVENT_H
#define ALIRSNMINIEVENT_H

//
// Mini-Event
// Contains only the useful quantities computed on the event
// which can be used for event mixing, or for direct output
// when doing analysis w.r. to multiplicity or event plane, for example.
//

#include <TArrayI.h>
#include <TClonesArray.h>

class AliRsnMiniParticle;
class AliQnCorrectionsQnVector;

class AliRsnMiniEvent : public TObject {
public:

   AliRsnMiniEvent() : fID(-1), fVz(0.0), fMult(0.0), fRefMult(0.0),  fTracklets(0.0), fAngle(0.0), fQnVector(0), fLeading(-1), fParticles("AliRsnMiniParticle", 0) {}
   ~AliRsnMiniEvent() {fParticles.Delete();}
   AliRsnMiniEvent(const AliRsnMiniEvent &copy);
   AliRsnMiniEvent &operator=(const AliRsnMiniEvent &copy);

   Int_t              &ID()        {return fID;}
   Float_t            &Vz()        {return fVz;}
   Float_t            &Mult()      {return fMult;}
   Float_t            &RefMult()   {return fRefMult;}
   Float_t            &Tracklets() {return fTracklets;}
   Float_t            &Angle()     {return fAngle;}
   TClonesArray       &Particles() {return fParticles;}
   Bool_t              IsEmpty()   {return fParticles.IsEmpty();}

   Int_t               CountParticles(TArrayI &found, Char_t charge = 0, Int_t cutID = -1);
   AliRsnMiniParticle *GetParticle(Int_t i);
   AliRsnMiniParticle *LeadingParticle();
   void                AddParticle(AliRsnMiniParticle copy);

   void                SetQnVector(AliQnCorrectionsQnVector *v) { fQnVector = v; }
   AliQnCorrectionsQnVector *GetQnVector() { return fQnVector; }

private:

   Int_t         fID;         // ID number
   Float_t       fVz;         // z-position of vertex
   Float_t       fMult;       // multiplicity or centrality
   Float_t       fRefMult;    // reference multiplicity
   Float_t       fTracklets;  // tracklets
   Float_t       fAngle;      // angle of reaction plane to main reference frame
   AliQnCorrectionsQnVector *fQnVector; //! Qn Vector

   Int_t         fLeading;    // index of leading particle
   TClonesArray  fParticles;  // list of selected particles

   ClassDef(AliRsnMiniEvent, 6)
};

#endif
