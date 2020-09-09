package action_script
{
   import flash.display.MovieClip;
   import flash.external.ExternalInterface;
   
   public class Callback
   {
       
      
      public var SeTypeDecide:int;
      
      public var SeTypeCancel:int;
      
      public var SeTypeNg:int;
      
      public var SeTypeCarsol:int;
      
      public var SeTypePageChnage:int;
      
      public var SeTypeWindowOpen:int;
      
      public var SeTypeWindowClose:int;
      
      public var SeTypeChatWindow:int;
      
      public var SeTypeChatText:int;
      
      public var SeTypeChatRefrect:int;
      
      public var SeTypeTitleStart:int;
      
      public var SeTypeThankYou:int;
      
      public var SeTypeCount:int;
      
      public var SeTypeInvalid:int;
      
      public var m_mc_target:MovieClip;
      
      public var m_user_data:Array;
      
      public var m_user_data_valid_flag:Array;
      
      public function Callback(param1:int)
      {
         var user_data_size:int = param1;
         super();
         SeTypeInvalid = -1;
         SeTypeDecide = 0;
         SeTypeCancel = 1;
         SeTypeNg = 2;
         SeTypeCarsol = 3;
         SeTypePageChnage = 4;
         SeTypeWindowOpen = 5;
         SeTypeWindowClose = 6;
         SeTypeChatWindow = 7;
         SeTypeChatText = 8;
         SeTypeChatRefrect = 9;
         SeTypeTitleStart = 10;
         SeTypeThankYou = 11;
         SeTypeCount = 12;
         m_mc_target = null;
         m_user_data = new Array(user_data_size);
         m_user_data_valid_flag = new Array(user_data_size);
         var i:int = 0;
         while(user_data_size > i)
         {
            m_user_data_valid_flag[i] = false;
            i++;
         }
         try
         {
            ExternalInterface.addCallback("GotoAndPlayLabel",AddCallbackGotoAndPlayLabel);
            ExternalInterface.addCallback("GotoAndPlayFrame",AddCallbackGotoAndPlayFrame);
            ExternalInterface.addCallback("GotoAndStopLabel",AddCallbackGotoAndStopLabel);
            ExternalInterface.addCallback("GotoAndStopFrame",AddCallbackGotoAndStopFrame);
            ExternalInterface.addCallback("SetUserData",AddCallbackSetUserDataInt);
            ExternalInterface.addCallback("SetUserData",AddCallbackSetUserDataString);
            return;
         }
         catch(e:Error)
         {
            trace("ACTION LIB EXCEPTION" + e.toString());
            return;
         }
      }
      
      public function CallbackUserData(param1:String, param2:int, param3:int) : void
      {
         var str:String = param1;
         var user_data0:int = param2;
         var user_data1:int = param3;
         try
         {
            ExternalInterface.call(str,user_data0,user_data1);
            return;
         }
         catch(e:Error)
         {
            trace("ACTION LIB EXCEPTION" + e.toString());
            return;
         }
      }
      
      public function CallbackUserDataString(param1:String, param2:int, param3:String) : void
      {
         var str:String = param1;
         var user_data0:int = param2;
         var user_data1:String = param3;
         try
         {
            ExternalInterface.call(str,user_data0,user_data1);
            return;
         }
         catch(e:Error)
         {
            trace("ACTION LIB EXCEPTION" + e.toString());
            return;
         }
      }
      
      public function CallbackCancel() : void
      {
         CallbackUserData("cancel",0,0);
      }
      
      public function CallbackDecide(param1:int) : void
      {
         CallbackUserData("decide",param1,0);
      }
      
      public function CallbackExit() : void
      {
         CallbackUserData("exit",0,0);
      }
      
      public function CallbackSe(param1:int) : void
      {
         CallbackUserData("playSe",param1,0);
      }
      
      public function SetCallbacTarget(param1:MovieClip) : void
      {
         m_mc_target = param1;
      }
      
      public function GetUserDataInt(param1:int) : int
      {
         if(param1 < 0)
         {
            return 0;
         }
         if(param1 >= m_user_data.length)
         {
            return 0;
         }
         return m_user_data[param1];
      }
      
      public function GetUserDataString(param1:int) : String
      {
         if(param1 < 0)
         {
            return "";
         }
         if(param1 >= m_user_data.length)
         {
            return "";
         }
         return m_user_data[param1];
      }
      
      public function SetUserDataValidFlag(param1:int, param2:Boolean) : void
      {
         if(param1 < 0)
         {
            return;
         }
         if(param1 >= m_user_data_valid_flag.length)
         {
            return;
         }
         m_user_data_valid_flag[param1] = param2;
      }
      
      public function GetUserDataValidFlag(param1:int) : Boolean
      {
         if(param1 < 0)
         {
            return false;
         }
         if(param1 >= m_user_data_valid_flag.length)
         {
            return false;
         }
         return m_user_data_valid_flag[param1];
      }
      
      public function AddCallbackGotoAndPlayLabel(param1:String) : void
      {
         m_mc_target.gotoAndPlay(param1);
      }
      
      public function AddCallbackGotoAndPlayFrame(param1:int) : void
      {
         m_mc_target.gotoAndPlay(param1);
      }
      
      public function AddCallbackGotoAndStopLabel(param1:String) : void
      {
         m_mc_target.gotoAndStop(param1);
      }
      
      public function AddCallbackGotoAndStopFrame(param1:int) : void
      {
         m_mc_target.gotoAndStop(param1);
      }
      
      public function AddCallbackSetUserDataInt(param1:int, param2:int) : void
      {
         if(param1 < 0)
         {
            return;
         }
         if(param1 >= m_user_data.length)
         {
            return;
         }
         m_user_data[param1] = param2;
         m_user_data_valid_flag[param1] = true;
      }
      
      public function AddCallbackSetUserDataString(param1:int, param2:String) : void
      {
         if(param1 < 0)
         {
            return;
         }
         if(param1 >= m_user_data.length)
         {
            return;
         }
         m_user_data[param1] = param2;
         m_user_data_valid_flag[param1] = true;
      }
   }
}
