void run_eve_cosmic()
{
    TString spiritroot = TString(gSystem -> Getenv("VMCWORKDIR"))+"/";
    //TString input     = "data/run3448.reco.develop.1995.0f5154b7.root";
    TString input     = "data/run3448.reco.root";
    //TString input     = "data/run3448.reco.full512.root";
    //TString input     = "/home/ejungwoo/data/spirit/cosmic2023/run3448_s0.reco.develop.1995.0f5154b7.root";
    //TString input     = "/home/ejungwoo/data/spirit/cosmic2023/run3448_tb_from30to500.root";
    TString output    = "data/eve.root";
    //TString parameter = spiritroot + "parameters/ST.parameters.Commissioning_201604.par";
    TString parameter = "ST.parameters.Cosmic2023.par";
    TString geomety   = spiritroot + "geometry/geomSpiRIT.man.root";

    FairLogger *logger = FairLogger::GetLogger();
    logger -> SetLogToScreen(true);

    STEveManager *eve = new STEveManager();
    eve -> SetInputFile(input);         // Set input file (string)
    eve -> SetParInputFile(parameter);  // Set parameter file (string)
    eve -> SetOutputFile(output);       // Set output file (string)
    eve -> SetBackgroundColor(kWhite);  // Set background color (Color_t)
    eve -> SetGeomFile(geomety);        // Set geometry file (string)
    eve -> SetVolumeTransparency(80);   // Set geometry transparency (integer, 0~100)
    eve -> SetViewerPoint(-0.7, 1.1);   // Set camera angle (theta, phi)

    STEveDrawTask *draw = new STEveDrawTask();
    draw -> SetRendering("mc",         false);
    draw -> SetRendering("digi",       false);
    draw -> SetRendering("hit",        false);
    draw -> SetRendering("hitbox",     true);
    draw -> SetRendering("helixhit",   false);
    draw -> SetRendering("helix",      false);
    draw -> SetRendering("cluster",    false);
    draw -> SetRendering("recotrack",  false);

    const char *dataList = "list_run3448.txt";
    {
        STDecoderTask *decoder = new STDecoderTask();
        decoder -> SetUseSeparatedData(true);
        decoder -> SetPersistence(false);
        decoder -> SetUseGainCalibration(false);
        decoder -> SetGGNoiseData("");
        decoder -> SetDataList(dataList);
        decoder -> SetEventID(0);
    }

    eve -> AddEveTask(draw);
    eve -> Init();

    gStyle -> SetPalette(kRainBow);
}
