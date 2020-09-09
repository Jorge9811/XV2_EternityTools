package CHARASELE_fla
{
   import action_script.CharaSele;
   import flash.display.MovieClip;
   
   public dynamic class MainTimeline extends MovieClip
   {
       
      
      public var cha_skill:MovieClip;
      
      public var press2P:MovieClip;
      
      public var cha_select_cur:MovieClip;
      
      public var cha_frame:MovieClip;
      
      public var cha_parameter:MovieClip;
      
      public var timer:MovieClip;
      
      public var cha_arrow:MovieClip;
      
      public var cha_new:MovieClip;
      
      public var cha_select:MovieClip;
      
      public var m_main:CharaSele;
	  
	  public var aaa1:_CMN_M_frame_61;
	  public var aaa2:mc_arw_l_56;
	  public var aaa3:mc_arw_r_59;
	  public var aaa4:mc_btnact_chamyset_22;
	  public var aaa5:mc_cha_arrow_54;
	  public var aaa6:mc_cha_cursol_41;
	  public var aaa7:mc_cha_new_46;
	  public var aaa8:mc_cha_parameter_1;
	  public var aaa9:mc_cha_select_34;
	  public var aaa10:mc_cha_skill_49;
	  public var aaa11:mc_chara_btnact_36;
	  public var aaa12:mc_chara_new_47;
	  public var aaa13:mc_chara_set_35;
	  public var aaa14:mc_ready_btnact_enm_12;
	  public var aaa15:mc_ready_btnact_fri_17;
	  public var aaa16:mc_ready_nest_enm_9;
	  public var aaa17:mc_ready_nest_fri_16;
	  public var aaa18:mc_timer_65;
	  public var aaa19:sys_2Ppress_62;
      
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
            m_main = new CharaSele();
         }
         m_main.Initialize(this);
         stop();
      }
   }
}
