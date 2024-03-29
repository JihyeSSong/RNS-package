//
// Class AliRsnCutEventUtils
//
// It works with ESD and AOD events.
//
// authors: F. Bellini (fbellini@cern.ch)

#ifndef ALIRSNCUTEVENTUTILS_H
#define ALIRSNCUTEVENTUTILS_H

#include "AliRsnCut.h"

class AliVVertex;
class AliAnalysisUtils;

class AliRsnCutEventUtils : public AliRsnCut {
 public:

  AliRsnCutEventUtils(const char *name = "cutEventUtils", Bool_t rmFirstEvInChunck = kFALSE, Bool_t checkPileUppA2013 = kTRUE);
  AliRsnCutEventUtils(const AliRsnCutEventUtils &copy);
  AliRsnCutEventUtils &operator=(const AliRsnCutEventUtils &copy);
  virtual ~AliRsnCutEventUtils() {;};
  
  void           SetRemovePileUppA2013(Bool_t doit = kTRUE) {fCheckPileUppA2013 = doit;}
  void           SetRemoveFirstEvtInChunk(Bool_t doit = kTRUE) {fIsRmFirstEvInChunck = doit;}
  void           SetUseMVPlpSelection(Bool_t useMVPlpSelection = kFALSE) { fUseMVPlpSelection = useMVPlpSelection;}
  void           SetUseVertexSelection2013pA(Bool_t vtxpA2013 = kTRUE, Double_t maxVtxZ = 10.0) {fUseVertexSelection2013pA = vtxpA2013; fMaxVtxZ = maxVtxZ;}
  void           SetUseVertexSelection2013pAIDspectra(Bool_t enable = kTRUE, Double_t maxVtxZ = 10.0) {fUseVertexSelection2013pAspectra = enable; fUseVertexSelection2013pA = kFALSE; fMaxVtxZ = maxVtxZ;}
  Bool_t         IsSelected(TObject *object);
  AliAnalysisUtils* GetAnalysisUtils() { return fUtils; }
  void           SetAnalysisUtils(AliAnalysisUtils* utils){ fUtils = utils; }
  void           SetMinPlpContribMV(Int_t minPlpContribMV) { fMinPlpContribMV = minPlpContribMV;}
  void           SetMinPlpContribSPD(Int_t minPlpContribSPD) { fMinPlpContribSPD = minPlpContribSPD;}
  void           SetFilterNSDeventsDPMJETpA2013(Bool_t doit = kFALSE) {fFilterNSDeventsDPMJETpA2013 = doit;}
  Bool_t         IsVertexSelected2013pAIDspectra(AliVEvent *event);
  void           SetCheckIncompleteDAQ(Bool_t doit = kFALSE) {fCheckIncompleteDAQ = doit;}
 private:
  
  Bool_t              fIsRmFirstEvInChunck; // if kTRUE, remove the first event in the chunk (pA2013)
  Bool_t              fCheckPileUppA2013; // check and reject pileupped events (pA2013)
  Bool_t              fUseMVPlpSelection; // check for pile-up from multiple vtx 
  Int_t               fMinPlpContribMV; // min. n. of MV pile-up contributors
  Int_t               fMinPlpContribSPD; // min. n. of pile-up contributors from SPD
  Bool_t              fUseVertexSelection2013pA;// check and reject vertex of events for pA2013
  Bool_t              fUseVertexSelection2013pAspectra;// check and reject vertex of events for pA2013
  Double_t            fMaxVtxZ;//max selected z_vtx
  Bool_t              fFilterNSDeventsDPMJETpA2013;//enable filter for NSD events in DPMJET MC for pA
  Bool_t              fCheckIncompleteDAQ;// check if DAQ has set the incomplete event attributes (pp 13 TeV)

  AliAnalysisUtils  * fUtils; //pointer to the AliAnalysisUtils object

  ClassDef(AliRsnCutEventUtils, 5)
    
    };

#endif
