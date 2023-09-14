#include <unistd.h>

void run_reco_cosmic
(
     Int_t fRunNo = 3448,
     Int_t fNumEvents = 500,
     std::vector<Int_t> fSkipEventArray = {},
     TString fPathToData = "data/"
)
{
    Bool_t fUseGainMatching = kTRUE;
    TString fSpiRITROOTPath = TString(gSystem -> Getenv("VMCWORKDIR"))+"/";
    TString fParameterFile = "ST.parameters.PhysicsRuns_201707.par";
    //TString fGainMatchingFile = fSpiRITROOTPath + "parameters/RelativeGain.list";
    TString fGainMatchingFile = "RelativeGain.list";
    TString sRunNo = TString::Itoa(fRunNo, 10);

    TString version; {
        TString name = fSpiRITROOTPath + "VERSION.compiled";
        std::ifstream vfile(name);
        vfile >> version;
        vfile.close();
    }

    //TString par = fSpiRITROOTPath+"parameters/"+fParameterFile;
    TString par = "ST.parameters.Cosmic2023.par";
    TString geo = fSpiRITROOTPath+"geometry/geomSpiRIT.man.root";
    TString inp = TString(gSystem -> Getenv("PWD"))+"/list_run"+sRunNo+".txt";
    //TString out = fPathToData+"run"+sRunNo+".reco."+version+".root";
    //TString log = fPathToData+"run"+sRunNo+"."+version+".log";
    TString out = fPathToData+"run"+sRunNo+".reco.root";
    TString log = fPathToData+"run"+sRunNo+".log";

    FairLogger *logger = FairLogger::GetLogger();
    logger -> SetLogToScreen(true);

    FairParAsciiFileIo* parReader = new FairParAsciiFileIo();
    parReader -> open(par);

    FairRunAna* run = new FairRunAna();
    run -> SetGeomFile(geo);
    run -> SetOutputFile(out);
    run -> GetRuntimeDb() -> setSecondInput(parReader);

    STDecoderTask *decoder = new STDecoderTask();
    decoder -> SetUseSeparatedData(true);
    decoder -> SetPersistence(true);
    decoder -> SetUseGainCalibration(true);
    decoder -> SetDataList(inp);
    //decoder -> SetTbRange(30, 257);
    decoder -> SetTbRange(0, 400);
    if (fUseGainMatching)
        decoder -> SetGainMatchingData(fGainMatchingFile);
    decoder -> SetEventID(0);

    auto preview = new STEventPreviewTask();
    preview -> SetSkippingEvents(fSkipEventArray);
    preview -> SetPersistence(true);

    auto psa = new STPSAETask();
    psa -> SetPersistence(true);
    psa -> SetPulserData("pulser_117ns_50tb.dat");

    run -> AddTask(decoder);
    run -> AddTask(preview);
    run -> AddTask(psa);
    run -> Init();
    run -> Run(0, fNumEvents);

    auto outFile = FairRootManager::Instance() -> GetOutFile();

    cout << "Log    : " << log << endl;
    cout << "Input  : " << inp << endl;
    cout << "Output : " << out << endl;

    gApplication -> Terminate();
}
