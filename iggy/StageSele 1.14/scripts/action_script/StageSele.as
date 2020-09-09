package action_script
{
   import flash.display.Bitmap;
   import flash.display.MovieClip;
   import flash.events.Event;
   import flash.events.KeyboardEvent;
   import flash.ui.Keyboard;
   import flash.utils.getTimer;
   
   public class StageSele
   {
      
      private static const StageFrameNum:int = 35;
      
      private static const ImageFrameInvalid:int = StageFrameNum + 1;
      
      private static const StartSelectIndex:int = 0;
      
      private static const StageID_LBY:int = 0;
      
      private static const StageID_BFlbb:int = 1;
      
      private static const StageID_BFlby:int = 2;
      
      private static const StageID_BFpln:int = 3;
      
      private static const StageID_BFrck:int = 4;
      
      private static const StageID_BFrcn:int = 5;
      
      private static const StageID_BFnmc:int = 6;
      
      private static const StageID_BFnms:int = 7;
      
      private static const StageID_BFnmb:int = 8;
      
      private static const StageID_BFtwc:int = 9;
      
      private static const StageID_BFtwn:int = 10;
      
      private static const StageID_BFtwh:int = 11;
      
      private static const StageID_BFcel:int = 12;
      
      private static const StageID_BFceb:int = 13;
      
      private static const StageID_BFkoh:int = 14;
      
      private static const StageID_BFsin:int = 15;
      
      private static const StageID_BFsky:int = 16;
      
      private static const StageID_BFsei:int = 17;
      
      private static const StageID_BFkai:int = 18;
      
      private static const StageID_BFspe:int = 19;
      
      private static const StageID_BFspv:int = 20;
      
      private static const StageID_BFice:int = 21;
      
      private static const StageID_BFtfl:int = 22;
      
      private static const StageID_BFgen:int = 23;
      
      private static const StageID_BFten:int = 24;
      
      private static const StageID_BFund:int = 25;
      
      private static const StageID_DMtwh:int = 26;
      
      private static const StageID_BFlnd:int = 27;
      
      private static const StageID_BFlnc:int = 28;
      
      private static const StageID_BFhel:int = 29;
      
      private static const StageID_BFtre:int = 30;
      
      private static const StageID_BFtok:int = 31;
      
      private static const StageID_BFsmt:int = 32;
      
      private static const StageID_BFtwf:int = 33;
      
      private static const StageID_BFpot:int = 34;
      
      public static const StageIDNum:int = 35;
      
      private static const StageIDInvlaid:int = -1;
      
      private static const IndexNumRow:int = 3;
      
      private static const IndexNumColumn_All:int = 11;
      
      private static const IndexNumColumn_Local:int = 3;
      
      private static const NewIconMax:int = 33;
      
      private static const kGotoSelectedStatusTime:int = 2000;
      
	  //////////
	  // Commenting things that will go unused now
      //private static const StageIdTable:Array = [StageID_BFten,StageID_BFsei,StageID_BFkoh,StageID_BFpln,StageID_BFrck,StageID_BFrcn,StageID_BFnmc,StageID_BFnmb,StageID_BFtre,StageID_BFlnd,StageID_BFcel,StageID_BFceb,StageID_BFtwh,StageID_BFtwn,StageID_BFtwc,StageID_DMtwh,StageID_BFsin,StageID_BFsky,StageID_BFlnc,StageID_BFhel,StageID_BFkai,StageID_BFspe,StageID_BFspv,StageID_BFund,StageID_BFice,StageID_BFtfl,StageID_BFgen,StageID_BFtok];
      
      //private static const StageIdTable_DLC2:Array = [StageID_BFsmt];
      
      //private static const StageIdTable_DLC4:Array = [StageID_BFtwf];
      
      //private static const StageIdTable_DLC8:Array = [StageID_BFpot];
      
      private static const StageIdInfo:Array = [ImageFrameInvalid,ImageFrameInvalid,1,2,3,4,5,7,6,10,11,12,8,9,13,15,14,16,17,19,20,21,22,23,24,18,26,25,27,28,30,29,31,32,33];
      
      //private static const LocalBattle_StageIdTable:Array = [StageID_BFten,StageID_BFnmc,StageID_BFsei,StageID_BFcel,StageID_DMtwh,StageID_BFkai,StageID_BFtfl];
      
      //private static const LocalBattle_StageIdTable_DLC2:Array = [StageID_BFsmt];
      
      //private static const LocalBattle_StageIdTable_DLC8:Array = [StageID_BFpot];
      //////////
	  
      private static const StageNameInvalid:String = "???";
      
      public static const ReceiveTypeFlagCancel:int = 0;
      
      public static const ReceiveTypeMode:int = 1;
      
      public static const ReceiveTypeStageStrStart:int = ReceiveTypeMode + 1;
      
      public static const ReceiveTypeStageStrEnd:int = ReceiveTypeStageStrStart + StageIDNum - 1;
      
      public static const ReceiveTypeStageUnlockStart:int = ReceiveTypeStageStrEnd + 1;
      
      public static const ReceiveTypeStageUnlockEnd:int = ReceiveTypeStageUnlockStart + StageIDNum - 1;
      
      public static const ReceiveTypeImageStrStart:int = ReceiveTypeStageUnlockEnd + 1;
      
      public static const ReceiveTypeImageStrEnd:int = ReceiveTypeImageStrStart + StageIDNum - 1;
      
      public static const ReceiveTypeSelectedStart:int = ReceiveTypeImageStrEnd + 1;
      
      public static const ReceiveTypeSelectedEnd:int = ReceiveTypeSelectedStart + StageIDNum - 1;
      
      public static const ReceiveTypeTime:int = ReceiveTypeSelectedEnd + 1;
      
      public static const ReceiveTypeFlagExit:int = ReceiveTypeTime + 1;
      
      public static const ReceiveType_DlcFlag_1:int = ReceiveTypeFlagExit + 1;
      
      public static const ReceiveType_DlcFlag_2:int = ReceiveType_DlcFlag_1 + 1;
      
      public static const ReceiveType_DlcFlag_3:int = ReceiveType_DlcFlag_2 + 1;
      
      public static const ReceiveType_DlcFlag_4:int = ReceiveType_DlcFlag_3 + 1;
      
      public static const ReceiveType_DlcFlag_8:int = ReceiveType_DlcFlag_4 + 1;
      
      public static const ReceiveTypeNum:int = ReceiveType_DlcFlag_8 + 1;
      
      private static const SendType_DecideTime:int = 0;
      
      private static const SendType_SelectedStage:int = 1;
      
      private static const SendType_CountDownSe:int = 2;
      
      private static const SendType_EnableOptionBGM:int = 3;
       
      
      private var m_callback:Callback = null;
      
      private var m_timer:CountDownTimer = null;
      
      private var m_timeline:MovieClip = null;
      
      private var m_select_index:int;
      
      private var m_stage_image:Array;
      
      private var m_stage_table:Array;
      
      private var m_selected_stage:Array;
      
      private var m_unlock_flag:Array;
      
      private var m_select_timer:int;
      
      private var m_start_time:int;
      
      private var m_index_num_column:int;
      
      private var m_pre_time:int;
      
      private var m_start:int;
      
      private var m_flag_next:Boolean;
	  
	   //////////
	  //private static const XV2P_TestString:String = "[24,0][17,0][14,0][3,0][4,0][5,0][6,0][8,0][30,0][27,0][12,0][13,0][11,0][10,0][9,0][26,0][15,0][16,0][28,0][29,0][18,0][19,0][20,0][25,0][21,0][22,0][23,0][31,0][32,2][33,4][34,8]";
	  //private static const XV2P_TestStringLocal:String = "[24,0][6,0][17,0][12,0][26,0][18,0][22,0][32,2][34,8]";	

	  private var SSNum:int = 0;
	   
	  private var all_stages:Array = null;
	  private var all_local_stages:Array = null;
	  private var my_stages:Array = null;
	  
	  private var start_index:int = 0;
	  private var first_run:Boolean = true;
	  
	  private var local_original_mode:Boolean = false;
	  
	  private var last_selection:int = -1;
	  private var saved_rnd_selection:int = -1;
	  //////////
	  
	  //////////
	  private function GetStages(local:Boolean, stages:Array) : void
	  {
		  //var SlotsString:String = (local) ? XV2P_TestStringLocal : XV2P_TestString;
		  var SlotsString:String = (local) ? XV2Patcher.GetLocalStagesSlotsData() : XV2Patcher.GetStagesSlotsData();
		  var i:int = 0;
		 		  
		  while (i < SlotsString.length)
		  {
		     var has_dlc:int = 1;
			 
			 if (SlotsString.substr(i, 1) != "[")
			 {
				trace("Format error 1\n");
				return;
			 }
			 
			 i++;
			 
			 var pos:int = SlotsString.indexOf("]", i);
			 if (pos == -1)
			 {
				trace("Format error 2\n");
				return;
			 }
			 
			 var stage:String = SlotsString.substring(i,pos);
			 var fields:Array = stage.split(",");
			 
			 if (fields.length != 2)
			 {
				trace("Invalid number of elements: " + fields.length);
				return;
			 }			 
			
			 var dlc:int = int(fields[1]);
			 
			 if (dlc == 2)
			 {
				has_dlc = m_callback.GetUserDataInt(ReceiveType_DlcFlag_2);
			 }
			 else if (dlc == 4)
			 {
				has_dlc = m_callback.GetUserDataInt(ReceiveType_DlcFlag_4);
			 }
			 else if (dlc == 8)
			 {
				has_dlc = m_callback.GetUserDataInt(ReceiveType_DlcFlag_8);
			 }

			 if (has_dlc != 0)
			 {
				 stages.push(int(fields[0]));
			 }
			 
			 i = pos+1;
		  }
	  }
	  
	  private function setImage(mc:MovieClip, id:int) : void
      {
         var bm:Bitmap = null;
         var _loc5_:String = null;
         if(!mc)
         {
            return;
         }
		 
         while(mc.numChildren > 1)
         {
            mc.removeChildAt(mc.numChildren-1);
         } 
		 
		 //trace(mc.numChildren);
		 
		 if (id < StageIDNum) // Original stages get the icon data from the iggytex frame thing
			return;
			
		 if (id >= SSNum)
			return;
	   
	     bm = new Bitmap(null);
	     IggyFunctions.setTextureForBitmap(bm, XV2Patcher.GetStageIconString(id));		
		 
		 //trace("bm w/h = " + bm.width + "/" + bm.height);
		 //trace("mc w/h = " + mc.width + "/" + mc.height);
		 
		 // These numbers give the same exact aspect as original. Don't question the numbers!		 
		 bm.scaleX = 0.6666; // mc.width / bm.width;
		 bm.scaleY = 0.6666; // mc.height / bm.height;
		 
		 // For the record, 80/28 = 240/84 (icon size) = 150/52.5 (mc size)		 
		 bm.x -= 80;
		 bm.y -= 28;		 
           
         mc.addChild(bm);         
      }	  
	  //////////
      
      public function StageSele()
      {
         super();
         m_callback = new Callback(ReceiveTypeNum);
         m_timer = new CountDownTimer();
         m_timeline = null;
         m_select_index = 0;
         m_stage_image = null;
         m_selected_stage = null;
         m_unlock_flag = null;
         m_select_timer = 0;
         m_index_num_column = 0;
         m_pre_time = 99;
         m_start = 0;
         m_flag_next = false;
      }
      
      public function Initialize(param1:MovieClip) : void
      {
         m_timeline = param1;
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,updatePhaseWaitStart);
         m_timeline.visible = false;
         m_timeline.cmn_m_frame.visible = false;
         m_timeline.stage_img.visible = false;
         m_timeline.stage_new.visible = false;
         m_timeline.stage_select.visible = false;
         m_timeline.cmn_m_frame.stop();
         m_timeline.stage_img.stop();
         m_timeline.stage_select.stop();
         m_timer.Initialize(m_timeline.timer.nest._CMN_M_B_mc_timer,null);
         m_select_index = 0;
         m_stage_table = m_stage_table;
         m_flag_next = false;
      }
      
      private function updatePhaseWaitStart(param1:Event) : void
      {
         var _loc2_:int = 0;
         var _loc3_:int = 0;
         var _loc17_:Boolean = false;
         var _loc18_:int = 0;
         var _loc19_:Bitmap = null;
         var _loc20_:String = null;
         var _loc21_:int = 0;
         var _loc22_:MovieClip = null;
         var _loc23_:MovieClip = null;
         var _loc24_:int = 0;
         var _loc25_:int = 0;
         var _loc26_:Boolean = false;
         var _loc27_:int = 0;
         var _loc28_:Boolean = false;
         var _loc29_:int = 0;
         var _loc30_:int = 0;
         var _loc31_:int = 0;
         var _loc32_:int = 0;
         _loc2_ = 0;
         while(ReceiveTypeNum > _loc2_)
         {
            _loc17_ = m_callback.GetUserDataValidFlag(_loc2_);
            if(!_loc17_)
            {
               return;
            }
            _loc2_++;
         }
         m_timeline.stage_select.visible = false;
         m_timeline.stage_new.visible = false;
		 
		  //////////	
		 if (first_run)
		 {
			 first_run = false;
			 SSNum = XV2Patcher.GetNumSsStages();
			 
			 all_stages = new Array();		 	
			 GetStages(false, all_stages);
			
			 all_local_stages = new Array();		 
			 GetStages(true, all_local_stages);	
		 }
		 //////////		 
		 
         var _loc4_:int = m_callback.GetUserDataInt(ReceiveTypeMode);
		 //////////
		 // Replaced code
         /*switch(_loc4_)
         {
            case 0:
               m_stage_table = StageIdTable;
               m_index_num_column = IndexNumColumn_All;
               break;
            case 1:
               m_stage_table = LocalBattle_StageIdTable;
               m_index_num_column = IndexNumColumn_Local;
               break;
            default:
               m_stage_table = StageIdTable;
               m_index_num_column = IndexNumColumn_All;
         }
         var _loc5_:int = m_callback.GetUserDataInt(ReceiveType_DlcFlag_1);
         var _loc6_:int = m_callback.GetUserDataInt(ReceiveType_DlcFlag_2);
         var _loc7_:int = m_callback.GetUserDataInt(ReceiveType_DlcFlag_3);
         var _loc8_:int = m_callback.GetUserDataInt(ReceiveType_DlcFlag_4);
         var _loc9_:int = m_callback.GetUserDataInt(ReceiveType_DlcFlag_8);
         if(_loc4_ == 0)
         {
            if(_loc6_ == 1)
            {
               m_stage_table = m_stage_table.concat(StageIdTable_DLC2);
            }
            if(_loc8_ == 1)
            {
               m_stage_table = m_stage_table.concat(StageIdTable_DLC4);
            }
            if(_loc9_ == 1)
            {
               m_stage_table = m_stage_table.concat(StageIdTable_DLC8);
            }
         }
         else if(_loc4_ == 1)
         {
            if(_loc6_ == 1)
            {
               m_stage_table = m_stage_table.concat(LocalBattle_StageIdTable_DLC2);
            }
            if(_loc9_ == 1)
            {
               m_stage_table = m_stage_table.concat(LocalBattle_StageIdTable_DLC8);
            }
         }*/
		 switch (_loc4_) 
		 {
			case 1:
				my_stages = all_local_stages;		
				local_original_mode = (my_stages.length >= 7 && my_stages.length <= 9);
				break;
			default:
				my_stages = all_stages;	
				local_original_mode = false;
		 }		 
		 
		 var end_index:int = start_index + NewIconMax;
		 if (end_index > my_stages.length)
		 {
			end_index = my_stages.length;
		 }
		 
		 m_stage_table = my_stages.slice(start_index, end_index);
		 m_index_num_column = m_stage_table.length / 3;
		 
		 if ((m_stage_table.length % 3) != 0)
			m_index_num_column++;
			
		 //trace("Length = " + m_stage_table.length);
		 //trace("Columns = " + m_index_num_column);		 
		 //////////
		 
         var _loc10_:int = 0;
		 
         //////////if(_loc4_ == 1)
		 if (local_original_mode)
		 //////////
         {
            m_timeline.gotoAndStop(2);
         }
         else
         {
            m_timeline.gotoAndStop(1);
         }
         _loc2_ = 0;
         while(m_stage_table.length > _loc2_)
         {
            _loc10_ = StartSelectIndex + _loc2_;
            _loc10_ = _loc10_ % m_stage_table.length;
            if(!checkUnlockStage(calcStageId(_loc10_)))
            {
               _loc2_++;
               continue;
            }
            m_select_index = _loc10_;
            break;
         }
         m_timeline.cmn_m_frame.visible = true;
         m_timeline.stage_img.visible = true;
         m_timeline.stage_select.visible = false;
         m_timeline.cmn_m_frame.cmn_m_frame.gotoAndPlay("start");
         m_timeline.stage_img.gotoAndPlay("start");
         //////////m_stage_image = new Array(StageIDNum);
		 m_stage_image = new Array(SSNum);
		 //////////
         _loc2_ = 0;
         while(m_stage_table.length > _loc2_)
         {
            _loc18_ = calcStageId(_loc2_);
            if(_loc18_ != StageIDInvlaid)
            {
               _loc19_ = new Bitmap(null);
               //////////_loc20_ = m_callback.GetUserDataString(ReceiveTypeImageStrStart + _loc18_);
			   if (_loc18_ >= StageIDNum && _loc18_ < SSNum)
			   {
				   _loc20_ = XV2Patcher.GetStageImageString(_loc18_);
			   }
			   else
			   {
				   _loc20_ = m_callback.GetUserDataString(ReceiveTypeImageStrStart + _loc18_)
			   }
			   //////////
			   
               IggyFunctions.setTextureForBitmap(_loc19_,_loc20_);
               _loc19_.scaleX = 1024 / _loc19_.width;
               _loc19_.scaleY = 256 / _loc19_.height;
               m_stage_image[_loc18_] = _loc19_;
            }
            _loc2_++;
         }
         //////////m_selected_stage = new Array(StageIDNum);
         //m_unlock_flag = new Array(StageIDNum);
		 m_selected_stage = new Array(SSNum);
		 m_unlock_flag = new Array(SSNum);
		 //////////
         _loc2_ = 0;
         //////////while(StageIDNum > _loc2_)
		 while (SSNum > _loc2_)
		 //////////
         {
            //////////_loc21_ = m_callback.GetUserDataInt(ReceiveTypeSelectedStart + _loc2_);
			if (_loc2_ >= StageIDNum)
			{
				_loc21_ = XV2Patcher.StageHasBeenSelectedBefore(_loc2_);
			}
			else
			{
				_loc21_ = m_callback.GetUserDataInt(ReceiveTypeSelectedStart + _loc2_);
			}
			//////////
            m_selected_stage[_loc2_] = _loc21_;
            m_unlock_flag[_loc2_] = false;
            _loc2_++;
         }
         var _loc11_:MovieClip = m_timeline.stage_new;
         var _loc12_:MovieClip = null;
         _loc2_ = 0;
         while(_loc2_ < NewIconMax)
         {
            _loc12_ = _loc11_["stage_new_" + _loc2_];
            _loc12_.visible = false;
            _loc2_++;
         }
         m_timeline.stage_new.gotoAndPlay("start");
         m_timeline.stage_new.visible = true;
         var _loc13_:MovieClip = m_timeline.stage_select;
         _loc3_ = 0;
         while(m_index_num_column > _loc3_)
         {
            _loc22_ = _loc13_["stage_icn_set" + _loc3_];
            _loc2_ = 0;
            while(IndexNumRow > _loc2_)
            {
               _loc23_ = _loc22_["nest_stage_sel" + (_loc2_ + 1)];
			   //////////
			   _loc23_.visible = true;
			   //////////
               _loc24_ = calcStageListIndex(_loc2_,_loc3_);
               _loc25_ = calcStageId(_loc24_);
               _loc12_ = null;
               if(_loc25_ != StageIDInvlaid)
               {
                  _loc12_ = _loc11_["stage_new_" + _loc24_];
               }
               if(_loc25_ == StageIDInvlaid)
               {
                  _loc23_.visible = false;
                  if(_loc12_)
                  {
                     _loc12_.visible = false;
                  }
               }
               else
               {
                  _loc26_ = false;
                  if(_loc25_ != StageIDInvlaid)
                  {
                     _loc29_ = m_stage_table[_loc24_];
                     _loc30_ = m_selected_stage[_loc29_];
                     if(m_selected_stage[m_stage_table[_loc24_]] == 1)
                     {
                        _loc26_ = false;
                     }
                     else
                     {
                        _loc26_ = true;
                     }
                  }
                  _loc27_ = getStageFrame(_loc29_);
                  _loc23_.btn_act.img_icon_frm.gotoAndStop(_loc27_);
				  //////////
				  setImage(_loc23_.btn_act, _loc25_);
				  //////////
                  _loc28_ = checkUnlockStage(_loc25_);
                  if(!_loc28_)
                  {
                     _loc23_.gotoAndStop("lock");
                     _loc26_ = false;
                  }
                  else
                  {
                     _loc23_.btn_act.img_icon_frm.gotoAndStop(_loc27_);
                     _loc23_.gotoAndStop("off");
                  }
                  if(_loc12_)
                  {
                     _loc12_.visible = _loc26_;
                     _loc31_ = !!_loc26_?1:0;
                     _loc32_ = m_stage_table[_loc24_];
                     m_unlock_flag[_loc32_] = _loc31_;
                  }
               }
               _loc2_++;
            }
            _loc3_++;
         }
		 
		  //////////
		 // We have to hide these icons aswell
		 if (!local_original_mode)
		 {
			var i:int;			
			
			for (i = m_index_num_column; i < IndexNumColumn_All; i++)
			{
				var row:int;
				_loc22_ = _loc13_["stage_icn_set" + i];
				
				for (row = 0; row < IndexNumRow; row++)
				{
					_loc23_ = _loc22_["nest_stage_sel" + (row + 1)];
					_loc23_.visible = false;
				}
			}
		 }
		 
		 if (saved_rnd_selection >= 0 && saved_rnd_selection < m_stage_table.length)
		 {
			m_select_index = saved_rnd_selection;
			saved_rnd_selection = -1;
		 }
		 
		 //////////
		 
         var _loc14_:MovieClip = m_timeline.stage_img;
         var _loc15_:int = calcStageId(m_select_index);
         _loc14_.sys_stagename.sys_stagename.autoSize = "left";
         _loc14_.sys_stagename.sys_stagename.wordWrap = false;
         _loc14_.sys_stagename.sys_stagename.htmlText = getStageName(_loc15_);
         var _loc16_:int = m_callback.GetUserDataInt(ReceiveTypeTime);
         if(_loc16_ < 0)
         {
            m_timeline.timer.visible = false;
         }
         else
         {
            m_timeline.timer.visible = true;
            m_timer.Start(_loc16_,cbFuncEndTimer);
         }
         m_start = 1;
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseWaitStart);
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,updatePhaseOpen);
      }
      
      private function updatePhaseOpen(param1:Event) : void
      {
         var _loc4_:int = 0;
         var _loc5_:int = 0;
         var _loc6_:Boolean = false;
         var _loc7_:MovieClip = null;
         var _loc8_:int = 0;
         var _loc9_:MovieClip = null;
         var _loc10_:int = 0;
         var _loc11_:int = 0;
         var _loc12_:int = 0;
         var _loc13_:Boolean = false;
         m_timeline.visible = true;
         var _loc2_:MovieClip = m_timeline.stage_select;
         _loc2_.visible = true;
         if(m_start == 1)
         {
            m_timeline.stage_select.gotoAndPlay("start");
            m_timeline.stage_new.gotoAndPlay("start");
            if(m_stage_table.length > 10)
            {
               m_timeline.stage_new["stage_new_28"].visible = false;
               m_timeline.stage_new["stage_new_29"].visible = false;
            }
            _loc4_ = 0;
            while(_loc4_ < m_stage_table.length)
            {
               _loc5_ = m_stage_table[_loc4_];
               _loc6_ = m_unlock_flag[_loc5_] == 1?true:false;
               m_timeline.stage_new["stage_new_" + _loc4_].visible = _loc6_;
               _loc4_++;
            }
            m_start = 0;
         }
         var _loc3_:int = 0;
         while(m_index_num_column > _loc3_)
         {
            _loc7_ = _loc2_["stage_icn_set" + _loc3_];
            _loc8_ = 0;
            while(IndexNumRow > _loc8_)
            {
               _loc9_ = _loc7_["nest_stage_sel" + (_loc8_ + 1)];
               _loc10_ = calcStageListIndex(_loc8_,_loc3_);
               _loc11_ = calcStageId(_loc10_);
               _loc12_ = getStageFrame(_loc11_);
               _loc9_.btn_act.img_icon_frm.gotoAndStop(_loc12_);
               _loc13_ = checkUnlockStage(_loc11_);
               if(!_loc13_)
               {
                  _loc9_.gotoAndStop("lock");
               }
               else
               {
                  _loc9_.btn_act.img_icon_frm.gotoAndStop(_loc12_);
                  _loc9_.gotoAndStop("off");
               }
               _loc8_++;
            }
            _loc3_++;
         }
         if(m_timeline.stage_img.currentFrame != Utility.GetLabelEndFrame(m_timeline.stage_img,"start"))
         {
            return;
         }
         changeSelectStage(m_select_index);
         updateSelectSel();
         m_timeline.select_sel.visible = true;
         m_timeline.select_sel.gotoAndPlay("on");
         m_callback.CallbackUserData("user",SendType_EnableOptionBGM,1);
         clearSelectTimer();
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseOpen);
         m_timeline.stage.addEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         m_flag_next = false;
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,updatePhaseMain);
      }
      
      private function updatePhaseMain(param1:Event) : void
      {
         var _loc8_:int = 0;
         var _loc9_:* = undefined;
         var _loc10_:MovieClip = null;
         var _loc11_:int = 0;
         var _loc12_:MovieClip = null;
         var _loc13_:int = 0;
         var _loc14_:int = 0;
         var _loc15_:int = 0;
         var _loc16_:Boolean = false;
         var _loc17_:Bitmap = null;
         var _loc18_:int = 0;
         if(m_timer)
         {
            _loc8_ = m_timer.GetTime();
            if(m_pre_time != _loc8_)
            {
               if(_loc8_ < 10 && _loc8_ > 0)
               {
                  _loc9_ = 8;
                  m_callback.CallbackUserData("user",SendType_CountDownSe,_loc9_);
               }
               m_pre_time = _loc8_;
            }
         }
         var _loc2_:MovieClip = m_timeline.stage_select;
         var _loc3_:int = 0;
         while(m_index_num_column > _loc3_)
         {
            _loc10_ = _loc2_["stage_icn_set" + _loc3_];
            _loc11_ = 0;
            while(IndexNumRow > _loc11_)
            {
               _loc12_ = _loc10_["nest_stage_sel" + (_loc11_ + 1)];
               _loc13_ = calcStageListIndex(_loc11_,_loc3_);
               _loc14_ = calcStageId(_loc13_);
               _loc15_ = getStageFrame(_loc14_);
               _loc12_.btn_act.img_icon_frm.gotoAndStop(_loc15_);
               _loc16_ = checkUnlockStage(_loc14_);
               if(!_loc16_)
               {
                  _loc12_.gotoAndStop("lock");
               }
               else
               {
                  _loc12_.btn_act.img_icon_frm.gotoAndStop(_loc15_);
                  _loc12_.gotoAndStop("off");
               }
               _loc11_++;
            }
            _loc3_++;
         }
         updateSelectSel();
         updateSelectedTimer();
         var _loc4_:int = calcStageId(m_select_index);
         var _loc5_:MovieClip = m_timeline.stage_img;
         if(_loc5_.stage_img.currentFrame == Utility.GetLabelEndFrame(_loc5_.stage_img,"push_out"))
         {
            _loc5_.stage_img.gotoAndPlay("push_in");
            _loc5_.sys_stagename.sys_stagename.autoSize = "left";
            _loc5_.sys_stagename.sys_stagename.wordWrap = false;
            _loc5_.sys_stagename.sys_stagename.htmlText = getStageName(_loc4_);
            if(_loc5_.stage_img.stageimg.numChildren > 0)
            {
               _loc5_.stage_img.stageimg.removeChildAt(0);
            }
            _loc17_ = m_stage_image[_loc4_];
            if(_loc17_)
            {
               _loc5_.stage_img.stageimg.addChildAt(_loc17_,0);
            }
         }
         else if(_loc5_.stage_img.currentFrame == Utility.GetLabelEndFrame(_loc5_.stage_img,"push_in"))
         {
            _loc5_.stage_img.gotoAndPlay("wait");
         }
         var _loc6_:int = m_callback.GetUserDataInt(ReceiveTypeFlagExit);
         if(_loc6_)
         {
            m_flag_next = true;
         }
         var _loc7_:MovieClip = m_timeline.select_sel;
         if(_loc7_.currentLabel == "push")
         {
            m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         }
         if(_loc7_.currentFrame == Utility.GetLabelEndFrame(_loc7_,"push"))
         {
            m_callback.CallbackDecide(_loc4_);
            sendSelectedStage();
            _loc18_ = -1;
            if(m_timer)
            {
               _loc18_ = m_timer.GetTime();
            }
            m_callback.CallbackUserData("user",SendType_DecideTime,_loc18_);
            m_flag_next = true;
         }
         if(!m_flag_next)
         {
            return;
         }
         m_timeline.cmn_m_frame.cmn_m_frame.gotoAndPlay("end");
         m_timeline.stage_img.gotoAndPlay("end");
         m_timeline.stage_select.gotoAndPlay("end");
         m_timeline.stage_new.gotoAndPlay("end");
         m_timer.End();
         m_timeline.timer.gotoAndPlay("end");
         m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseMain);
         m_flag_next = false;
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,updatePhaseEnd);
      }
      
      private function updatePhaseEnd(param1:Event) : void
      {
         updateSelectSel();
         if(m_timeline.stage_img.currentFrame != Utility.GetLabelEndFrame(m_timeline.stage_img,"end"))
         {
            return;
         }
         m_timeline.select_sel.visible = false;
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseEnd);
         m_callback.CallbackExit();
      }
      
      private function getStageFrame(param1:int) : int
      {
         //////////
		 if (param1 >= StageIDNum && param1 < SSNum)
			return 0;
		 //////////
		 
		 var _loc2_:int = ImageFrameInvalid;
         if(0 <= param1 && param1 < StageIdInfo.length)
         {
            _loc2_ = StageIdInfo[param1];
         }
         return _loc2_;
      }
      
      private function getStageName(param1:int) : String
      {
          //////////
		 if (param1 >= StageIDNum && param1 < SSNum)
			return XV2Patcher.GetStageName(param1);
		 //////////
		 
		 if(0 <= param1 && param1 < StageIdInfo.length)
         {
            return m_callback.GetUserDataString(ReceiveTypeStageStrStart + param1);
         }
         return StageNameInvalid;
      }
      
      private function calcStageListIndex(param1:int, param2:int) : int
      {
         return param1 + param2 * IndexNumRow;
      }
      
      private function calcIconIndexRow(param1:int) : int
      {
         if(param1 < 0 || m_stage_table.length <= param1)
         {
            return -1;
         }
         return param1 % IndexNumRow;
      }
      
      private function calcIconIndexColumn(param1:int) : int
      {
         if(param1 < 0 || m_stage_table.length <= param1)
         {
            return -1;
         }
         return param1 / IndexNumRow;
      }
      
      private function calcStageId(param1:int) : int
      {
         if(param1 < 0 || m_stage_table.length <= param1)
         {
            return StageIDInvlaid;
         }
         return m_stage_table[param1];
      }
      
      private function checkUnlockStage(param1:*) : Boolean
      {
          //////////
		 if (param1 >= StageIDNum && param1 < SSNum)
			return true;
		 //////////
		 
		 if(param1 < 0 || StageIDNum <= param1)
         {
            return false;
         }
         return Boolean(m_callback.GetUserDataInt(ReceiveTypeStageUnlockStart + param1));
      }
      
      private function updateSelectedTimer() : *
      {
         m_select_timer = getTimer();
         var _loc1_:int = m_select_timer - m_start_time;
         if(_loc1_ > kGotoSelectedStatusTime)
         {
            sendSelectedStage();
         }
      }
      
      private function sendSelectedStage() : *
      {
         var _loc1_:int = calcIconIndexColumn(m_select_index);
         var _loc2_:int = calcIconIndexRow(m_select_index);
         var _loc3_:int = calcStageListIndex(_loc2_,_loc1_);
         var _loc4_:MovieClip = m_timeline.stage_new;
         var _loc5_:MovieClip = _loc4_["stage_new_" + _loc3_];
         var _loc6_:int = m_stage_table[_loc3_];
         if(_loc5_)
         {
            if(m_selected_stage[_loc6_] == 0)
            {
               m_selected_stage[_loc6_] = 1;
               m_callback.CallbackUserData("user",SendType_SelectedStage,_loc6_);
            }
            _loc5_.visible = false;
         }
      }
      
      private function clearSelectTimer() : *
      {
         m_select_timer = 0;
         m_start_time = getTimer();
      }
      
      private function changeLR(param1:Boolean) : void
      {
         var _loc4_:int = 0;
         var _loc5_:int = 0;
         var _loc6_:Boolean = false;
         var _loc2_:int = calcIconIndexColumn(m_select_index);
         var _loc3_:int = calcIconIndexRow(m_select_index);
         clearSelectTimer();
         while(true)
         {
            if(param1)
            {
               _loc2_--;
               if(_loc2_ < 0)
               {
                  _loc2_ = m_index_num_column - 1;
                  _loc3_--;
                  _loc3_ = _loc3_ + IndexNumRow;
                  _loc3_ = _loc3_ % IndexNumRow;
               }
            }
            else
            {
               _loc2_++;
               if(m_index_num_column <= _loc2_)
               {
                  _loc2_ = 0;
                  _loc3_++;
                  _loc3_ = _loc3_ + IndexNumRow;
                  _loc3_ = _loc3_ % IndexNumRow;
               }
            }
            _loc4_ = calcStageListIndex(_loc3_,_loc2_);
            _loc5_ = calcStageId(_loc4_);
            _loc6_ = checkUnlockStage(_loc5_);
            if(!_loc6_)
            {
               continue;
            }
            break;
         }
         changeSelectStage(_loc4_);
      }
      
      private function changeUD(param1:Boolean) : void
      {
         var _loc4_:int = 0;
         var _loc5_:int = 0;
         var _loc6_:Boolean = false;
         var _loc2_:int = calcIconIndexColumn(m_select_index);
         var _loc3_:int = calcIconIndexRow(m_select_index);
         clearSelectTimer();
         while(true)
         {
            if(param1)
            {
               _loc3_--;
               if(_loc3_ < 0)
               {
                  _loc3_ = IndexNumRow - 1;
                  _loc2_--;
                  _loc2_ = _loc2_ + m_index_num_column;
                  _loc2_ = _loc2_ % m_index_num_column;
               }
            }
            else
            {
               _loc3_++;
               if(IndexNumRow <= _loc3_)
               {
                  _loc3_ = 0;
                  _loc2_++;
                  _loc2_ = _loc2_ + m_index_num_column;
                  _loc2_ = _loc2_ % m_index_num_column;
               }
            }
            _loc4_ = calcStageListIndex(_loc3_,_loc2_);
            _loc5_ = calcStageId(_loc4_);
            _loc6_ = checkUnlockStage(_loc5_);
            if(!_loc6_)
            {
               continue;
            }
            break;
         }
         changeSelectStage(_loc4_);
      }
      
      private function updateSelectSel() : void
      {
         var _loc1_:MovieClip = m_timeline.stage_select;
         var _loc2_:int = calcIconIndexColumn(m_select_index);
         var _loc3_:MovieClip = _loc1_["stage_icn_set" + _loc2_];
         var _loc4_:int = calcIconIndexRow(m_select_index);
         var _loc5_:MovieClip = _loc3_["nest_stage_sel" + (_loc4_ + 1)];
         var _loc6_:MovieClip = m_timeline.select_sel;
         _loc6_.x = _loc1_.x + _loc3_.x + _loc5_.x;
         _loc6_.y = _loc1_.y + _loc3_.y + _loc5_.y;
         _loc6_.alpha = _loc5_.alpha;
         var _loc7_:int = calcStageId(m_select_index);
         var _loc8_:int = getStageFrame(_loc7_);
         _loc6_.btn_act.img_icon_frm.gotoAndStop(_loc8_);
		 //////////
		 if (last_selection != m_select_index)
		 {
			setImage(_loc6_.btn_act, _loc7_);
			last_selection = m_select_index;
		 }
		 /////////
      }
      
      private function changeSelectStage(param1:int) : void
      {
         var _loc2_:MovieClip = m_timeline.stage_img;
         if(_loc2_.stage_img.currentLabel != "push_out")
         {
            _loc2_.stage_img.gotoAndPlay("push_out");
         }
         m_select_index = param1;
      }
      
      private function cbFuncEndTimer() : void
      {
         pushEnter();
      }
      
      private function pushU() : void
      {
         changeUD(true);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushD() : void
      {
         changeUD(false);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushL() : void
      {
         changeLR(true);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushR() : void
      {
         changeLR(false);
         m_callback.CallbackSe(m_callback.SeTypeCarsol);
      }
      
      private function pushRandom() : void
      {
         //////////
		 /*var _loc4_:int = 0;
         var _loc1_:int = 0;
         clearSelectTimer();
         var _loc2_:int = 0;
         _loc1_ = 0;
         while(m_stage_table.length > _loc1_)
         {
            _loc4_ = StartSelectIndex + _loc1_;
            _loc4_ = _loc4_ % m_stage_table.length;
            if(checkUnlockStage(calcStageId(_loc4_)))
            {
               _loc2_++;
            }
            _loc1_++;
         }
         var _loc3_:int = 0;
         if(_loc2_ > 1)
         {
            _loc3_ = Math.random() * _loc2_;
         }
         _loc1_ = 0;
         while(_loc3_ > _loc1_)
         {
            changeLR(false);
            _loc1_++;
         }
         m_callback.CallbackSe(m_callback.SeTypeCarsol);*/
		 
		 // Let's reimplement this shit
		 
		 var unlocked_stages_idx:Array = new Array();
		 var i:int;
		 
		 for (i = 0; i < my_stages.length; i++)
		 {
			if (checkUnlockStage(my_stages[i]))
			{
				unlocked_stages_idx.push(i);
			}
		 }
		 
		 if (unlocked_stages_idx.length == 0) // Wtf
			return;
			
		 var end_index:int = start_index + NewIconMax;
		 if (end_index > my_stages.length)
			end_index = my_stages.length;			
		  
		 var rnd:int = Math.random() * unlocked_stages_idx.length;
		 
		 if (rnd >= unlocked_stages_idx.length)
			rnd = unlocked_stages_idx.length-1;
			
		 var next_stage_idx = unlocked_stages_idx[rnd];
		 
		 if (next_stage_idx >= start_index && next_stage_idx < end_index)
		 {
			// We aren't changing pages
			changeSelectStage(next_stage_idx-start_index);
			m_callback.CallbackSe(m_callback.SeTypeCarsol);
		 }
		 else
		 {
			start_index = (next_stage_idx/NewIconMax);
			start_index *= NewIconMax;
			saved_rnd_selection = next_stage_idx - start_index;
			reload();
		 }		 
		 //////////
      }
      
      private function pushEnter() : void
      {
         m_callback.CallbackUserData("user",SendType_EnableOptionBGM,0);
         var _loc1_:MovieClip = m_timeline.select_sel;
         _loc1_.gotoAndPlay("push");
         m_callback.CallbackSe(m_callback.SeTypeDecide);
         var _loc2_:int = m_callback.GetUserDataInt(ReceiveTypeTime);
         if(_loc2_ >= 0)
         {
            m_timer.Stop();
         }
      }
      
      private function pushCancel() : void
      {
         if(!m_callback.GetUserDataInt(ReceiveTypeFlagCancel))
         {
            return;
         }
         m_flag_next = true;
         m_callback.CallbackSe(m_callback.SeTypeCancel);
         m_callback.CallbackCancel();
      }
	  
	  //////////
	  private function reload() : void
	  {
		 // Finalization
		 m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseMain);
		 
		 /*m_timeline.cmn_m_frame.cmn_m_frame.gotoAndPlay("end");
         m_timeline.stage_img.gotoAndPlay("end");
         m_timeline.stage_select.gotoAndPlay("end");
         m_timeline.stage_new.gotoAndPlay("end");*/		 
         
		 // Reinitialization
		 m_timeline.stage.addEventListener(Event.ENTER_FRAME,updatePhaseWaitStart);
         m_timeline.visible = false;
         m_timeline.cmn_m_frame.visible = false;
         m_timeline.stage_img.visible = false;
         m_timeline.stage_new.visible = false;
         m_timeline.stage_select.visible = false;
         m_timeline.cmn_m_frame.stop();
         m_timeline.stage_img.stop();
         m_timeline.stage_select.stop();
         //m_timer.Initialize(m_timeline.timer.nest._CMN_M_B_mc_timer,null);
		 m_timeline.stop();
         m_select_index = 0;
         //m_stage_table = m_stage_table;
         m_flag_next = false;
		 
		 var col:int;		 
		 
		 for (col = 0; col < m_index_num_column; col++)
		 {
			var row:int;
			var icn_set:MovieClip = m_timeline.stage_select["stage_icn_set" + col];
			
			for (row = 0; row < IndexNumRow; row++)
			{            
               var nest = icn_set["nest_stage_sel" + (row + 1)]; 
			   
			   while (nest.btn_act.numChildren > 1)
			   {
					nest.btn_act.removeChildAt(nest.btn_act.numChildren-1);
			   }
			   
			   nest.visible = false;
			}
		 }
		 
		 while (m_timeline.select_sel.btn_act.numChildren > 1)
		 {
			m_timeline.select_sel.btn_act.removeChildAt(m_timeline.select_sel.btn_act.numChildren-1);
		 }
		 
		 m_timeline.select_sel.visible = false;
		 
		 var img:MovieClip = m_timeline.stage_img.stage_img.stageimg;
		 
		 if (img.numChildren > 0)
			img.removeChildAt(0);		 
		 
		 // Our things
		 last_selection = -1;
	  }	  
	  
	  private function pushKeyL() : void
	  {
		  var next_index:int = start_index - NewIconMax;
		  
		  if (next_index < 0)
			return;
			
		  start_index = next_index;
		  reload();
	  }
	  
	  private function pushKeyR() : void
	  {
		  var next_index:int = start_index + NewIconMax;
		  
		  if (next_index >= my_stages.length)
			return;
			
		  start_index = next_index;
		  reload();
	  }
	  //////////
      
      private function checkKey(param1:KeyboardEvent) : void
      {
         switch(param1.keyCode)
         {
            case Keyboard.UP:
               pushU();
               break;
            case Keyboard.DOWN:
               pushD();
               break;
            case Keyboard.LEFT:
               pushL();
               break;
            case Keyboard.RIGHT:
               pushR();
               break;
            case 89:
            case Keyboard.END:
               pushRandom();
               break;
            case Keyboard.ESCAPE:
               pushCancel();
               break;
            case Keyboard.ENTER:
               pushEnter();
			//////////
			   break;
			case Keyboard.DELETE:
               pushKeyL();
               break;
            case Keyboard.PAGE_DOWN:
               pushKeyR();
               break;
			//////////
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
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseWaitStart);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseOpen);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseMain);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,updatePhaseEnd);
         m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         m_timer.Destroy();
         m_timer = null;
         m_timeline.visible = false;
         m_timeline = null;
      }
   }
}
