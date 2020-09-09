package action_script
{
   import flash.display.MovieClip;
   import flash.events.Event;
   import flash.events.KeyboardEvent;
   
   public class Test
   {
       
      
      public function Test()
      {
         super();
      }
      
      public static function Run(param1:MovieClip, param2:StageSele) : void
      {
         var i:int = 0;
         var StageName:Array = null;
         var checkKey:Function = null;
         var waitRequestList:Function = null;
         var timeline:MovieClip = param1;
         var main:StageSele = param2;
         checkKey = function(param1:KeyboardEvent):void
         {
            switch(param1.keyCode)
            {
               case 49:
                  if(main)
                  {
                     main.TestDestroy();
                  }
                  main = new StageSele();
                  main.Initialize(timeline);
                  main.SetUserDataInt(StageSele.ReceiveTypeFlagCancel,1);
                  i = 0;
                  while(StageSele.StageIDNum > i)
                  {
                     main.SetUserDataString(StageSele.ReceiveTypeStageStrStart + i,StageName[i]);
                     main.SetUserDataString(StageSele.ReceiveTypeImageStrStart + i,StageName[i]);
                     main.SetUserDataInt(StageSele.ReceiveTypeStageUnlockStart + i,i % 3);
                     main.SetUserDataInt(StageSele.ReceiveTypeSelectedStart + i,0);
                     i++;
                  }
                  main.SetUserDataInt(StageSele.ReceiveTypeTime,5);
                  main.SetUserDataInt(StageSele.ReceiveTypeFlagExit,0);
                  break;
               case 50:
                  break;
               case 51:
                  break;
               case 52:
                  break;
               case 53:
                  break;
               case 54:
                  break;
               case 55:
                  break;
               case 56:
            }
         };
         waitRequestList = function(param1:Event):void
         {
            if(!main)
            {
               return;
            }
         };
         i = 0;
         var j:int = 0;
         var k:int = 0;
         var l:int = 0;
         StageName = ["ロビー","ロビー（破壊）","ロビー（バトルスペース）","ポッド着陸地点","岩山","岩山（夜）","ナメック星","フリーザの宇宙船","崩壊するナメック星","西の都（カプセルコーポ）","西の都（商業区）","西の都（居住区）","セルゲーム会場","セルゲーム会場（破壊）","荒野","神様の神殿","空","精神と時の部屋","界王神界","宇宙（地球）","宇宙（惑星ベジータ）","氷河","ツフル星","原野","天下一武道会会場","地底湖","時の狭間","ナメック星（最長老の家）","列島(晴)","列島(曇)","Unknown Stage"];
         timeline.stage.addEventListener(KeyboardEvent.KEY_DOWN,checkKey);
         timeline.stage.addEventListener(Event.ENTER_FRAME,waitRequestList);
      }
   }
}
