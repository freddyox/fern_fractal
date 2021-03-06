#include "TCanvas.h"
#include "TMarker.h"
#include "TStyle.h"
#include "TFile.h"
#include "TRandom.h"
#include "TVectorD.h"
#include "TGraph.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TVector2.h"
#include "TMatrixD.h"

#include <iostream>
#include <vector>
#include <map>

class AffTransform{

 private:
  double fA, fB, fC, fD, fE, fF;

public:
  AffTransform(float a, float b, float c,
	    float d, float e, float f);
  ~AffTransform(){;}
  void affine(TVector2 &v);
  TVector2 affine2(TVector2 v);
};

AffTransform::AffTransform(float a, float b, float c,
		     float d, float e, float f) : fA(a), fB(b), fC(c),
						  fD(d), fE(e), fF(f) {
}
void AffTransform::affine( TVector2 &v ){
  double xn = v.X(); double yn = v.Y();
  v.SetX( xn*fA + yn*fB + fE );
  v.SetY( xn*fC + yn*fD + fF );
}

TVector2 AffTransform::affine2(TVector2 v){
  TVector2 vnew(v.X()*fA + v.Y()*fB + fE,
		v.X()*fC + v.Y()*fD + fF );
  return vnew;
}

void fern_gen(){

  TRandom *fRand = new TRandom(0);

  int nbinx = 2000;
  int nbiny = 1000;
  TCanvas *canv = new TCanvas("canv","",nbinx,nbiny);

  TFile *fOut = new TFile("output.root","RECREATE");
  
  TH1D* xbins = new TH1D("xbins","",nbinx,0.0,1.0);
  TH1D* ybins = new TH1D("ybins","",nbiny,0.0,1.0);
  TH2D* xybins = new TH2D("xybins","", nbinx,0.25,0.75,nbiny,0.0,1.0);
  xbins->SetXTitle("X bin frequency");
  ybins->SetXTitle("Y bin frequency");
  xybins->SetXTitle("X bin frequency");
  xybins->SetYTitle("Y bin frequency");


  std::map<int,TH2D*> animate;
  for(int i=0; i<=8; i++){
    TH2D* temp = new TH2D(Form("fern_%d",i),"", nbinx,0.25,0.75,nbiny,0.0,1.0);
    temp->SetXTitle("X bin frequency");
    temp->SetYTitle("Y bin frequency");
    animate[i] = temp;
  }
  
  int color = kGreen+3;
  float size = 0.1;
  int N = 1e8;

  // True barnsley
  // AffTransform *f1 = new AffTransform(0.0,   0.0,   0.0,  0.16, 0.0, 0.0);
  // AffTransform *f2 = new AffTransform(0.85,  0.04, -0.04, 0.85, 0.0, 1.6);
  // AffTransform *f3 = new AffTransform(0.2,  -0.26,  0.23, 0.22, 0.0, 1.6);
  // AffTransform *f4 = new AffTransform(-0.15, 0.28,  0.26, 0.24, 0.0, 0.44);
  // Modifed (found on http://www.home.aone.net.au/~byzantium/ferns/fractal.html)
  AffTransform *f1 = new AffTransform( 0.000,   0.0,   0.0,  0.2,   0.0, -0.12);
  AffTransform *f2 = new AffTransform( 0.845, 0.035, -0.035, 0.82,  0.0, 1.6);
  AffTransform *f3 = new AffTransform( 0.200, -0.31,  0.255, 0.245, 0.0, 0.29);
  AffTransform *f4 = new AffTransform(-0.150, 0.24,  0.25,  0.20,   0.0, 0.68);
  // Modifed x2 (found on wiki)
  // AffTransform *f1 = new AffTransform( 0.000,   0.0,   0.0,  0.25,   0.0,  -0.4);
  // AffTransform *f2 = new AffTransform( 0.950, 0.005, -0.005, 0.93,  -0.002, 0.5);
  // AffTransform *f3 = new AffTransform( 0.035, -0.20,  0.16, 0.04, -0.09, 0.02);
  // AffTransform *f4 = new AffTransform(-0.04, 0.2,  0.16,  0.04,  0.083, 0.12); 
  
  TVector2 origin(0.0,0.0);
  TVector2 xn(origin.X(), origin.Y());

  TMarker *temp = new TMarker(origin.X(),origin.Y(),20);
  temp->SetMarkerColor(color);
  temp->SetMarkerSize(size);

  std::vector<double> Xpts, Ypts;
   
  std::vector<TMarker*> Markers;
  Markers.push_back( temp );
  int power_count = 0;
  
  for(int i=0; i<N; i++){
    int random = fRand->Integer(100);
    bool didwetransform = false;
    TVector2 xnew;
    if(random<2){
      didwetransform = true;
      xnew = f1->affine2(xn);
    }
    if(random>1 && random<=85 ){
      didwetransform = true;
      xnew = f2->affine2(xn);
    }
    if( random > 85 && random <= 92 ){
      didwetransform = true;
      xnew = f3->affine2(xn);
    }
    if( random > 92 && random <= 99 ){
      didwetransform = true;
      xnew = f4->affine2(xn);
    }
    if( didwetransform ){
      double xtemp = 0.5+xnew.X()/10.0;
      double ytemp = xnew.Y()/10.0;
      xbins->Fill( xtemp );
      ybins->Fill( ytemp );
      xybins->Fill( xtemp, ytemp );
      TMarker *mtemp = new TMarker(xtemp,ytemp,20);
      mtemp->SetMarkerColor(color);
      mtemp->SetMarkerSize(size);

      animate[power_count]->Fill(xtemp,ytemp);
      
      if( i >= pow(10,power_count) ){
	power_count++;
	std::cout << power_count << " " << i << std::endl;
      }

      Markers.push_back( mtemp );
      Xpts.push_back(xnew.X());
      Ypts.push_back(xnew.Y());
      xn = xnew;
    } else {
      std::cout << "ERROR!!! " << random << std::endl;
    }
  }

  // std::vector<TMarker*>::iterator vit;
  // for(vit = Markers.begin(); vit!=Markers.end(); vit++){
  //   (*vit)->Draw("same");
  // }

  // TCanvas *c2 = new TCanvas("c2","",400,900);
  // TGraph *gtemp = new TGraph(Xpts.size(), &(Xpts[0]),
  // 			     &(Ypts[0]));
  // std::cout <<  Xpts.size() << std::endl;
  // gtemp->SetMarkerStyle(20);
  // gtemp->SetMarkerSize(size);
  // gtemp->SetMarkerColor(color);
  // c2->cd();
  // gtemp->Draw("AP");
   // gStyle->SetOptStat(0);
   // for(int i=51; i<=112; i++){
   //  gStyle->SetPalette(i);
   //  TCanvas *c3 = new TCanvas("c3","",500, 900);
   //  c3->cd();
   //  c3->SetFillColor(1);
   //  gPad->SetLogz();
   //  xybins->Draw("colz");
   //  c3->SaveAs(Form("zfractal_fern_%d.png",i));
   //  c3->Close();
   // }
  
  gStyle->SetOptStat(0);
  std::map<int,TH2D*>::iterator mit;
  for(mit = animate.begin(); mit!=animate.end(); mit++){
    gStyle->SetPalette(68);
    TCanvas *c3 = new TCanvas("c3","",500, 900);
    c3->cd();
    c3->SetFillColor(1);
    gPad->SetLogz();
    (mit->second)->Draw("colz");
    c3->SaveAs(Form("1_fern_%d.png",mit->first));
    c3->Close();
  }
  

  fOut->Write();
  return;
}
