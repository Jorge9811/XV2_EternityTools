package action_script
{
   import flash.display.Bitmap;
   import flash.display.MovieClip;
   import flash.events.Event;
   import flash.events.KeyboardEvent;
   import flash.external.ExternalInterface;
   import flash.ui.Keyboard;
   
   public class CharaSele
   {
      
      private static const ButtonMax:int = 2;
      
      public static const PlayerNumFri:int = 2;
      
      public static const PlayerNumEnm:int = 3;
      
      public static const PlayerMax:int = 1 + PlayerNumFri + PlayerNumEnm;
      
      public static const SkillMax:int = 8;
      
      public static const CharaVarIndexNum:int = 32;
      
      //////////
	  //public static const CharacterMax:int = 128;
	  public static const CharacterMax:int = 850;
	  //////////
      
      public static const CustomListMax:int = 256;
      
      public static const CharacterTableData:int = 12;
      
      public static const CharacterTableMax:int = 512;
      
      public static var VarTypeCode:int = 0;
      
      public static var VarTypeMid:int = 1;
      
      public static var VarTypeModelPreset:int = 2;
      
      public static var VarTypeUnlockIndex:int = 3;
      
      public static var VarTypeFlag_GK2:int = 4;
      
      public static var VarTypeVoiceIdList:int = 5;
      
      public static var VarTypeDlcKeyFlag:int = 6;
      
      public static var VarTypeCustomCostume:int = 7;
      
      public static var VarTypeAvatarSlotID:int = 8;
      
      public static var VarTypeAfterTU9Order:int = 9;
      
      public static var VarTypeNum:int = 10;
      
      public static var InvalidCode:String = "";
      
      public static var AvatarCode:String = "HUM";
      
      public static const ReceiveType_FlagUseCancel:int = 0;
      
      public static const ReceiveType_PlayerFriNum:int = 1;
      
      public static const ReceiveType_PlayerEnmNum:int = 2;
      
      public static const ReceiveType_PartyNpcNum:int = 3;
      
      public static const ReceiveType_FlagSelectAvatar:int = 4;
      
      public static const ReceiveType_FlagLocalBattle:int = 5;
      
      public static const ReceiveType_Flag2pController:int = 6;
      
      public static const ReceiveType_Str2pController:int = 7;
      
      public static const ReceiveType_Time:int = 8;
      
      public static const ReceiveType_CharaNameStr:int = 9;
      
      public static const ReceiveType_NameOption_GK2:int = 10;
      
      public static const ReceiveType_NameOption_LvText:int = 11;
      
      public static const ReceiveType_NameOption_LvNum:int = 12;
      
      public static const ReceiveType_VariationNameStr:int = 13;
      
      public static const ReceiveType_TarismanHeaderStr:int = 14;
      
      public static const ReceiveType_TarismanNameStr:int = 15;
      
      public static const ReceiveType_ImageStrStart:int = 16;
      
      public static const ReceiveType_ImageStrEnd:int = ReceiveType_ImageStrStart + CharacterMax - 1;
      
      public static const ReceiveType_UnlockVarStart:int = ReceiveType_ImageStrEnd + 1;
      
      public static const ReceiveType_UnlockVarEnd:int = ReceiveType_UnlockVarStart + CharaVarIndexNum * CharacterMax - 1;
      
      public static const ReceiveType_KeyStrL1:int = ReceiveType_UnlockVarEnd + 1;
      
      public static const ReceiveType_KeyStrR1:int = ReceiveType_KeyStrL1 + 1;
      
      public static const ReceiveType_KeyStrL2:int = ReceiveType_KeyStrR1 + 1;
      
      public static const ReceiveType_KeyStrR2:int = ReceiveType_KeyStrL2 + 1;
      
      public static const ReceiveType_KeyStrRU:int = ReceiveType_KeyStrR2 + 1;
      
      public static const ReceiveType_KeyStrRD:int = ReceiveType_KeyStrRU + 1;
      
      public static const ReceiveType_KeyStrRL:int = ReceiveType_KeyStrRD + 1;
      
      public static const ReceiveType_KeyStrRR:int = ReceiveType_KeyStrRL + 1;
      
      public static const ReceiveType_KeyStrSingleLS:int = ReceiveType_KeyStrRR + 1;
      
      public static const ReceiveType_KeyStrSingleRS:int = ReceiveType_KeyStrSingleLS + 1;
      
      public static const ReceiveType_KeyStrSingleU:int = ReceiveType_KeyStrSingleRS + 1;
      
      public static const ReceiveType_KeyStrSingleD:int = ReceiveType_KeyStrSingleU + 1;
      
      public static const ReceiveType_KeyStrSingleL:int = ReceiveType_KeyStrSingleD + 1;
      
      public static const ReceiveType_KeyStrSingleR:int = ReceiveType_KeyStrSingleL + 1;
      
      public static const ReceiveType_SkillNameStrStart:int = ReceiveType_KeyStrSingleR + 1;
      
      public static const ReceiveType_SkillNameStrEnd:int = ReceiveType_SkillNameStrStart + SkillMax - 1;
      
      public static const ReceiveType_ImageStrNpcStart:int = ReceiveType_SkillNameStrEnd + 1;
      
      public static const ReceiveType_ImageStrNpcEnd:int = ReceiveType_ImageStrNpcStart + PlayerNumFri - 1;
      
      public static const ReceiveType_CharaSelectedStart:int = ReceiveType_ImageStrNpcEnd + 1;
      
      public static const ReceiveType_CharaSelectedEnd:int = ReceiveType_CharaSelectedStart + CharacterMax - 1;
      
      public static const ReceiveType_CharaVariationStart:int = ReceiveType_CharaSelectedEnd + 1;
      
      public static const ReceiveType_CharaVariationEnd:int = ReceiveType_CharaVariationStart + CharaVarIndexNum - 1;
      
      public static const ReceiveType_DLCUnlockFlag:int = ReceiveType_CharaVariationEnd + 1;
      
      public static const ReceiveType_JoyConSingleFlag:int = ReceiveType_DLCUnlockFlag + 1;
      
      public static const ReceiveType_WaitLoadNum:int = ReceiveType_JoyConSingleFlag;
      
      public static const ReceiveType_CostumeNum:int = ReceiveType_JoyConSingleFlag + 1;
      
      public static const ReceiveType_CharacterTableStart:int = ReceiveType_CostumeNum + 1;
      
      public static const ReceiveType_CostumeID:int = ReceiveType_CharacterTableStart;
      
      public static const ReceiveType_CID:int = ReceiveType_CostumeID + 1;
      
      public static const ReceiveType_MID:int = ReceiveType_CID + 1;
      
      public static const ReceiveType_PID:int = ReceiveType_MID + 1;
      
      public static const ReceiveType_UnlockNum:int = ReceiveType_PID + 1;
      
      public static const ReceiveType_Gokuaku:int = ReceiveType_UnlockNum + 1;
      
      public static const ReceiveType_SelectVoice1:int = ReceiveType_Gokuaku + 1;
      
      public static const ReceiveType_SelectVoice2:int = ReceiveType_SelectVoice1 + 1;
      
      public static const ReceiveType_DlcKey:int = ReceiveType_SelectVoice2 + 1;
      
      public static const ReceiveType_CustomCostume:int = ReceiveType_DlcKey + 1;
      
      public static const ReceiveType_AvatarSlotID:int = ReceiveType_CustomCostume + 1;
      
      public static const ReceiveType_AfterTU9Order:int = ReceiveType_AvatarSlotID + 1;
      
      public static const ReceiveType_CharacterTableEnd:int = ReceiveType_CharacterTableStart + CharacterTableMax * CharacterTableData;
      
      public static const ReceiveType_UseCustomList:int = ReceiveType_CharacterTableEnd + 1;
      
      public static const ReceiveType_CustomListNum:int = ReceiveType_UseCustomList + 1;
      
      public static const ReceiveType_CustomList_CID_Start:int = ReceiveType_CustomListNum + 1;
      
      public static const ReceiveType_CustomList_CID_End:int = ReceiveType_CustomList_CID_Start + CustomListMax - 1;
      
      public static const ReceiveType_CustomList_MID_Start:int = ReceiveType_CustomList_CID_End + 1;
      
      public static const ReceiveType_CustomList_MID_End:int = ReceiveType_CustomList_MID_Start + CustomListMax - 1;
      
      public static const ReceiveType_CustomList_PID_Start:int = ReceiveType_CustomList_MID_End + 1;
      
      public static const ReceiveType_CustomList_PID_End:int = ReceiveType_CustomList_PID_Start + CustomListMax - 1;
      
      public static const ReceiveType_CustomList_PartnerJudge_Start:int = ReceiveType_CustomList_PID_End + 1;
      
      public static const ReceiveType_CustomList_PartnerJudge_End:int = ReceiveType_CustomList_PartnerJudge_Start + CustomListMax - 1;
      
      public static const ReceiveType_Num:int = ReceiveType_CustomList_PartnerJudge_End + 1;
      
      private static const SendType_SelectCode:int = 0;
      
      private static const SendType_SelectVariation:int = 1;
      
      private static const SendType_SelectCustomFlag:int = 2;
      
      private static const SendType_SelectMid:int = 3;
      
      private static const SendType_SelectModelPreset:int = 4;
      
      private static const SendType_CurrentPlayerIndex:int = 5;
      
      private static const SendType_RequestCharaInfo:int = 6;
      
      private static const SendType_RequestImageStr:int = 7;
      
      private static const SendType_RequestNewInfo:int = 8;
      
      private static const SendType_RequestUnlock:int = 9;
      
      private static const SendType_RequestDecide:int = 10;
      
      private static const SendType_RequestSetFlagSkill:int = 11;
      
      private static const SendType_SelectUnlockIndex:int = 12;
      
      private static const SendType_RequestPlayVoice:int = 13;
      
      private static const SendType_SelectListIndex:int = 14;
      
      private static const SendType_RequestCountDownSe:int = 15;
      
      private static const SendType_EnableOptionBGM:int = 16;
      
      private static const SendType_AvatarSlotID:int = 17;
      
      private static const IndexNumRow:int = 3;
      
      private static const IndexNumColumn:int = 7;
      
      private static const IndexNumCharaNewIcon:int = 26;
      
      private static const IndexNumCharaVariationNewIcon:int = 20;
      
      private static const PlayerIndexOwn:int = 0;
      
      private static const PlayerIndexFriStart:int = PlayerIndexOwn + 1;
      
      private static const PlayerIndexFriEnd:int = PlayerIndexFriStart + PlayerNumFri - 1;
      
      private static const PlayerIndexEnmStart:int = PlayerIndexFriEnd + 1;
      
      private static const PlayerIndexEnmEnd:int = PlayerIndexEnmStart + PlayerNumEnm - 1;
      
      private static const PlayerTeamTypeOwn:int = 0;
      
      private static const PlayerTeamTypeFri:int = 1;
      
      private static const PlayerTeamTypeEnm:int = 2;
      
      private static const PlayerTeamTypeInvalid:int = -1;
      
      private static var SelectInfoTypeListIndex:int = 0;
      
      private static var SelectInfoTypeVarIndex:int = 1;
      
      private static var SelectInfoTypeNum:int = 2;
      
      public static var CharaListShiftCount:Array = new Array();
       
      
      private var m_callback:Callback = null;
      
      private var m_timeline:MovieClip = null;
      
      private var m_timer:CountDownTimer = null;
      
      private var m_current_player_index:int;
      
      private var m_current_select_index:int;
      
      private var m_select_info:Array;
      
      private var m_select_row:int;
      
      private var m_select_column:int;
      
      private var m_select_column_start:int;
      
      private var m_select_icon_row:int;
      
      private var m_select_icon_column:int;
      
      private var m_select_var:int;
      
      private var m_flag_skill:Boolean;
      
      private var m_chara_face:Array;
      
      private var m_chara_face_npc:Array;
      
      private var m_flag_unlock:Array;
      
      private var m_flag_change_player:Boolean;
      
      private var m_flag_decide:Boolean;
      
      private var m_flag_exit:Boolean;
      
      private var m_skill_str_width_default:Number;
      
      private var m_skill_str_scalex_default:Number;
      
      private var m_icon_show_wait:Array;
      
      private var m_chara_list:Array;
      
      private var m_chara_list_num:int = 0;
      
      private var m_chara_num_column:int = 0;
      
      private var m_pre_time:int = 99;
      
      private var m_selected_index_list:Array;
      
      private var m_flag_customlist = false;
      
      private var isKeyboardControllerActive:Boolean;
	  
      public function CharaSele()
      {
         super();
         trace("[CHARASELE] CharaSele()");
         m_callback = new Callback(ReceiveType_Num);
         m_timeline = null;
         m_timer = new CountDownTimer();
         m_current_player_index = 0;
         m_current_select_index = 0;
         m_select_info = new Array(PlayerMax);
         m_selected_index_list = new Array(PlayerMax);
         var i:int = 0;
         var j:int = 0;
         i = 0;
         while(m_select_info.length > i)
         {
            m_select_info[i] = new Array(SelectInfoTypeNum);
            m_select_info[i][SelectInfoTypeListIndex] = 0;
            m_select_info[i][SelectInfoTypeVarIndex] = 0;
            i++;
         }
         i = 0;
         while(m_selected_index_list.length > i)
         {
            m_selected_index_list[i] = -1;
            i++;
         }
         m_select_row = 0;
         m_select_column = 0;
         m_select_column_start = 0;
         m_select_icon_row = -1;
         m_select_icon_column = -1;
         m_select_var = 0;
         m_flag_skill = false;
         m_flag_change_player = false;
         m_flag_decide = false;
         m_flag_exit = false;
         m_skill_str_width_default = 0;
         m_skill_str_scalex_default = 1;
         m_icon_show_wait = new Array(IndexNumCharaVariationNewIcon);
         i = 0;
         while(i < m_icon_show_wait.length)
         {
            m_icon_show_wait[i] = 0;
            i++;
         }
         m_chara_list = null;
         m_chara_list_num = 0;
         m_chara_num_column = 0;
         i = 0;
         while(i < CharacterMax)
         {
            m_callback.AddCallbackSetUserDataInt(ReceiveType_CharaSelectedStart + i,0);
            i++;
         }
         i = 0;
         while(i < CharaVarIndexNum)
         {
            m_callback.AddCallbackSetUserDataInt(ReceiveType_CharaVariationStart + i,0);
            i++;
         }
         try
         {
            ExternalInterface.addCallback("ForcingCancel",pushKeyCancel);
            return;
         }
         catch(e:Error)
         {
            trace("[CHARASELE] ForcingCancel EXCEPTION");
            return;
         }
      }
      
      public function Initialize(param1:MovieClip) : void
      {
         m_timeline = param1;
         m_timeline.visible = false;
         m_timeline.cha_frame.visible = false;
         m_timeline.cha_skill.visible = false;
         m_timeline.cha_select.visible = false;
         m_timeline.cha_parameter.visible = false;
         m_timeline.cha_arrow.visible = false;
         m_timeline.cha_select_cur.visible = false;
         m_timeline.press2P.visible = false;
         m_timer.Initialize(m_timeline.timer.nest._CMN_M_B_mc_timer,null);
         m_current_player_index = 0;
         m_select_row = 0;
         m_select_column = 0;
         m_select_column_start = 0;
         m_select_var = 0;
         m_select_icon_row = -1;
         m_select_icon_column = -1;
         m_flag_skill = false;
         m_flag_change_player = false;
         m_flag_decide = false;
         m_skill_str_width_default = m_timeline.cha_skill.inact_skill.skill01.sys_skill.width;
         m_skill_str_scalex_default = m_timeline.cha_skill.inact_skill.skill01.sys_skill.scaleX;
         m_pre_time = 99;
         m_chara_list = null;
         m_chara_list_num = 0;
         m_chara_num_column = 0;
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,waitCharaInfo);
      }
      
      private function waitCharaInfo(param1:Event) : void
      {
         var _loc2_:int = 0;
         var _loc3_:int = 0;
         var _loc4_:int = 0;
         var _loc6_:* = undefined;
         var _loc7_:int = 0;
         var _loc8_:int = 0;
         var _loc9_:int = 0;
         var _loc10_:Array = null;
         var _loc11_:int = 0;
         var _loc12_:String = null;
         var _loc13_:int = 0;
         var _loc14_:int = 0;
         var _loc15_:int = 0;
         var _loc16_:Array = null;
         var _loc17_:Boolean = false;
         var _loc18_:Boolean = false;
         var _loc19_:int = 0;
         var _loc20_:int = 0;
         var _loc21_:int = 0;
         if(!m_callback)
         {
            return;
         }
         if(!m_callback.GetUserDataValidFlag(ReceiveType_DLCUnlockFlag))
         {
            trace("Wait...NotReceive:DlcFlag");
            return;
         }
         if(!m_callback.GetUserDataValidFlag(ReceiveType_UseCustomList))
         {
            return;
         }
         var _loc5_:int = m_callback.GetUserDataInt(ReceiveType_UseCustomList);
         if(_loc5_)
         {
            m_flag_customlist = true;
            if(!m_callback.GetUserDataValidFlag(ReceiveType_CustomListNum))
            {
               return;
            }
            _loc6_ = m_callback.GetUserDataInt(ReceiveType_CustomListNum);
            _loc2_ = 0;
            while(_loc2_ < _loc6_)
            {
               if(!m_callback.GetUserDataValidFlag(ReceiveType_CustomList_CID_Start + _loc2_))
               {
                  return;
               }
               if(!m_callback.GetUserDataValidFlag(ReceiveType_CustomList_MID_Start + _loc2_))
               {
                  return;
               }
               if(!m_callback.GetUserDataValidFlag(ReceiveType_CustomList_PID_Start + _loc2_))
               {
                  return;
               }
               if(!m_callback.GetUserDataValidFlag(ReceiveType_CustomList_PartnerJudge_Start + _loc2_))
               {
                  return;
               }
               _loc2_++;
            }
            _loc7_ = m_callback.GetUserDataInt(ReceiveType_DLCUnlockFlag);
            if(!m_callback.GetUserDataValidFlag(ReceiveType_CostumeNum))
            {
               return;
            }
            _loc8_ = m_callback.GetUserDataInt(ReceiveType_CostumeNum);
            _loc9_ = ReceiveType_CharacterTableStart;
            while(_loc9_ < ReceiveType_CharacterTableStart + _loc8_ * CharacterTableData)
            {
               if(!m_callback.GetUserDataValidFlag(_loc9_))
               {
                  return;
               }
               _loc9_++;
            }
            m_chara_list = recieveList(_loc7_);
            if(!m_callback.GetUserDataValidFlag(ReceiveType_CharacterTableEnd + 1))
            {
               return;
            }
            _loc10_ = new Array();
            _loc2_ = 0;
            while(_loc2_ < _loc6_)
            {
               _loc12_ = m_callback.GetUserDataString(ReceiveType_CustomList_CID_Start + _loc2_);
               _loc13_ = m_callback.GetUserDataInt(ReceiveType_CustomList_MID_Start + _loc2_);
               _loc14_ = m_callback.GetUserDataInt(ReceiveType_CustomList_PID_Start + _loc2_);
               _loc15_ = m_callback.GetUserDataInt(ReceiveType_CustomList_PartnerJudge_Start + _loc2_);
               _loc10_.push([_loc12_,_loc13_,_loc14_,_loc15_]);
               _loc2_++;
            }
            _loc2_ = 0;
            while(_loc2_ < m_chara_list.length)
            {
               _loc16_ = m_chara_list[_loc2_];
               _loc17_ = true;
               _loc3_ = 0;
               while(_loc3_ < _loc16_.length)
               {
                  _loc18_ = true;
                  _loc4_ = 0;
                  while(_loc4_ < _loc10_.length)
                  {
                     if(_loc16_[_loc3_][0] == _loc10_[_loc4_][0])
                     {
                        if(_loc10_[_loc4_][1] == -1 || _loc16_[_loc3_][1] == _loc10_[_loc4_][1])
                        {
                           if(_loc10_[_loc4_][2] == -1 || _loc16_[_loc3_][2] == _loc10_[_loc4_][2])
                           {
                              if(!(_loc10_[_loc4_][3] != 1 && _loc16_[_loc3_][VarTypeCustomCostume]))
                              {
                                 _loc18_ = false;
                                 _loc17_ = false;
                              }
                           }
                        }
                     }
                     _loc4_++;
                  }
                  if(_loc18_)
                  {
                     _loc16_.splice(_loc3_,1);
                     _loc3_--;
                  }
                  _loc3_++;
               }
               if(_loc17_)
               {
                  m_chara_list.splice(_loc2_,1);
                  _loc2_--;
               }
               _loc2_++;
            }
            _loc11_ = 3 - m_chara_list.length % 3;
            _loc2_ = 0;
            while(_loc11_ > _loc2_)
            {
               m_chara_list.concat([[["",0,0,0]]]);
               _loc2_++;
            }
         }
         else
         {
            _loc19_ = m_callback.GetUserDataInt(ReceiveType_DLCUnlockFlag);
            if(!m_callback.GetUserDataValidFlag(ReceiveType_CostumeNum))
            {
               return;
            }
            _loc20_ = m_callback.GetUserDataInt(ReceiveType_CostumeNum);
            _loc21_ = ReceiveType_CharacterTableStart;
            while(_loc21_ < ReceiveType_CharacterTableStart + _loc20_ * CharacterTableData)
            {
               if(!m_callback.GetUserDataValidFlag(_loc21_))
               {
                  return;
               }
               _loc21_++;
            }
            m_chara_list = recieveList(_loc19_);
            if(!m_callback.GetUserDataValidFlag(ReceiveType_CharacterTableEnd + 1))
            {
               return;
            }
         }
         m_chara_list_num = m_chara_list.length;
         m_chara_num_column = (m_chara_list_num - 1) / IndexNumRow + 1;
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitCharaInfo);
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,requestUnlock);
      }
      
      private function recieveList(param1:int) : Array
      {
         var _loc4_:int = 0;
         var _loc8_:int = 0;
         var _loc9_:Array = null;
         var _loc10_:Array = null;
         var _loc11_:String = null;
         var _loc12_:int = 0;
         var _loc13_:int = 0;
         var _loc14_:int = 0;
         var _loc15_:int = 0;
         var _loc16_:Array = null;
         var _loc17_:int = 0;
         var _loc18_:int = 0;
         var _loc19_:int = 0;
         var _loc20_:int = 0;
         var _loc21_:int = 0;
         var _loc22_:Array = null;
         var _loc23_:Boolean = false;
         var _loc24_:int = 0;
         var _loc25_:int = 0;
         var _loc26_:int = 0;
         var _loc2_:Array = new Array();
         var _loc3_:int = m_callback.GetUserDataInt(ReceiveType_CostumeNum);
		 
		 //////////
		 var map_cc:Array = new Array();
		 var main_cac_index:int = -1; // Added int 1.10
		 var dummy_goks:Array = new Array(); // Added in 1.10
		 var other_cacs:Array = new Array(); // Added in 1.10
		 //////////
		 
         _loc4_ = 0;
         while(_loc4_ < _loc3_)
         {
            _loc9_ = new Array();
            while(true)
            {
               _loc10_ = new Array();
               _loc11_ = m_callback.GetUserDataString(ReceiveType_CID + _loc4_ * CharacterTableData);
               _loc12_ = m_callback.GetUserDataInt(ReceiveType_MID + _loc4_ * CharacterTableData);
               _loc13_ = m_callback.GetUserDataInt(ReceiveType_PID + _loc4_ * CharacterTableData);
               _loc14_ = m_callback.GetUserDataInt(ReceiveType_UnlockNum + _loc4_ * CharacterTableData);
               _loc15_ = m_callback.GetUserDataInt(ReceiveType_Gokuaku + _loc4_ * CharacterTableData);
               _loc16_ = new Array();
               _loc16_.push(m_callback.GetUserDataInt(ReceiveType_SelectVoice1 + _loc4_ * CharacterTableData));
               _loc16_.push(m_callback.GetUserDataInt(ReceiveType_SelectVoice2 + _loc4_ * CharacterTableData));
               _loc17_ = m_callback.GetUserDataInt(ReceiveType_DlcKey + _loc4_ * CharacterTableData);
               _loc18_ = m_callback.GetUserDataInt(ReceiveType_CustomCostume + _loc4_ * CharacterTableData);
               _loc19_ = m_callback.GetUserDataInt(ReceiveType_AvatarSlotID + _loc4_ * CharacterTableData);
               _loc20_ = m_callback.GetUserDataInt(ReceiveType_AfterTU9Order + _loc4_ * CharacterTableData);
               _loc10_.push(_loc11_);
               _loc10_.push(_loc12_);
               _loc10_.push(_loc13_);
               _loc10_.push(_loc14_);
               _loc10_.push(_loc15_);
               _loc10_.push(_loc16_);
               _loc10_.push(_loc17_);
               _loc10_.push(_loc18_);
               _loc10_.push(_loc19_);
               _loc10_.push(_loc20_);
               _loc9_.push(_loc10_);
			   
			   //////////
			   
			   if (main_cac_index < 0) // 1.10
			   {
			       main_cac_index = _loc19_; 
			   }
			   
			   if (_loc18_)
			   {
			       var first:Array = _loc9_[0];
				   var key:String = first[0] + first[1].toString();
				   map_cc[key] = _loc10_;
			   }
			   else
			   {
				   // 1.10
				   if (_loc17_ < 0 && _loc11_ == "GOK")
				   {
				       dummy_goks.push(_loc10_);					   
				   }
				   else if (_loc19_ != main_cac_index && _loc11_ == "HUM")
				   {
				       other_cacs.push(_loc10_);
				   }
			   }
			   //////////
			   
               _loc21_ = m_callback.GetUserDataInt(ReceiveType_CostumeID + (_loc4_ + 1) * CharacterTableData);
               if(_loc21_ == 0)
               {
                  break;
               }
               _loc4_++;
            }
            _loc2_.push(_loc9_);
            _loc4_++;
         }
		 
		 //////////
		 // XV2Patcher slots 		 
		 var i:int = 0;
		 var SlotsString:String = XV2Patcher.GetSlotsData();
		 
		 _loc2_ = new Array(); 	
		 
		 // Loop for xv2patcher
		 while (i < SlotsString.length)
		 {
			_loc9_ = new Array();
			
			if (SlotsString.substr(i, 1) != "{")
			{
				trace("Format error 1\n");
				return null;
			}
			
			i++;
			
			while (SlotsString.substr(i, 1) == "[")
			{
				i++;				
				_loc10_ = new Array();
				
				var pos:int = SlotsString.indexOf("]", i);
				if (pos == -1)
				{
					trace("Format error 2\n");
					return null;
				}
				
				var costume:String = SlotsString.substring(i,pos);
				var fields:Array = costume.split(",");
				
				if (fields.length != 8)
				{
					trace("Invalid number of elements: " + fields.length);
					return null;
				}
				
				_loc10_.push(fields[0]); // Code
				_loc10_.push(int(fields[1])); // Mid
				_loc10_.push(int(fields[2])); // Model preset
				_loc10_.push(int(fields[3])); // Unlock index
				_loc10_.push(int(fields[4])); // Flag GK2				
				_loc10_.push(new Array(int(fields[5]), int(fields[6]))); // Voices id list
				_loc10_.push(int(fields[7])); // Dlc key flag
				_loc10_.push(0); // Custom costume flag
				_loc10_.push(main_cac_index); // Avatar ID (new in 1.10)
				_loc10_.push(-1); // After TU9 Order, whatever this shit is. It was added in 1.14, let's set it to -1...
				
				i = pos+1;
				_loc9_.push(_loc10_);
			}
			
			if (_loc9_.length > 0)
			{
				var first:Array = _loc9_[0];
				var key:String = first[0] + first[1].toString();
				var cc:Array = map_cc[key];
				
				if (cc != undefined)
				{
					_loc9_.push(cc);
				}
			}
			
			if (SlotsString.substr(i, 1) != "}")
			{
				trace("Format error 3\n");
				return null;
			}
			
			i++;
			_loc2_.push(_loc9_);
		 }
		 
		 // 1.10: insert the dummy goks now		 
		 for (i = 0; i < dummy_goks.length; i++)
		 {
	         var dummy_slot:Array = new Array();
			 dummy_slot.push(dummy_goks[i]);
			 _loc2_.push(dummy_slot);
		 }	

		 // 1.10: insert the other cacs now
		 var cac_slot:Array;
		 var current_cac:int = -1;		 
		 for (i = 0; i < other_cacs.length; i++)
		 {
	         if (other_cacs[i][8] != current_cac) // current avatar id
			 {
                 current_cac = other_cacs[i][8];
				 
				 if (i != 0)
                 {
					_loc2_.push(cac_slot); // put previous
				 }
				 
				 cac_slot = new Array();
			 }
			 
			 cac_slot.push(other_cacs[i]);
			 
			 if (i == other_cacs.length-1)
			 {
				_loc2_.push(cac_slot);
			 }
		 }
		 
		 //////////	
		 
         var _loc5_:int = 0;
         var _loc6_:int = 0;
         _loc4_ = 0;
         while(_loc4_ < _loc2_.length)
         {
            _loc22_ = _loc2_[_loc4_];
            _loc23_ = true;
            _loc24_ = 0;
            while(_loc24_ < _loc22_.length)
            {
               if((param1 & _loc22_[_loc24_][VarTypeDlcKeyFlag]) == 0)
               {
                  _loc22_.splice(_loc24_,1);
                  _loc24_--;
               }
               else
               {
                  _loc23_ = false;
               }
               _loc24_++;
            }
            if(_loc23_)
            {
               _loc2_.splice(_loc4_,1);
               _loc4_--;
            }
            else
            {
               _loc25_ = _loc6_;
               _loc26_ = _loc22_[0][VarTypeAfterTU9Order];
               if(_loc26_ == -1)
               {
                  _loc25_ = _loc6_;
               }
               else
               {
                  _loc25_ = _loc26_ + 122;
                  _loc6_--;
               }
               CharaListShiftCount.push(_loc25_);
            }
            _loc4_++;
            _loc6_++;
         }
         var _loc7_:int = 3 - _loc2_.length % 3;
         _loc4_ = 0;
         while(_loc7_ > _loc4_)
         {
            _loc2_.concat([[[InvalidCode,0,0,0]]]);
            _loc4_++;
         }
         return _loc2_;
      }
      
      private function requestUnlock(param1:Event) : void
      {
         var _loc4_:Boolean = false;
         var _loc5_:Array = null;
         var _loc6_:Boolean = false;
         var _loc7_:int = 0;
         var _loc8_:Array = null;
         var _loc9_:String = null;
         var _loc10_:int = 0;
         var _loc11_:int = 0;
         var _loc12_:int = 0;
         var _loc13_:int = 0;
         var _loc14_:int = 0;
         var _loc2_:int = 0;
         var _loc3_:int = 0;
         _loc2_ = 0;
         while(CharacterMax > _loc2_)
         {
            _loc4_ = false;
            _loc5_ = getCharaInfo(_loc2_);
            if(!_loc5_)
            {
               _loc3_ = 0;
               while(CharaVarIndexNum > _loc3_)
               {
                  SetUserDataInt(ReceiveType_UnlockVarStart + _loc2_ * CharaVarIndexNum + _loc3_,0);
                  _loc3_++;
               }
               SetUserDataString(ReceiveType_ImageStrStart + _loc2_,"");
            }
            else if(_loc5_.length <= _loc3_)
            {
               _loc3_ = 0;
               while(CharaVarIndexNum > _loc3_)
               {
                  SetUserDataInt(ReceiveType_UnlockVarStart + _loc2_ * CharaVarIndexNum + _loc3_,0);
                  _loc3_++;
               }
               SetUserDataString(ReceiveType_ImageStrStart + _loc2_,"");
            }
            else
            {
               _loc6_ = false;
               _loc3_ = CharaVarIndexNum - 1;
               while(0 <= _loc3_)
               {
                  _loc7_ = _loc2_ * CharaVarIndexNum + _loc3_;
                  if(_loc5_.length <= _loc3_)
                  {
                     SetUserDataInt(ReceiveType_UnlockVarStart + _loc7_,0);
                  }
                  else
                  {
                     _loc8_ = _loc5_[_loc3_];
                     _loc9_ = _loc8_[VarTypeCode];
                     if(_loc9_ == InvalidCode)
                     {
                        SetUserDataInt(ReceiveType_UnlockVarStart + _loc7_,0);
                     }
                     else
                     {
                        _loc10_ = _loc8_[VarTypeMid];
                        _loc4_ = true;
                        if(!m_callback.GetUserDataValidFlag(ReceiveType_UnlockVarStart + _loc7_))
                        {
                           _loc11_ = _loc8_[VarTypeUnlockIndex];
                           _loc12_ = _loc8_[VarTypeAvatarSlotID];
                           _loc13_ = _loc8_[VarTypeCustomCostume];
                           _loc14_ = _loc3_;
                           m_callback.CallbackUserData("user",SendType_AvatarSlotID,_loc12_);
                           m_callback.CallbackUserDataString("user",SendType_SelectCode,_loc9_);
                           m_callback.CallbackUserData("user",SendType_SelectUnlockIndex,_loc11_);
                           m_callback.CallbackUserData("user",SendType_SelectVariation,_loc14_);
                           m_callback.CallbackUserData("user",SendType_SelectCustomFlag,_loc13_);
                           m_callback.CallbackUserData("user",SendType_SelectMid,_loc10_);
                           m_callback.CallbackUserData("user",SendType_RequestUnlock,_loc7_);
                           m_callback.CallbackUserData("user",SendType_RequestImageStr,_loc2_);
                           m_callback.CallbackUserData("user",SendType_RequestNewInfo,CharaListShiftCount[_loc2_]);
                        }
                     }
                  }
                  _loc3_--;
               }
               if(!_loc4_)
               {
                  SetUserDataString(ReceiveType_ImageStrStart + _loc2_,"");
               }
            }
            _loc2_++;
         }
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,requestUnlock);
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,waitUnlock);
      }
      
      //////////
	  // Decompiler failed with this function (produced bad result), so here it is manually decompiled from version 1.11
	  // If the function doesn't change in the future, just carry it over the next version
	  //////////
	  // Decompiled (manual) version
	  
	  private function waitUnlock(param1:Event) : void
	  {
		 var _loc5_:int = 0;
         var _loc6_:int = 0;
         var _loc7_:int = 0;
         var _loc2_:int = 0;
         var _loc3_:int = 0;
		 
		 for (; _loc3_ < m_select_info.length; _loc3_++)
		 {
			_loc5_ = -1;
            _loc2_ = 0;
			
			for (; _loc2_ < CharacterMax; _loc2_++)
			{
			   _loc6_ = PlayerIndexOwn + _loc2_;
			   if (!m_callback.GetUserDataValidFlag(ReceiveType_FlagSelectAvatar))
				  return;
				  
			   _loc7_ = m_callback.GetUserDataInt(ReceiveType_FlagSelectAvatar); 
			    if(m_flag_customlist)
			    {
				   _loc7_ = 1;
				   _loc5_ = 0;
			    }
				
				if (_loc3_ == PlayerIndexOwn || _loc7_ || !checkAvatar(_loc6_))
				{
				   if (checkUnlockChara(_loc6_))
				   {
				      _loc5_ = _loc6_;
					  break;
                   }					  
				}
			}
			
			if (_loc5_ < 0)
			   return;
			   
			m_select_row = calcIconIndexRow(_loc5_);
			m_select_column = calcIconIndexColumn(_loc5_);
			m_current_player_index = _loc3_;
			m_select_var = 0;
			setSelectChara();
		 }
		 
		 m_current_player_index = PlayerIndexOwn;
		 var _loc4_:* = m_select_info[m_current_player_index][SelectInfoTypeListIndex];
		 m_select_row = calcIconIndexRow(_loc4_);
		 m_select_column = calcIconIndexColumn(_loc4_);
		 m_select_var = m_select_info[m_current_player_index][SelectInfoTypeVarIndex];
		 sendCharaInfo(m_current_player_index);
		 m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitUnlock);
		 m_timeline.stage.addEventListener(Event.ENTER_FRAME,waitStart);
	  }
	  
	  /* Pre 1.09 version 
	  private function waitUnlock(param1:Event) : void
      {
        //////////
		// This function was deleted, and replaced by the same function of the 1.08 decompilation
		// Otherwise, we got a black screen
		// That was probably caused by a failure of the decompiler (urgh)
		//////////		
		 
		 var _loc5_:int = 0;
         var _loc6_:int = 0;
         var _loc7_:int = 0;
         var _loc2_:int = 0;
         var _loc3_:int = 0;
         _loc3_ = 0;
         while(true)
         {
            if(m_select_info.length <= _loc3_)
            {
               m_current_player_index = PlayerIndexOwn;
               var _loc4_:* = m_select_info[m_current_player_index][SelectInfoTypeListIndex];
               m_select_row = calcIconIndexRow(_loc4_);
               m_select_column = calcIconIndexColumn(_loc4_);
               m_select_var = m_select_info[m_current_player_index][SelectInfoTypeVarIndex];
               sendCharaInfo(m_current_player_index);
               m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitUnlock);
               m_timeline.stage.addEventListener(Event.ENTER_FRAME,waitStart);
               return;
            }
            _loc5_ = -1;
            _loc2_ = 0;
            for(; CharacterMax > _loc2_; _loc2_++)
            {
               _loc6_ = PlayerIndexOwn + _loc2_;
               _loc6_ = _loc6_ % CharacterMax;
               _loc7_ = m_callback.GetUserDataInt(ReceiveType_FlagSelectAvatar);
               if(m_flag_customlist && m_chara_list.length == 1)
               {
                  _loc7_ = 1;
               }
               if(_loc3_ != PlayerIndexOwn)
               {
                  if(!_loc7_)
                  {
                     if(checkAvatar(_loc6_))
                     {
                        continue;
                     }
                  }
               }
               if(!checkUnlockChara(_loc6_))
               {
                  continue;
               }
               _loc5_ = _loc6_;
               break;
            }
            if(_loc5_ < 0)
            {
               break;
            }
            m_select_row = calcIconIndexRow(_loc5_);
            m_select_column = calcIconIndexColumn(_loc5_);
            m_current_player_index = _loc3_;
            m_select_var = 0;
            setSelectChara();
            _loc3_++;
         }
      }*/
	        
      private function waitStart(param1:Event) : void
      {
         var _loc2_:int = 0;
         _loc2_ = 0;
         while(ReceiveType_WaitLoadNum > _loc2_)
         {
            if(!m_callback.GetUserDataValidFlag(_loc2_))
            {
               return;
            }
            _loc2_++;
         }
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitStart);
         startMain();
      }
      
      private function ResetIcons() : void
      {
         var _loc1_:String = m_callback.GetUserDataString(ReceiveType_KeyStrL2);
         var _loc2_:String = m_callback.GetUserDataString(ReceiveType_KeyStrR2);
         var _loc3_:String = m_callback.GetUserDataString(ReceiveType_KeyStrRU);
         var _loc4_:String = m_callback.GetUserDataString(ReceiveType_KeyStrRD);
         var _loc5_:String = m_callback.GetUserDataString(ReceiveType_KeyStrRL);
         var _loc6_:String = m_callback.GetUserDataString(ReceiveType_KeyStrRR);
         isKeyboardControllerActive = _loc1_.indexOf("src=\"kb") > 0;
         m_timeline.cha_skill.inact_skill.skill01.sys_com01.htmlText = _loc2_;
         m_timeline.cha_skill.inact_skill.skill01.sys_com02.htmlText = _loc3_;
         m_timeline.cha_skill.inact_skill.skill01.sys_com03.htmlText = "";
         m_timeline.cha_skill.inact_skill.skill01.sys_01.visible = false;
         m_timeline.cha_skill.inact_skill.skill02.sys_com01.htmlText = _loc2_;
         m_timeline.cha_skill.inact_skill.skill02.sys_com02.htmlText = _loc5_;
         m_timeline.cha_skill.inact_skill.skill02.sys_com03.htmlText = "";
         m_timeline.cha_skill.inact_skill.skill02.sys_01.visible = false;
         m_timeline.cha_skill.inact_skill.skill03.sys_com01.htmlText = _loc2_;
         m_timeline.cha_skill.inact_skill.skill03.sys_com02.htmlText = _loc6_;
         m_timeline.cha_skill.inact_skill.skill03.sys_com03.htmlText = "";
         m_timeline.cha_skill.inact_skill.skill03.sys_01.visible = false;
         m_timeline.cha_skill.inact_skill.skill04.sys_com01.htmlText = _loc2_;
         m_timeline.cha_skill.inact_skill.skill04.sys_com02.htmlText = _loc4_;
         m_timeline.cha_skill.inact_skill.skill04.sys_com03.htmlText = "";
         m_timeline.cha_skill.inact_skill.skill04.sys_01.visible = false;
         if(isKeyboardControllerActive)
         {
            m_timeline.cha_skill.inact_skill.skill05.sys_com01.htmlText = _loc1_;
            m_timeline.cha_skill.inact_skill.skill05.sys_com02.htmlText = _loc3_;
            m_timeline.cha_skill.inact_skill.skill05.sys_com03.htmlText = "";
            m_timeline.cha_skill.inact_skill.skill05.sys_01.visible = false;
            m_timeline.cha_skill.inact_skill.skill06.sys_com01.htmlText = _loc1_;
            m_timeline.cha_skill.inact_skill.skill06.sys_com02.htmlText = _loc5_;
            m_timeline.cha_skill.inact_skill.skill06.sys_com03.htmlText = "";
            m_timeline.cha_skill.inact_skill.skill06.sys_01.visible = false;
            m_timeline.cha_skill.inact_skill.skill07.sys_com01.htmlText = _loc1_;
            m_timeline.cha_skill.inact_skill.skill07.sys_com02.htmlText = _loc4_;
            m_timeline.cha_skill.inact_skill.skill07.sys_com03.htmlText = "";
            m_timeline.cha_skill.inact_skill.skill07.sys_01.visible = false;
            m_timeline.cha_skill.inact_skill.skill08.sys_com01.htmlText = _loc1_;
            m_timeline.cha_skill.inact_skill.skill08.sys_com02.htmlText = _loc6_;
            m_timeline.cha_skill.inact_skill.skill08.sys_com03.htmlText = "";
            m_timeline.cha_skill.inact_skill.skill08.sys_01.visible = false;
         }
         else
         {
            m_timeline.cha_skill.inact_skill.skill05.sys_com01.htmlText = _loc1_;
            m_timeline.cha_skill.inact_skill.skill05.sys_com02.htmlText = _loc2_;
            m_timeline.cha_skill.inact_skill.skill05.sys_com03.htmlText = _loc3_;
            m_timeline.cha_skill.inact_skill.skill05.sys_01.visible = true;
            m_timeline.cha_skill.inact_skill.skill06.sys_com01.htmlText = _loc1_;
            m_timeline.cha_skill.inact_skill.skill06.sys_com02.htmlText = _loc2_;
            m_timeline.cha_skill.inact_skill.skill06.sys_com03.htmlText = _loc5_;
            m_timeline.cha_skill.inact_skill.skill06.sys_01.visible = true;
            m_timeline.cha_skill.inact_skill.skill07.sys_com01.htmlText = _loc1_;
            m_timeline.cha_skill.inact_skill.skill07.sys_com02.htmlText = _loc2_;
            m_timeline.cha_skill.inact_skill.skill07.sys_com03.htmlText = _loc4_;
            m_timeline.cha_skill.inact_skill.skill07.sys_01.visible = true;
            m_timeline.cha_skill.inact_skill.skill08.sys_com01.htmlText = _loc1_;
            m_timeline.cha_skill.inact_skill.skill08.sys_com02.htmlText = _loc2_;
            m_timeline.cha_skill.inact_skill.skill08.sys_com03.htmlText = _loc6_;
            m_timeline.cha_skill.inact_skill.skill08.sys_01.visible = true;
         }
         var _loc7_:String = m_callback.GetUserDataString(ReceiveType_TarismanHeaderStr);
         m_timeline.cha_skill.inact_skill.skill09.sys_com01.htmlText = _loc7_;
         m_timeline.cha_parameter.nest_clothes.sys_clothes.htmlText = "";
         var _loc8_:String = m_callback.GetUserDataString(ReceiveType_KeyStrL1);
         m_timeline.cha_parameter.nest_clothes.sys_l.htmlText = _loc8_;
         var _loc9_:String = m_callback.GetUserDataString(ReceiveType_KeyStrR1);
         m_timeline.cha_parameter.nest_clothes.sys_r.htmlText = _loc9_;
      }
      
      private function startMain() : void
      {
         var _loc19_:int = 0;
         var _loc20_:MovieClip = null;
         var _loc21_:int = 0;
         var _loc22_:MovieClip = null;
         var _loc23_:MovieClip = null;
         var _loc24_:Array = null;
         var _loc25_:Bitmap = null;
         var _loc26_:String = null;
         var _loc27_:String = null;
         var _loc28_:String = null;
         var _loc29_:Bitmap = null;
         var _loc30_:MovieClip = null;
         var _loc31_:int = 0;
         var _loc32_:MovieClip = null;
         var _loc1_:int = 0;
         m_timeline.visible = true;
         m_timeline.cha_frame.cmn_CMN_M_frame.visible = true;
         m_timeline.cha_skill.visible = false;
         m_timeline.cha_select.visible = true;
         m_timeline.cha_parameter.visible = true;
         m_timeline.cha_arrow.visible = true;
         var _loc2_:int = m_callback.GetUserDataInt(ReceiveType_PlayerFriNum);
         var _loc3_:int = m_callback.GetUserDataInt(ReceiveType_PlayerEnmNum);
         if(_loc2_ == 0 && _loc3_ == 0)
         {
            m_timeline.cha_parameter.nest_ready.visible = false;
            m_timeline.cha_parameter.ready_base.visible = false;
            m_timeline.cha_parameter.ready_base.vs.visible = false;
         }
         else
         {
            m_timeline.cha_parameter.nest_ready.visible = true;
            m_timeline.cha_parameter.ready_base.visible = true;
            if(_loc3_ > 0)
            {
               m_timeline.cha_parameter.ready_base.vs.visible = true;
            }
            else
            {
               m_timeline.cha_parameter.ready_base.vs.visible = false;
            }
         }
         var _loc4_:int = m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle);
         if(_loc4_)
         {
            m_timeline.cha_parameter.icon_1P.play();
            m_timeline.cha_parameter.icon_1P.visible = true;
            m_timeline.cha_parameter.icon_2P.play();
            m_timeline.cha_parameter.icon_2P.visible = false;
         }
         else
         {
            m_timeline.cha_parameter.icon_1P.stop();
            m_timeline.cha_parameter.icon_1P.visible = false;
            m_timeline.cha_parameter.icon_2P.stop();
            m_timeline.cha_parameter.icon_2P.visible = false;
            m_timeline.press2P.stop();
            m_timeline.press2P.visible = false;
         }
         var _loc5_:MovieClip = getReadyIconMc(0);
         _loc5_.visible = true;
         _loc1_ = 0;
         while(PlayerNumFri > _loc1_)
         {
            _loc19_ = PlayerIndexFriStart + _loc1_;
            _loc20_ = getReadyIconMc(_loc19_);
            if(_loc1_ < _loc2_)
            {
               _loc20_.visible = true;
            }
            else
            {
               _loc20_.visible = false;
            }
            _loc1_++;
         }
         _loc1_ = 0;
         while(PlayerNumEnm > _loc1_)
         {
            _loc21_ = PlayerIndexEnmStart + _loc1_;
            _loc22_ = getReadyIconMc(_loc21_);
            if(_loc1_ < _loc3_)
            {
               _loc22_.visible = true;
            }
            else
            {
               _loc22_.visible = false;
            }
            _loc1_++;
         }
         m_timeline.cha_parameter.sys_charaName.sys_charaName.htmlText = "";
         if(m_timeline.cha_parameter.sys_skill_header)
         {
            m_timeline.cha_parameter.sys_skill_header.sys_skill_header.htmlText = "";
         }
         setSelectChara();
         var _loc6_:MovieClip = m_timeline.cha_new;
         _loc1_ = 0;
         while(IndexNumCharaNewIcon > _loc1_)
         {
            _loc23_ = _loc6_["chara_new_" + _loc1_];
            if(!checkCharaNewIconVisible(_loc1_))
            {
               _loc23_.visible = false;
            }
            _loc1_++;
         }
         m_chara_face = new Array(m_chara_list_num);
         _loc1_ = 0;
         while(m_chara_list_num > _loc1_)
         {
            _loc24_ = getCharaInfo(_loc1_);
            _loc25_ = null;
            if(_loc24_ && _loc24_.length > 0)
            {
               _loc26_ = _loc24_[0][VarTypeCode];
               _loc27_ = m_callback.GetUserDataString(ReceiveType_ImageStrStart + _loc1_);
               if(_loc26_ != InvalidCode)
               {
                  _loc25_ = new Bitmap(null);
                  IggyFunctions.setTextureForBitmap(_loc25_,_loc27_);
                  _loc25_.scaleX = 256 / _loc25_.width;
                  _loc25_.scaleY = 128 / _loc25_.height;
               }
            }
            m_chara_face[_loc1_] = _loc25_;
            _loc1_++;
         }
         m_chara_face_npc = new Array(PlayerNumFri);
         _loc1_ = 0;
         while(PlayerNumFri > _loc1_)
         {
            _loc28_ = m_callback.GetUserDataString(ReceiveType_ImageStrNpcStart + _loc1_);
            _loc29_ = new Bitmap(null);
            IggyFunctions.setTextureForBitmap(_loc29_,_loc28_);
            _loc29_.scaleX = 256 / _loc29_.width;
            _loc29_.scaleY = 128 / _loc29_.height;
            m_chara_face_npc[_loc1_] = _loc29_;
            _loc1_++;
         }
         var _loc7_:MovieClip = m_timeline.cha_select_cur;
         _loc7_.icn_lock.visible = false;
         ResetIcons();
         var _loc8_:int = m_callback.GetUserDataInt(ReceiveType_JoyConSingleFlag);
         var _loc9_:String = m_callback.GetUserDataString(ReceiveType_KeyStrL2);
         var _loc10_:String = m_callback.GetUserDataString(ReceiveType_KeyStrR2);
         var _loc11_:String = m_callback.GetUserDataString(ReceiveType_KeyStrRU);
         var _loc12_:String = m_callback.GetUserDataString(ReceiveType_KeyStrRD);
         var _loc13_:String = m_callback.GetUserDataString(ReceiveType_KeyStrRL);
         var _loc14_:String = m_callback.GetUserDataString(ReceiveType_KeyStrRR);
         if(_loc8_ == 1)
         {
            _loc9_ = m_callback.GetUserDataString(ReceiveType_KeyStrSingleLS);
            _loc10_ = m_callback.GetUserDataString(ReceiveType_KeyStrSingleRS);
            _loc11_ = m_callback.GetUserDataString(ReceiveType_KeyStrSingleU);
            _loc12_ = m_callback.GetUserDataString(ReceiveType_KeyStrSingleD);
            _loc13_ = m_callback.GetUserDataString(ReceiveType_KeyStrSingleL);
            _loc14_ = m_callback.GetUserDataString(ReceiveType_KeyStrSingleR);
         }
         m_timeline.cha_skill.inact_skill.skill01.sys_com01.htmlText = _loc10_;
         m_timeline.cha_skill.inact_skill.skill01.sys_com02.htmlText = _loc11_;
         m_timeline.cha_skill.inact_skill.skill01.sys_com03.htmlText = "";
         m_timeline.cha_skill.inact_skill.skill01.sys_01.visible = false;
         m_timeline.cha_skill.inact_skill.skill02.sys_com01.htmlText = _loc10_;
         m_timeline.cha_skill.inact_skill.skill02.sys_com02.htmlText = _loc13_;
         m_timeline.cha_skill.inact_skill.skill02.sys_com03.htmlText = "";
         m_timeline.cha_skill.inact_skill.skill02.sys_01.visible = false;
         m_timeline.cha_skill.inact_skill.skill03.sys_com01.htmlText = _loc10_;
         m_timeline.cha_skill.inact_skill.skill03.sys_com02.htmlText = _loc14_;
         m_timeline.cha_skill.inact_skill.skill03.sys_com03.htmlText = "";
         m_timeline.cha_skill.inact_skill.skill03.sys_01.visible = false;
         m_timeline.cha_skill.inact_skill.skill04.sys_com01.htmlText = _loc10_;
         m_timeline.cha_skill.inact_skill.skill04.sys_com02.htmlText = _loc12_;
         m_timeline.cha_skill.inact_skill.skill04.sys_com03.htmlText = "";
         m_timeline.cha_skill.inact_skill.skill04.sys_01.visible = false;
         m_timeline.cha_skill.inact_skill.skill05.sys_com01.htmlText = _loc9_;
         m_timeline.cha_skill.inact_skill.skill05.sys_com02.htmlText = _loc10_;
         m_timeline.cha_skill.inact_skill.skill05.sys_com03.htmlText = _loc11_;
         m_timeline.cha_skill.inact_skill.skill06.sys_com01.htmlText = _loc9_;
         m_timeline.cha_skill.inact_skill.skill06.sys_com02.htmlText = _loc10_;
         m_timeline.cha_skill.inact_skill.skill06.sys_com03.htmlText = _loc13_;
         m_timeline.cha_skill.inact_skill.skill07.sys_com01.htmlText = _loc9_;
         m_timeline.cha_skill.inact_skill.skill07.sys_com02.htmlText = _loc10_;
         m_timeline.cha_skill.inact_skill.skill07.sys_com03.htmlText = _loc12_;
         m_timeline.cha_skill.inact_skill.skill08.sys_com01.htmlText = _loc9_;
         m_timeline.cha_skill.inact_skill.skill08.sys_com02.htmlText = _loc10_;
         m_timeline.cha_skill.inact_skill.skill08.sys_com03.htmlText = _loc14_;
         var _loc15_:String = m_callback.GetUserDataString(ReceiveType_TarismanHeaderStr);
         m_timeline.cha_skill.inact_skill.skill09.sys_com01.htmlText = _loc15_;
         m_timeline.cha_parameter.nest_clothes.sys_clothes.htmlText = "";
         var _loc16_:String = m_callback.GetUserDataString(ReceiveType_KeyStrL1);
         var _loc17_:String = m_callback.GetUserDataString(ReceiveType_KeyStrR1);
         if(_loc8_)
         {
            _loc16_ = _loc9_;
            _loc17_ = _loc10_;
         }
         m_timeline.cha_parameter.nest_clothes.sys_l.htmlText = _loc16_;
         m_timeline.cha_parameter.nest_clothes.sys_r.htmlText = _loc17_;
         _loc1_ = 0;
         while(PlayerMax > _loc1_)
         {
            if(_loc1_ == m_current_player_index)
            {
               setReadyIcon(_loc1_,true,false);
            }
            else
            {
               _loc31_ = m_callback.GetUserDataInt(ReceiveType_PartyNpcNum);
               if(0 <= _loc1_ - 1 && _loc1_ - 1 < _loc31_)
               {
                  setReadyIcon(_loc1_,false,true);
               }
               else
               {
                  setReadyIcon(_loc1_,false,false);
               }
            }
            _loc30_ = getReadyIconMc(_loc1_);
            if(PlayerIndexOwn == _loc1_)
            {
               _loc30_.cmn_icn_you.visible = true;
            }
            else
            {
               _loc30_.cmn_icn_you.visible = false;
            }
            _loc1_++;
         }
         _loc1_ = 0;
         while(IndexNumCharaNewIcon > _loc1_)
         {
            _loc32_ = getMcChamysetNewIcon(_loc1_);
            _loc32_.visible = false;
            _loc1_++;
         }
         updateVariation();
         var _loc18_:int = m_callback.GetUserDataInt(ReceiveType_Time);
         if(_loc18_ <= 0)
         {
            m_timeline.timer.visible = false;
         }
         else
         {
            m_timeline.timer.visible = true;
            m_timer.Start(_loc18_,cbFuncEndTimer);
         }
         if(m_chara_num_column <= IndexNumColumn)
         {
            m_timeline.cha_arrow.visible = false;
         }
         trace(" ***** BGM OPTION ENABLE");
         m_callback.CallbackUserData("user",SendType_EnableOptionBGM,1);
         m_timeline.cha_select.gotoAndPlay("start");
         m_timeline.cha_parameter.gotoAndPlay("start");
         m_timeline.cha_frame.cmn_CMN_M_frame.gotoAndPlay("start");
         m_timeline.cha_arrow.gotoAndPlay("start");
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,waitMain);
      }
      
      private function waitMain(param1:Event) : void
      {
         if(m_timeline.cha_parameter.currentFrame != Utility.GetLabelEndFrame(m_timeline.cha_parameter,"start"))
         {
            return;
         }
         setSelectChara();
         updateVariation();
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitMain);
         m_timeline.stage.addEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,main);
      }
      
      private function main(param1:Event) : void
      {
         var _loc16_:int = 0;
         var _loc17_:* = undefined;
         var _loc18_:* = undefined;
         var _loc19_:Array = null;
         var _loc20_:int = 0;
         var _loc21_:int = 0;
         var _loc22_:int = 0;
         var _loc2_:int = 0;
         var _loc3_:int = m_select_info[m_current_player_index][SelectInfoTypeListIndex];
         var _loc4_:Array = getCharaInfo(_loc3_);
         var _loc5_:int = m_callback.GetUserDataInt(ReceiveType_Time);
         if(_loc5_ > 0)
         {
            if(m_timer)
            {
               _loc16_ = m_timer.GetTime();
               if(m_pre_time != _loc16_)
               {
                  if(_loc16_ < 10)
                  {
                     _loc17_ = 8;
                     m_callback.CallbackUserData("user",SendType_RequestCountDownSe,_loc17_);
                  }
                  m_pre_time = _loc16_;
               }
               if(_loc16_ <= _loc5_)
               {
                  m_timeline.timer.nest._CMN_M_B_mc_timer.visible = true;
               }
               else
               {
                  m_timeline.timer.nest._CMN_M_B_mc_timer.visible = false;
               }
            }
         }
         updateLocalBattle();
         updateCharaIcon();
         var _loc6_:* = m_callback.GetUserDataString(ReceiveType_CharaNameStr);
         var _loc7_:String = "";
         if(_loc4_[m_select_var][VarTypeFlag_GK2] == true)
         {
            _loc7_ = m_callback.GetUserDataString(ReceiveType_NameOption_GK2);
         }
         _loc6_ = _loc6_ + _loc7_;
         var _loc8_:int = m_callback.GetUserDataInt(ReceiveType_NameOption_LvNum);
         if(_loc8_ != -1)
         {
            _loc18_ = m_callback.GetUserDataString(ReceiveType_NameOption_LvText);
            _loc6_ = _loc6_ + " (";
            _loc6_ = _loc6_ + _loc18_;
            _loc6_ = _loc6_ + _loc8_.toString();
            _loc6_ = _loc6_ + ")";
         }
         m_timeline.cha_parameter.sys_charaName.sys_charaName.htmlText = _loc6_;
         var _loc9_:String = m_callback.GetUserDataString(ReceiveType_VariationNameStr);
         m_timeline.cha_parameter.nest_clothes.sys_charavari_name.sys_charavari_name.htmlText = _loc9_;
         updateSkill();
         updateVariation();
         var _loc10_:MovieClip = m_timeline.cha_select;
         var _loc11_:MovieClip = _loc10_["chara_icn_set0" + (m_select_icon_column + 1)];
         var _loc12_:MovieClip = _loc11_["nest_charaselect0" + (m_select_icon_row + 1)];
         if(!m_flag_change_player)
         {
            return;
         }
         m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         var _loc13_:int = 0;
         if(m_flag_decide)
         {
            _loc13_ = m_current_select_index - 1;
            if(0 <= _loc13_ && _loc13_ < m_selected_index_list.length)
            {
               m_selected_index_list[_loc13_] = _loc3_;
            }
         }
         else
         {
            _loc13_ = m_current_select_index;
            if(0 <= _loc13_ && _loc13_ < m_selected_index_list.length)
            {
               m_selected_index_list[_loc13_] = -1;
            }
         }
         if(m_flag_decide)
         {
            if(m_timeline.cha_parameter.currentLabel == "start" || m_timeline.cha_parameter.currentLabel == "wait")
            {
               m_timeline.cha_parameter.gotoAndPlay("push");
               if(_loc4_)
               {
                  _loc19_ = _loc4_[m_select_var][VarTypeVoiceIdList];
                  _loc20_ = Math.floor(Math.random() * _loc19_.length);
                  _loc21_ = _loc19_[_loc20_];
                  m_callback.CallbackUserData("user",SendType_RequestPlayVoice,_loc21_);
               }
               trace(" ----- BGM OPTION DISABLE");
               m_callback.CallbackUserData("user",SendType_EnableOptionBGM,0);
            }
            if(m_timeline.cha_parameter.currentFrame < Utility.GetLabelEndFrame(m_timeline.cha_parameter,"push"))
            {
               return;
            }
         }
         if(m_timeline.cha_parameter.currentLabel == "end_comp")
         {
            m_timeline.cha_parameter.nest_clothes.visible = false;
            if(m_flag_exit)
            {
               end();
               return;
            }
            changeCurrentPlayer(m_flag_decide);
            updatePage();
            setSelectChara();
            sendCharaInfo(m_current_player_index);
            m_flag_decide = false;
            m_flag_change_player = false;
            var _loc14_:Boolean = false;
            if(m_current_player_index < 0 || PlayerMax <= m_current_player_index)
            {
               _loc14_ = true;
            }
            if(_loc14_)
            {
               end();
               return;
            }
            var _loc15_:int = m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle);
            if(!_loc15_)
            {
               if(_loc5_ > 0)
               {
                  m_timer.Start(_loc5_ + 1,cbFuncEndTimer);
                  m_timeline.timer.nest._CMN_M_B_mc_timer.visible = false;
               }
            }
            m_timeline.cha_parameter.gotoAndPlay("start");
            m_timeline.cha_parameter.nest_clothes.visible = true;
            m_timeline.stage.addEventListener(KeyboardEvent.KEY_DOWN,checkKey);
            trace(" ***** BGM OPTION ENABLE");
            m_callback.CallbackUserData("user",SendType_EnableOptionBGM,1);
            return;
         }
         if(m_timeline.cha_parameter.currentLabel == "end")
         {
            return;
         }
         trace(" ----- BGM OPTION DISABLE");
         m_callback.CallbackUserData("user",SendType_EnableOptionBGM,0);
         m_timeline.cha_parameter.gotoAndPlay("end");
         if(!m_flag_exit)
         {
            _loc22_ = -1;
            if(m_timer)
            {
               _loc22_ = m_timer.GetTime();
            }
            m_callback.CallbackUserData("user",SendType_RequestDecide,_loc22_);
         }
      }
      
      private function end() : void
      {
         m_timeline.cha_select.gotoAndPlay("end");
         m_timeline.cha_arrow.gotoAndPlay("end");
         if(m_timeline.cha_skill.currentFrame <= Utility.GetLabelEndFrame(m_timeline.cha_skill,"wait"))
         {
            m_timeline.cha_skill.gotoAndPlay("end");
         }
         m_timeline.cha_frame.cmn_CMN_M_frame.gotoAndPlay("end");
         m_timeline.cha_select_cur.visible = false;
         m_timer.End();
         m_timeline.timer.gotoAndPlay("end");
         var _loc1_:int = m_callback.GetUserDataInt(ReceiveType_Time);
         if(_loc1_ > 0)
         {
            m_timer.Stop();
         }
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,main);
         m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         if(m_current_player_index < 0)
         {
            m_callback.CallbackCancel();
         }
         else
         {
            m_callback.CallbackDecide(1);
         }
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,waitEnd);
      }
      
      private function waitEnd(param1:Event) : void
      {
         if(m_timeline.cha_select.currentFrame != Utility.GetLabelEndFrame(m_timeline.cha_select,"end"))
         {
            return;
         }
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitEnd);
         m_callback.CallbackExit();
      }
      
      private function cbFuncEndTimer() : void
      {
         pushKeyDecide();
      }
      
      private function updateLocalBattle() : void
      {
         var _loc1_:int = m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle);
         if(!_loc1_)
         {
            return;
         }
         var _loc2_:int = m_callback.GetUserDataInt(ReceiveType_Time);
         var _loc3_:int = m_callback.GetUserDataInt(ReceiveType_Flag2pController);
         if(_loc3_)
         {
            if(m_timeline.press2P.currentLabel != "end" && m_timeline.press2P.currentLabel != "end_comp")
            {
               m_timeline.press2P.gotoAndPlay("end");
               if(_loc2_ >= 0)
               {
                  if(m_current_player_index != 0)
                  {
                     if(m_timer)
                     {
                        m_timer.Start(_loc2_ + 1,cbFuncEndTimer);
                        m_timeline.timer.nest._CMN_M_B_mc_timer.visible = false;
                     }
                  }
               }
            }
         }
         else if(m_timeline.press2P.currentLabel != "start" && m_timeline.press2P.currentLabel != "wait")
         {
            m_timeline.press2P.gotoAndPlay("start");
            m_timeline.press2P.nest.sys.sys.htmlText = m_callback.GetUserDataString(ReceiveType_Str2pController);
         }
         switch(m_current_player_index)
         {
            case 0:
               if(!m_timeline.cha_parameter.icon_1P.visible)
               {
                  m_timeline.cha_parameter.icon_1P.gotoAndPlay("start");
               }
               m_timeline.cha_parameter.icon_1P.visible = true;
               m_timeline.cha_parameter.icon_2P.visible = false;
               break;
            case 3:
               m_timeline.cha_parameter.icon_1P.visible = false;
               if(!m_timeline.cha_parameter.icon_2P.visible)
               {
                  m_timeline.cha_parameter.icon_2P.gotoAndPlay("start");
                  if(_loc2_ >= 0)
                  {
                     if(m_timer)
                     {
                        if(_loc3_)
                        {
                           m_timer.Start(_loc2_ + 1,cbFuncEndTimer);
                           m_timeline.timer.nest._CMN_M_B_mc_timer.visible = false;
                        }
                        else
                        {
                           m_timer.Stop();
                        }
                     }
                  }
               }
               m_timeline.cha_parameter.icon_2P.visible = true;
               break;
            default:
               m_timeline.cha_parameter.icon_1P.visible = false;
               m_timeline.cha_parameter.icon_2P.visible = false;
         }
      }
      
      private function getCharaInfo(param1:int) : Array
      {
         if(param1 < 0)
         {
            return null;
         }
         if(m_chara_list_num <= param1)
         {
            return null;
         }
         if(m_chara_list[param1][0] < 0)
         {
            return null;
         }
         return m_chara_list[param1];
      }
      
      public function GetSelectVarInfo(param1:int) : Array
      {
         if(param1 < 0 || PlayerMax <= param1)
         {
            return null;
         }
         var _loc2_:int = m_select_info[param1][SelectInfoTypeListIndex];
         var _loc3_:Array = getCharaInfo(_loc2_);
         if(!_loc3_)
         {
            return null;
         }
         var _loc4_:int = m_select_info[param1][SelectInfoTypeVarIndex];
         _loc4_ = _loc4_ + _loc3_.length;
         _loc4_ = _loc4_ % _loc3_.length;
         return _loc3_[_loc4_];
      }
      
      private function getReadyIconMc(param1:int) : MovieClip
      {
         var _loc2_:int = m_callback.GetUserDataInt(ReceiveType_PlayerFriNum);
         var _loc3_:int = 0;
         if(_loc2_ < param1)
         {
            _loc3_ = param1;
         }
         else
         {
            _loc3_ = _loc2_ - param1;
         }
         var _loc4_:MovieClip = m_timeline.cha_parameter.nest_ready["btnact_rb0" + (_loc3_ + 1)];
         return _loc4_;
      }
      
      private function calcCharaListIndex(param1:int, param2:int) : int
      {
         return param1 + param2 * IndexNumRow;
      }
      
      private function calcIconIndexRow(param1:int) : int
      {
         if(param1 < 0 || m_chara_list_num <= param1)
         {
            return -1;
         }
         return param1 % IndexNumRow;
      }
      
      private function calcIconIndexColumn(param1:int) : int
      {
         if(param1 < 0 || m_chara_list_num <= param1)
         {
            return -1;
         }
         return param1 / IndexNumRow;
      }
      
      private function getVarIndexNum(param1:int) : int
      {
         var _loc2_:int = 0;
         var _loc3_:int = 0;
         _loc2_ = 0;
         while(CharaVarIndexNum > _loc2_)
         {
            if(checkUnlockVar(param1,_loc2_))
            {
               _loc3_++;
            }
            _loc2_++;
         }
         return _loc3_;
      }
      
      private function updatePage() : void
      {
         var _loc3_:int = 0;
         var _loc4_:int = 0;
         var _loc5_:int = 0;
         var _loc9_:MovieClip = null;
         var _loc10_:MovieClip = null;
         var _loc1_:Boolean = false;
         var _loc2_:Boolean = false;
         if(m_select_column < m_select_column_start)
         {
            m_select_column_start = m_select_column;
            _loc1_ = true;
            _loc2_ = true;
         }
         else if(m_select_column_start + IndexNumColumn <= m_select_column)
         {
            m_select_column_start = m_select_column - IndexNumColumn + 1;
            _loc1_ = true;
         }
         if(!_loc1_)
         {
            _loc9_ = getMcChamysetNewIcon(_loc3_);
            if(_loc9_)
            {
               _loc9_.visible = false;
            }
            return;
         }
         var _loc6_:MovieClip = m_timeline.cha_select;
         var _loc7_:MovieClip = m_timeline.cha_new;
         var _loc8_:MovieClip = null;
         _loc4_ = -1;
         while(IndexNumColumn + 1 > _loc4_)
         {
            _loc10_ = _loc6_["chara_icn_set0" + (_loc4_ + 1)];
            if(_loc2_)
            {
               _loc10_.gotoAndPlay("push_l");
               m_timeline.cha_arrow.spinbtn_l.cmn_CMN_M_B_mc_spinbtn_l.gotoAndPlay("push");
               _loc5_ = 0;
               while(IndexNumCharaNewIcon > _loc5_)
               {
                  _loc8_ = _loc7_["chara_new_" + _loc5_];
                  _loc8_.gotoAndPlay("push_l");
                  _loc5_++;
               }
            }
            else
            {
               _loc10_.gotoAndPlay("push_r");
               m_timeline.cha_arrow.spinbtn_r.cmn_CMN_M_B_mc_spinbtn_r.gotoAndPlay("push");
               _loc5_ = 0;
               while(IndexNumCharaNewIcon > _loc5_)
               {
                  _loc8_ = _loc7_["chara_new_" + _loc5_];
                  _loc8_.gotoAndPlay("push_r");
                  _loc5_++;
               }
            }
            _loc4_++;
         }
      }
      
      private function changeLR(param1:Boolean) : void
      {
         var _loc2_:int = 0;
         var _loc3_:int = 0;
         var _loc4_:Boolean = false;
         while(true)
         {
            if(param1)
            {
               if(m_select_column == 0 && m_select_row == 0)
               {
                  break;
               }
               m_select_column--;
               if(m_select_column < 0)
               {
                  m_select_column = m_chara_num_column - 1;
                  m_select_row--;
                  m_select_row = m_select_row + IndexNumRow;
                  m_select_row = m_select_row % IndexNumRow;
               }
            }
            else
            {
               m_select_column++;
               if(m_chara_num_column <= m_select_column)
               {
                  m_select_column = 0;
                  m_select_row++;
                  m_select_row = m_select_row + IndexNumRow;
                  m_select_row = m_select_row % IndexNumRow;
               }
            }
            _loc2_ = calcCharaListIndex(m_select_row,m_select_column);
            _loc3_ = m_callback.GetUserDataInt(ReceiveType_FlagSelectAvatar);
            if(!_loc3_)
            {
               if(checkAvatar(_loc2_))
               {
                  if(checkSelectedAvatar(_loc2_))
                  {
                     continue;
                  }
               }
            }
            _loc4_ = checkUnlockChara(_loc2_);
            if(!_loc4_)
            {
               continue;
            }
            m_select_var = 0;
            return;
         }
         changeUD(true);
      }
      
      private function changeUD(param1:Boolean) : void
      {
         var _loc2_:int = 0;
         var _loc3_:int = 0;
         var _loc4_:Boolean = false;
         while(true)
         {
            if(param1)
            {
               m_select_row--;
               if(m_select_row < 0)
               {
                  m_select_row = IndexNumRow - 1;
                  m_select_column--;
                  m_select_column = m_select_column + m_chara_num_column;
                  m_select_column = m_select_column % m_chara_num_column;
               }
            }
            else
            {
               m_select_row++;
               if(IndexNumRow <= m_select_row)
               {
                  m_select_row = 0;
                  m_select_column++;
                  m_select_column = m_select_column + m_chara_num_column;
                  m_select_column = m_select_column % m_chara_num_column;
               }
            }
            _loc2_ = calcCharaListIndex(m_select_row,m_select_column);
            _loc3_ = m_callback.GetUserDataInt(ReceiveType_FlagSelectAvatar);
            if(!_loc3_)
            {
               if(checkAvatar(_loc2_))
               {
                  if(checkSelectedAvatar(_loc2_))
                  {
                     continue;
                  }
               }
            }
            _loc4_ = checkUnlockChara(_loc2_);
            if(!_loc4_)
            {
               continue;
            }
            break;
         }
         m_select_var = 0;
      }
      
      private function changeVar(param1:Boolean) : void
      {
         var _loc4_:Boolean = false;
         var _loc2_:int = calcCharaListIndex(m_select_row,m_select_column);
         var _loc3_:Array = getCharaInfo(_loc2_);
         if(!_loc3_)
         {
            return;
         }
         while(true)
         {
            if(param1)
            {
               m_select_var--;
            }
            else
            {
               m_select_var++;
            }
            m_select_var = m_select_var + _loc3_.length;
            m_select_var = m_select_var % _loc3_.length;
            _loc4_ = checkUnlockVar(_loc2_,m_select_var);
            if(!_loc4_)
            {
               continue;
            }
            break;
         }
      }
      
      private function checkPlayerTeamType(param1:int) : int
      {
         var _loc2_:int = PlayerTeamTypeInvalid;
         if(param1 == PlayerIndexOwn)
         {
            _loc2_ = PlayerTeamTypeOwn;
         }
         else if(PlayerIndexFriStart <= param1 && param1 <= PlayerIndexFriEnd)
         {
            _loc2_ = PlayerTeamTypeFri;
         }
         else if(PlayerIndexEnmStart <= param1 && param1 <= PlayerIndexEnmEnd)
         {
            _loc2_ = PlayerTeamTypeEnm;
         }
         return _loc2_;
      }
      
      private function setReadyIcon(param1:int, param2:Boolean, param3:Boolean) : void
      {
         var _loc5_:int = 0;
         var _loc6_:Boolean = false;
         var _loc7_:int = 0;
         var _loc8_:int = 0;
         var _loc9_:int = 0;
         var _loc10_:int = 0;
         if(param1 < 0 || PlayerMax <= param1)
         {
            return;
         }
         var _loc4_:MovieClip = getReadyIconMc(param1);
         if(param3)
         {
            _loc5_ = checkPlayerTeamType(param1);
            _loc6_ = false;
            _loc7_ = param1 - 1;
            switch(_loc5_)
            {
               case PlayerTeamTypeOwn:
                  _loc4_.gotoAndStop("blue_team");
                  break;
               case PlayerTeamTypeFri:
                  _loc4_.gotoAndStop("blue_team");
                  _loc8_ = m_callback.GetUserDataInt(ReceiveType_PlayerFriNum);
                  _loc9_ = m_callback.GetUserDataInt(ReceiveType_PartyNpcNum);
                  if(0 <= _loc7_ && _loc7_ < _loc9_)
                  {
                     _loc6_ = true;
                  }
                  break;
               case PlayerTeamTypeEnm:
                  _loc4_.gotoAndStop("red_team");
            }
            _loc4_.sys_ready.text = "OK";
            if(_loc6_)
            {
               setImageFriendNpc(_loc4_.icn_chara_lit.chara_img,_loc7_);
            }
            else
            {
               _loc10_ = calcCharaListIndex(m_select_row,m_select_column);
               setImage(_loc4_.icn_chara_lit.chara_img,_loc10_,true);
            }
         }
         else
         {
            _loc4_.gotoAndStop("off");
            _loc4_.sys_ready.text = "---";
         }
         if(param2)
         {
            _loc4_.btnact_ready.gotoAndPlay("on");
         }
         else
         {
            _loc4_.btnact_ready.gotoAndPlay("off");
         }
      }
      
      private function changeCurrentPlayer(param1:Boolean) : void
      {
         var _loc5_:Boolean = false;
         var _loc6_:int = 0;
         var _loc7_:int = 0;
         var _loc8_:int = 0;
         var _loc9_:int = 0;
         var _loc10_:int = 0;
         var _loc2_:int = 0;
         setReadyIcon(m_current_player_index,false,param1);
         var _loc3_:int = m_callback.GetUserDataInt(ReceiveType_PlayerFriNum);
         var _loc4_:int = m_callback.GetUserDataInt(ReceiveType_PlayerEnmNum);
         do
         {
            if(param1)
            {
               m_current_player_index++;
            }
            else
            {
               m_current_player_index--;
            }
            _loc5_ = false;
            _loc6_ = checkPlayerTeamType(m_current_player_index);
            switch(_loc6_)
            {
               case PlayerTeamTypeFri:
                  _loc7_ = m_current_player_index - PlayerIndexFriStart;
                  _loc8_ = m_callback.GetUserDataInt(ReceiveType_PartyNpcNum);
                  if(_loc8_ > _loc7_)
                  {
                     _loc5_ = false;
                  }
                  else if(_loc3_ > _loc7_)
                  {
                     _loc5_ = true;
                  }
                  break;
               case PlayerTeamTypeEnm:
                  _loc9_ = m_current_player_index - PlayerIndexEnmStart;
                  if(_loc4_ > _loc9_)
                  {
                     _loc5_ = true;
                  }
                  break;
               case PlayerTeamTypeOwn:
               default:
                  _loc5_ = true;
            }
         }
         while(!_loc5_);
         
         setReadyIcon(m_current_player_index,true,false);
         if(0 <= m_current_player_index && m_current_player_index < PlayerMax)
         {
            _loc10_ = m_select_info[m_current_player_index][SelectInfoTypeListIndex];
            m_select_row = calcIconIndexRow(_loc10_);
            m_select_column = calcIconIndexColumn(_loc10_);
            m_select_var = m_select_info[m_current_player_index][SelectInfoTypeVarIndex];
            if(checkAvatar(_loc10_) && checkSelectedAvatar(_loc10_))
            {
               changeUD(false);
            }
         }
      }
      
      private function setSelectChara() : void
      {
         var _loc1_:int = 0;
         var _loc2_:int = 0;
         var _loc13_:int = 0;
         var _loc14_:MovieClip = null;
         var _loc15_:MovieClip = null;
         var _loc16_:MovieClip = null;
         var _loc17_:Boolean = false;
         var _loc18_:Boolean = false;
         var _loc19_:int = 0;
         var _loc20_:int = 0;
         if(m_current_player_index < 0 || PlayerMax <= m_current_player_index)
         {
            return;
         }
         var _loc3_:int = calcCharaListIndex(m_select_row,m_select_column);
         var _loc4_:Array = getCharaInfo(_loc3_);
         var _loc5_:int = -1;
         if(_loc4_)
         {
            _loc5_ = _loc4_.length;
            _loc1_ = 0;
            while(_loc5_ > _loc1_)
            {
               _loc13_ = m_select_var + _loc1_;
               _loc13_ = _loc13_ % _loc5_;
               if(!checkUnlockVar(_loc3_,_loc13_))
               {
                  _loc1_++;
                  continue;
               }
               m_select_var = _loc13_;
               break;
            }
         }
         var _loc6_:int = m_select_row;
         var _loc7_:int = m_select_column - m_select_column_start;
         var _loc8_:MovieClip = m_timeline.cha_select;
         var _loc9_:MovieClip = _loc8_["chara_icn_set0" + (_loc7_ + 1)];
         var _loc10_:MovieClip = _loc9_["nest_charaselect0" + (_loc6_ + 1)];
         m_select_icon_row = _loc6_;
         m_select_icon_column = _loc7_;
         _loc1_ = 0;
         while(IndexNumCharaNewIcon > _loc1_)
         {
            _loc14_ = getMcChamysetNewIcon(_loc1_);
            if(_loc14_)
            {
               _loc14_.visible = false;
            }
            _loc1_++;
         }
         _loc1_ = 0;
         while(CharaVarIndexNum > _loc1_)
         {
            _loc15_ = getMcChamyset(_loc1_);
            _loc16_ = getMcChamysetNewIcon(_loc1_);
            if(_loc15_)
            {
               _loc17_ = checkUnlockVar(_loc3_,_loc1_);
               _loc15_.btnact_off.visible = true;
               if(_loc1_ < _loc5_)
               {
                  if(_loc17_)
                  {
                     if(_loc1_ == m_select_var)
                     {
                        if(_loc15_.currentLabel != "on")
                        {
                           _loc15_.gotoAndPlay("on");
                           _loc15_.btnact_ef.visible = true;
                           _loc15_.btnact_on.visible = true;
                           _loc15_.btnact_off.visible = false;
                        }
                     }
                     else
                     {
                        _loc15_.gotoAndPlay("off");
                        _loc15_.btnact_ef.visible = false;
                        _loc15_.btnact_on.visible = false;
                        _loc15_.btnact_off.visible = true;
                     }
                     if(!_loc16_)
                     {
                     }
                     _loc18_ = false;
                     if(_loc4_[_loc1_][VarTypeCustomCostume])
                     {
                        _loc18_ = true;
                     }
                     _loc19_ = 1;
                     _loc20_ = 2;
                     if(_loc18_)
                     {
                        _loc15_.btnact_ef.gotoAndStop(_loc20_);
                        _loc15_.btnact_on.gotoAndStop(_loc20_);
                        _loc15_.btnact_off.gotoAndStop(_loc20_);
                     }
                     else
                     {
                        _loc15_.btnact_ef.gotoAndStop(_loc19_);
                        _loc15_.btnact_on.gotoAndStop(_loc19_);
                        _loc15_.btnact_off.gotoAndStop(_loc19_);
                     }
                  }
                  else
                  {
                     _loc15_.gotoAndPlay("lock");
                     _loc15_.btnact_ef.visible = false;
                     _loc15_.btnact_on.visible = false;
                     _loc15_.btnact_off.visible = true;
                     if(_loc16_)
                     {
                        _loc16_.visible = false;
                     }
                  }
               }
               else
               {
                  _loc15_.gotoAndPlay("off");
                  _loc15_.btnact_ef.visible = false;
                  _loc15_.btnact_on.visible = false;
                  _loc15_.btnact_off.visible = false;
                  if(_loc16_)
                  {
                     _loc16_.visible = false;
                  }
               }
            }
            else if(_loc16_)
            {
               _loc16_.visible = false;
            }
            _loc1_++;
         }
         var _loc11_:int = getVarIndexNum(_loc3_);
         var _loc12_:* = false;
         if(_loc11_ > 1)
         {
            _loc12_ = true;
         }
         m_timeline.cha_parameter.nest_clothes.sys_r.visible = _loc12_;
         m_timeline.cha_parameter.nest_clothes.sys_l.visible = _loc12_;
         m_select_info[m_current_player_index][SelectInfoTypeListIndex] = _loc3_;
         m_select_info[m_current_player_index][SelectInfoTypeVarIndex] = m_select_var;
      }
      
      public function sendCharaInfo(param1:int) : void
      {
         if(param1 < 0)
         {
            return;
         }
         if(PlayerMax <= param1)
         {
            return;
         }
         var _loc2_:Array = GetSelectVarInfo(param1);
         if(!_loc2_)
         {
            SetUserDataString(CharaSele.ReceiveType_CharaNameStr,"???");
            return;
         }
         var _loc3_:String = _loc2_[VarTypeCode];
         var _loc4_:int = m_select_info[param1][SelectInfoTypeVarIndex];
         var _loc5_:int = _loc2_[VarTypeMid];
         var _loc6_:int = _loc2_[VarTypeModelPreset];
         var _loc7_:int = calcCharaListIndex(m_select_row,m_select_column);
         var _loc8_:int = _loc2_[VarTypeCustomCostume];
         var _loc9_:int = _loc2_[VarTypeAvatarSlotID];
         m_callback.CallbackUserData("user",SendType_CurrentPlayerIndex,param1);
         m_callback.CallbackUserData("user",SendType_AvatarSlotID,_loc9_);
         m_callback.CallbackUserDataString("user",SendType_SelectCode,_loc3_);
         m_callback.CallbackUserData("user",SendType_SelectVariation,_loc4_);
         m_callback.CallbackUserData("user",SendType_SelectCustomFlag,_loc8_);
         m_callback.CallbackUserData("user",SendType_SelectMid,_loc5_);
         m_callback.CallbackUserData("user",SendType_SelectModelPreset,_loc6_);
         m_callback.CallbackUserData("user",SendType_RequestCharaInfo,0);
         m_callback.CallbackUserData("user",SendType_SelectListIndex,CharaListShiftCount[_loc7_]);
         m_callback.SetUserDataValidFlag(ReceiveType_CharaNameStr,false);
      }
      
      private function setImage(param1:MovieClip, param2:int, param3:Boolean) : void
      {
         var _loc4_:Bitmap = null;
         var _loc5_:String = null;
         if(!param1)
         {
            return;
         }
         while(param1.numChildren > 0)
         {
            param1.removeChildAt(0);
         }
         if(!checkUnlockChara(param2))
         {
            return;
         }
         if(m_chara_face[param2])
         {
            _loc4_ = m_chara_face[param2];
            if(param3)
            {
               _loc5_ = m_callback.GetUserDataString(ReceiveType_ImageStrStart + param2);
               _loc4_ = new Bitmap(null);
               IggyFunctions.setTextureForBitmap(_loc4_,_loc5_);
               _loc4_.scaleX = 256 / _loc4_.width;
               _loc4_.scaleY = 128 / _loc4_.height;
            }
            param1.addChild(_loc4_);
         }
      }
      
      private function setImageFriendNpc(param1:MovieClip, param2:int) : void
      {
         var _loc3_:Bitmap = null;
         if(!param1)
         {
            return;
         }
         if(param2 < 0 || PlayerNumFri <= param2)
         {
            return;
         }
         while(param1.numChildren > 0)
         {
            param1.removeChildAt(0);
         }
         if(m_chara_face_npc[param2])
         {
            _loc3_ = m_chara_face_npc[param2];
            param1.addChild(_loc3_);
         }
      }
      
      private function updateCharaIcon() : void
      {
         var _loc1_:int = 0;
         var _loc2_:int = 0;
         var _loc7_:MovieClip = null;
         var _loc8_:MovieClip = null;
         var _loc9_:int = 0;
         var _loc10_:MovieClip = null;
         var _loc11_:MovieClip = null;
         var _loc12_:Array = null;
         var _loc13_:int = 0;
         var _loc14_:int = 0;
         var _loc3_:MovieClip = m_timeline.cha_select;
         var _loc4_:MovieClip = m_timeline.cha_new;
         _loc2_ = -1;
         while(IndexNumColumn + 1 > _loc2_)
         {
            _loc7_ = _loc3_["chara_icn_set0" + (_loc2_ + 1)];
            _loc1_ = 0;
            while(IndexNumRow > _loc1_)
            {
               _loc8_ = _loc7_["nest_charaselect0" + (_loc1_ + 1)];
               if(_loc8_)
               {
                  _loc9_ = calcCharaListIndex(_loc1_,_loc2_ + m_select_column_start);
                  _loc10_ = _loc4_["chara_new_" + ((_loc2_ + 1) * 3 + _loc1_)];
                  setImage(_loc8_.chara_img,_loc9_,false);
                  if(checkNoChara(_loc9_))
                  {
                     _loc8_.visible = false;
                     _loc10_.visible = false;
                  }
                  else if(checkUnlockChara(_loc9_))
                  {
                     _loc8_.visible = true;
                     _loc10_.visible = checkCharaNewIconVisible(_loc9_);
                     _loc11_ = m_timeline.cha_select_cur;
                     _loc12_ = getCharaInfo(_loc9_);
                     _loc13_ = _loc12_[VarTypeAvatarSlotID];
                     _loc14_ = m_callback.GetUserDataInt(ReceiveType_FlagSelectAvatar);
                     if(!_loc14_ && m_current_player_index != PlayerIndexOwn && checkAvatar(_loc9_) && checkSelectedAvatar(_loc9_))
                     {
                        _loc8_.gotoAndPlay("grayout");
                        if(checkSelectedAvatar(_loc9_))
                        {
                           if(m_select_column - m_select_column_start == _loc2_ && m_select_row == _loc1_)
                           {
                              setImage(_loc11_.chara_sel.chara_img,_loc9_,false);
                           }
                        }
                     }
                     else if(m_select_icon_column == _loc2_ && m_select_icon_row == _loc1_)
                     {
                        _loc11_.visible = true;
                        _loc11_.x = _loc3_.x + _loc7_.x + _loc8_.x;
                        _loc11_.y = _loc3_.y + _loc7_.y + _loc8_.y;
                        setImage(_loc11_.chara_sel.chara_img,_loc9_,false);
                        if(m_flag_decide)
                        {
                           if(_loc11_.currentLabel != "push")
                           {
                              _loc11_.gotoAndPlay("push");
                           }
                        }
                        else if(_loc11_.currentLabel != "on")
                        {
                           _loc11_.gotoAndPlay("on");
                        }
                     }
                     else
                     {
                        _loc8_.gotoAndPlay("off");
                     }
                  }
                  else
                  {
                     _loc8_.visible = true;
                     _loc8_.gotoAndPlay("lock");
                     _loc10_.visible = false;
                  }
               }
               _loc1_++;
            }
            _loc2_++;
         }
         var _loc5_:MovieClip = _loc3_["chara_icn_set00"];
         if(m_select_column_start <= 0)
         {
            _loc5_.visible = false;
         }
         else
         {
            _loc5_.visible = true;
         }
         var _loc6_:MovieClip = _loc3_["chara_icn_set0" + (IndexNumColumn + 1)];
         if(m_chara_num_column <= m_select_column_start + IndexNumColumn)
         {
            _loc6_.visible = false;
         }
         else
         {
            _loc6_.visible = true;
         }
      }
      
      private function updateSkill() : void
      {
         var _loc3_:String = null;
         var _loc4_:Number = NaN;
         var _loc5_:Number = NaN;
         var _loc6_:String = null;
         var _loc7_:Number = NaN;
         var _loc8_:Number = NaN;
         var _loc1_:int = 0;
         ResetIcons();
         var _loc2_:MovieClip = m_timeline.cha_skill;
         if(m_flag_skill)
         {
            if(_loc2_.currentFrame > Utility.GetLabelEndFrame(_loc2_,"wait"))
            {
               _loc2_.gotoAndPlay("start");
               _loc2_.visible = true;
            }
            _loc1_ = 0;
            while(SkillMax > _loc1_)
            {
               _loc6_ = m_callback.GetUserDataString(ReceiveType_SkillNameStrStart + _loc1_);
               _loc2_.inact_skill["skill0" + (_loc1_ + 1)].sys_skill.scaleX = m_skill_str_scalex_default;
               _loc2_.inact_skill["skill0" + (_loc1_ + 1)].sys_skill.autoSize = "left";
               _loc2_.inact_skill["skill0" + (_loc1_ + 1)].sys_skill.htmlText = _loc6_;
               _loc7_ = _loc2_.inact_skill["skill0" + (_loc1_ + 1)].sys_skill.width;
               _loc8_ = m_skill_str_scalex_default;
               if(m_skill_str_width_default < _loc7_)
               {
                  _loc8_ = m_skill_str_width_default / _loc7_;
               }
               _loc2_.inact_skill["skill0" + (_loc1_ + 1)].sys_skill.scaleX = _loc8_;
               _loc1_++;
            }
            _loc3_ = m_callback.GetUserDataString(ReceiveType_TarismanNameStr);
            _loc2_.inact_skill.skill09.sys_skill.scaleX = m_skill_str_scalex_default;
            _loc2_.inact_skill.skill09.sys_skill.autoSize = "left";
            _loc2_.inact_skill.skill09.sys_skill.htmlText = _loc3_;
            _loc4_ = _loc2_.inact_skill.skill09.sys_skill.width;
            _loc5_ = m_skill_str_scalex_default;
            if(m_skill_str_width_default < _loc4_)
            {
               _loc5_ = m_skill_str_width_default / _loc4_;
            }
            _loc2_.inact_skill.skill09.sys_skill.scaleX = _loc5_ - 0.15;
            _loc2_.inact_skill.skill09.sys_skill.scaleY = 1;
         }
         else if(_loc2_.currentFrame <= Utility.GetLabelEndFrame(_loc2_,"wait"))
         {
            _loc2_.gotoAndPlay("end");
         }
      }
      
      private function updateVariation() : *
      {
         var _loc2_:int = 0;
         var _loc3_:MovieClip = null;
         var _loc4_:Boolean = false;
         var _loc5_:Boolean = false;
         var _loc1_:int = calcCharaListIndex(m_select_row,m_select_column);
         if(checkNoChara(_loc1_))
         {
            _loc1_ = 0;
         }
         _loc2_ = 0;
         while(IndexNumCharaVariationNewIcon > _loc2_)
         {
            _loc3_ = getMcVariationNewIcon(_loc2_);
            _loc4_ = checkUnlockVar(_loc1_,_loc2_);
            if(_loc4_)
            {
               _loc5_ = checkVariationNewIconVisible(_loc1_,_loc2_);
               if(_loc3_.visible != _loc5_)
               {
                  if(m_icon_show_wait[_loc2_] < 2)
                  {
                     m_icon_show_wait[_loc2_] = m_icon_show_wait[_loc2_] + 1;
                  }
                  else
                  {
                     m_icon_show_wait[_loc2_] = 0;
                     _loc3_.visible = _loc5_;
                  }
               }
               else
               {
                  m_icon_show_wait[_loc2_] = 0;
               }
            }
            else
            {
               _loc3_.visible = false;
            }
            _loc2_++;
         }
      }
      
      private function getMcChamyset(param1:int) : MovieClip
      {
         var _loc2_:MovieClip = null;
         if(param1 < 9)
         {
            _loc2_ = m_timeline.cha_parameter.nest_clothes["btnact_chamyset_0" + (param1 + 1)];
         }
         else
         {
            _loc2_ = m_timeline.cha_parameter.nest_clothes["btnact_chamyset_" + (param1 + 1)];
         }
         return _loc2_;
      }
      
      private function getMcChamysetNewIcon(param1:int) : MovieClip
      {
         var _loc2_:MovieClip = null;
         _loc2_ = m_timeline.cha_new["chara_new_" + param1];
         return _loc2_;
      }
      
      private function getMcVariationNewIcon(param1:int) : MovieClip
      {
         var _loc2_:MovieClip = null;
         _loc2_ = m_timeline.cha_parameter.nest_clothes["charavari_new_" + param1];
         return _loc2_;
      }
      
      private function checkUnlockVar(param1:int, param2:int) : Boolean
      {
         if(param1 < 0 || m_chara_list_num <= param1)
         {
            return false;
         }
         if(param2 < 0 || CharaVarIndexNum <= param2)
         {
            return false;
         }		 
         var _loc3_:int = param1 * CharaVarIndexNum + param2;
         return Boolean(m_callback.GetUserDataInt(ReceiveType_UnlockVarStart + _loc3_));
      }
      
      private function checkUnlockChara(param1:*) : Boolean
      {
		 if(param1 < 0 || m_chara_list_num <= param1)
         {
            return false;
         }
		 
         var _loc2_:int = 0;
         _loc2_ = 0;
         while(CharaVarIndexNum > _loc2_)
         {
            if(checkUnlockVar(param1,_loc2_))
            {
               return true;
            }
            _loc2_++;
         }
         return false;
      }
      
      private function checkAvatar(param1:int) : Boolean
      {
         var _loc2_:Array = getCharaInfo(param1);
         if(!_loc2_)
         {
            return false;
         }
         if(_loc2_.length <= 0)
         {
            return false;
         }
         var _loc3_:String = _loc2_[0][VarTypeCode];
         if(_loc3_ != AvatarCode)
         {
            return false;
         }
         return true;
      }
      
      private function checkNoChara(param1:int) : Boolean
      {
         var _loc2_:Array = getCharaInfo(param1);
         if(!_loc2_)
         {
            return true;
         }
         if(_loc2_.length <= 0)
         {
            return true;
         }
         var _loc3_:String = _loc2_[0][VarTypeCode];
         if(_loc3_ == InvalidCode)
         {
            return true;
         }
         return false;
      }
      
      private function checkCharaNewIconVisible(param1:int) : Boolean
      {
         if(checkNoChara(param1))
         {
            return false;
         }
         if(checkAvatar(param1))
         {
            return false;
         }
         if(CharaListShiftCount.length <= param1)
         {
            return false;
         }
         var _loc2_:int = CharaListShiftCount[param1];
         return m_callback.GetUserDataInt(ReceiveType_CharaSelectedStart + _loc2_) == 0;
      }
      
      private function checkVariationNewIconVisible(param1:int, param2:int) : Boolean
      {
         if(checkNoChara(param1))
         {
            return false;
         }
         if(checkAvatar(param1))
         {
            return false;
         }
         var _loc3_:Array = getCharaInfo(param1);
         if(param2 >= _loc3_.length)
         {
            return false;
         }
         return m_callback.GetUserDataInt(ReceiveType_CharaVariationStart + param2) == 0;
      }
      
      private function checkSelectedAvatar(param1:int) : Boolean
      {
         if(param1 == -1)
         {
            return false;
         }
         var _loc2_:int = 0;
         while(_loc2_ < m_selected_index_list.length)
         {
            if(m_selected_index_list[_loc2_] == param1)
            {
               return true;
            }
            _loc2_++;
         }
         return false;
      }
      
      private function pushLeft() : void
      {
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         changeLR(true);
         updatePage();
         setSelectChara();
         sendCharaInfo(m_current_player_index);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushRight() : void
      {
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         changeLR(false);
         updatePage();
         setSelectChara();
         sendCharaInfo(m_current_player_index);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushUp() : void
      {
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         changeUD(true);
         updatePage();
         setSelectChara();
         sendCharaInfo(m_current_player_index);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushDown() : void
      {
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         changeUD(false);
         updatePage();
         setSelectChara();
         sendCharaInfo(m_current_player_index);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushKeyL() : void
      {
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         changeVar(true);
         setSelectChara();
         sendCharaInfo(m_current_player_index);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushKeyR() : void
      {
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         changeVar(false);
         setSelectChara();
         sendCharaInfo(m_current_player_index);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushKeySkill() : void
      {
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         if(m_flag_skill)
         {
            m_flag_skill = false;
            m_callback.CallbackUserData("user",SendType_RequestSetFlagSkill,0);
         }
         else
         {
            m_flag_skill = true;
            m_callback.CallbackUserData("user",SendType_RequestSetFlagSkill,1);
         }
      }
      
      private function pushKeyRandom() : void
      {
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         var _loc1_:int = 0;
         var _loc2_:int = 0;
         _loc1_ = 0;
         while(m_chara_list_num > _loc1_)
         {
            if(checkUnlockChara(_loc1_))
            {
               _loc2_++;
            }
            _loc1_++;
         }
         if(m_current_player_index != PlayerIndexOwn)
         {
            _loc2_--;
         }
         var _loc3_:int = 0;
         if(_loc2_ > 1)
         {
            _loc3_ = Math.floor(Math.random() * _loc2_);
         }
         _loc1_ = 0;
         while(_loc3_ > _loc1_)
         {
            changeUD(false);
            updatePage();
            _loc1_++;
         }
         var _loc4_:int = calcCharaListIndex(m_select_row,m_select_column);
         var _loc5_:int = getVarIndexNum(_loc4_);
         var _loc6_:int = 0;
         if(_loc5_ > 1)
         {
            _loc6_ = Math.floor(Math.random() * _loc5_);
         }
         _loc1_ = 0;
         while(_loc6_ > _loc1_)
         {
            changeVar(false);
            _loc1_++;
         }
         setSelectChara();
         sendCharaInfo(m_current_player_index);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushKeyDecide() : void
      {
         if(m_flag_change_player)
         {
            return;
         }
         if(m_current_player_index != PlayerIndexOwn && m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle) && !m_callback.GetUserDataInt(ReceiveType_Flag2pController))
         {
            return;
         }
         m_current_select_index++;
         m_flag_change_player = true;
         m_flag_decide = true;
         m_callback.CallbackSe(m_callback.SeTypeDecide);
      }
      
      private function pushKeyCancel() : void
      {
         var _loc2_:int = 0;
         if(m_flag_change_player)
         {
            return;
         }
         var _loc1_:int = m_callback.GetUserDataInt(ReceiveType_FlagUseCancel);
         if(!_loc1_)
         {
            _loc2_ = m_callback.GetUserDataInt(ReceiveType_FlagLocalBattle);
            if(_loc2_)
            {
               return;
            }
            if(m_current_player_index <= PlayerIndexOwn)
            {
               return;
            }
         }
         m_current_select_index--;
         m_flag_change_player = true;
         m_flag_decide = false;
         m_callback.CallbackSe(m_callback.SeTypeCancel);
      }
      
      private function pushStart() : void
      {
         var _loc1_:int = m_callback.GetUserDataInt(ReceiveType_PlayerEnmNum);
         if(_loc1_ > 0)
         {
            return;
         }
         if(m_current_player_index == PlayerIndexOwn)
         {
            return;
         }
         m_flag_change_player = true;
         m_flag_decide = false;
         m_flag_exit = true;
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function checkKey(param1:KeyboardEvent) : void
      {
         trace("[CHARASELE] checkKey " + param1.keyCode);
         trace("hello");
         if(m_flag_decide)
         {
            return;
         }
         switch(param1.keyCode)
         {
            case Keyboard.ENTER:
               pushKeyDecide();
               break;
            case Keyboard.ESCAPE:
            case 83:
               pushKeyCancel();
               break;
            case Keyboard.LEFT:
               pushLeft();
               break;
            case Keyboard.RIGHT:
               pushRight();
               break;
            case Keyboard.UP:
               pushUp();
               break;
            case Keyboard.DOWN:
               pushDown();
               break;
            case Keyboard.DELETE:
               pushKeyL();
               break;
            case Keyboard.PAGE_DOWN:
               pushKeyR();
               break;
            case 88:
               pushKeySkill();
               break;
            case 89:
            case Keyboard.END:
               ////////// Fix to zoom bug
			   if (isKeyboardControllerActive || param1.keyCode == 89)
					pushKeyRandom();
			   //////////
               break;
            case Keyboard.SPACE:
               pushStart();
         }
      }
      
      public function SetUserDataInt(param1:int, param2:int) : *
      {
         m_callback.AddCallbackSetUserDataInt(param1,param2);
      }
      
      public function SetUserDataString(param1:int, param2:String) : *
      {
         m_callback.AddCallbackSetUserDataString(param1,param2);
      }
      
      public function TestDestroy() : void
      {
         m_callback = null;
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,requestUnlock);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitUnlock);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitStart);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitMain);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,main);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,waitEnd);
         m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         m_timeline.visible = false;
         m_timeline = null;
         m_timer.Destroy();
         m_timer = null;
      }
      
      public function TestCheckChangeSelect() : Boolean
      {
         return !m_callback.GetUserDataValidFlag(ReceiveType_CharaNameStr);
      }
      
      public function TestGetCharaList() : Array
      {
         return m_chara_list;
      }
      
      public function TestGetCharaVarInfo() : Array
      {
         return GetSelectVarInfo(m_current_player_index);
      }
      
      public function TestGetCurrentPlayerIndex() : int
      {
         return m_current_player_index;
      }
   }
}
