#include "Fitter.h"
#include "TTree.h"
#include "TCanvas.h"
#include "TF1.h"
#include "TFile.h"
#include "TGraph.h"
#include "TLegend.h"
#include "TMath.h"
#include "TRandom.h"

ClassImp(Glauber::Fitter)

// -----   Default constructor   -------------------------------------------
Glauber::Fitter::Fitter(std::unique_ptr<TTree> tree) 
{
    fSimTree = std::move(tree); 
    std::cout << fSimTree->GetEntries() << std::endl;
    
    if (!fSimTree) {
        std::cout << "SetSimHistos: *** Error - " << std::endl;
        exit(EXIT_FAILURE);
    }
    
    fSimTree->SetBranchAddress("B",  &fB);
    fSimTree->SetBranchAddress("Npart",  &fNpart);
    fSimTree->SetBranchAddress("Ncoll",  &fNcoll);
    fSimTree->SetBranchAddress("Ecc1",  &fEcc1);
    fSimTree->SetBranchAddress("Psi1",  &fPsi1);
    fSimTree->SetBranchAddress("Ecc2",  &fEcc2);
    fSimTree->SetBranchAddress("Psi2",  &fPsi2);
    fSimTree->SetBranchAddress("Ecc3",  &fEcc3);
    fSimTree->SetBranchAddress("Psi3",  &fPsi3);
    fSimTree->SetBranchAddress("Ecc4",  &fEcc4);
    fSimTree->SetBranchAddress("Psi4",  &fPsi4);
    fSimTree->SetBranchAddress("Ecc5",  &fEcc5);
    fSimTree->SetBranchAddress("Psi5",  &fPsi5);
}

void Glauber::Fitter::Init(int nEntries, TString fmode)
{

    if ( nEntries < 0 || nEntries > fSimTree->GetEntries() ){
        std::cout << "Init: *** ERROR - number of entries < 0 or less that number of entries in input tree" << std::endl;
        std::cout << "Init: *** number of entries in input tree = " << fSimTree->GetEntries() << std::endl;        
        exit(EXIT_FAILURE);
    }

    const int BMax  = int (fSimTree->GetMaximum("B") );
    const int NpartMax  = int (fSimTree->GetMaximum("Npart") );
    const int NcollMax  = int (fSimTree->GetMaximum("Ncoll") );
    
    const float Ecc1Min  =  fSimTree->GetMinimum("Ecc1");
    const float Psi1Min  =  fSimTree->GetMinimum("Psi1");
    const float Ecc2Min  =  fSimTree->GetMinimum("Ecc2");
    const float Psi2Min  =  fSimTree->GetMinimum("Psi2");
    const float Ecc3Min  =  fSimTree->GetMinimum("Ecc3");
    const float Psi3Min  =  fSimTree->GetMinimum("Psi3");
    const float Ecc4Min  =  fSimTree->GetMinimum("Ecc4");
    const float Psi4Min  =  fSimTree->GetMinimum("Psi4");
    const float Ecc5Min  =  fSimTree->GetMinimum("Ecc5");
    const float Psi5Min  =  fSimTree->GetMinimum("Psi5");
    const float Ecc1Max  =  fSimTree->GetMaximum("Ecc1");
    const float Psi1Max  =  fSimTree->GetMaximum("Psi1");
    const float Ecc2Max  =  fSimTree->GetMaximum("Ecc2");
    const float Psi2Max  =  fSimTree->GetMaximum("Psi2");
    const float Ecc3Max  =  fSimTree->GetMaximum("Ecc3");
    const float Psi3Max  =  fSimTree->GetMaximum("Psi3");
    const float Ecc4Max  =  fSimTree->GetMaximum("Ecc4");
    const float Psi4Max  =  fSimTree->GetMaximum("Psi4");
    const float Ecc5Max  =  fSimTree->GetMaximum("Ecc5");
    const float Psi5Max  =  fSimTree->GetMaximum("Psi5");
    
    fBHisto      = TH1F ("fBHisto",  "B",  BMax/fBinSize,  0, BMax );
    fNpartHisto  = TH1F ("fNpartHisto",  "Npart",  NpartMax/fBinSize,  0, NpartMax );
    fNcollHisto  = TH1F ("fNcollHisto",  "Ncoll",  NcollMax/fBinSize,  0, NcollMax );
    fEcc1Histo  = TH1F ("fEcc1Histo",  "#epsilon1",  (Ecc1Max-Ecc1Min)/0.01  Ecc1Min, Ecc1Max );
    fPsi1Histo  = TH1F ("fPsi1Histo",  "#psi1",  (Psi1Max-Psi1Min)/0.01  Psi1Min, Psi1Max );
    fEcc2Histo  = T1F ("fEcc2Histo",  "#epsilon2",  (Ecc2Max-Ecc2Min)/0.01  Ecc2Min, Ecc2Max );
    fPsi2Histo  = TH1F ("fPsi2Histo",  "#psi2",  (Psi2Max-Psi2Min)/0.01  Psi2Min, Psi2Max );
    fEcc3Histo  = TH1F ("fEcc3Histo",  "#epsilon3",  (Ecc3Max-Ecc3Min)/0.01  Ecc3Min, Ecc3Max );
    fPsi3Histo  = TH1F ("fPsi3Histo",  "#psi3",  (Psi3Max-Psi3Min)/0.01  Psi3Min, Psi3Max );
    fEcc4Histo  = TH1F ("fEcc4Histo",  "#epsilon4",  (Ecc4Max-Ecc4Min)/0.01  Ecc4Min, Ecc4Max );
    fPsi4Histo  = TH1F ("fPsi4Histo",  "#psi4",  (Psi4Max-Psi4Min)/0.01  Psi4Min, Psi4Max );
    fEcc5Histo  = TH1F ("fEcc5Histo",  "#epsilon5",  (Ecc5Max-Ecc5Min)/0.01  Ecc5Min, Ecc5Max );
    fPsi5Histo  = TH1F ("fPsi5Histo",  "#psi5",  (Psi5Max-Psi5Min)/0.01  Psi5Min, Psi5Max );
    
    for (int i=0; i<nEntries; i++)
    	{
       	fSimTree->GetEntry(i);
        fBHisto.Fill(fB);
       	fNcollHisto.Fill(fNcoll);
       	fNpartHisto.Fill(fNpart);
        fEcc1Histo.Fill(fEcc1);
        fPsi1Histo.Fill(fPsi1);
        fEcc2Histo.Fill(fEcc2);
        fPsi2Histo.Fill(fPsi2);
        fEcc3Histo.Fill(fEcc3);
        fPsi3Histo.Fill(fPsi3);
        fEcc4Histo.Fill(fEcc4);
        fPsi4Histo.Fill(fPsi4);
        fEcc5Histo.Fill(fEcc5);
        fPsi5Histo.Fill(fPsi5);
    	}
    std::cout << fSimTree->GetEntries() << std::endl;


    fNbins = fDataHisto.GetNbinsX();

    while ( fDataHisto.GetBinContent(fNbins-1) == 0)
        fNbins--;

    fNbins++;

    const float min = fDataHisto.GetXaxis()->GetXmin();
    const float max = fDataHisto.GetXaxis()->GetXmax();

    fMaxValue = min + (max - min)*fNbins/fDataHisto.GetNbinsX() ;

    std::cout << "fNbins = " << fNbins << std::endl;
    std::cout << "fMaxValue = " << fMaxValue << std::endl;
}

float Glauber::Fitter::Nancestors(float f) const
{
    if       (fMode == "Default")    return f*fNpart + (1-f)*fNcoll;
    else if  (fMode == "PSD")        return f-fNpart;
    else if  (fMode == "Npart")      return TMath::Power(fNpart, f); 
    else if  (fMode == "Ncoll")      return TMath::Power(fNcoll, f);
    
    return -1.;
}

float Glauber::Fitter::NancestorsMax(float f) const
{
    const int NpartMax = fNpartHisto.GetXaxis()->GetXmax() ;  // some magic
    const int NcollMax = fNcollHisto.GetXaxis()->GetXmax() ; //TODO 
    
    if       (fMode == "Default")    return f*NpartMax + (1-f)*NcollMax;
    else if  (fMode == "PSD")        return f;
    else if  (fMode == "Npart")      return TMath::Power(NpartMax, f); 
    else if  (fMode == "Ncoll")      return TMath::Power(NcollMax, f);
    
    return -1.;
}

/*
 * take Glauber MC data from fSimTree
 * Populate fGlauberFitHisto with NBD x Na
 */

void Glauber::Fitter::SetGlauberFitHisto (float f, float mu, float k, int n, Bool_t Norm2Data)
{    
    fGlauberFitHisto = TH1F("glaub", "", fNbins*1.3, 0, 1.3*fMaxValue);
    fB_VS_Multiplicity = TH2F("B_VS_Multiplicity", "B VS Multiplicity;nHits;B, fm", fNbins*1.3, 0, 1.3*fMaxValue, 200, 0, 20);
    fNpart_VS_Multiplicity = TH2F("Npart_VS_Multiplicity", "N_{part} VS Multiplicity;nHits;N_{part}", fNbins*1.3, 0, 1.3*fMaxValue, 10000, 0, 10000);
    fNcoll_VS_Multiplicity = TH2F("Ncoll_VS_Multiplicity", "N_{coll} VS Multiplicity;nHits;N_{coll}", fNbins*1.3, 0, 1.3*fMaxValue, 10000, 0, 10000);
    fEcc1_VS_Multiplicity = TH2F("Ecc1_VS_Multiplicity", "#epsilon1 VS Multiplicity;nHits;#epsilon1", fNbins*1.3, 0, 1.3*fMaxValue, 100, 0, 1);
    fPsi1_VS_Multiplicity = TH2F("Psi1_VS_Multiplicity", "#psi1 VS Multiplicity;nHits;#psi1", fNbins*1.3, 0, 1.3*fMaxValue, 2*TMath::pi/0.01, -1*TMath::pi, -1*TMath::pi);
    fEcc2_VS_Multiplicity = TH2F("Ecc2_VS_Multiplicity", "#epsilon2 VS Multiplicity;nHits;#epsilon2", fNbins*1.3, 0, 1.3*fMaxValue, 100, 0, 1);
    fPsi2_VS_Multiplicity = TH2F("Psi2_VS_Multiplicity", "#psi2 VS Multiplicity;nHits;#psi2", fNbins*1.3, 0, 1.3*fMaxValue, 2*TMath::pi/0.01, -1*TMath::pi, -1*TMath::pi);
    fEcc3_VS_Multiplicity = TH2F("Ecc3_VS_Multiplicity", "#epsilon3 VS Multiplicity;nHits;#epsilon3", fNbins*1.3, 0, 1.3*fMaxValue, 100, 0, 1);
    fPsi3_VS_Multiplicity = TH2F("Psi3_VS_Multiplicity", "#psi3 VS Multiplicity;nHits;#psi3", fNbins*1.3, 0, 1.3*fMaxValue, 2*TMath::pi/0.01, -1*TMath::pi, -1*TMath::pi);
    fEcc4_VS_Multiplicity = TH2F("Ecc4_VS_Multiplicity", "#epsilon4 VS Multiplicity;nHits;#epsilon4", fNbins*1.3, 0, 1.3*fMaxValue, 100, 0, 1);
    fPsi4_VS_Multiplicity = TH2F("Psi4_VS_Multiplicity", "#psi4 VS Multiplicity;nHits;#psi4", fNbins*1.3, 0, 1.3*fMaxValue, 2*TMath::pi/0.01, -1*TMath::pi, -1*TMath::pi);
    fEcc5_VS_Multiplicity = TH2F("Ecc5_VS_Multiplicity", "#epsilon5 VS Multiplicity;nHits;#epsilon5", fNbins*1.3, 0, 1.3*fMaxValue, 100, 0, 1);
    fPsi5_VS_Multiplicity = TH2F("Psi5_VS_Multiplicity", "#psi5 VS Multiplicity;nHits;#psi5", fNbins*1.3, 0, 1.3*fMaxValue, 2*TMath::pi/0.01, -1*TMath::pi, -1*TMath::pi);
    
    fGlauberFitHisto.SetName("glaub_fit_histo");
    
    SetNBDhist(mu,  k);

    std::unique_ptr<TH1F> htemp {(TH1F*)fNbdHisto.Clone("htemp")}; // WTF??? Not working without pointer
    for (int i=0; i<n; i++)
    {
        fSimTree->GetEntry(i);
        const int Na = int(Nancestors(f));
                
        float nHits {0.};
        for (int j=0; j<Na; j++) nHits += int(htemp->GetRandom());
        fGlauberFitHisto.Fill(nHits);
        fB_VS_Multiplicity.Fill(nHits,fB);
        fNpart_VS_Multiplicity.Fill(nHits,fNpart);
        fNcoll_VS_Multiplicity.Fill(nHits,fNcoll);
        fEcc1_VS_Multiplicity.Fill(nHits,fEcc1);
        fPsi1_VS_Multiplicity.Fill(nHits,fPsi1);
        fEcc2_VS_Multiplicity.Fill(nHits,fEcc2);
        fPsi2_VS_Multiplicity.Fill(nHits,fPsi2);
        fEcc3_VS_Multiplicity.Fill(nHits,fEcc3);
        fPsi3_VS_Multiplicity.Fill(nHits,fPsi3);
        fEcc4_VS_Multiplicity.Fill(nHits,fEcc4);
        fPsi4_VS_Multiplicity.Fill(nHits,fPsi4);
        fEcc5_VS_Multiplicity.Fill(nHits,fEcc5);
        fPsi5_VS_Multiplicity.Fill(nHits,fPsi5);
    }
    if (Norm2Data)
        NormalizeGlauberFit();
}


void Glauber::Fitter::NormalizeGlauberFit ()
{
    
    int fGlauberFitHistoInt {0}; 
    int fDataHistoInt {0};
    
    const int lowchibin = fFitMinBin;
    const int highchibin = fFitMaxBin<fNbins ? fFitMaxBin : fNbins;
    
    for (int i=lowchibin; i<highchibin; i++)
    {
        fGlauberFitHistoInt += fGlauberFitHisto.GetBinContent(i+1);
        fDataHistoInt += fDataHisto.GetBinContent(i+1);
    }

    const float ScaleFactor = (float)fDataHistoInt/fGlauberFitHistoInt;
    
//     std::cout << "Scale = " << Scale << std::endl;
    fGlauberFitHisto.Scale(ScaleFactor);    
}

/**
 *
 * @param mu mean value of negative binominal distribution (we are looking for it)
 * @param chi2 return value (indicates good match)
 * @param mu_min lower search edge for mean value NBD
 * @param mu_max upper search edge for mean value NBD
 * @param f parameter of Na
 * @param k NBD parameter
 * @param nEvents
 * @param nIter
 */
void Glauber::Fitter::FindMuGoldenSection (float *mu, float *chi2, float*chi2_error, float mu_min, float mu_max, float f, float k, int nEvents, int nIter, int n)
{
    const float phi {(1+TMath::Sqrt(5))/2};

    /* left */
    float mu_1 = mu_max - (mu_max-mu_min)/phi;

    /* right */
    float mu_2 = mu_min + (mu_max-mu_min)/phi;
    
    SetGlauberFitHisto (f, mu_1, k, nEvents);
    float chi2_mu1 = GetChi2 ();
    float chi2_mu1_error = GetChi2Error ();
    
    SetGlauberFitHisto (f, mu_2, k, nEvents);
    float chi2_mu2 = GetChi2 ();
    float chi2_mu2_error = GetChi2Error ();
    
    for (int j=0; j<nIter; j++)
    {        
        if (chi2_mu1 > chi2_mu2)
        {
            mu_min = mu_1;
            mu_1 = mu_2;
            mu_2 = mu_min + (mu_max-mu_min)/phi;
            chi2_mu1 = chi2_mu2;
            SetGlauberFitHisto (f, mu_2, k, nEvents);
            chi2_mu2 = GetChi2 ();
	    chi2_mu2_error = GetChi2Error ();
        }
        else
        {
            mu_max = mu_2;
            mu_2 = mu_1;
            mu_1 = mu_max - (mu_max-mu_min)/phi; 
            chi2_mu2 = chi2_mu1;
            SetGlauberFitHisto (f, mu_1, k, nEvents);
            chi2_mu1 = GetChi2 (); 
  	    chi2_mu1_error = GetChi2Error ();           
        }
        
        std::cout << "n = " << n+j << " f = "  << f << " k = " << k << " mu1 = " << mu_1 << " mu2 = " << mu_2 << " chi2_mu1 = " << chi2_mu1  << " chi2_mu2 = " << chi2_mu2 << std::endl;
    }

    /* take min(mu) */
    *mu = (chi2_mu1 < chi2_mu2) ? mu_1 : mu_2;
    /* take min(chi2) */
    *chi2 = (chi2_mu1 < chi2_mu2) ? chi2_mu1 : chi2_mu2;
    /* take min(chi2_error) */
    *chi2_error = (chi2_mu1 < chi2_mu2) ? chi2_mu1_error : chi2_mu2_error;
}

/**
 * Find the best match
 *
 * @param return value of best fit parameters
 * @param f0 lower search edge for parameter of Na, for which chi2 will be calculated
 * @param f1 upper search edge for parameter of Na, for which chi2 will be calculated
 * @param k0 lower search edge for NBD parameter
 * @param k1 upper search edge for NBD parameter
 * @param nEvents
 */
float Glauber::Fitter::FitGlauber (float *par, Float_t f0, Float_t f1, Int_t k0, Int_t k1, Int_t nEvents)
{
    float f_fit{-1};
    float mu_fit{-1}; 
    float k_fit{-1};
    float Chi2Min {1e10};
    float Chi2Min_error {0};

    const TString filename = Form ( "%s/fit_%4.2f_%d_%d_%d.root", fOutDirName.Data(), f0, k0, k1, fFitMinBin );
    
//     std::unique_ptr<TFile> file {TFile::Open(filename, "recreate")};    
//     std::unique_ptr<TTree> tree {new TTree("test_tree", "tree" )};

    TFile* file {TFile::Open(filename, "recreate")};    
    TTree* tree {new TTree("test_tree", "tree" )};
    
    TH1F h1("h1", "", fNbins, 0, fMaxValue);
           
    float f, mu, k, chi2, chi2_error, sigma;

    tree->Branch("f",    &f,    "f/F");   
    tree->Branch("mu",   &mu,   "mu/F");   
    tree->Branch("k",    &k,    "k/F");   
    tree->Branch("chi2", &chi2, "chi2/F");
    tree->Branch("chi2_error", &chi2_error, "chi2_error/F");
    tree->Branch("sigma",&sigma,"sigma/F");   

    int n=1;
    for (float i=f0; i<=f1; i=i+0.1000000)
    {
	    f=i;
	    for (int j=k0; j<=k1; j++)
	    {
		mu = fMaxValue / NancestorsMax(f) ;
		k = j;
		const float mu_min = 0.7*mu;
		const float mu_max = 1.0*mu;

    if (fNiter == 0) fNiter = 2;

		FindMuGoldenSection (&mu, &chi2, &chi2_error, mu_min, mu_max, f, k, nEvents, fNiter, n);
		n=n+fNiter;
		sigma = ( mu/k + 1 ) * mu;
		
		tree->Fill();
		
		if (chi2 < Chi2Min)
		{
		    f_fit = f;
		    mu_fit = mu;
		    k_fit = k;
		    Chi2Min = chi2;
		    Chi2Min_error = chi2_error;
		    fBestFitHisto = fGlauberFitHisto;
		    fBestB_VS_Multiplicity=fB_VS_Multiplicity;
	 	    fBestNpart_VS_Multiplicity=fNpart_VS_Multiplicity;
	 	    fBestNcoll_VS_Multiplicity=fNcoll_VS_Multiplicity;
            fBestEcc1_VS_Multiplicity=fEcc1_VS_Multiplicity;
            fBestPsi1_VS_Multiplicity=fPsi1_VS_Multiplicity;
            fBestEcc2_VS_Multiplicity=fEcc2_VS_Multiplicity;
            fBestPsi2_VS_Multiplicity=fPsi2_VS_Multiplicity;
            fBestEcc3_VS_Multiplicity=fEcc3_VS_Multiplicity;
            fBestPsi3_VS_Multiplicity=fPsi3_VS_Multiplicity;
            fBestEcc4_VS_Multiplicity=fEcc4_VS_Multiplicity;
            fBestPsi4_VS_Multiplicity=fPsi4_VS_Multiplicity;
            fBestEcc5_VS_Multiplicity=fEcc5_VS_Multiplicity;
            fBestPsi5_VS_Multiplicity=fPsi5_VS_Multiplicity;
		}            

	    } 
    }

    tree->Write();
    file->Write();
    file->Close();

    par[0] = f_fit;
    par[1] = mu_fit;
    par[2] = k_fit;
    par[3] = Chi2Min_error;
    
    return Chi2Min;
}

/**
 * Compare fGlauberFitHisto with fDataHisto
 * @return chi2 value
 */
float Glauber::Fitter::GetChi2 () const 
{
    float chi2 {0.0};

    const int lowchibin = fFitMinBin;
    const int highchibin = fFitMaxBin<fNbins ? fFitMaxBin : fNbins;
    
    for (int i=lowchibin; i<=highchibin; ++i) 
    {
        if (fDataHisto.GetBinContent(i) < 1.0) continue;
        const float error2 = TMath::Power(fDataHisto.GetBinError(i), 2) + TMath::Power(fGlauberFitHisto.GetBinError(i), 2);
//	std::cout<<"i="<<i<<"  DataError="<<fDataHisto.GetBinError(i)<<std::endl;
//	std::cout<<"i="<<i<<"  GlauberError="<<fGlauberFitHisto.GetBinError(i)<<std::endl;
        const float diff2 = TMath::Power((fGlauberFitHisto.GetBinContent(i) - fDataHisto.GetBinContent(i)), 2) / error2;
//	std::cout<<"i="<<i<<"  DataContent="<<fDataHisto.GetBinContent(i)<<std::endl;
//	std::cout<<"i="<<i<<"  GlauberContent="<<fGlauberFitHisto.GetBinContent(i)<<std::endl;

        chi2 += diff2;
    }
    
    chi2 = chi2 / (highchibin - lowchibin + 1);
    return chi2;
}

/**
 * Compare fGlauberFitHisto with fDataHisto
 * @return chi2 error value
 */
float Glauber::Fitter::GetChi2Error () const 
{
    float chi2_error {0.0};

    const int lowchibin = fFitMinBin;
    const int highchibin = fFitMaxBin<fNbins ? fFitMaxBin : fNbins;
    
    for (int i=lowchibin; i<=highchibin; ++i) 
    {
        if (fDataHisto.GetBinContent(i) < 1.0) continue;
        const float error2 = TMath::Power(fDataHisto.GetBinError(i), 2) + TMath::Power(fGlauberFitHisto.GetBinError(i), 2);
//	std::cout<<"i="<<i<<"  DataError="<<fDataHisto.GetBinError(i)<<std::endl;
//	std::cout<<"i="<<i<<"  GlauberError="<<fGlauberFitHisto.GetBinError(i)<<std::endl;
        const float diff = (fGlauberFitHisto.GetBinContent(i) - fDataHisto.GetBinContent(i));
//	std::cout<<"i="<<i<<"  DataContent="<<fDataHisto.GetBinContent(i)<<std::endl;
//	std::cout<<"i="<<i<<"  GlauberContent="<<fGlauberFitHisto.GetBinContent(i)<<std::endl;
	const float error_diff = (fGlauberFitHisto.GetBinError(i) - fDataHisto.GetBinError(i));

        chi2_error += TMath::Power(diff*error_diff/error2, 2);
    }
    
    chi2_error = 2*TMath::Power(chi2_error,0.5) / (highchibin - lowchibin + 1);
    return chi2_error;
}

/**
 * Populates histogram nbd_<mean>_<k> with values of NBD
 * @param mu
 * @param k
 */
void Glauber::Fitter::SetNBDhist(float mu, float k)
{
    // Interface for TH1F.
    const int nBins = (mu+1.)*3 < 10 ? 10 : (mu+1.)*3;
    
    fNbdHisto = TH1F ("fNbdHisto", "", nBins, 0, nBins);
    fNbdHisto.SetName("nbd");
    
    for (int i=0; i<nBins; ++i) 
    {
        const float val = NBD(i, mu, k);
        if (val>1e-10) fNbdHisto.SetBinContent(i+1, val);
//         std::cout << "val " << val << std::endl;    
    }
}

/**
 * Negative Binomial Distribution (by definition)
 * @param n argument
 * @param mu mean
 * @param k argument
 * @return NBD for a given parameters
 */
float Glauber::Fitter::NBD(float n, float mu, float k) const
{
    // Compute NBD.
    float F;
    float f;

    if (n+k > 100.0) 
    {
        // log method for handling large numbers
        F  = TMath::LnGamma(n + k)- TMath::LnGamma(n + 1.)- TMath::LnGamma(k);
        f  = n * TMath::Log(mu/k) - (n + k) * TMath::Log(1.0 + mu/k);
        F += f;
        F = TMath::Exp(F);
    } 
    else 
    {
        F  = TMath::Gamma(n + k) / ( TMath::Gamma(n + 1.) * TMath::Gamma(k) );
        f  = n * TMath::Log(mu/k) - (n + k) * TMath::Log(1.0 + mu/k);
        f  = TMath::Exp(f);
        F *= f;
    }

    return F;
}
/**
 * Creates histo with a given model parameter distribution
 * @param range observable range
 * @param name name of the MC-Glauber model parameter 
 * @param par array with fit parameters
 * @param Nevents
 * @return pointer to the histogram 
 */
std::unique_ptr<TH1F> Glauber::Fitter::GetModelHisto (const float range[2], TString name, const float par[3], int nEvents)
{    
    const float f =  par[0];
    const float mu = par[1];
    const float k = par[2];
    
    float modelpar{-999.};
    fSimTree->SetBranchAddress(name, &modelpar);
        
    SetNBDhist(mu, k);
  
//     TRandom random;  
//     random.SetSeed(mu*k);

    std::unique_ptr<TH1F> hModel(new TH1F ("hModel", "name", 100, fSimTree->GetMinimum(name),  fSimTree->GetMaximum(name)) );

    for (int i=0; i<nEvents; i++)
    {
        fSimTree->GetEntry(i);
        const int Na = int(Nancestors(f));
        float nHits{0.};
        for (int j=0; j<Na; ++j) nHits += (int)fNbdHisto.GetRandom();
        
        if ( nHits > range[0] && nHits < range[1] ){
            hModel->Fill(modelpar);
        }
            
    }
    
    return std::move(hModel);
    
}
