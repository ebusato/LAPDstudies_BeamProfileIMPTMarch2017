
double Maximum(TGraph* g)
{
	double maximum = -1;
	for(int i=0; i<g->GetN(); i++)
	{
		double x, y;
		g->GetPoint(i, x, y);
		if(y > maximum) {
			maximum = y;
		}
	}
	return maximum;
}

class Result {
public:
double up;
double low;
double max;
};

Result FWHM(TGraph* g)
{
	double low = -1;
	double up = -1;
	double maximum = Maximum(g);
	cout << "maximum = " << maximum << endl;
	for(int i=0; i<g->GetN(); i++)
	{
		double x, y;
		g->GetPoint(i, x, y);
		if(y > maximum/2. && low == -1) {
			low = x;
		}
		if(y < maximum/2. && low != -1 && up == -1) {
			up = x;
		}
	}
	Result res;
	res.up = up;
	res.low = low;
	res.max = maximum;
	return res;
}

void DrawGraph(TString fileName, TString canvasName, TString xTitle, TString yTitle)
{
	TCanvas* c = new TCanvas(canvasName.Data(), canvasName.Data(), 700, 600);
	TNtuple* nt = new TNtuple("nt", "nt", "y:i");
	nt->ReadFile(fileName.Data());
	int n = nt->Draw("i:y", "", "goff");
	TGraph *g = new TGraph(n,nt->GetV2(),nt->GetV1());
	g->GetXaxis()->SetTitle(xTitle.Data());
	g->GetYaxis()->SetTitle(yTitle.Data());
	g->GetXaxis()->SetTitleSize(0.06);
	g->GetYaxis()->SetTitleSize(0.06);
	g->GetXaxis()->SetLabelSize(0.055);
	g->GetYaxis()->SetLabelSize(0.055);
	g->GetXaxis()->SetTitleOffset(1.35);
	g->GetYaxis()->SetTitleOffset(1.35);
	g->Draw("ap");
	
	Result res = FWHM(g);
	
	TArrow* arr = new TArrow(res.low, res.max/2., res.up, res.max/2., 0.03, "<|-|>");
	arr->SetLineColor(kRed);
	arr->SetFillColor(kRed);
	arr->Draw();
	TLatex l;
	l.SetTextColor(kRed);
	l.SetTextSize(0.045);
	l.DrawLatex((res.up + res.low)/2. - 10, res.max/2. - 8, Form("FWHM = %.1f mm", res.up - res.low));
	cout << "FWHM = " << res.up - res.low << endl;
	c->SaveAs((canvasName+".png").Data());
}

void BeamProfile()
{
	gStyle->SetPadGridX(1);
	gStyle->SetPadGridY(1);
	
	DrawGraph("Horizontal.txt", "cHor", "Position along X [mm]", "Intensity [a. u.]");
		
	DrawGraph("Vertical.txt", "cVer", "Position along Y [mm]", "Intensity [a. u.]");

}