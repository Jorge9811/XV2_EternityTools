package POPUP_PAUSE_fla
{
   import flash.display.MovieClip;
   
   public dynamic class mc_btn_btnact_4 extends MovieClip
   {
       
      
      public var sys_btn:MovieClip;
      
      public function mc_btn_btnact_4()
      {
         super();
         addFrameScript(7,frame8,15,frame16,65,frame66,115,frame116);
      }
      
      function frame8() : *
      {
         stop();
      }
      
      function frame16() : *
      {
         stop();
      }
      
      function frame66() : *
      {
         gotoAndPlay("gray_on");
      }
      
      function frame116() : *
      {
         gotoAndPlay("on");
      }
   }
}
