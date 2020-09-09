package POPUP_PAUSE_fla
{
   import action_script.PopupPause;
   import flash.display.MovieClip;
   
   public dynamic class MainTimeline extends MovieClip
   {
       
      
      public var pausemenu:MovieClip;
      
      public var m_main:PopupPause;
	  
	  public var aaa:mc_btn_btnact_4;
	  public var aaa2:mc_pause_1;
	  public var aaa3:mc_pausea_nest_3;
      
      public function MainTimeline()
      {
         super();
         addFrameScript(0,frame1);
      }
      
      function frame1() : *
      {
         m_main = null;
         if(!m_main)
         {
            m_main = new PopupPause();
         }
         m_main.Initialize(this);
         stop();
      }
   }
}
