package STAGESELE_fla
{
   import action_script.StageSele;
   import action_script.Test;
   import flash.display.MovieClip;
   
   public dynamic class MainTimeline extends MovieClip
   {
       
      
      public var stage_new:MovieClip;
      
      public var stage_img:MovieClip;
      
      public var timer:MovieClip;
      
      public var select_sel:MovieClip;
      
      public var cmn_m_frame:MovieClip;
      
      public var stage_select:MovieClip;
      
      public var m_main:StageSele;
	  
	  public var aaaa1:_CMN_M_frame_20;
	  public var aaaa2:mc_btnact_frame_15;
	  public var aaaa3:mc_btnact_stagesel_3;
	  public var aaaa4:mc_btnact_stagesel2_32;
	  public var aaaa5:mc_inact_stage_9;
	  public var aaaa6:mc_inact_stageimage_11;
	  public var aaaa7:mc_select_23;
	  public var aaaa8:mc_select2_33;
	  public var aaaa9:mc_stage_icn_set_2;
	  public var aaaa10:mc_stage_icn_set2_31;
	  public var aaaa11:mc_stage_new_26;
	  public var aaaa12:mc_stage_new2_34;
	  public var aaaa13:mc_stage_select_1;
	  public var aaaa14:mc_stage_select2_30;
	  public var aaaa15:mc_timer_28;
      
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
            m_main = new StageSele();
         }
         m_main.Initialize(this);
         stop();
         stop();
         Test.Run(this,m_main);
      }
   }
}
