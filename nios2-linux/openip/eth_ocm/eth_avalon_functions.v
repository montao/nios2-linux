//////////////////////////////////////////////////////////////////////
////                                                              ////
////  eth_avalon_functions.v                                      ////
////                                                              ////
////  This file is a patch used in conjunction with the           ////
////  Ethernet IP core project.                                   ////
////  http://www.opencores.org/projects/ethmac/                   ////
////                                                              ////
////  Author(s):                                                  ////
////      - Jakob Jones (jrjonsie@gmail.com)                      ////
////                                                              ////
////  All additional information is available in the Readme.txt   ////
////  file.                                                       ////
////                                                              ////
//////////////////////////////////////////////////////////////////////
//Function for rounding a number up to next power of 2
function integer nextPow2;
    input   [31:0]  in;
    begin
        nextPow2 = 32'd1;
        while(nextPow2<in)
            nextPow2 = nextPow2 << 1;
    end
endfunction

//constant log base 2 function for how many bits required to represent an
//number
function integer clogb2;
    input [31:0] depth;
    begin
        for(clogb2=0; depth>0; clogb2=clogb2+1)
        depth = depth >> 1;
    end
endfunction

//constant min function to find the smaller of two numbers
function integer min;
    input   [31:0] a;
    input   [31:0] b;
    begin
        if(a>b) min = b;
        else    min = a;
    end
endfunction

//constant max function to find the larger  of two numbers
function integer max;
    input   [31:0] a;
    input   [31:0] b;
    begin
        if(a>b) max = a;
        else    max = b;
    end
endfunction

