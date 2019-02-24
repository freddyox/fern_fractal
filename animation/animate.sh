#!/bin/bash

convert -delay 70 -loop 0 1_fern_{0..8}.png fern.gif

#delay is 10/100 seconds inbetween images. No loops, and no problems
#iterating through my directory with the inclusion of {0..149} (150 files)

#for(int i=0; i<360; i++ ){ 
#    gPad->GetView()->RotateView(i,2*i);
#    gPad->Modified();
#    gPad->Update();
#    if(i%10==0) {
#	    TString temp;
	#    temp.Form("Animation1/RandomWalk_%d.png", i);
	#    gPad->SaveAs(temp);
	#}
#}
