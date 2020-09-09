package action_script
{
   import flash.display.MovieClip;
   import flash.events.Event;
   import flash.events.KeyboardEvent;
   import flash.ui.Keyboard;
   
   //////////
   import POPUP_PAUSE_fla.mc_btn_btnact_4;
   /////////
   
   public class PopupPause
   {
      
      public static const ListMax:int = 6;
      
      public static const ReceiveTypeListNum:int = 0;
      
      public static const ReceiveTypeListStrStart:int = ReceiveTypeListNum + 1;
      
      public static const ReceiveTypeListStrEnd:int = ReceiveTypeListStrStart + ListMax - 1;
      
      public static const ReceiveTypeListValidFlagStart:int = ReceiveTypeListStrEnd + 1;
      
      public static const ReceiveTypeListValidFlagEnd:int = ReceiveTypeListValidFlagStart + ListMax - 1;
      
      public static const ReceiveTypeFlagEnd:int = ReceiveTypeListValidFlagEnd + 1;
      
      public static const ReceiveTypeInitSelectIndex:int = ReceiveTypeFlagEnd + 1;
      
      public static const ReceiveTypeNum:int = ReceiveTypeInitSelectIndex + 1;
      
      public static const SendType_SelectIndex:int = 0;
       
      
      private var m_callback:Callback = null;
      
      private var m_button_list:ButtonList = null;
      
      private var m_timeline:MovieClip = null;
      
      private var m_flag_decide:Boolean = false;
	  
	  //////////
	  /*private static const TOGGLE_BATTLE_UI:int = 0;
	  private static const DUMP_AG_PORTRAIT:int = 1;
	  private static const START_MOB_CTRL:int = 2;*/
	  private var TOGGLE_BATTLE_UI_VAR:int = -1;
	  private var DUMP_AG_PORTRAIT_VAR:int = -1;
	  private var START_MOB_CTRL_VAR:int = -1;
	  
	  private var xv2p_added_buttons:Array = new Array();
	  private var xv2p_mobs_idx:Array = new Array();
	  private var xv2p_extended:Boolean = false;
	  private var num_orig:int;
	  private var xv2p_add_where:int = 0;
	  private var disable_mob_ctrl:Boolean = false;
	  
	  public function SetupControlMobs() : Array
	  {
		var allies_str:String = XV2Patcher.GetPlayerAllies();
		
		if (allies_str.length == 0)
			return new Array();
		
		var allies_str_arr = allies_str.split(",");
		var	allies:Array = new Array(allies_str_arr.length);
		
		if (allies.length == 1 && allies_str_arr[0] == "!")
		{
			allies[0] = -1;
			disable_mob_ctrl = true;
			return allies;
		}
		
		var i:int;
		
		for (i = 0; i < allies.length; i++)
		{
			allies[i] = int(allies_str_arr[i]);			
		}

		return allies;
	  }
	   
	  public function SetupAddedButtons() : void
	  {
		if (XV2Patcher.IsToggleUIEnabled())
		{
			TOGGLE_BATTLE_UI_VAR = 0;
			xv2p_added_buttons.push("TOGGLE_BATTLE_UI");
		}		
		
		if (XV2Patcher.CanDumpAutoGenPortrait())
		{
			DUMP_AG_PORTRAIT_VAR = xv2p_added_buttons.length;
			xv2p_added_buttons.push("Dump " + XV2Patcher.GetFirstAutoGenPortraitCharName() + " portrait");
		}		
		
		xv2p_mobs_idx = SetupControlMobs();
		
		if (xv2p_mobs_idx.length > 0)
		{
			START_MOB_CTRL_VAR = xv2p_added_buttons.length;
			
			var i:int;
			
			for (i = 0; i < xv2p_mobs_idx.length; i++)
			{
				if (disable_mob_ctrl)
				{
					xv2p_added_buttons.push("Take control disabled (opened gate)");
				}
				else
				{			
					var str:String = "Take control of " + XV2Patcher.GetMobName(xv2p_mobs_idx[i]);				
					xv2p_added_buttons.push(str);
				}
			}		
		}
	  }
	  //////////
      
      public function PopupPause()
      {
         super();
         m_callback = new Callback(ReceiveTypeNum);
         //////////m_button_list = new ButtonList(ListMax);
		 SetupAddedButtons();
		 m_button_list = new ButtonList(ListMax + xv2p_added_buttons.length);
		 //////////
         m_timeline = null;
         m_flag_decide = false;
      }
      
      public function Initialize(param1:MovieClip) : void
      {
         var _loc2_:int = 0;
         m_timeline = param1;
         //////////m_timeline.pausemenu.visible = true;
		 m_timeline.pausemenu.visible = false;
		 m_timeline.pausemenu.nest.visible = false;
		 m_timeline.visible = false;
		 /////////
         m_timeline.pausemenu.gotoAndStop("start");
         _loc2_ = 0;
         while(ListMax > _loc2_)
         {
            m_timeline.pausemenu.nest["btn" + (_loc2_ + 1)].visible = false;
            _loc2_++;
         }
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,phaseWaitStart);
      }
      
      public function phaseWaitStart(param1:Event) : void
      {
         var _loc2_:int = 0;
         var _loc4_:* = false;
         var _loc5_:MovieClip = null;
         _loc2_ = 0;
         while(ReceiveTypeNum > _loc2_)
         {
            _loc4_ = Boolean(m_callback.GetUserDataValidFlag(_loc2_));
            if(!_loc4_)
            {
               return;
            }
            _loc2_++;
         }
         var _loc3_:int = m_callback.GetUserDataInt(ReceiveTypeListNum);
         _loc2_ = 0;
         while(_loc3_ > _loc2_)
         {
            _loc5_ = m_timeline.pausemenu.nest["btn" + (_loc2_ + 1)];
            Utility.SetAutoSizedText(_loc5_.sys_btn.sys,m_callback.GetUserDataString(ReceiveTypeListStrStart + _loc2_));
            //////////_loc5_.visible = true;
			_loc5_.visible = false;
			//////////
            _loc4_ = true;
            if(m_callback.GetUserDataValidFlag(ReceiveTypeListValidFlagStart + _loc2_))
            {
               _loc4_ = m_callback.GetUserDataInt(ReceiveTypeListValidFlagStart + _loc2_) != 0;
            }
            if(!_loc4_)
            {
               _loc5_.gotoAndPlay("gray_off");
            }
            m_button_list.AddButton(_loc5_,_loc2_,_loc4_);
            _loc2_++;
         }
		 
		 //////////		 
		 num_orig = _loc3_;
		 
		 if (XV2Patcher.ShouldExtendPauseMenu() && xv2p_added_buttons.length > 0)
		 {		 
			 if (num_orig == 5 || num_orig == 4) // 5 -> pq, free battle. 4 -> main quest or quests where you can't select char
			 {
				xv2p_add_where = 3;
				xv2p_extended = true;
			 }	
			 else if (num_orig == 6)
			 {
				// Training mode
				xv2p_add_where = 4;
				xv2p_extended = true;
			 }			
		 }
		 //////////
		 
         m_button_list.ChangeButton(0);
		 m_button_list.ChangeButton(m_callback.GetUserDataInt(ReceiveTypeInitSelectIndex));
         m_callback.CallbackUserData("user",SendType_SelectIndex,m_button_list.GetSelectButton());
         m_timeline.pausemenu.nest.gotoAndPlay("ver" + _loc3_);		
         m_timeline.visible = true;
         m_timeline.pausemenu.visible = true;
         m_timeline.pausemenu.gotoAndPlay("start");
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseWaitStart);
         m_flag_decide = false;
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,phaseOpen);
      }
	  
	  //////////
	  private function resizeButton(button : MovieClip) : void
	  {
		var rh:Number = button.height;
		var ah:Number = button.getBounds(m_timeline).height;
		
		//trace(button.getBounds(m_timeline));
		button.height = rh / (ah / rh);
	  }
	  
	  private function addNewButton(str : String, distance_Y : Number, top_y: Number, pos : int, enable : Boolean) : Number
	  {
		 var newButton:mc_btn_btnact_4  = new mc_btn_btnact_4 ();
		 m_timeline.pausemenu.nest.addChild(newButton);
		 Utility.SetAutoSizedText(newButton.sys_btn.sys, str);
		 newButton.visible = true;
		 resizeButton(newButton);
		 
		 if (pos == 0)	
			newButton.y = m_timeline.pausemenu.nest["btn1"].y - xv2p_added_buttons.length * 2;
		 else
			newButton.y =  top_y + distance_Y * pos;
			
		 if (!enable)
			newButton.gotoAndPlay("gray_off");
		 
		 m_button_list.AddButton(newButton, -1, enable);
		 return newButton.y;
	  }	  
	  //////////
      
      public function phaseOpen(param1:Event) : void
      {
         if(m_timeline.pausemenu.currentLabel != "wait")
         {
            return;
         }
		 
		 //////////
		 var i:int;
		 
		 // Make things visible NOW
		 m_timeline.visible = true;
		 m_timeline.pausemenu.visible = true;
		 m_timeline.pausemenu.nest.visible = true;
		 
		 for (i = 0; i < num_orig; i++)
			m_timeline.pausemenu.nest["btn" + (i+1)].visible = true;
		 
		 if (xv2p_extended)
		 {			 
			 var top_y:Number = 0;
			 
			 var rh:Number = m_timeline.pausemenu.nest["btn1"].height;
			 m_timeline.pausemenu.nest.height += 90*xv2p_added_buttons.length ;
			 var ah:Number = m_timeline.pausemenu.nest["btn1"].getBounds(m_timeline).height;
			 
			 var distance_Y:Number = m_timeline.pausemenu.nest["btn2"].y - m_timeline.pausemenu.nest["btn1"].y;
			 distance_Y /= (ah / rh);
			 
			 for (i = 0; i < num_orig; i++)
			 {
				resizeButton(m_timeline.pausemenu.nest["btn" + (i+1)]);
			 }
			  
			 if (xv2p_add_where != 0)
			 {
				m_timeline.pausemenu.nest["btn1"].y -= xv2p_added_buttons.length * 2;
				top_y = m_timeline.pausemenu.nest["btn1"].y;
			 }
			 
			 for (i = 0; i < xv2p_added_buttons.length; i++)
			 {
				var enable:Boolean = true;
				
				if (i == TOGGLE_BATTLE_UI_VAR)
				{
					if (XV2Patcher.IsBattleUIHidden())
					{
						xv2p_added_buttons[i] = "Show UI";
					}
					else
					{
						xv2p_added_buttons[i] = "Hide UI";
					}
				}
				else if (i == DUMP_AG_PORTRAIT_VAR)
				{
					// Nothing now
				}
				else if (i == START_MOB_CTRL_VAR)
				{
					if (disable_mob_ctrl)
						enable = false;
				}
								
				var y:int = addNewButton(xv2p_added_buttons[i], distance_Y, top_y, i+xv2p_add_where, enable);
				
				if (i == 0 && xv2p_add_where == 0)
					top_y = y;
			 }
			 
			 if (xv2p_add_where == 0)
				i = 0;
			 else
				i = 1;
			 
			 for (; i < num_orig; i++)
			 {
				 var real_i:int = i;
				 
				 if (real_i >= xv2p_add_where)
					real_i += xv2p_added_buttons.length;
				 
				 m_timeline.pausemenu.nest["btn" + (i+1)].y = top_y + distance_Y * real_i;
			 }
			 
			 m_button_list.SetXV2PInfo(xv2p_add_where, xv2p_added_buttons.length, num_orig);
			 
			 if (xv2p_add_where == 0)
			 {
				m_button_list.ChangeButton(num_orig);
			 }
		 }
		 else
		 {
			m_button_list.SetXV2PInfo(-1, -1, -1);
		 }
		 /////////
		 
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseOpen);
         m_flag_decide = false;
         m_callback.SetUserDataValidFlag(ReceiveTypeFlagEnd,false);
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,phaseMain);
         m_timeline.stage.addEventListener(KeyboardEvent.KEY_DOWN,checkKey);
      }
	  
	  //////////
	  private function runXV2PFunction(func : int) : void
	  {
		
		if (func < 0)
			return;
		
		if (!disable_mob_ctrl && func >= START_MOB_CTRL_VAR && func < (START_MOB_CTRL_VAR + xv2p_mobs_idx.length))
		{
			var i:int = func-START_MOB_CTRL_VAR;
			
			XV2Patcher.TakeControlOfMob(xv2p_mobs_idx[i]);
			return;
		}
		
		switch (func)
		{
			case TOGGLE_BATTLE_UI_VAR:
				XV2Patcher.ToggleBattleUI();
			break;
			
			case DUMP_AG_PORTRAIT_VAR:
				XV2Patcher.DumpAutoGenPortrait();
			break;
		}
	  }
	  /////////
      
      public function phaseMain(param1:Event) : void
      {
		 var _loc2_:Boolean = m_callback.GetUserDataValidFlag(ReceiveTypeFlagEnd);
         if(!m_flag_decide && !_loc2_)
         {
            return;
         }
         var _loc3_:int = m_button_list.GetSelectButton();
         if(_loc2_)
         {
            _loc3_ = -1;
         }
		 
		 //////////
		 if (xv2p_extended && _loc3_ >= num_orig)
		 {
			if (m_flag_decide)
			{
				runXV2PFunction(_loc3_ - num_orig);
			}
			
			_loc3_ = -1;
		 }
		 //////////
		 
         m_callback.CallbackDecide(_loc3_);
         if(m_flag_decide)
         {
            m_callback.CallbackSe(m_callback.SeTypeDecide);
         }
         else
         {
            m_callback.CallbackSe(m_callback.SeTypeCancel);
         }
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseMain);
         m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         m_flag_decide = false;
         m_timeline.stage.addEventListener(Event.ENTER_FRAME,phaseWaitClose);
      }
      
      public function phaseWaitClose(param1:Event) : void
      {
         var _loc2_:Boolean = m_callback.GetUserDataValidFlag(ReceiveTypeFlagEnd);
         if(!_loc2_)
         {
            return;
         }
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseWaitClose);
         m_flag_decide = false;
         var _loc3_:* = m_callback.GetUserDataInt(ReceiveTypeFlagEnd);
         if(!_loc3_)
         {
            m_timeline.stage.addEventListener(Event.ENTER_FRAME,phaseMain);
            m_timeline.stage.addEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         }
         else
         {
            m_timeline.pausemenu.gotoAndPlay("end");
            m_timeline.stage.addEventListener(Event.ENTER_FRAME,phaseClose);
         }
      }
      
      public function phaseClose(param1:Event) : void
      {
         if(m_timeline.pausemenu.currentLabel != "end_comp")
         {
            return;
         }
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseClose);
         m_callback.CallbackExit();
      }
      
      private function checkKey(param1:KeyboardEvent) : void
      {
         switch(param1.keyCode)
         {
            case Keyboard.ENTER:
            case 68:
               if(m_button_list.GetSelectState())
               {
                  m_flag_decide = true;
               }
               else
               {
                  m_callback.CallbackSe(m_callback.SeTypeNg);
               }
               break;
            case Keyboard.UP:
               m_callback.CallbackSe(m_callback.SeTypeCarsol);
               m_button_list.SelectPrevButton();
               m_callback.CallbackUserData("user",SendType_SelectIndex,m_button_list.GetSelectButton());
               break;
            case Keyboard.DOWN:
               m_callback.CallbackSe(m_callback.SeTypeCarsol);
               m_button_list.SelectNextButton();
               m_callback.CallbackUserData("user",SendType_SelectIndex,m_button_list.GetSelectButton());
               break;
            case Keyboard.SPACE:
            case 76:
            case 82:
            case 67:
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
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseWaitStart);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseOpen);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseMain);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseWaitClose);
         m_timeline.stage.removeEventListener(Event.ENTER_FRAME,phaseClose);
         m_timeline.stage.removeEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         m_timeline.visible = false;
         m_timeline = null;
      }
      
      public function TestCheckUserDataValidFlag(param1:int) : Boolean
      {
         return m_callback.GetUserDataValidFlag(param1);
      }
   }
}
