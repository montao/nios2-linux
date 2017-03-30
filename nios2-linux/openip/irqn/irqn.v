module irqn (/*AUTOARG*/
   // Outputs
   avs_s0_irq,
   // Inputs
   avs_s0_export_irqn, gls_clk, gls_reset_n, avs_s0_write,
   avs_s0_writedata
   );
   input avs_s0_export_irqn;
   input gls_clk;   
   input gls_reset_n;   
   input avs_s0_write;
   input [0:0] avs_s0_writedata;
   output avs_s0_irq;

   assign avs_s0_irq = ~avs_s0_export_irqn;

endmodule
