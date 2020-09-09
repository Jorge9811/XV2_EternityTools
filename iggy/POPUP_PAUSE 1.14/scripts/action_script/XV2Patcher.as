package action_script
{
   import flash.external.ExternalInterface;

   public class XV2Patcher
   { 
	  // From version 02 of the XV2Patcher external interface, all internal names must start with XV2P.
	  // This is for the xv2patchet to quickly filter out the original native functions of the game.
   
	  public static const XV2_PATCHER_AS3_TAG:String = "XV2_PATCHER_AS3_V06";
      
      public function XV2Patcher()
      {
         super();
      }
      
      public static function HelloWorld() : String
      {
         try 
		 {
			return ExternalInterface.call("XV2PHelloWorld");
		 } catch (e: Error)
		 {
		 }
		 
		 return "This doesn't work";
      }
	  
	  public static function GetSlotsData() : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetSlotsData");
		} catch(e: Error)
		{
		}
		
		return "{}";
	  }
	  
	  public static function IsBattleUIHidden() : Boolean
	  {
		try
		{
			return (ExternalInterface.call("XV2PIsBattleUIHidden") != 0);
		} catch (e: Error)
		{			
		}
		
		return false;
	  }
	  
	  public static function ToggleBattleUI() : void
	  {
		try
		{
			ExternalInterface.call("XV2PToggleBattleUI");
		} catch (e: Error)
		{
		}
	  }
	  
	  public static function GetFirstAutoGenPortraitCharName() : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetFirstAutoGenPortraitCharName");
		} catch (e: Error)
		{
		}
		
		return "";
	  }
	  
	  public static function CanDumpAutoGenPortrait() : Boolean
	  {
		try
		{
			return (ExternalInterface.call("XV2PCanDumpAutoGenPortrait") != 0);
		} catch (e: Error)
		{
		}
		
		return false;
	  }
	  
	  public static function DumpAutoGenPortrait() : void
	  {
		try
		{
			ExternalInterface.call("XV2PDumpAutoGenPortrait");
		} catch (e: Error)
		{
		}
	  }
	  
	  public static function GetNumSsStages() : int
	  {
		try
		{
			return ExternalInterface.call("XV2PGetNumSsStages");
		} catch (e: Error)
		{
		}
		
		return 0;
	  }
	  
	  public static function GetStagesSlotsData() : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetStagesSlotsData");
		} catch (e: Error)
		{
		}
		
		return "";
	  }
	  
	  public static function GetLocalStagesSlotsData() : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetLocalStagesSlotsData");
		} catch (e: Error)
		{
		}
		
		return "";
	  }
	  
	  public static function GetStageName(ssid : int) : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetStageName" + ssid.toString());
		} catch (e: Error)
		{
		}
		
		return "";
	  }
	  
	  public static function GetStageImageString(ssid : int) : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetStageImageString" + ssid.toString());
		} catch (e: Error)
		{
		}
		
		return "";
	  }
	  
	  public static function GetStageIconString(ssid : int) : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetStageIconString" + ssid.toString());
		} catch (e: Error)
		{
		}
		
		return "";
	  }
	  
	  public static function StageHasBeenSelectedBefore(ssid : int) : int
	  {
		try
		{
			return ExternalInterface.call("XV2PStageHasBeenSelectedBefore" + ssid.toString());
		} catch (e: Error)
		{
		}
		
		return 0;
	  }
	  
	  public static function ShouldExtendPauseMenu() : Boolean
	  {
		try
		{
			return (ExternalInterface.call("XV2PShouldExtendPauseMenu") != 0);
		}
		catch (e: Error)
		{
		}
		
		return false;
	  }
	  
	  public static function GetPlayerAllies() : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetPlayerAllies");
		} catch (e: Error)
		{
		}
		
		return "";
	  }
	  
	  public static function GetMobName(idx : int) : String
	  {
		try
		{
			return ExternalInterface.call("XV2PGetMobName" + idx.toString());
		} catch (e: Error)
		{
		}
		
		return "";
	  }
	  
	  public static function TakeControlOfMob(idx : int) : void
	  {
		try
		{
			ExternalInterface.call("XV2PTakeControlOfMob" + idx.toString());
		} catch (e: Error)
		{
		}
	  }
	  
	  public static function IsToggleUIEnabled() : Boolean
	  {
		try
		{
			return (ExternalInterface.call("XV2PIsToggleUIEnabled") != 0);
		} catch (e: Error)
		{
		}
		
		return false;
	  }
   }
}
