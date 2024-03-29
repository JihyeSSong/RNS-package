/***************************************************************************
// Chitrasen Jena (cjena@cern.ch) -last modified on 15 Oct 2016

// General macro to configure the RSN analysis task.
// It calls all configs desired by the user, by means
// of the boolean switches defined in the first lines.
// ---
// Inputs:
//  1) flag to know if running on MC or data
//  2) path where all configs are stored
// ---
// Returns:
//  kTRUE  --> initialization successful
//  kFALSE --> initialization failed (some config gave errors)
//
****************************************************************************/

AliRsnMiniAnalysisTask * AddTaskPhiPbPb5TeV
(
 Bool_t      isMC = kFALSE,
 Bool_t      isPP = kFALSE,
 Int_t       Strcut = 2015,
 Int_t       customQualityCutsID = 1,
 AliRsnCutSetDaughterParticle::ERsnDaughterCutSet cutKaCandidate=AliRsnCutSetDaughterParticle::kTPCpidTOFveto3s,
 Float_t     nsigmaK = 3.0,
 Bool_t      enableMonitor = kTRUE,
 Int_t       nmix = 5,
 Float_t     maxDiffVzMix = 1.0,
 Float_t     maxDiffMultMix = 5.0,
 Double_t    vtxZcut = 10.0,
 Double_t    Ylab = 0.5,
 TString     outNameSuffix = "PbPb"
 )
{  
  //-------------------------------------------
  // event cuts
  //-------------------------------------------
  UInt_t      triggerMask=AliVEvent::kINT7;
  Bool_t      rejectPileUp=kTRUE;
  Int_t       MultBins=1;//Using AliMultSelection for centrality determination

  //-------------------------------------------
  // pair cuts
  //-------------------------------------------
  Double_t    minYlab=-Ylab;
  Double_t    maxYlab=Ylab;

  //
  // -- INITIALIZATION ----------------------------------------------------------------------------
  // retrieve analysis manager
  //
  AliAnalysisManager *mgr = AliAnalysisManager::GetAnalysisManager();
   if (!mgr) {
      ::Error("AddTaskPhiPbPb5TeV", "No analysis manager to connect to.");
      return NULL;
   } 

   // create the task and configure 
   TString taskName = Form("Phi%s%s", (isPP? "pp" : "PbPb"), (isMC ? "MC" : "Data"));
   
   AliRsnMiniAnalysisTask *task = new AliRsnMiniAnalysisTask(taskName.Data(), isMC);
   //task->SelectCollisionCandidates(triggerMask);//AOD
   task->UseESDTriggerMask(triggerMask);//ESD
   if (isPP) 
     task->UseMultiplicity("QUALITY");
   else
     //task->UseCentrality("V0M");   
     task->UseMultiplicity("AliMultSelection_V0M");//For Run II

   // set event mixing options
   task->UseContinuousMix();
   //task->UseBinnedMix();
   task->SetNMix(nmix);
   task->SetMaxDiffVz(maxDiffVzMix);
   task->SetMaxDiffMult(maxDiffMultMix);
   task->UseMC(isMC);
   ::Info("AddTaskPhiPbPb5TeV", Form("Event mixing configuration: \n events to mix = %i \n max diff. vtxZ = cm %5.3f \n max diff multi = %5.3f \n", nmix, maxDiffVzMix, maxDiffMultMix));
   
   mgr->AddTask(task);

   //
   // -- EVENT CUTS (same for all configs) ---------------------------------------------------------
   //  
   // cut on primary vertex:
   // - 2nd argument --> |Vz| range
   // - 3rd argument --> minimum required number of contributors
   // - 4th argument --> tells if TPC stand-alone vertexes must be accepted

   AliRsnCutPrimaryVertex* cutVertex=0;
  if(!MultBins){
    cutVertex=new AliRsnCutPrimaryVertex("cutVertex",vtxZcut,0,kFALSE);
    cutVertex->SetCheckZResolutionSPD();
    cutVertex->SetCheckDispersionSPD();
    cutVertex->SetCheckZDifferenceSPDTrack();
  }

  AliRsnCutEventUtils* cutEventUtils=0;
  cutEventUtils=new AliRsnCutEventUtils("cutEventUtils",kTRUE,rejectPileUp);
  if(!MultBins){
    cutEventUtils->SetCheckIncompleteDAQ();
    cutEventUtils->SetCheckSPDClusterVsTrackletBG();
    cutEventUtils->SetCheckInelGt0SPDtracklets();
  }else{
    cutEventUtils->SetCheckAcceptedMultSelection();
  }
  if(isPP && (!isMC) && cutVertex){ 
    cutVertex->SetCheckPileUp(rejectPileUp);// set the check for pileup  
    ::Info("AddTaskPhiPbPb5TeV", Form(":::::::::::::::::: Pile-up rejection mode: %s", (rejectPileUp)?"ON":"OFF"));
  }
  
  
  // define and fill cut set for event cut
  AliRsnCutSet* eventCuts=0;
  if(cutEventUtils || cutVertex){
    eventCuts=new AliRsnCutSet("eventCuts",AliRsnTarget::kEvent);

    if(cutEventUtils && cutVertex){
      eventCuts->AddCut(cutEventUtils);
      eventCuts->AddCut(cutVertex);
      eventCuts->SetCutScheme(Form("%s&%s",cutEventUtils->GetName(),cutVertex->GetName()));
    }else if(cutEventUtils && !cutVertex){
      eventCuts->AddCut(cutEventUtils);
      eventCuts->SetCutScheme(Form("%s",cutEventUtils->GetName()));
    }else if(!cutEventUtils && !cutVertex){
      eventCuts->AddCut(cutVertex);
      eventCuts->SetCutScheme(Form("%s",cutVertex->GetName()));
    }

   // set cuts in task
   task->SetEventCuts(eventCuts);
  }   




   //
   // -- EVENT-ONLY COMPUTATIONS -------------------------------------------------------------------
   //   
   //vertex
   Int_t vtxID = task->CreateValue(AliRsnMiniValue::kVz, kFALSE);
   AliRsnMiniOutput *outVtx = task->CreateOutput("eventVtx", "HIST", "EVENT");
   outVtx->AddAxis(vtxID, 400, -20.0, 20.0);
   
   //multiplicity or centrality
   Int_t multID = task->CreateValue(AliRsnMiniValue::kMult, kFALSE);
   AliRsnMiniOutput *outMult = task->CreateOutput("eventMult", "HIST", "EVENT");
   if (isPP) 
     outMult->AddAxis(multID, 400, 0.0, 400.0);
   else
     outMult->AddAxis(multID, 100, 0.0, 100.0);
   
   //
   // -- PAIR CUTS (common to all resonances) ------------------------------------------------------
   //

   AliRsnCutMiniPair *cutY = new AliRsnCutMiniPair("cutRapidity", AliRsnCutMiniPair::kRapidityRange);
   cutY->SetRangeD(-0.5, 0.5);
   AliRsnCutSet *cutsPair = new AliRsnCutSet("pairCuts", AliRsnTarget::kMother);
   cutsPair->AddCut(cutY);
   cutsPair->SetCutScheme(cutY->GetName());
   
   //
   // -- CONFIG ANALYSIS --------------------------------------------------------------------------
   //
   //gROOT->LoadMacro("$ALICE_PHYSICS/PWGLF/RESONANCES/macros/mini/ConfigPhiPbPb5TeV.C");
   gROOT->LoadMacro("ConfigPhiPbPb5TeV.C");
   if (!ConfigPhiPbPb5TeV(task, isMC, isPP, "", cutsPair, Strcut, customQualityCutsID, cutKaCandidate, nsigmaK, enableMonitor)) return 0x0;

   //
   // -- CONTAINERS --------------------------------------------------------------------------------
   //
   TString outputFileName = AliAnalysisManager::GetCommonFileName();
   //  outputFileName += ":Rsn";
   Printf("AddTaskPhiPbPb5TeV - Set OutputFileName : \n %s\n", outputFileName.Data() );
   
   AliAnalysisDataContainer *output = mgr->CreateContainer(Form("RsnOut_%s",outNameSuffix.Data()), 
							   TList::Class(), 
							   AliAnalysisManager::kOutputContainer, 
							   outputFileName);
   mgr->ConnectInput(task, 0, mgr->GetCommonInputContainer());
   mgr->ConnectOutput(task, 1, output);
   
   return task;
}
