package action_script
{
   import flash.display.MovieClip;
   import flash.text.TextField;
   
   public class Utility
   {
       
      
      public function Utility()
      {
         super();
      }
      
      public static function GetLabelEndFrame(param1:MovieClip, param2:String) : int
      {
         var _loc3_:int = param1.totalFrames - 1;
         var _loc4_:int = 0;
         while(_loc4_ < param1.currentLabels.length)
         {
            if(param1.currentLabels[_loc4_].name == param2 && _loc4_ + 1 < param1.currentLabels.length)
            {
               _loc3_ = param1.currentLabels[_loc4_ + 1].frame - 1;
            }
            _loc4_++;
         }
         return _loc3_;
      }
      
      public static function SetAutoSizedText(param1:TextField, param2:String) : void
      {
         var _loc4_:Number = NaN;
         var _loc5_:String = null;
         var _loc6_:Number = NaN;
         var _loc3_:MovieClip = param1.parent as MovieClip;
         if(!_loc3_)
         {
            return;
         }
         if(_loc3_.user_add_property_default_width == undefined)
         {
            _loc4_ = param1.width;
            _loc5_ = param1.getTextFormat().align;
            _loc6_ = param1.x;
            _loc3_.user_add_property_default_width = _loc4_;
            _loc3_.user_add_property_default_align = _loc5_;
            _loc3_.user_add_property_default_x = _loc6_;
            param1.autoSize = param1.getTextFormat().align;
         }
         SetSizedText(param1,param2,_loc3_.user_add_property_default_width,_loc3_.user_add_property_default_align,_loc3_.user_add_property_default_x);
      }
      
      public static function SetSizedText(param1:TextField, param2:String, param3:Number, param4:String, param5:Number) : void
      {
         param1.autoSize = param4;
         param1.scaleX = 1;
         if(param4 == "right")
         {
            param1.x = param5 + param3 - param1.width;
         }
         else if(param4 == "center")
         {
            param1.x = param5 + (param3 - param1.width) / 2;
         }
         param1.text = param2;
         if(param3 < param1.width)
         {
            param1.scaleX = param3 / param1.width;
            param1.x = param5;
         }
      }
      
      public static function SetAutoSizedhtmlText(param1:TextField, param2:String, param3:Number = 0) : void
      {
         var _loc5_:Number = NaN;
         var _loc6_:String = null;
         var _loc7_:Number = NaN;
         var _loc4_:MovieClip = param1.parent as MovieClip;
         if(!_loc4_)
         {
            return;
         }
         if(_loc4_.user_add_property_default_width == undefined)
         {
            _loc5_ = param1.width;
            _loc6_ = param1.getTextFormat().align;
            _loc7_ = param1.x;
            _loc4_.user_add_property_default_width = _loc5_;
            _loc4_.user_add_property_default_align = _loc6_;
            _loc4_.user_add_property_default_x = _loc7_;
            param1.autoSize = param1.getTextFormat().align;
            param1.wordWrap = false;
         }
         SetSizedhtmlText(param1,param2,_loc4_.user_add_property_default_width,_loc4_.user_add_property_default_align,_loc4_.user_add_property_default_x);
      }
      
      public static function SetSizedhtmlText(param1:TextField, param2:String, param3:Number, param4:String, param5:Number) : void
      {
         if(param2 == "")
         {
            param2 = " ";
         }
         param1.autoSize = param4;
         param1.scaleX = 1;
         if(param4 == "right")
         {
            param1.x = param5 + param3 - param1.width;
         }
         else if(param4 == "center")
         {
            param1.x = param5 + (param3 - param1.width) / 2;
         }
         param1.htmlText = param2;
         if(param3 < param1.width)
         {
            param1.scaleX = param3 / param1.width;
            param1.x = param5;
         }
      }
      
      public static function GetNotReplacedString(param1:String, param2:Boolean = false) : String
      {
         if(param2)
         {
            return "\t" + param1;
         }
         return param1 + "\t";
      }
      
      public static function ReplaceStringForHtmlText(param1:String) : String
      {
         var _loc5_:String = null;
         var _loc6_:Boolean = false;
         var _loc7_:int = 0;
         var _loc2_:Array = [{
            "src":"<",
            "dst":"&lt;"
         },{
            "src":">",
            "dst":"&gt;"
         },{
            "src":"&",
            "dst":"&amp;"
         },{
            "src":"\"",
            "dst":"&quot;"
         },{
            "src":"\'",
            "dst":"&apos;"
         }];
         var _loc3_:String = "";
         var _loc4_:int = 0;
         while(_loc4_ < param1.length)
         {
            _loc5_ = param1.charAt(_loc4_);
            _loc6_ = false;
            _loc7_ = 0;
            while(_loc7_ < _loc2_.length)
            {
               if(_loc5_ == _loc2_[_loc7_].src)
               {
                  _loc3_ = _loc3_ + _loc2_[_loc7_].dst;
                  _loc6_ = true;
                  break;
               }
               _loc7_++;
            }
            if(!_loc6_)
            {
               _loc3_ = _loc3_ + _loc5_;
            }
            _loc4_++;
         }
         return _loc3_;
      }
   }
}
