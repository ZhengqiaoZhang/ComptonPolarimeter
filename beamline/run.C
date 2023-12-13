void run()
{
gROOT->ProcessLine(".L DetectorTree.C");
gROOT->ProcessLine("DetectorTree aa");
gROOT->ProcessLine("aa.Loop()");


}
