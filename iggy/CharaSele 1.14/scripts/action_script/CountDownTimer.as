package action_script
{
   import flash.display.MovieClip;
   import flash.events.TimerEvent;
   import flash.utils.Timer;
   
   public class CountDownTimer
   {
      
      private static const DigitMax:uint = 3;
       
      
      private var m_mc_counter:MovieClip = null;
      
      private var m_mc_base:MovieClip = null;
      
      private var m_timer:Timer = null;
      
      private var m_cb_func_end:Function = null;
      
      private var m_warn_count:uint;
      
      public function CountDownTimer(param1:uint = 9)
      {
         super();
         m_mc_counter = null;
         m_mc_base = null;
         m_timer = null;
         m_cb_func_end = null;
         m_warn_count = param1;
      }
      
      public function Destroy() : void
      {
         destroyTimer();
         m_mc_base = null;
         m_mc_counter = null;
      }
      
      public function Initialize(param1:MovieClip, param2:MovieClip = null) : void
      {
         m_mc_counter = param1;
         m_mc_base = param2;
         m_timer = null;
         m_cb_func_end = null;
         m_mc_counter.visible = false;
         if(m_mc_base)
         {
            m_mc_base.visible = false;
         }
      }
      
      public function Start(param1:int, param2:Function = null) : void
      {
         if(param1 < 0)
         {
            return;
         }
         param1 = capMaxTime(param1);
         initDigit(param1);
         if(m_timer)
         {
            destroyTimer();
            setMcTimer(param1,"move");
            createTimer(param1,param2);
            return;
         }
         m_mc_counter.visible = true;
         setMcTimer(param1,"start");
         if(m_mc_base)
         {
            m_mc_base.visible = true;
            m_mc_base.gotoAndPlay("start");
         }
         createTimer(param1,param2);
      }
      
      public function End() : void
      {
         destroyTimer();
         if(m_mc_base)
         {
            m_mc_base.gotoAndPlay("end");
         }
         setMcTimer(-1,"end");
      }
      
      public function Stop() : void
      {
         if(m_timer)
         {
            m_timer.removeEventListener(TimerEvent.TIMER,countTimer);
            m_timer.removeEventListener(TimerEvent.TIMER_COMPLETE,compTimer);
         }
      }
      
      public function GetTime() : int
      {
         if(!m_timer)
         {
            return -1;
         }
         var _loc1_:int = m_timer.repeatCount - m_timer.currentCount;
         return _loc1_;
      }
      
      private function capMaxTime(param1:int) : int
      {
         var _loc2_:int = Math.pow(10,DigitMax);
         if(_loc2_ <= param1)
         {
            param1 = _loc2_ - 1;
         }
         return param1;
      }
      
      private function initDigit(param1:int) : void
      {
         var _loc2_:int = DigitMax;
         var _loc3_:int = 10;
         var _loc4_:int = 1;
         while(_loc4_ < DigitMax)
         {
            if(param1 < _loc3_)
            {
               _loc2_ = _loc4_;
               break;
            }
            _loc3_ = _loc3_ * 10;
            _loc4_++;
         }
         m_mc_counter.gotoAndStop(DigitMax + 1 - _loc2_);
      }
      
      private function createTimer(param1:int, param2:Function = null) : void
      {
         if(m_timer)
         {
            return;
         }
         m_cb_func_end = param2;
         m_timer = new Timer(1000,param1);
         m_timer.addEventListener(TimerEvent.TIMER,countTimer);
         m_timer.addEventListener(TimerEvent.TIMER_COMPLETE,compTimer);
         m_timer.start();
      }
      
      private function destroyTimer() : void
      {
         if(!m_timer)
         {
            return;
         }
         m_timer.stop();
         m_timer.removeEventListener(TimerEvent.TIMER,countTimer);
         m_timer.removeEventListener(TimerEvent.TIMER_COMPLETE,compTimer);
         m_timer = null;
         m_cb_func_end = null;
      }
      
      private function countTimer(param1:TimerEvent) : void
      {
         var _loc2_:int = m_timer.repeatCount - m_timer.currentCount;
         setMcTimer(_loc2_,"move");
         initDigit(_loc2_);
      }
      
      private function compTimer(param1:TimerEvent) : void
      {
         m_cb_func_end();
         Stop();
      }
      
      private function setMcTimer(param1:int, param2:String) : void
      {
         var _loc6_:int = 0;
         var _loc3_:* = param1 <= m_warn_count;
         var _loc4_:int = param1;
         var _loc5_:int = 1;
         while(_loc5_ <= DigitMax)
         {
            if(0 <= _loc4_)
            {
               if(m_mc_counter["nmb_" + _loc5_])
               {
                  _loc6_ = _loc4_ % 10 + 1;
                  if(_loc3_)
                  {
                     _loc6_ = _loc6_ + 10;
                  }
                  m_mc_counter["nmb_" + _loc5_].nmb01.gotoAndStop(_loc6_);
               }
               _loc4_ = _loc4_ / 10;
            }
            if(param2)
            {
               if(m_mc_counter["nmb_" + _loc5_])
               {
                  m_mc_counter["nmb_" + _loc5_].gotoAndPlay(param2);
               }
            }
            _loc5_++;
         }
      }
   }
}
