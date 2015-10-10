double func1(double*x, double* par)
{
   if (x[0]<1e-5) return 0;
   double pdf = ROOT::Math::chisquared_pdf(x[0],par[0]);
   //printf("x[0] %f par[0] %f prob %f\n", x[0], par[0], pdf);
   return pdf;
}

void chk1()
{
   TCanvas* c1 = new TCanvas("c1");
   TH2F* h2 = new TH2F("h2", "chisquare probability density function; chi2; prob", 100, 0, 10, 100, 0, 1);
   h2->SetStats(0);
   h2->Draw();

   TLegend *leg = new TLegend(0.7,0.4,0.9,0.9);
   for (int ndf=1; ndf<10; ndf++) {
      TF1* f1 = new TF1("func1", func1, 0, 10, 1);
      f1->SetParameter(0,ndf);
      f1->SetLineColor(ndf);
      leg->AddEntry(f1,Form("ndf=%d",ndf), "lpf");
      f1->Draw("same");
   }
   leg->Draw();
   c1->Print("c1.pdf");
}

void chk2(double ndf)
{
   TCanvas* c2 = new TCanvas("c2");
   TF1* f1 = new TF1("func1", func1, 0, 10, 1);
   f1->SetParameter(0,ndf);
   TH1F* h1 = new TH1F("h1",Form("chisquare probability function (ndf=%d);chi2;entries",ndf), 100, 0, 10);
   h1->FillRandom("func1", 10000);
   h1->Draw();
   c2->Print("c2.pdf");
}

void chk3(int ndf)
{
   TCanvas* c3 = new TCanvas("c3");
   TF1* f1 = new TF1("func1", func1, 0, 100, 1);
   f1->SetParameter(0,ndf);
   TH1F* h1 = new TH1F("h1",Form("chisquare probability function (ndf=%d);chi2;entries", ndf), 200, 0, ndf*2);
   TH1F* h2 = new TH1F("h2",Form("p-value distribution (ndf=%d);p-value;entries", ndf), 120, 0, 1.2);
   TH1F* h1s[10];
   TH1F* h2s[10];
   for (int i=0; i<10; i++) {
      h1s[i] = new TH1F(Form("h1%d",i),Form("h1%d",i), 200, 0, ndf*2);
      h2s[i] = new TH1F(Form("h2%d",i),Form("h2%d",i), 120, 0, 1.2);
      h1s[i]->SetFillColor(i+1);
      h2s[i]->SetFillColor(i+1);
   }
   for (int i=0; i<10000; i++) {
      double x = f1->GetRandom();
      double prob = TMath::Prob(x, ndf);
      int bin = prob*100.0/10.0;
      h1->Fill(x);
      h2->Fill(prob);
      h1s[bin]->Fill(x);
      h2s[bin]->Fill(prob);
   }

   c3->Divide(1,2);
   c3->cd(1);
   h1->Draw();
   for (int i=0; i<10; i++) {
      h1s[i]->Draw("same");
   }
   c3->cd(2);
   h2->Draw("colz");
   for (int i=0; i<10; i++) {
      h2s[i]->Draw("same");
   }
   c3->Print("c3.pdf");
}
